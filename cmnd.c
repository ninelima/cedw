
#include "ced.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"


extern struct editrec *edfirst;
extern struct editrec *ed;
extern struct linerec *blckline;
extern struct coords cursor;
extern int insmode;
extern int edcurno;
extern int edcount;
extern int blck;
extern struct coords blckstr, blckend;
extern int scrn_x, scrn_y;
extern int tablen;
extern short frdonly;



cmnd_change()  {

        //TreeFuncChange( ed->hTreeItem );
        ed->changef = -1;
        }



cmnd_ins()  {
        insmode = (insmode) ? 0 : 1;
        vdeo_crs( insmode );
        stsl_insmode( insmode );
        }


cmnd_ctrlcr()  {
        if( strlen(ed->curline->strz))  ed->curcrds.x = strlen(ed->curline->strz);
        cmnd_cr();
        }


cmnd_shftcr()  {
        ed->curcrds.x = strlen(ed->curline->strz);
        cmnd_cr();
        }


cmnd_cr()  {
        int sc = 0, fc, x, up;
        struct linerec *ln;

        cmnd_change();
        if( up = blck )  blck_del( );
        x = ed->curcrds.x;
        strstb( ed->curline->strz );
        if( ! edit_newline( &ln ))  return(0);
        undo_cr( up );
        if(( fc = strchra( ed->curline->strz )) == -1)  {
                ed->curline->strz[0] = '\0';
                ln->strz[0] = '\0';
                fc = ed->curcrds.x;
                }
        else if( ed->curcrds.x <= fc )  {
                if( ! edit_sizeline( ln, strlen( ed->curline->strz )))  return(0);
                strcpy( ln->strz, ed->curline->strz );
                ed->curline->strz[0] = '\0';
                }
        else if( ed->curcrds.x < strlen(ed->curline->strz ))  {
                if( ! edit_sizeline( ln, fc + strlen( &ed->curline->strz[ ed->curcrds.x ])))  return(0);
                for( sc = 0; sc < fc; ++sc )
                        ln->strz[ sc ] = CE_KEY_SPACE;
                strcpy( &ln->strz[ fc ], &ed->curline->strz[ ed->curcrds.x ] );
                ed->curline->strz[ ed->curcrds.x ] = '\0';
                strstb( ed->curline->strz );
                }
        else
                ln->strz[0] = '\0';
        ln->prev = ed->curline;
        ln->next = ed->curline->next;
        if (ed->curline->next != NULL)
                ed->curline->next->prev = ln;
        ed->curline->next = ln;
        ed->nolines += 1;
        ed->curcrds.x = fc;
        if (cursor.y < scrn_y-1-1)  {
                cursor.y += 1;
                }
        else    {
                ed->displcrds.y += 1;
                ed->displline = ed->displline->next;
                }
        bmrk_inslines( edcurno, ed->curcrds.y + ( x ? 1 : 0 ), 1 );
        tree_inslines( ed->hTreeItem, ed->curcrds.y + ( x ? 1 : 0 ), 1 );
        ed->curcrds.y += 1;
        crma_insline( ed->curline );
        crma_line( ed->curline );
        crma_line( ln );
        ed->curline = ed->curline->next;
        scrn_adjust();
        }


cmnd_tab()  {
        int d, l, n, fr;

        cmnd_change();
        if ((d = (ed->curcrds.x % tablen)) == 0)    // determine number of spaces to insert
                d = tablen;                    // to next tab stop
        else
                d = tablen - d;
        l = strlen(ed->curline->strz);
        if (ed->curcrds.x < l)  {    // insert tab
                if( ! edit_sizeline( ed->curline, l + d ))  return(0);
                undo_tab( d );
                for( n = 0; n < d; ++n )
                        strinsc( ed->curline->strz, ' ', ( ed->curcrds.x )++ );
                }
        else    {               // tab at end-of-line
                if( ! edit_sizeline( ed->curline, ed->curcrds.x + d ))  return(0);
                for( ; l < ed->curcrds.x + d; ++l )       // pad line end with spaces upto the cursor
                        ed->curline->strz[l] = CE_KEY_SPACE;
                ed->curcrds.x += d;  // adjust cursor
                ed->curline->strz[ ed->curcrds.x ] = '\0';
                }
        fr = crma_line( ed->curline );
        if( ed->curcrds.x > ed->displcrds.x + ( scrn_x-1 ))
                scrn_adjust();
        else    {
                cursor.x = ed->curcrds.x - ed->displcrds.x;
                if( fr )  
                        scrn_rescrn();
                else    {
                        scrn_reline();
                        scrn_cursor();
                        }
                }
        }



cmnd_bs()  {
        int f, d, fr;
        struct linerec *ln;

        cmnd_change();
        strstb(ed->curline->strz);
        f = strchra(ed->curline->strz);
        if (ed->curcrds.x == 0)  {   // join with previous line
                if (ed->curcrds.y == 0)      // cannot if first line
                        edit_beep( );
                else    {
                        ln = ed->curline;
                        ed->curline = ed->curline->prev;        // move to prev. line
                        ed->curcrds.x = strlen( ed->curline->strz );
                        if( ! edit_sizeline( ed->curline, ed->curcrds.x + strlen( ln->strz )))  return(0);
                        strcpy( &ed->curline->strz[ ed->curcrds.x ], ln->strz );
                        ed->curline->next = ln->next;        // dispose of line
                        if( ln->next != NULL )
                                ed->curline->next->prev = ed->curline;
                        edit_freeline( ln );
                        ed->nolines -= 1;
                        ed->curcrds.y -= 1;
                        if (cursor.y != 0)      // adjust cursor and window
                                cursor.y -= 1;
                        else    {
                                ed->displcrds.y -= 1;
                                ed->displline = ed->displline->prev;
                                }
                        undo_bscr( );
                        crma_delline( ed->curline );
                        crma_line( ed->curline );
                        scrn_adjust();
                        bmrk_dellines( edcurno, ed->curcrds.y, 1 );
                        tree_dellines( ed->hTreeItem, ed->curcrds.y, 1 );
                        }
                }
        else if (ed->curcrds.x <= f || f == -1)  {   // delete to prev. tab stop
                if ((d = (ed->curcrds.x % tablen)) == 0)
                        d = tablen;
                ed->curcrds.x -= d;
                if (f != -1)
                        strdel(ed->curline->strz, ed->curcrds.x, d);
                undo_deltab( d );
                fr = crma_line( ed->curline );
                if( ed->curcrds.x < ed->displcrds.x )
                        scrn_adjust();
                else    {
                        cursor.x = ed->curcrds.x - ed->displcrds.x;
                        if( fr )
                                scrn_rescrn();
                        else    {
                                scrn_reline();
                                scrn_cursor();
                                }
                        }
                }
        else if (ed->curcrds.x <= strlen(ed->curline->strz))  {  // delete one char
                ed->curcrds.x -= 1;
                undo_bs();
                strdel(ed->curline->strz, ed->curcrds.x, 1);
                fr = crma_line( ed->curline );
                if (cursor.x > 0)  {
                        cursor.x -= 1;
                        if( fr )
                                scrn_rescrn();
                        else    {
                                scrn_cursor();
                                scrn_reline();
                                }
                        }
                else
                        scrn_adjust();
                }
        else    {
                ed->curcrds.x -= 1;
                if (cursor.x > 0)  {
                        cursor.x -= 1;
                        scrn_cursor();
                        scrn_reline();
                        }
                else
                        scrn_adjust();
                }
        /*else  {
                ed->curcrds.x = strstb(ed->curline->strz);       / delete all trailing blanks /
                scrn_adjust();
                }*/
        }


cmnd_del()  {
        int c, a, len;
        struct linerec *ln;

        cmnd_change();
        len = strstb( ed->curline->strz );
        if( ed->curcrds.x < len )  {  // delete char under cursor
                undo_del();
                strdel( ed->curline->strz, ed->curcrds.x, 1 );
                if( crma_line( ed->curline ))
                        scrn_rescrn();
                else    {
                        scrn_reline();
                        scrn_cursor();
                        }
                }
        else  if( ed->curcrds.y < ed->nolines - 1 )  {    // join lines if not the last line
                a = strchra( ed->curline->next->strz );
                if( ! edit_sizeline( ed->curline, ed->curcrds.x + strlen( ed->curline->next->strz ) - a ))  return(0);
                for( c = len; c < ed->curcrds.x; ++c )        // pad with spaces to cursor
                        ed->curline->strz[c] = CE_KEY_SPACE;
                strslb( ed->curline->next->strz );
                strcpy( &ed->curline->strz[ ed->curcrds.x ], ed->curline->next->strz );
                ln = ed->curline->next;      // dispose of line
                ed->curline->next = ed->curline->next->next;
                if( ed->curline->next != NULL )
                        ed->curline->next->prev = ed->curline;
                edit_freeline( ln );
                ed->nolines -= 1;
                undo_delcr( a );
                crma_delline( ed->curline );
                crma_line( ed->curline );
                scrn_rescrn();
                bmrk_dellines( edcurno, ed->curcrds.y, 1 );
                tree_dellines( ed->hTreeItem, ed->curcrds.y, 1 );
                }
        }


cmnd_del_line()  {
        struct linerec *ln;

        cmnd_change();
        if (ed->curcrds.y == ed->nolines - 1)  { // dont remove last line
                if (ed->curline->strz[0] == '\0')
                        edit_beep( );
                else    {
                        undo_cutlines( ed->curline, 1 );
                        ed->curline->strz[0] = '\0';
                        ed->curcrds.x = 0;
                        scrn_adjust();
                        }
                }
        else    {
                ln = ed->curline;
                ed->curline = ed->curline->next;
                if (ed->curcrds.y != 0)  {   // not first line to dispose of
                        ln->prev->next = ed->curline;
                        ed->curline->prev = ln->prev;
                        if( ln == ed->displline )
                                ed->displline = ed->curline;
                        }
                else    {       // dispose of first line
                        ed->firstline = ed->curline;
                        ed->curline->prev = NULL;
                        ed->displline = ed->curline;
                        }
                if( strstb( ln->strz ))  {       // line empty ?
                        if( ! undo_dellines( ln, 1 ))
                                edit_freeline( ln );
                        }
                else    {
                        ed->curcrds.x = 0;
                        undo_delcr( 0 );
                        edit_freeline( ln );
                        }
                ed->nolines -= 1;
                if ((ed->curcrds.x = strchra(ed->curline->strz)) == -1)      // move cursor to first char
                        ed->curcrds.x = 0;
                crma_delline( ed->curline );        // 2001/11/16
                crma_line( ed->curline );           // 2001/11/16
                scrn_adjust();
                bmrk_dellines( edcurno, ed->curcrds.y, 1 );
                tree_dellines( ed->hTreeItem, ed->curcrds.y, 1 );
                }
        }

/*
cmnd_del_line_right()  {
        ed->curline->strz[ed->curcrds.x] = '\0';
        scrn_reline();
        }


cmnd_del_line_left()  {
        int f;

        if (ed->curcrds.x != 0)  {   // cannot if at line beginning
                if ((f = strchra(ed->curline->strz)) == -1)  {      // line is empty
                        ed->curline->strz[0] = '\0';
                        ed->curcrds.x = 0;
                        }
                else if (ed->curcrds.x > f)  {       // delete all text left of cursor
                        if (ed->curcrds.x < strlen(ed->curline->strz))
                                strdel(ed->curline->strz, f, ed->curcrds.x - f);
                        else
                                ed->curline->strz[f] = '\0';
                        ed->curcrds.x = f;
                        }
                else    {       // delete all spaces left of cursor to line beginning
                        strdel(ed->curline->strz, 0, ed->curcrds.x);
                        ed->curcrds.x = 0;
                        }
                scrn_adjust();
                }
        }
*/



cmnd_right()  {

        if (cursor.x < scrn_x-2)  {
                ++cursor.x;
                }
        else    {
                ed->displcrds.x += 1;
                scrn_rescrn();
                }
        ed->curcrds.x += 1;
        scrn_cursor();
        }


cmnd_left()  {

        if (ed->curcrds.x > 0)  {
                if (cursor.x > 0)  {
                        --cursor.x;
                        }
                else    {
                        ed->displcrds.x -= 1;
                        scrn_rescrn();
                        }
                ed->curcrds.x -= 1;
                scrn_cursor();
                }
        }



//char f[] = { "~!@#$%^&*()+=-|\\;:'\">.<,/?]}[{" };

cmnd_word_left()  {
        int n;

        n = ed->curcrds.x;
        if (n > 0)  {
                --n;
                while (ed->curline->strz[n] == ' ')
                        if (--n == 0)  break;
                if (n != 0)  {
                        if( isalnumu( ed->curline->strz[ n ] ))  {
                                while( isalnumu( ed->curline->strz[ n ] ))  {
                                        if( --n == 0 )  break;
                                        }
                                if( n )  {
                                        ++n;
                                        }
                                else  if( ! isalnumu( ed->curline->strz[ n ] ))  {
                                        ++n;
                                        }
                                }
                        else    {
                                while( ! isalnumu( ed->curline->strz[ n ] ))  {
                                        if( --n == 0 )  break;
                                        if( ed->curline->strz[ n ] == ' ' )  break;
                                        }
                                if( n )  {
                                        ++n;
                                        }
                                else  if( isalnumu( ed->curline->strz[ n ] ))  {
                                        ++n;
                                        }
                                }
                        }
 
                ed->curcrds.x = n;
                if( ed->curcrds.x < ed->displcrds.x )  {
                        scrn_adjust();
                        }
                else    {
                        cursor.x = ed->curcrds.x - ed->displcrds.x;
                        scrn_reline();
                        scrn_cursor();
                        }
                }
        }


cmnd_word_right()  {
        int n, l;

        n = ed->curcrds.x;
        l = strlen(ed->curline->strz);
        if (n < l)  {
                if( ed->curline->strz[n] == ' ' )  {
                        while( ed->curline->strz[n] == ' ' )
                                if (++n == l)  break;
                        }
                else if( isalnumu( ed->curline->strz[ n ] ))  {
                    if( n < l )  {
                            while( isalnumu( ed->curline->strz[ n ] ))
                                    if( ++n == l)  break;
                            while( ed->curline->strz[n] == ' ' )
                                    if (++n == l)  break;
                            }
                    }
                else   {
                    if( n < l )  {
                            while( ! isalnumu( ed->curline->strz[ n ] ))  {
                                    if( ++n == l)  break;
                                    if( ed->curline->strz[ n ] == ' ' )  break;
                                    }
                            while( ed->curline->strz[n] == ' ' )
                                    if (++n == l)  break;
                            }
                    }
                ed->curcrds.x = n;
                if ((ed->curcrds.x < ed->displcrds.x) || (ed->curcrds.x > ed->displcrds.x + (scrn_x-1)))  {
                        scrn_adjust();
                        }
                else    {
                        cursor.x = ed->curcrds.x - ed->displcrds.x;
                        scrn_reline();
                        scrn_cursor();
                        }
                }
        }


cmnd_up()  {

        strstb(ed->curline->strz);
        if (ed->curcrds.y != 0)  {
                if (cursor.y != 0)  {
                        cursor.y -= 1;
                        ed->curcrds.y -= 1;
                        ed->curline = ed->curline->prev;
                        scrn_cursor();
                        }
                else    {
                        ed->displcrds.y -= 1;
                        ed->curcrds.y -= 1;
                        ed->curline = ed->curline->prev;
                        ed->displline = ed->displline->prev;
                        scrn_rescrn();
                        }
                }
        }


cmnd_down()  {

        strstb(ed->curline->strz);  /* SC ???? ed->curcrds*/
        if (ed->curcrds.y == ed->nolines - 1)  {
                if ((strchra(ed->curline->strz) != -1) && (ed->changef != 2))  {
                        if( ! edit_newline( &ed->curline->next )) {
                                blck_unmark();          // in case called from blck_down
                                return(0);
                                }
                        ed->nolines += 1;
                        ed->curline->next->prev = ed->curline;
                        }
                }
        if (ed->curcrds.y < ed->nolines - 1)  {
                if (cursor.y < scrn_y-1-1)  {   // ?? !!
                        cursor.y += 1;
                        ed->curcrds.y += 1;
                        ed->curline = ed->curline->next;
                        scrn_cursor();
                        }
                else    {
                        ed->displcrds.y += 1;
                        ed->curcrds.y += 1;
                        ed->curline = ed->curline->next;
                        ed->displline = ed->displline->next;
                        scrn_rescrn();
                        }
                }
        }


cmnd_scroll_down()  {

        strstb(ed->curline->strz);  /* SC ???? ed->curcrds */
        if (ed->curcrds.y == ed->nolines - 1)  {
                if ((strchra(ed->curline->strz) != -1) && (ed->changef != 2))  {
                        if( ! edit_newline( &ed->curline->next ))
                                return(0);
                        ed->nolines += 1;
                        ed->curline->next->prev = ed->curline;
                        }
                }
        if (ed->curcrds.y < ed->nolines - 1)  {
                ed->displcrds.y += 1;
                ed->curcrds.y += 1;
                ed->curline = ed->curline->next;
                ed->displline = ed->displline->next;
                scrn_rescrn();
                }
        }


cmnd_scroll_up()  {

        strstb(ed->curline->strz);
        if( ed->displline != ed->firstline )  {
                ed->displcrds.y -= 1;
                ed->curcrds.y -= 1;
                ed->curline = ed->curline->prev;
                ed->displline = ed->displline->prev;
                scrn_rescrn();
                }
        }



cmnd_page_up()  {
        int td, i;

        strstb(ed->curline->strz);  /* SC ed->curcrds*/
        if (ed->displcrds.y != 0)  {
                if (ed->displcrds.y >= scrn_y-1)  {
                        for (i = 0; i < scrn_y-1; ++i)  {
                                ed->displline = ed->displline->prev;
                                ed->curline = ed->curline->prev;
                                }
                        ed->curcrds.y -= scrn_y-1;
                        ed->displcrds.y -= scrn_y-1;
                        }
                else    {
                        td = ed->displcrds.y;
                        ed->displcrds.y = 0;
                        ed->displline = ed->firstline;
                        if (ed->curcrds.y >= scrn_y-1)  {
                                for (i = 0; i < scrn_y-1; ++i)
                                        ed->curline = ed->curline->prev;
                                ed->curcrds.y -= scrn_y-1;
                                cursor.y -= scrn_y-1 - td;
                                }
                        else
                                cursor.y += td;
                        }
                scrn_rescrn();
                }
        }


cmnd_page_down()  {
        int ti, i;

        strstb(ed->curline->strz);
        if (ed->displcrds.y + scrn_y-1 < ed->nolines)  {
                ti = ed->nolines - (ed->displcrds.y + scrn_y-1);
                if (ti > scrn_y-1)  {
                        for (i = 0; i < scrn_y-1; i++)  {
                                ed->displline = ed->displline->next;
                                ed->curline = ed->curline->next;
                                }
                        ed->curcrds.y += scrn_y-1;
                        ed->displcrds.y += scrn_y-1;
                        }
                else    {
                        for (i = 0; i < ti; ++i)
                                ed->displline = ed->displline->next;
                        ed->displcrds.y += ti;
                        if (ed->curcrds.y + scrn_y-1 < ed->nolines)  {
                                for (i = 0; i < scrn_y-1; ++i)
                                        ed->curline = ed->curline->next;
                                ed->curcrds.y += scrn_y-1;
                                cursor.y += scrn_y-1 - ti;
                                }
                        else
                                cursor.y -= ti;
                        }
                scrn_rescrn();
                }
        }


cmnd_file_home()  {

        ed->curcrds.x = 0;
        ed->curcrds.y = 0;
        ed->curline = ed->firstline;
        ed->displcrds.x = 0;
        ed->displcrds.y = 0;
        ed->displline = ed->firstline;
        cursor.x = 0;
        cursor.y = 0;
        scrn_rescrn();
        }


cmnd_file_end()  {

        strstb(ed->curline->strz);
        if (ed->displcrds.y + scrn_y-1 < ed->nolines)  {
                for (; ed->displcrds.y + scrn_y-1 < ed->nolines; ed->displcrds.y += 1)
                        ed->displline = ed->displline->next;
                cursor.y = scrn_y-1-1;
                }
        else
                cursor.y = ed->nolines-1 - ed->displcrds.y;
        for (; ed->curcrds.y < ed->nolines-1; ed->curcrds.y += 1)
                ed->curline = ed->curline->next;
        if (strchra(ed->curline->strz) != -1)
                cmnd_down();
        ed->displcrds.x = 0;
        cursor.x = 0;
        ed->curcrds.x = 0;
        scrn_rescrn();
        }


cmnd_line_home()  {
        int f;

        if ((f = strchra(ed->curline->strz)) == -1)
                ed->curcrds.x = 0;
        else if (ed->curcrds.x == f)
                ed->curcrds.x = 0;
        else ed->curcrds.x = f;
        if( ed->curcrds.x < ed->displcrds.x )  {
                scrn_adjust();
                }
        else    {
                cursor.x = ed->curcrds.x - ed->displcrds.x;
                scrn_reline();
                scrn_cursor();
                }
        }


cmnd_line_end()  {

        ed->curcrds.x = strlen(ed->curline->strz);
        if ((ed->curcrds.x < ed->displcrds.x )||( ed->curcrds.x > ed->displcrds.x + (scrn_x-2)))  {
                scrn_adjust();
                }
        else    {
                cursor.x = ed->curcrds.x - ed->displcrds.x;
                scrn_reline();
                scrn_cursor();
                }
        }



cmnd_scroll_right()  {

        ed->displcrds.x += 1;
        scrn_rescrn();
        ed->curcrds.x += 1;
        scrn_cursor();
        }


cmnd_page_right()  {
        int n;

        for( n = 0; n < scrn_x - 1; ++n )  {
                ed->displcrds.x += 1;
                ed->curcrds.x += 1;
                }
        scrn_rescrn();
        scrn_cursor();
        }


cmnd_scroll_left()  {

        if( ed->displcrds.x > 0 )  {
                ed->displcrds.x -= 1;
                scrn_rescrn();
                ed->curcrds.x -= 1;
                scrn_cursor();
                }
        }


cmnd_page_left()  {
        int n;
        
        for( n = 0; n < scrn_x - 1; ++n )  {
                if( ed->displcrds.x == 0 )  break;
                ed->displcrds.x -= 1;
                ed->curcrds.x -= 1;
                }
        scrn_rescrn();
        scrn_cursor();
        }




cmnd_char( int ichr )  {
        int c, p, len, fr;

        cmnd_change();
        if( p = blck )  blck_del( );
        len = strlen( ed->curline->strz );
        if (ed->curcrds.x >= len)  {
                if( ! edit_sizeline( ed->curline, ed->curcrds.x + 1 ))  return(0);
                for( c = len; c < ed->curcrds.x; ++c )
                        ed->curline->strz[ c ] = CE_KEY_SPACE;
                ed->curline->strz[ ed->curcrds.x ] = ichr;
                ed->curline->strz[ ed->curcrds.x + 1 ] = '\0';
                if( ichr != CE_KEY_SPACE )  undo_char( ichr, p );
                }
        else    {
                if (insmode)  {
                        undo_char( ichr, p );
                        if( ! edit_sizeline( ed->curline, len + 1 ))  return(0);
                        strinsc( ed->curline->strz, ichr, ed->curcrds.x );
                        }
                else    {
                        undo_charovr( ichr, p );
                        ed->curline->strz[ ed->curcrds.x ] = ichr;
                        }
                }
        fr = crma_line( ed->curline );
        ed->curcrds.x += 1;
        if (cursor.x < scrn_x-1)  {
                ++cursor.x;
                if( fr )
                        scrn_rescrn();
                else    {
                        scrn_cursor();
                        scrn_reline();
                        }
                }
        else    {
                ed->displcrds.x += 1;
                scrn_rescrn();
                }
        }


cmnd_file_next()  {
        if (ed->next != NULL)  {
                edcurno += 1;
                ed = ed->next;
                cursor.x = ed->curcrds.x - ed->displcrds.x;
                cursor.y = ed->curcrds.y - ed->displcrds.y;
                }
        else    {
                edit_new();
                }
        scrn_statsl();
        scrn_rescrn();
        file_2reload();
        }


cmnd_file_prev()  {
        if (ed->prev != NULL)  {
                edcurno -= 1;
                ed = ed->prev;
                cursor.x = ed->curcrds.x - ed->displcrds.x;
                cursor.y = ed->curcrds.y - ed->displcrds.y;
                scrn_statsl();
                scrn_rescrn();
                }
        file_2reload();
        }


cmnd_file_last()  {
        while (ed->next != NULL)  {
                edcurno += 1;
                ed = ed->next;
                }
        edit_new();
        cursor.x = ed->curcrds.x - ed->displcrds.x;
        cursor.y = ed->curcrds.y - ed->displcrds.y;
        scrn_statsl();
        scrn_rescrn();
        fmru_setcur();
        file_2reload();
        }


cmnd_file_first()  {
        edcurno = 1;
        ed = edfirst;
        cursor.x = ed->curcrds.x - ed->displcrds.x;
        cursor.y = ed->curcrds.y - ed->displcrds.y;
        scrn_statsl();
        scrn_rescrn();
        fmru_setcur();
        file_2reload();
        }



cmnd_file_select( struct editrec *edn )  {
        int edtmp;
        struct editrec *edt;
        
        blck_unmark();
        edtmp = 1;
        for( edt = edfirst; edt != edn; ( edt = edt->next, edtmp += 1 ))  ;
        ed = edn;
        edcurno = edtmp;
        cursor.x = ed->curcrds.x - ed->displcrds.x;
        cursor.y = ed->curcrds.y - ed->displcrds.y;
        scrn_statsl();
        scrn_rescrn();
        fmru_setcur();
        file_2reload();
        }


cmnd_goto_displ( int linenr )  {
        int linec;
        struct linerec *ln;

        ln = ed->firstline;
        linenr -= 1;
        for( linec = 0; linec < linenr; ++linec )  {
                if( ln->next == NULL )
                        break;
                ln = ln->next;
                }
        ed->displline = ln;
        ed->displcrds.y = linec;
        if (ed->curcrds.y < ed->displcrds.y)  {
                ed->curline = ed->displline;
                ed->curcrds.y = ed->displcrds.y;
                }
        else if((ed->curcrds.y - ed->displcrds.y) > scrn_y-2)  {
                ed->curline = ed->displline;
                ed->curcrds.y = ed->displcrds.y;
                for( linec = 0; linec < scrn_y - 2; ++linec )  {
                        if( ed->curline->next == NULL )
                                break;
                        ed->curline = ed->curline->next;
                        ed->curcrds.y += 1;
                        }
                }
        cursor.y = ed->curcrds.y - ed->displcrds.y;
        scrn_rescrn();
        }



cmnd_goto_dispx( int x )  {

        ed->displcrds.x = x;
        ed->curcrds.x = x + cursor.x;
        scrn_rescrn();
        scrn_cursor();
        }



cmnd_goto_line( int linenr, int linepos )  {
        int linec;
        struct linerec *ln;

        ed->curline = ed->firstline;
        ln = ed->curline;
        linenr -= 1;
        for( linec = 0; linec < linenr; ++linec )  {
                if( ln->next == NULL )  {
                        linepos = 0;
                        break;
                        }
                ln = ln->next;
                }
        ed->curline = ln;
        ed->curcrds.x = linepos;
        ed->curcrds.y = linec;
        //ed->displline = ed->firstline;    ???????
        ed->displcrds.x = 0;
        //ed->displcrds.y = 0;
        //cursor.x = 0;
        cursor.y = 0;

        scrn_offset();
        scrn_statsl();
        scrn_adjust();
        }


cmnd_goto_file( int filenr, int linenr, int linepos )  {
        int edittmp;
        struct editrec *edt;

        edittmp = 1;
        for( edt = edfirst; edt != NULL; ( edt = edt->next, edittmp += 1 ))  {
                if( edittmp == filenr )  {
                        ed = edt;
                        edcurno = edittmp;
                        cursor.x = ed->curcrds.x - ed->displcrds.x;
                        cursor.y = ed->curcrds.y - ed->displcrds.y;
                        if( linenr )  cmnd_goto_line( linenr, linepos );
                        else    {
                                scrn_statsl();
                                scrn_rescrn();
                                }
                        fmru_setcur();
                        file_2reload();
                        return( -1 );
                        }
                }
        return( 0 );
        }



fmru_setcur( )  {
        struct editrec *edt;

        for( edt = edfirst; edt != NULL; edt = edt->next )  {
                if( edt->mru < ed->mru )
                        edt->mru += 1;
                }
        ed->mru = 1;
        }


fmru_goto( )  {
        int edtmp;
        struct editrec *edt;

        edtmp = 1;
        for( edt = edfirst; edt != NULL; ( edt = edt->next, edtmp += 1 ))  {
                if( edt->mru == ( ed->mru == edcount ? 1 : ed->mru + 1 ))  {
                        ed = edt;
                        edcurno = edtmp;
                        cursor.x = ed->curcrds.x - ed->displcrds.x;
                        cursor.y = ed->curcrds.y - ed->displcrds.y;
                        scrn_statsl();
                        scrn_rescrn();
                        file_2reload();
                        return( -1 );
                        }
                }
        }


int fmru_getfno( int mru )  {
        int edtmp;
        struct editrec *edt;

        edtmp = 1;
        for( edt = edfirst; edt != NULL; ( edt = edt->next, edtmp += 1 ))  {
                if( mru == edt->mru )  {
                        return( edtmp );
                        }
                }
        return( 0 );
        }


fmru_fclose( int mru )  {
        int edtmp;
        struct editrec *edt;

        edtmp = 1;
        for( edt = edfirst; edt != NULL; ( edt = edt->next, edtmp += 1 ))  {
                if( mru < edt->mru )  {
                        edt->mru -= 1;
                        }
                }
        return( 0 );
        }
 
