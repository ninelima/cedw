 

#include "stdio.h"
#include "string.h"

#include "ced.h"

char* file_getfname( char * );


extern struct editrec *edfirst;
extern struct editrec *ed;
extern struct coords cursor;
extern int edcurno;
extern int edcount;
extern char mrfname[];
extern int blck;
extern struct linerec *blckline;
extern struct coords blckstr, blckend;
extern int fileos;


int filemodchkf = 0;
int restoredirf = 0;

int fncnt;
char fpath[CE_LEN_FILEPATH];
char fnlist[CE_MAX_FILES+1][CE_LEN_FILENAME];   // !!!! 100k Dynamic Mem ??



file_open( char *fn )  {
        int n, fx, yn;
        char fname[CE_LEN_FILEPATH];
        int ostype;
        struct ftimerec ftime;
        struct editrec *edt;

        ostype = fileos;
        fname[0] = '\0';
        if( *fn )  {
                if( strlen( fn ) >= CE_LEN_FILEPATH )  {
                        dlg_err( "File Path Too Long" );
                        return( 0 );
                        }
                strcpy( fname, fn );
                fncnt = file_nxplode( fname, fpath, fnlist );
                if( fncnt == 0 )
                        return(0);
                if( fncnt == 1 )  {
                        if ( ! (fx = file_exist( fname, &ftime )))
                                return(0);
                        if( fx == -1 )
                                if (dlg_ocfile("File Does Not Exist !  Create", "File Open", fname ) != 1)
                                        return(0);
                        }
                }
        else    {
                while( 1 )  {
                        if( !( dlg_fileopen( fname, &ostype, &fncnt, fpath, fnlist )))  return(0);
                        if( ! fncnt )  {
                                if( *fname == '\0' )  {
                                        dlg_err( "No File Name Specified" );
                                        return(0);
                                        }
                                /*else if( fname[ strlen( fname )- 1 ] == '\\' )  {
                                        dlg_err( "No File Name Specified" );
                                        return(0);
                                        }*/
                                edit_wait(-1);
                                fncnt = file_nxplode( fname, fpath, fnlist );
                                edit_wait(0);
                                }
                        if( fncnt > 1 )
                                break;
                        if(( fncnt == 1 )&&(( strchr( fname, '*' ) != NULL )||( strchr( fname, '?' ) != NULL )))
                                break;
                        if( fncnt == 0 )
                                continue;
                        edit_wait(-1);
                        fx = file_exist( fname, &ftime );
                        edit_wait(0);
                        if ( ! fx )
                                continue;
                        if( fx == -1 )  {
                                if( yn = dlg_yncfile("File Does Not Exist !  Create", "File Open", fname))  {
                                        if( yn == 1 )
                                                break;
                                        else
                                                continue;
                                        }
                                else
                                        return(0);
                                }
                        else
                                break;
                        }
                }
        edit_wait(-1);
        for( n = 0; n < fncnt; ++n )  {
                strcpy(fname, fpath);
                strcat(fname, fnlist[n]);
                yn = 1;
                for( edt = edfirst; edt != NULL; edt = edt->next )  {
                        if( stricmp( fname, edt->fname )== 0 )  {
                                scrn_statsl();
                                scrn_rescrn();
                                yn = dlg_yncfile("File Already Loaded !  Open Duplicate", "File Open", fname);
                                break;
                                }
                        }
                if( yn == 0 )
                        break;
                else  if( yn == -1 )
                        continue;

                if (! edit_new())  return(0);
                strcpy(ed->fname, fname);
                ed->ostype = ostype;
                scrn_statsl();
                scrn_rescrn();
                if ( ! file_read(ed->fname, ed->firstline, &ed->nolines, &ed->ostype, &ed->ftime))  {
                        ed->fname[0] = '\0';
                        edit_free();
                        }
                else  {
                        crma_file( ed );
                        tree_insfile( fnlist[ n ] );
                    }
                }
        edit_wait(0);
        return(-1);
        }




file_save()  {                  // FIX : else if's could be smoother but it works: most can be removed
        int yn, fx;
        struct ftimerec ft;

        if (ed->changef == -1)  {
                if (ed->fname[0] == '\0')  {
                        while( 1 )  {
                                if( !( dlg_filesave( ed->fname, &ed->ostype )))
                                        return(0);
                                if(( strchr( ed->fname, '*' ) != NULL )||( strchr( ed->fname, '?' ) != NULL ))  {
                                        dlg_okstop( "Wildcard Filenames Not Allowed !", "File Save" );
                                        continue;
                                        }
                                if( !( fx = file_exist( ed->fname, &ed->ftime )))
                                        continue;
                                else if( fx == 1 )  {
                                        if( yn = dlg_yncfile("File Already Exist !  Overwrite", "File Save", ed->fname))  {
                                                if( yn == 1 )  {
                                                        edit_wait(-1);
                                                        if( ! file_write(ed->fname, ed->firstline, ed->nolines, ed->ostype, &ed->ftime ))  {
                                                                edit_wait(0);
                                                                return(0);
                                                                }
                                                        edit_wait(0);
                                                        ed->changef = 0;
                                                        crma_file( ed );
                                                        tree_insfile( file_getfname( ed->fname ));
                                                        tree_selfile( ed->hTreeItem, -1 );
                                                        return(-1);
                                                        }
                                                else
                                                        continue;
                                                }
                                        else    {
                                                ed->fname[0] = '\0';
                                                return( 0 );
                                                }
                                        }
                                else    {       // fx == -1
                                        edit_wait(-1);
                                        if( ! file_write(ed->fname, ed->firstline, ed->nolines, ed->ostype, &ed->ftime))  {
                                                edit_wait(0);
                                                return(0);
                                                }
                                        edit_wait(0);
                                        ed->changef = 0;
                                        crma_file( ed );
                                        tree_insfile( file_getfname( ed->fname ));
                                        tree_selfile( ed->hTreeItem, -1 );
                                        return(-1);
                                        }
                                }
                        }
                else    {
                        edit_wait(-1);
                        if( ! file_time( ed->fname, &ft ))  {
                                edit_wait(0);
                                return(0);
                                }
                        edit_wait(0);
                        if(( ! file_tcmp( &ed->ftime, &ft ))||( ed->reloadf ))  {
                                if( yn = dlg_ync( "File Possibly Modified Externaly !  Overwrite", "File Save" ))  {
                                        if( yn == -1 )
                                                return( -1 );
                                        }
                                else
                                        return(0);
                                }
                        edit_wait(-1);
                        if ( ! file_write(ed->fname, ed->firstline, ed->nolines, ed->ostype, &ed->ftime))  {
                                edit_wait(0);
                                return(0);
                                }
                        edit_wait(0);
                        ed->changef = 0;
                        ed->reloadf = 0;
                        return(-1);
                        }
                }
        return(-1);
        }


file_saveall()  {
        int ecn, yna;
        struct editrec *edt;

        ecn = edcurno;
        edt = ed;
        edcurno = 1;
        yna = 0;
        for( ed = edfirst; ed != NULL; ( ed = ed->next, edcurno += 1 ))  {
                if (ed->changef == -1)  {
                        scrn_statsl();
                        scrn_rescrn();
                        if( yna == 2 )  {
                                if( ! file_save())
                                        return(0);
                                }
                        else    {
                                if( !( yna = dlg_ynafile("File Modified !  Save Changes", "File Save")))
                                        break;
                                if(( yna == 1 )||( yna == 2 ))  {
                                        if( ! file_save())
                                                return(0);
                                        }
                                }
                        }
                }
        edcurno = ecn;
        if (ed != NULL)  {
                ed = edt;
                return(0);
                }
        ed = edt;
        return(-1);
        }


file_close()  {

        if( ! fileclose())  return(0);
        edit_wait(-1);
        edit_free();
        edit_wait(0);
        return(-1);
        }


file_closeall()  {
        int f, ecn;
        struct editrec *edt;

        f = -1;
        if (ed->changef == -1)  {
                if (! fileclose())  {
                        return(0);
                        }
                }
        ecn = edcurno;
        edt = ed;
        edcurno = 1;
        for (ed = edfirst; ed != NULL; (ed = ed->next, edcurno += 1))  {
                if (ed == edt)
                        continue;
                if (ed->changef == -1)  {
                        scrn_statsl();
                        scrn_rescrn();
#ifdef _WIN32
                        tree_selfile( ed->hTreeItem, 0 );
#endif
                        if (! fileclose())  {
                                f = 0;
                                break;
                                }
                        }
                }
        if (f != 0)  {
                edcurno = ecn;
                ed = edt;
                scrn_statsl();
                scrn_rescrn();
                edcurno = 1;
                ed = edfirst;
                edit_wait(-1);
                while( edcount > 1 )  {
                        edit_free();
                        }
                edit_free();
                edit_wait(0);
                return(-1);
                }
        edcurno = ecn;
        ed = edt;
        return(0);
        }



fileclose()
        {
        int yn;

        if (ed->changef == -1)  {
                if( yn = dlg_ync("File Modified !  Save Changes", "File Close" ) )  {
                        if (yn == 1)
                                if( ! file_save())
                                        return(0);
                        }
                else
                        return( 0 );
                }
        return(-1);
        }


file_saveas()  {
        int fx, yn;
        char fname[CE_LEN_FILEPATH];
        struct ftimerec ftime;
        int ostype;

        ostype = ed->ostype;
        fname[0] = '\0';
        while( 1 )  {
                if( !( dlg_filesaveas(( ( blck )&&( blckend.y != blckstr.y ) ? "Save Selection" : "File Save As" ), fname, &ostype )))  return(0);
                if( !( fx = file_exist( fname, &ftime )))  {
                        continue;
                        }
                else if( fx == 1 )  {
                        if (yn = dlg_yncfile("File Already Exist !  Overwrite", "File Save As", fname))  {
                                if( yn == 1 )
                                        break;
                                continue;
                                }
                        return(0);
                        }
                break;
                }

        if( (blck )&&( blckend.y != blckstr.y ))  {
                edit_wait(-1);
                if( ! file_write( fname, blckline, blckend.y - blckstr.y, ostype, &ftime ))  {
                        edit_wait(0);
                        return(0);
                        }
                edit_wait(0);
                return(-1);
                }
        else    {
                strcpy( ed->fname, fname );
                ed->ostype = ostype;
                tree_delfile( ed->hTreeItem );
                edit_wait(-1);
                if( ! file_write( ed->fname, ed->firstline, ed->nolines, ostype, &ed->ftime ))  {
                        edit_wait(0);
                        return(0);
                        }
                edit_wait(0);
                ed->changef = 0;
                crma_file( ed );
                tree_insfile( file_getfname( fname ));
                tree_selfile( ed->hTreeItem, -1 );
                return(-1);
                }
        }



file_merge()  {
        char fname[CE_LEN_FILEPATH];
        int nolines, ostype;
        struct linerec *line;
        struct ftimerec ftime;
        int fx;

        fname[0] = '\0';
        while( 1 )  {
                if ( ! dlg_filemerge( fname, &ostype ))
                        return(0);
                if(( strchr( fname, '*' ) != NULL )||( strchr( fname, '?' ) != NULL ))  {
                        dlg_okstop( "Wildcard Filenames Not Allowed !", "File Insert" );
                        continue;
                        }
                if( !( fx = file_exist( fname, &ftime )))
                        continue;
                else if( fx == -1 )  {
                        dlg_okstop( "File Does Not Exist !", "File Insert" );        // dlg_okfile ???
                        continue;
                        }
                break;
                }
        if( edit_newline( &line ))  {
                edit_wait(-1);
                if( ! file_read(fname, line, &nolines, &ostype, &ftime))  {
                        edit_wait(0);
                        return(0);
                        }
                edit_merge( line, nolines );
                crma_file( ed );
                edit_wait(0);
                }
        return(-1);
        }



file_reload()  {

        if (ed->changef == -1)  {
                if (!dlg_oc("File Modified !  Discard Changes", "File Reload" ))
                        return(0);
                }
        undo_clear();
        ed->changef = 0;
        edit_freelines( ed->firstline->next );
        ed->firstline->strz[0] = '\0';
        ed->firstline->next = NULL;
        edit_wait(-1);
        if ( ! file_read(ed->fname, ed->firstline, &ed->nolines, &ed->ostype, &ed->ftime))  {
                ed->fname[0] = '\0';
                edit_free();
                edit_wait(0);
                return(0);
                }
        ed->reloadf = 0;
        crma_file( ed );
        edit_wait(0);
        return(-1);
        }



file_2reload()  {          // this function should probably be split in two, one to compare the file's dt and the other to do the reload with dialog ???
        int yn, x, y;
        struct ftimerec ft;

 
        if( ! filemodchkf )  return 0;
        if( !(*ed->fname)  ) return 0;

        edit_wait(-1);
        if( file_time_noerr( ed->fname, &ft ))  {
                edit_wait(0);
                if( ! file_tcmp( &ed->ftime, &ft ))  {
                        blck_unmark();
                        edit_beep();
                        if( yn = dlg_ynfile( "File Possibly Modified Externaly !  Reload", "File Modified", ed->fname ))  {
                                if( yn == 1 )  {
                                        x = ed->curcrds.x;
                                        y = ed->curcrds.y;
                                        if( file_reload())  {
                                                ed->displline = ed->firstline;
                                                ed->displcrds.x = 0;
                                                ed->displcrds.y = 0;
                                                cmnd_goto_line( y+1, x );
                                                }
                                        else    {
                                                scrn_rescrn();
                                                scrn_statsl();
                                                //ed->reloadf = -1;   not neccessary
                                                }
                                        }
                                else  {
                                        ed->reloadf = -1;
                                        ed->ftime = ft;
                                        }
                                }
                        }
                }
        edit_wait(0);
        return(-1);
        }



file_err( int errno, char *fname )  {

        edit_wait(0);
        dlg_errfile( "", fname );
        return(0);
        }
