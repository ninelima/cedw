/*
 *      All line and edit memory stuff
 *      No scrn_ calls
 *
 *
 *
 */

#include "stdio.h"
#include "string.h"
#include "malloc.h"

#include "ced.h"

extern int blck;
extern int assocdef;

int tablen = CE_LEN_TAB;
int filetab = CE_LEN_TAB;

int fileos = 0x03;

struct editrec *edfirst = NULL;
struct editrec *ed = NULL;
int edcurno = 1;
int edcount = 1;

char *editbufr;

extern int clpbcnt;
extern struct clpbrec clpbord[ ];


struct coords cursor;

int insmode = 1;



edit_ini()  {
        int n;
 
        if ((editbufr = malloc(CE_LEN_FILEBUFR)) == NULL)
                return(1);  // this is serious: quit completely
        if( clpb_ini() )
                return(1); 
        if ((edfirst = (struct editrec *)malloc(sizeof(struct editrec))) == NULL)
                return(1);  // this is serious: quit completely
        edfirst->prev = NULL;
        edfirst->next = NULL;
        ed = edfirst;
        if ((ed->undo = (struct undorec *)malloc(sizeof(struct undorec)* CE_MAX_UNDO )) == NULL)  {
                return(1);  // this is serious: quit completely
                }
        ed->undocnt = 0;
        ed->redocnt = 0;
        ed->undolns = 0;
        for( n = 0; n < CE_MAX_UNDO; ++n )  {
                ed->undo[ n ].line = NULL;
                ed->undo[ n ].cnt = 0;
                }
        if( ! edit_newline( &ed->firstline ))  {
                return(1);  // this is serious: quit completely
                }
        ed->next = NULL;
        ed->curline = ed->firstline;
        ed->displline = ed->firstline;
        ed->displcrds.x = 0;
        ed->displcrds.y = 0;
        ed->curcrds.x = 0;
        ed->curcrds.y = 0;
        ed->nolines = 1;
        ed->changef = 0;
        ed->reloadf = 0;
        ed->ostype = fileos;
        ed->fname[0] = '\0';
#ifdef _WIN32
        ed->ftime.lo = 0;
        ed->ftime.hi = 0;
#else
        ed->ftime.date = 0;
        ed->ftime.time = 0;
#endif
        cursor.x = 0;
        cursor.y = 0;
        insmode = 1;
        edcurno = 1;
        ed->mru = 1;
        ed->assoc = assocdef;
#ifdef _WIN32
        ed->hTreeItem = 0;
#endif
        ed = edfirst;
        bmrk_clearall( );
        }


edit_free()  {
        struct editrec *edt;
        struct linerec *ln;
        int ecn, mru;

        undo_clear( );
        edt = ed;
        ln = ed->firstline;
        ecn = edcurno;
        mru = ed->mru;
        if (ed == edfirst)  {
                if (edfirst->next == NULL)  {
                        ln = ed->firstline->next;
                        ed->nolines = 1;
                        ed->firstline->strz[0] = '\0';
                        ed->firstline->prev = NULL;
                        ed->firstline->next = NULL;
                        ed->firstline->atrs = NULL;
                        ed->firstline->atrc = 0;
                        ed->firstline->cmnt = 0;
                        ed->firstline->aspb = 0;
                        ed->firstline->htmb = 0;
                        ed->firstline->strb = 0;
                        ed->firstline->jvsb = 0;
                        ed->firstline->jvsc = 0;
                        ed->firstline->vbsb = 0;
                        ed->curline = ed->firstline;
                        ed->displline = ed->firstline;
                        ed->displcrds.y = 0;
                        ed->displcrds.x = 0;
                        ed->curcrds.x = 0;
                        ed->curcrds.y = 0;
                        ed->changef = 0;
                        ed->reloadf = 0;
                        ed->ostype = fileos;
                        ed->fname[0] = '\0';
                        ed->assoc = assocdef;
                        blck = 0;
                        }
                else    {
                        free( ed->undo );
                        edfirst = edfirst->next;
                        edfirst->prev = NULL;
                        ed = edfirst;
                        edcount -= 1;
                        free(edt);
                        }
                }
        else    {
                free( ed->undo );
                edcurno -= 1;
                ed->prev->next = ed->next;
                if (ed->next != NULL)
                        ed->next->prev = ed->prev;
                ed = ed->prev;
                edcount -= 1;
                free(edt);
                }
        edit_freelines( ln );
        cursor.x = ed->curcrds.x - ed->displcrds.x;
        cursor.y = ed->curcrds.y - ed->displcrds.y;
        bmrk_fclose( ecn );
        fmru_fclose( mru );
        fmru_setcur();
        return(-1);
        }



edit_new()  {
        int n;

        while (ed->next != NULL)  {
                edcurno += 1;
                ed = ed->next;
                }
        if( edcurno == CE_MAX_FILES )  {
                edit_err(6);
                return(0);
                }
        if ((ed->fname[0]) || (ed->changef))  {
                if ((ed->next = (struct editrec *)malloc(sizeof(struct editrec))) == NULL)  {
                        edit_err(1);
                        return(0);
                        }
                ed->next->prev = ed;
                ed = ed->next;
                ed->next = NULL;
                if ((ed->undo = (struct undorec *)malloc(sizeof(struct undorec)* CE_MAX_UNDO )) == NULL)  {
                        return(1);
                        }
                ed->undocnt = 0;
                ed->redocnt = 0;
                ed->undolns = 0;
                for( n = 0; n < CE_MAX_UNDO; ++n )  {
                        ed->undo[ n ].line = NULL;
                        ed->undo[ n ].cnt = 0;
                        }
                if( ! edit_newline( &ed->firstline ))  {
                        ed = ed->prev;
                        free(ed->next);
                        ed->next = NULL;
                        return(0);
                        }
                ed->curline = ed->firstline;
                ed->displline = ed->firstline;
                ed->displcrds.x = 0;
                ed->displcrds.y = 0;
                ed->curcrds.x = 0;
                ed->curcrds.y = 0;
                ed->nolines = 1;
                ed->changef = 0;
                ed->reloadf = 0;
                ed->ostype = fileos;
                ed->fname[0] = '\0';
#ifdef _WIN32
                ed->ftime.lo = 0;
                ed->ftime.hi = 0;
#else
                ed->ftime.date = 0;
                ed->ftime.time = 0;
#endif
                edcount += 1;
                edcurno += 1;
                ed->mru = edcurno;
                ed->assoc = assocdef;
#ifdef _WIN32
                ed->hTreeItem = 0;
#endif
                fmru_setcur();
                }
        cursor.x = ed->curcrds.x - ed->displcrds.x;
        cursor.y = ed->curcrds.y - ed->displcrds.y;
        return(-1);
        }



edit_get( struct linerec **line, int *linecnt, unsigned *linepos, int *lcrlf, char *fbufr, unsigned bufrlen )  {
        unsigned bufrpos;
        int d;
        static int oca = 0;

        if( *linecnt == 0 )  oca = 0;            /* reset oca only on first call to edit_get */
        if (! bufrlen)  return(0);
        bufrpos = 0;
        while (bufrpos != bufrlen)  {
                if (*linepos == (*line)->llen-1)  {
                        if( ! edit_sizeline( *line, *linepos + 1 ))  return(0);
                        }
                if (fbufr[bufrpos] == CE_KEY_CR)  {
                        *lcrlf |= 0x01;
                        if (++bufrpos == bufrlen)  break;
                        }
                if (fbufr[bufrpos] == CE_KEY_LF)  {
                        *lcrlf |= 0x02;
                        (*line)->strz[*linepos] = '\0';
                        if( ! edit_newline( &(*line)->next ))  {
                                ++(*linecnt);
                                return(0);
                                }
                        (*line)->next->prev = *line;
                        *line = (*line)->next;
                        *linepos = 0;
                        ++(*linecnt);
                        if (++bufrpos == bufrlen)  break;
                        }
                else if (fbufr[bufrpos] == CE_KEY_TAB)  {
                        ++bufrpos;
                        if ((d = (*linepos % filetab)) == 0)  d = filetab;  /* determine number of spaces to insert */
                        else  d = filetab - d;                             /* to next tab stop */
                        for (; d; --d)  {
                                (*line)->strz[(*linepos)++] = CE_KEY_SPACE;
                                if (*linepos == (*line)->llen-1)  {
                                        if( ! edit_sizeline( *line, *linepos + 1 ))  return(0);
                                        }
                                }
                        }
                else
                        (*line)->strz[(*linepos)++] = fbufr[bufrpos++];
                }
        return(-1);
        }



edit_newline( struct linerec **line )  {

        if(( *line = ( struct linerec * )malloc( sizeof( struct linerec ))) == NULL )  {
                edit_err(1);
                return(0);
                }
        (*line)->prev = NULL;
        (*line)->next = NULL;
        if(( (*line)->strz = ( char * )malloc( CE_LEN_LINE )) == NULL )  {
                edit_err(1);
                return(0);
                }
        (*line)->strz[0] = '\0';
        (*line)->llen = CE_LEN_LINE;
        /*if(( (*line)->atrs = ( struct atrsrec * )malloc( sizeof( struct atrsrec )* CE_MAX_CRMALINE )) == NULL )  {
                edit_err(1);
                return(0);
                }*/
        (*line)->atrs = NULL;
        (*line)->atrc = 0;
        (*line)->cmnt = 0;
        (*line)->aspb = 0;
        (*line)->jvsb = 0;
        (*line)->jvsc = 0;
        (*line)->vbsb = 0;
        (*line)->htmb = 0;
        (*line)->strb = 0;
        return(-1);
        }


/*edit_renewline( struct linerec **line )  {

        if(( *line = ( struct linerec *)realloc( *line, sizeof( struct linerec ) + (*line)->llen )) == NULL )  {
                edit_err(1);
                return(0);
                }
        (*line)->llen += CE_LEN_LINE;
        if( (*line)->prev != NULL )
                (*line)->prev->next = *line;
        if( (*line)->next != NULL )
                (*line)->next->prev = *line;
        return(-1);
        }*/


edit_sizeline( struct linerec *line, int len )  {       // !!!! Make Line smaller ??
        int n;

        len  += 1;      // Allow for str zero
        if( len > line->llen )  {
                n = len / CE_LEN_LINE + ( len % CE_LEN_LINE ? 1 : 0 );
                if(( line->strz = ( char * )realloc( line->strz, n * CE_LEN_LINE )) == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                line->llen = n * CE_LEN_LINE;
                }
        return(-1);
        }


edit_freeline( struct linerec *line )  {

        free( line->strz );
        free( line->atrs );
        free( line );
        }


edit_freelines( struct linerec *line )  {
        struct linerec *ld;

        while( line != NULL )  {
                ld = line;
                line = line->next;
                free( ld->strz );
                free( ld->atrs );
                free( ld );
                }
        }



edit_dellines( struct linerec *line, int linecnt )  {
        int n;
        struct linerec *lndel;

        ed->nolines -= linecnt;
        ed->curline = line;
        for (n = 0; n < linecnt; ++n)
                ed->curline = ed->curline->next;
        if (line == ed->firstline)  {
                if( ed->curline == NULL )  {
                        if( ! edit_newline( &ed->curline ))  return(0);
                        }
                ed->firstline = ed->curline;
                ed->displline = ed->curline;
                }
        else    {
                if( ed->curline == NULL )  {
                        if( ! edit_newline( &ed->curline ))  return(0);
                        ed->nolines += 1;
                        }
                line->prev->next = ed->curline;
                ed->curline->prev = line->prev;
                }
        for (n = 0; n < linecnt; ++n)  {
                if( line == ed->displline )
                        ed->displline = ed->curline;
                lndel = line;
                line = line->next;
                edit_freeline( lndel );
                }
        cmnd_change();
        bmrk_dellines( edcurno, ed->curcrds.y, linecnt );
        tree_dellines( ed->hTreeItem, ed->curcrds.y, linecnt );
        }



edit_cutlines( struct linerec *line, int linecnt )  {
        int n;
        struct linerec *ln = NULL;

        ed->nolines -= linecnt;
        ed->curline = line;
        for (n = 0; n < linecnt; ++n)  {
                crma_delline( ed->curline );
                ed->curline = ed->curline->next;
                }
        if (line == ed->firstline)  {
                if( ed->curline == NULL )  {
                        if( ! edit_newline( &ed->curline ))  return(0);
                        }
                ed->firstline = ed->curline;
                ed->displline = ed->curline;
                }
        else    {
                if( ed->curline == NULL )  {
                        if( ! edit_newline( &ed->curline ))  return(0);
                        ed->nolines += 1;
                        }
                line->prev->next = ed->curline;
                ed->curline->prev = line->prev;
                }
        line->prev = NULL;
        for (n = 0; n < linecnt; ++n)  {
                if( line == ed->displline )
                        ed->displline = ed->curline;
                ln = line;
                line = line->next;
                }
        if( ln )  ln->next = NULL;        // 2002/02/13  ln may be used without being initialized
        cmnd_change();
        bmrk_dellines( edcurno, ed->curcrds.y, linecnt );
        tree_dellines( ed->hTreeItem, ed->curcrds.y, linecnt );
        }


edit_inslines( struct linerec *line, int linecnt )  {
        struct linerec *ln1, *ln2;
        int n;
 
        ln1 = ln2 = line;
        for ( n = 0; n < linecnt - 1; ln2 = ln2->next )  // ?? Check for ln2 == NULL ??
                n += 1;
        if (ed->curline == ed->firstline)  {
                ln1->prev = NULL;
                ed->firstline = ln1;
                }
        else    {
                ln1->prev = ed->curline->prev;
                ed->curline->prev->next = ln1;
                }
        ed->curline->prev = ln2;
        ln2->next = ed->curline;
        if (ed->displline == ed->curline)
                ed->displline = ln1;
        ed->curline = ln1;
 
        ed->nolines += linecnt;

        if( ln1->prev != NULL )  {
                crma_insline( ln1->prev );
                crma_line( ln1->prev );
                }
        crma_insline( ln2 );
        ln2 = ln1;
        for( n = 0; n < linecnt; ++n )  {
                crma_line( ln2 );
                ln2 = ln2->next;
                }

        //ed->curcrds.x = 0;      ??
        cmnd_change();
        bmrk_inslines( edcurno, ed->curcrds.y, linecnt );
        tree_inslines( ed->hTreeItem, ed->curcrds.y, linecnt );
        }


edit_copylines( struct linerec **d, struct linerec *s, int linecnt )  {
        struct linerec *ln1, *ln2, *ln3;
        int n;

        n = 0;
        ln2 = NULL;
        ln3 = NULL;
        while (s != NULL)  {
                if( ! edit_newline( &ln1 ))  {
                        edit_freelines( ln2 );
                        ln2 = NULL;
                        return(0);
                        }
                if (ln2 == NULL)  {
                        ln2 = ln1;
                        ln3 = ln1;
                        ln3->prev = NULL;
                        }
                else    {
                        ln3->next = ln1;
                        ln3->next->prev = ln3;
                        ln3 = ln3->next;
                        }
                edit_copyline( ln3, s );
                s = s->next;
                if( ++n >= linecnt )
                        break;
                }
        if( ln3 )  ln3->next = NULL;        // 2002/02/13  ln3 may be used without being initialized
        *d = ln2;
        return(-1);
        }


edit_copyline( struct linerec *lnd, struct linerec *lns )  {

        if( ! edit_sizeline( lnd, strlen( lns->strz )))  return(0);
        strcpy( lnd->strz, lns->strz );
        if( lns->atrc )  {
                if(( lnd->atrs = ( struct atrsrec * )realloc( lnd->atrs, sizeof( struct atrsrec )* lns->atrc )) == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                memcpy( lnd->atrs, lns->atrs, sizeof( struct atrsrec )* lns->atrc );
                }
        else    {
                free( lnd->atrs );      // !!! check if this is really neccesary - it maybe always NULL anyway!!!
                lnd->atrs = NULL;
                }
        lnd->atrc = lns->atrc;
        return(-1);
        }


edit_splitline( struct linerec *lns, int x )  {
        struct linerec *lnn;

        strstb( lns );
        if( ! edit_newline( &lnn ))  return(0);
        if( ! edit_sizeline( lnn, strlen( &lns->strz[ x ] )))  return(0);
        strcpy( lnn->strz, &lns->strz[ x ] );
        lns->strz[ x ] = '\0';
        lnn->prev = lns;
        lnn->next = lns->next;
        if (lns->next != NULL)
                lns->next->prev = lnn;
        lns->next = lnn;
        ed->nolines += 1;
        crma_line( lns );
        crma_line( lnn );
        bmrk_inslines( edcurno, ed->curcrds.y + ( x ? 1 : 0 ), 1 );
        tree_inslines( ed->hTreeItem, ed->curcrds.y + ( x ? 1 : 0 ), 1 );
        return(-1);
        }


edit_joinline( struct linerec *ln, int x )  {
        int l, c;
        struct linerec *lnd;

        if( ln->next == NULL )  return(-1);
        l = strlen( ln->strz );
        if( ! edit_sizeline( ln, x + strlen( ln->next->strz )))  return(0);
        for( c = l; c < x; ++c )
                ln->strz[c] = CE_KEY_SPACE;
        strcpy( &ln->strz[ x ], ln->next->strz );

        lnd = ln->next;
        ln->next = ln->next->next;
        if( ln->next != NULL )
                ln->next->prev = ln;
        edit_freeline( lnd) ;
        ed->nolines -= 1;
        crma_line( ln );
        bmrk_dellines( edcurno, ed->curcrds.y, 1 );
        tree_dellines( ed->hTreeItem, ed->curcrds.y, 1 );
        }



edit_copystrn( struct linerec *ln, char *s, int c )  {

        if( ! edit_sizeline( ln, c ))  return(0);
        strncpy( ln->strz, s, c );
        ln->strz[ c ] = '\0';
        crma_line( ln );
        return(-1);
        }



edit_insspaces( struct linerec *ln, int x, int c )  {
        int l, n;

        l = strlen( ln->strz );
        if( ! edit_sizeline( ln, l + c ))  return(0);
        for( n = 0; n < c; ++n )
                strinsc( ln->strz, CE_KEY_SPACE, x++ );
        crma_line( ln );
        return(-1);
        }



edit_merge( struct linerec *line, int linecnt )  {

        if (ed->curline == ed->firstline)  {
                ed->firstline = line;
                ed->firstline->prev = NULL;
                while (line->next != NULL)
                        line = line->next;
                line->next = ed->curline;
                line->next->prev = line;
                ed->curline = ed->firstline;
                ed->displline = ed->curline;
                }
        else    {
                ed->curline->prev->next = line;
                line->prev = ed->curline->prev;
                while (line->next != NULL)
                        line = line->next;
                line->next = ed->curline;
                ed->curline = ed->curline->prev->next;
                line->next->prev = line;
                if (ed->displline == line->next)
                        ed->displline = ed->curline;
                }
        ed->nolines += linecnt;
        ed->displcrds.x = 0;
        ed->curcrds.x = 0;
        cursor.x = 0;
        cmnd_change();
        bmrk_inslines( edcurno, ed->curcrds.y, linecnt );
        tree_inslines( ed->hTreeItem, ed->curcrds.y, linecnt );
        }


edit_get_fname( int filenr, char *fname )  {
        int edno;
        struct editrec *edt;

        fname[ 0 ] = '\0';
        edno = 1;
        for( edt = edfirst; edt != NULL; ( edt = edt->next, edno += 1 ))  {
                if( edno == filenr )  {
                        strcpy( fname, edt->fname );
                        return(-1);
                        }
                }
        return( 0 );
        }


edit_get_curfno( )  {
        int edno;
        struct editrec *edt;

        edno = 1;
        for( edt = edfirst; edt != NULL; ( edt = edt->next, edno += 1 ))  {
                if( edt == ed )  {
                        return( edno );
                        }
                }
        return( 0 );
        }


edit_err( int errno )  {
        static char *errors[] =  {
                        "Oh Shit !",
                        "Memory Allocation Error",
                        "Line Overflow",
                        "File Write Error",
                        "Wildcard Filenames Not Allowed",
                        "Binary File",
                        "Maximum Files",
                        ""
                };

        edit_wait(0);
        dlg_err( errors[ errno ]);
        return(0);
        }


