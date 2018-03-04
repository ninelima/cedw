
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "ced.h"

extern struct editrec *ed;
extern struct coords cursor;
extern int insmode;
extern int edcurno;
extern int edcount;

int clpbcnt = 0;
struct clpbrec clpbord[ 10 ];


extern int scrn_x, scrn_y;
extern int tablen;
extern int undof;


int blck = 0;
struct linerec *blckline = NULL;
struct coords blckstr, blckend;


 
/*clpb_ins()  {
        int n;

        if( clpbcnt >= CE_MAX_CLPBORD )
                edit_freelines( clpbord[ CE_MAX_CLPBORD - 1 ].line );
        else
                clpbcnt += 1;
        for( n = clpbcnt - 1; n > 0; --n )
                memcpy( &clpbord[ n ], &clpbord[ n - 1 ], sizeof( struct clpbrec ));
        if( ! edit_newline( &clpbord[ 0 ].line ))
                return(0);
        clpbord[ 0 ].type = 0;
        return(-1);
        }*/


clpb_ini()  {
        int n;

        for( n = 0; n < 10; ++n )  {
                if( ! edit_newline( &clpbord[ n ].line ))
                        return(1);
                }
        return(0);
        }


clpb_freeall()  {
        int n;

        for( n = 0; n < 10; ++n )  {
                clpb_free( n );
                }
        }


clpb_free( int clpbnr )  {

        edit_freelines( clpbord[ clpbnr ].line->next );
        clpbord[ clpbnr ].line->next = NULL;
        clpbord[ clpbnr ].line->strz[ 0 ] = '\0';
        clpbord[ clpbnr ].type = 0;
        }



blck_unmark()  {
        if( blck )  {
                blck = 0;
                scrn_rescrn();
                }
        }


blck_beg()  {
        blck = 0;
        blckstr.x = ed->curcrds.x;
        blckstr.y = ed->curcrds.y;
        blckend.x = ed->curcrds.x;
        blckend.y = ed->curcrds.y;
        blckline = ed->curline;
        }


blck_end()  {
        blckend.x = ed->curcrds.x;
        blckend.y = ed->curcrds.y;
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  {
                blck = 0;
                }
        else    {
                if(( blckend.x >= blckstr.x )&&( blckend.y >= blckstr.y ))  {
                        blck = -1;
                        scrn_rescrn();
                        }
                }
        }


blck_line_home( ) {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.x = ed->curcrds.x;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        cmnd_line_home();
        if (ed->curcrds.x > blckstr.x)
                blckend.x = ed->curcrds.x;
        else
                blckstr.x = ed->curcrds.x;
        if (blckstr.y != blckend.y)  scrn_rescrn();
        else  scrn_reline();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }


blck_line_end() {
        if (! blck)  {
                /*blck = -1;*/
                blckstr.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.x = ed->curcrds.x;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        cmnd_line_end();
        if (! blck)
                blck = -1;
        if (ed->curcrds.x < blckend.x)
                blckstr.x = ed->curcrds.x;
        else
                blckend.x = ed->curcrds.x;
        if (blckstr.y != blckend.y)  scrn_rescrn();
        else  scrn_reline();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }


blck_word_right() {

        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        cmnd_word_right();
        if (ed->curcrds.x <= blckend.x)
                blckstr.x = ed->curcrds.x;
        else
                blckend.x = ed->curcrds.x;
        if (blckstr.y != blckend.y)  scrn_rescrn();
        else  scrn_reline();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }



blck_word_left()  {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.x = ed->curcrds.x;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        cmnd_word_left();
        if (ed->curcrds.x > blckstr.x)
                blckend.x = ed->curcrds.x;
        else
                blckstr.x = ed->curcrds.x;
        if (blckstr.y != blckend.y)  scrn_rescrn();
        else  scrn_reline();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }



blck_down() {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        if ((ed->curcrds.y < ed->nolines - 1)||((ed->curcrds.y == ed->nolines - 1)&&(strchra(ed->curline->strz) != -1)))  {
                if (ed->curcrds.y+1 <= blckend.y)  {
                        blckstr.y = ed->curcrds.y + 1;
                        blckline = ed->curline->next;
                        }
                else    {
                        blckend.y = ed->curcrds.y + 1;
                        }
                scrn_reline();
                cmnd_down();
                scrn_reline();
                }
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }


blck_up() {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        if (ed->curcrds.y > 0)  {
                if (ed->curcrds.y-1 >= blckstr.y)
                        blckend.y = ed->curcrds.y - 1;
                else    {
                        blckstr.y = ed->curcrds.y - 1;
                        blckline = ed->curline->prev;
                        }
                scrn_reline();
                cmnd_up();
                scrn_reline();
                }
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }


blck_left() {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        if (ed->curcrds.x > 0)  {
                cmnd_left();
                if (ed->curcrds.x >= blckstr.x)
                        blckend.x = ed->curcrds.x;
                else
                        blckstr.x = ed->curcrds.x;
                /*if (blckstr.x == blckend.x)
                        blck = 0;*/
                if (blckstr.y != blckend.y)  scrn_rescrn();
                else  scrn_reline();
                }
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }


blck_leftn( int n ) {
        int c;
 
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        if (ed->curcrds.x > 0)  {
                for( c = 0; c < n; ++c )  {
                        cmnd_left();
                        if (ed->curcrds.x >= blckstr.x)
                                blckend.x = ed->curcrds.x;
                        else
                                blckstr.x = ed->curcrds.x;
                        }
                /*if (blckstr.x == blckend.x)
                        blck = 0;*/
                if (blckstr.y != blckend.y)  scrn_rescrn();
                else  scrn_reline();
                }
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }



blck_right() {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        cmnd_right();
        if (ed->curcrds.x <= blckend.x)
                blckstr.x = ed->curcrds.x;
        else
                blckend.x = ed->curcrds.x;
        if (blckstr.y != blckend.y)  scrn_rescrn();
        else  scrn_reline();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }


blck_rightn( int n ) {
        int c;

        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        for( c = 0; c < n; ++c )  {
                cmnd_right();
                if (ed->curcrds.x <= blckend.x)
                        blckstr.x = ed->curcrds.x;
                else
                        blckend.x = ed->curcrds.x;
                }
        if (blckstr.y != blckend.y)  scrn_rescrn();
        else  scrn_reline();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }



blck_page_up() {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        cmnd_page_up();
        if (ed->curcrds.y > blckstr.y)
                blckend.y = ed->curcrds.y;
        else    {
                blckstr.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        scrn_rescrn();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }


blck_page_down()  {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        cmnd_page_down();
        if (ed->curcrds.y < blckend.y)  {
                blckstr.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        else    {
                blckend.y = ed->curcrds.y;
                }
        scrn_rescrn();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }


blck_file_home() {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        cmnd_file_home();
        if (ed->curcrds.y > blckstr.y)
                blckend.y = ed->curcrds.y;
        else    {
                blckstr.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        scrn_rescrn();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }



blck_file_end()  {
        if (! blck)  {
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x;
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        cmnd_file_end();
        if (ed->curcrds.y < blckend.y)  {
                blckstr.y = ed->curcrds.y;
                blckline = ed->curline;
                }
        else    {
                blckend.y = ed->curcrds.y;
                }
        scrn_rescrn();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }


blck_all()  {
        blck = -1;
        cmnd_file_end();
        blckend.x = ed->curcrds.x;
        blckend.y = ed->curcrds.y;
        cmnd_file_home();
        blckstr.x = ed->curcrds.x;
        blckstr.y = ed->curcrds.y;
        blckline = ed->curline;
        scrn_rescrn();
        if(( blckstr.x == blckend.x )&&( blckstr.y == blckend.y ))  blck = 0;
        }



blck_tab()  {
        int d, l;
        struct linerec *ln;

        if(( ! blck )||( blckstr.x != blckend.x ))  return(0);

        ln = blckline;
        for( d = 0; d + blckstr.y < blckend.y; ++d )  {
                l = strlen( ln->strz );
                if( ! edit_sizeline( ln, l + tablen ))  return(0);
                for( ; l >= 0; --l )
                        ln->strz[ l + tablen ] = ln->strz[ l ];
                for( l = 0; l < tablen; ++l )
                        ln->strz[l] = CE_KEY_SPACE;
                crma_line( ln );
                if(( ln = ln->next ) == NULL )
                        break;          // just in case
                }
        cmnd_change();
        undo_blcktab( blckstr.y, d );
        scrn_adjust();
        return(-1);
        }


blck_revtab()  {
        int d, l;
        struct linerec *ln;

        if(( ! blck )||( blckstr.x != blckend.x ))  return(0);

        ln = blckline;
        for (d = 0; d + blckstr.y < blckend.y; ++d)  {
                l = strchra(ln->strz);
                if ((l >= 0 )&&(l < tablen))
                        return(0);
                if ((ln = ln->next) == NULL)
                        break;          // just in case
                }
        ln = blckline;
        for (d = 0; d + blckstr.y < blckend.y; ++d)  {
                if (strchra(ln->strz) >= tablen)
                        strdel(ln->strz, 0, tablen);
                crma_line( ln );
                if ((ln = ln->next) == NULL)
                        break;          // just in case
                }
        undo_blckrevtab( blckstr.y, d );
        cmnd_change();
        scrn_adjust();
        return(-1);
        }


blck_space()  {
        int d, l, k;
        struct linerec *ln;

        if(( ! blck )||( blckstr.y == blckend.y ))   return(0);
        ln = blckline;
        for (d = 0; d + blckstr.y < blckend.y; ++d)  {
                l = strlen(ln->strz);
                k = 0;
                if( blckstr.x != blckend.x )  k = blckstr.x;
                if( ! edit_sizeline( ln, l + 1 ))  return(0);
                for( ; l >= k; --l )
                        ln->strz[ l+1 ] = ln->strz[ l ];
                ln->strz[ k ] = CE_KEY_SPACE;
                crma_line( ln );
                if(( ln = ln->next ) == NULL )
                        break;          // just in case
                }
        undo_blckspace( blckstr.x == blckend.x ? 0 : blckstr.x, blckstr.y, d );
        cmnd_change();
        scrn_adjust();
        return(-1);
        }


blck_revspace()  {
        int d, l, k;
        struct linerec *ln;

        if(( ! blck )||( blckstr.y == blckend.y ))   return(0);
        ln = blckline;
        for (d = 0; d + blckstr.y < blckend.y; ++d)  {
                l = strlen(ln->strz);
                k = 0;
                if( blckstr.x != blckend.x )  k = blckstr.x;
                if( k < l )  {
                        if( ln->strz[ k ] != CE_KEY_SPACE )
                                return(0);
                        }
                if ((ln = ln->next) == NULL)
                        break;          // just in case
                }
        ln = blckline;
        for (d = 0; d + blckstr.y < blckend.y; ++d)  {
                l = strlen(ln->strz);
                k = 0;
                if( blckstr.x != blckend.x )  k = blckstr.x;
                if( k < l )
                        strdel( ln->strz, k, 1 );
                crma_line( ln );
                if ((ln = ln->next) == NULL)
                        break;          // just in case
                }
        undo_blckrevspace( blckstr.x == blckend.x ? 0 : blckstr.x, blckstr.y, d );
        cmnd_change();
        scrn_adjust();
        return(-1);
        }


blck_del()  {
        int c, k, len, be;
        struct linerec *ln;

        if (! blck)  return(0);
        if (blckstr.y == blckend.y)  {
                len = strstb(ed->curline->strz);
                ed->curcrds.x = blckstr.x;
                if (blckstr.x < len)  {
                        be = (blckend.x >= len ? len : blckend.x);
                        //if( undof )  //undo_delchrs( ed->curline->strz, blckstr.x, be - blckstr.x );
                        undo_ovrlines( ed->curline, 1, ed->curcrds.y, 0 );
                        strdel(ed->curline->strz, blckstr.x, be - blckstr.x);
                        crma_line( ed->curline );
                        }
                }
        else  if (blckstr.x == blckend.x)  {
                k = blckend.y - blckstr.y;
                ed->nolines -= k;
                ed->displcrds.x = 0;
                ed->curcrds.x = 0;
                cursor.x = 0;
                ed->curcrds.y = blckstr.y;
                ed->curline = blckline;
                for (c = 0; c < k; ++c)  {
                        crma_delline( ed->curline );
                        ed->curline = ed->curline->next;
                        }
                if (blckstr.y <= ed->displcrds.y)  {
                        ed->displline = ed->curline;
                        ed->displcrds.y = blckstr.y;
                        cursor.y = 0;
                        }
                else    {
                        cursor.y = ed->curcrds.y - ed->displcrds.y;
                        }
                if (blckline == ed->firstline)  {
                        ed->firstline = ed->curline;
                        ed->curline->prev = NULL;
                        ed->displline = ed->curline;
                        }
                else    {
                        blckline->prev->next = ed->curline;
                        ed->curline->prev->next = NULL;
                        ed->curline->prev = blckline->prev;
                        }
                if( undof )  // Fix !!!
                        undo_dellines( blckline, k );
                else    {
                        for (c = 0; c < k; ++c)  {
                                ln = blckline;
                                blckline = blckline->next;
                                edit_freeline(ln);
                                }
                        }
                bmrk_dellines( edcurno, ed->curcrds.y, k );
                tree_dellines( ed->hTreeItem, ed->curcrds.y, k );
                }
        else    {
                k = blckend.y - blckstr.y;
                undo_ovrlines( blckline, k, blckstr.y, 0 );
                ln = blckline;
                for (c = 0; c < k; ++c)  {
                        len = strstb(ln->strz);
                        if (blckstr.x < len)  {
                                be = (blckend.x >= len ? len : blckend.x);
                                strdel(ln->strz, blckstr.x, be - blckstr.x);
                                }
                        crma_line( ln );
                        ln = ln->next;
                        }
                ed->curcrds.x = blckstr.x;
                }
        blck = 0;
        cmnd_change();
        scrn_adjust();
        return(-1);
        }


blck_cut( int clpbnr )  {
        int c, k, len, be;
        struct linerec *lnclpb, *lnblck;

        if (! blck)  return(0);
        clpb_free( clpbnr );
        if (blckstr.y == blckend.y)  {
                clpbord[ clpbnr ].type = 2;
                len = strstb(ed->curline->strz);
                ed->curcrds.x = blckstr.x;
                if (blckstr.x < len)  {
                        if (blckend.x >= len)  blckend.x = len;
                        //if( undof )  //undo_delchrs( ed->curline->strz, blckstr.x, blckend.x - blckstr.x );
                        undo_ovrlines( ed->curline, 1, ed->curcrds.y, 0 );
                        if( ! edit_sizeline( clpbord[ clpbnr ].line, blckend.x - blckstr.x ))  return(0);
                        strcut(clpbord[ clpbnr ].line->strz, ed->curline->strz, blckstr.x, blckend.x - blckstr.x);
                        crma_line( ed->curline );
                        }
                }
        else  if (blckstr.x == blckend.x)  {           // !!! ??? Replace vvvvvvv with edit_cutlines ???
                clpbord[ clpbnr ].type = 1;
                k = blckend.y - blckstr.y;
                ed->nolines -= k;
                ed->curline = blckline;
                for (c = 0; c < k; ++c)  {
                        crma_delline( ed->curline );
                        ed->curline = ed->curline->next;
                        }
                ed->displcrds.x = 0;
                ed->curcrds.x = 0;
                cursor.x = 0;
                ed->curcrds.y = blckstr.y;
                if (blckstr.y < ed->displcrds.y)  {
                        ed->displline = ed->curline;
                        ed->displcrds.y = blckstr.y;
                        cursor.y = 0;
                        }
                else    {
                        cursor.y = ed->curcrds.y - ed->displcrds.y;
                        }
                if (blckline == ed->firstline)  {
                        ed->firstline = ed->curline;
                        ed->curline->prev->next = NULL;
                        ed->curline->prev = NULL;
                        ed->displline = ed->curline;
                        }
                else    {
                        blckline->prev->next = ed->curline;
                        ed->curline->prev->next = NULL;
                        ed->curline->prev = blckline->prev;
                        }
                clpbord[ clpbnr ].line = blckline;
                undo_cutlines( blckline, k );
                bmrk_dellines( edcurno, ed->curcrds.y, k );
                tree_dellines( ed->hTreeItem, ed->curcrds.y, k );
                }
        else    {
                clpbord[ clpbnr ].type = 1;
                k = blckend.y - blckstr.y;
                undo_ovrlines( blckline, k, blckstr.y, 0 );
                lnclpb = clpbord[ clpbnr ].line;
                lnblck = blckline;
                len = strstb(lnblck->strz);
                if (blckstr.x < len)  {
                        be = (blckend.x >= len ? len : blckend.x);
                        if( ! edit_sizeline( lnclpb, be - blckstr.x ))  return(0);
                        strcut(lnclpb->strz, lnblck->strz, blckstr.x, be - blckstr.x);
                        lnclpb->strz[be - blckstr.x] = '\0';
                        }
                crma_line( lnblck );
                for (c = 1; c < k; ++c)  {
                        lnblck = lnblck->next;
                        len = strstb(lnblck->strz);
                        if( ! edit_newline( &lnclpb->next ))  {
                                clpbord[ clpbnr ].type = 0;
                                clpbord[ clpbnr ].line->strz[0] = '\0';
                                edit_freelines( clpbord[ clpbnr ].line->next );
                                clpbord[ clpbnr ].line->next = NULL;
                                break;
                                }
                        lnclpb->next->prev = lnclpb;
                        lnclpb = lnclpb->next;
                        if (blckstr.x < len)  {
                                be = (blckend.x >= len ? len : blckend.x);
                                if( ! edit_sizeline( lnclpb, be - blckstr.x ))  return(0);
                                strcut(lnclpb->strz, lnblck->strz, blckstr.x, be - blckstr.x);
                                lnclpb->strz[be - blckstr.x] = '\0';
                                }
                        crma_line( lnblck );
                        }
                ed->curcrds.x = blckstr.x;
                }
        blck = 0;
        cmnd_change();
        scrn_adjust();
        return(-1);
        }



blck_copy( int clpbnr )  {
        int c, k, len, be;
        struct linerec *lnblck, *lnclpb;

        if (! blck)  return(0);
        clpb_free( clpbnr );
        if (blckstr.y == blckend.y)  {
                len = strstb(ed->curline->strz);
                clpbord[ clpbnr ].type = 2;
                if (blckstr.x < len)  {
                        be = ( blckend.x >= len ? len : blckend.x );
                        edit_copystrn( clpbord[ clpbnr ].line, &ed->curline->strz[ blckstr.x ], be - blckstr.x );
                        }
                /*ed->curcrds.x = blckstr.x;*/
                }
        else    {
                clpbord[ clpbnr ].type = 1;
                k = blckend.y - blckstr.y;
                clpbord[ clpbnr ].line->strz[0] = '\0';
                len = strstb( blckline->strz );
                if ( blckstr.x == blckend.x )  {
                        edit_copyline( clpbord[ clpbnr ].line, blckline );
                        }
                else    {
                        if (blckstr.x < len)  {
                                be = (blckend.x >= len ? len : blckend.x);
                                edit_copystrn( clpbord[ clpbnr ].line, &blckline->strz[ blckstr.x ], be - blckstr.x );
                                }
                        }
                lnclpb = clpbord[ clpbnr ].line;
                lnblck = blckline;
                for (c = 1; c < k; ++c)  {
                        lnblck = lnblck->next;
                        len = strstb( lnblck->strz );
                        if( ! edit_newline( &lnclpb->next ))  {
                                clpbord[ clpbnr ].type = 0;
                                clpbord[ clpbnr ].line->strz[0] = '\0';
                                edit_freelines( clpbord[ 0 ].line->next );
                                clpbord[ clpbnr ].line->next = NULL;
                                break;
                                }
                        lnclpb->next->prev = lnclpb;
                        lnclpb = lnclpb->next;
                        lnclpb->next = NULL;
                        lnclpb->strz[0] = '\0';
                        if ( blckstr.x == blckend.x )  {
                                edit_copyline( lnclpb, lnblck );
                                }
                        else    {
                                if (blckstr.x < len)  {
                                        be = (blckend.x >= len ? len : blckend.x);
                                        edit_copystrn( lnclpb, &lnblck->strz[ blckstr.x ], be - blckstr.x );
                                        }
                                }
                        }
                }
        blck = 0;
        scrn_adjust();
        return(-1);
        }



blck_fill( char *fs )  {
        int c, k, len, n, fl;
        struct linerec *ln;

        if(! blck )  return(0);
        if( !( fl = strlen( fs )))  return(0);
        if( blckstr.y == blckend.y )  {
                if( undof )  undo_ovrlines( ed->curline, 1, ed->curcrds.y, 0 );
                len = strlen(ed->curline->strz);
 
                if( fl == 1 )  {
                        if( ! edit_sizeline( ed->curline, blckend.x ))  return(0);
                        for (c = len; c < blckstr.x; ++c)
                                ed->curline->strz[c] = CE_KEY_SPACE;
                        memset( &ed->curline->strz[ blckstr.x ], fs[0], ( size_t )( blckend.x - blckstr.x ));
                        if (blckend.x > len)  {
                                ed->curline->strz[ blckend.x ] = '\0';
                                }
                        }
                else    {
                        if( fl >  blckend.x - blckstr.x )
                                fl = blckend.x - blckstr.x;
                        if( ! edit_sizeline( ed->curline, blckstr.x + fl ))  return(0);
                        for (c = len; c < blckstr.x; ++c)
                                ed->curline->strz[c] = CE_KEY_SPACE;
                        strncpy( &ed->curline->strz[ blckstr.x ], fs, fl );
                        if (blckstr.x + fl > len)  {
                                ed->curline->strz[ blckstr.x + fl ] = '\0';
                                }
                        }
                crma_line( ed->curline );
                }
        else if (blckstr.x == blckend.x)  {
                edit_beep( );
                return(0);
                }
        else    {
                k = blckend.y - blckstr.y;
                if( undof )  undo_ovrlines( blckline, k, blckstr.y, 0 );
                ln = blckline;
                for (c = 0; c < k; ++c)  {
                        len = strstb(ln->strz);

                        if( fl == 1 )  {
                                if( ! edit_sizeline( ln, blckend.x ))  return(0);
                                for (n = len; n < blckstr.x; ++n)
                                        ln->strz[n] = CE_KEY_SPACE;
                                memset( &ln->strz[ blckstr.x ], fs[0], ( size_t )( blckend.x - blckstr.x ));
                                if (blckend.x > len)  {
                                        ln->strz[ blckend.x ] = '\0';
                                        }
                                }
                        else    {
                                if( fl > blckend.x - blckstr.x )
                                        fl = blckend.x - blckstr.x;
                                if( ! edit_sizeline( ln, blckstr.x  + fl ))  return(0);
                                for (n = len; n < blckstr.x; ++n)
                                        ln->strz[n] = CE_KEY_SPACE;
                                strncpy( &ln->strz[ blckstr.x ], fs, fl );
                                if ( blckstr.x + fl > len )  {
                                        ln->strz[ blckstr.x + fl ] = '\0';
                                        }
                                }
                        crma_line( ln );
                        ln = ln->next;
                        }
                }
        blck = 0;
        cmnd_change();
        scrn_adjust();
        return(-1);
        }



blck_paste( int clpbnr )  {
        int n, l, k, p;
        struct linerec *ln1, *ln2;

        //if( clpbnr > clpbcnt )  return( 0 );
        if(! clpbord[ clpbnr ].type )  return(0);
        if( p = blck )  blck_del( );
        if( clpbord[ clpbnr ].type == 2 )  {
                undo_ovrlines( ed->curline, 1, ed->curcrds.y, p );
                l = strlen( clpbord[ clpbnr ].line->strz );
                n = strlen( ed->curline->strz );
                if( n < ed->curcrds.x )  {
                        if( ! edit_sizeline( ed->curline, ed->curcrds.x ))  return(0);
                        while( n < ed->curcrds.x )
                                strinsc( ed->curline->strz, ' ', n++ );
                        }
                if( ! edit_sizeline( ed->curline, l + strlen( ed->curline->strz ) ))  return(0);
                for( n = 0; n < l; ++n )
                        strinsc( ed->curline->strz, clpbord[ clpbnr ].line->strz[ n ], ed->curcrds.x + n );
                crma_line( ed->curline );
                }
        else    {
                k = 1;
                for (ln2 = clpbord[ clpbnr ].line; ln2->next != NULL; ln2 = ln2->next)
                        k += 1;
                if( ! edit_copylines( &ln1, clpbord[ clpbnr ].line, k ))  return(0);
                if (ln1 != NULL)  {     // !!!! Replace vvvvvv with edit_inslines
                        k = 1;
                        for (ln2 = ln1; ln2->next != NULL; ln2 = ln2->next)  {
                                k += 1;
                                }
                        ed->nolines += k;
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

                        if( ln1->prev != NULL )  {
                                crma_insline( ln1->prev );
                                crma_line( ln1->prev );
                                }
                        crma_insline( ln2 );
                        ln2 = ln1;
                        for( n = 0; n < k; ++n )  {
                                crma_line( ln2 );
                                ln2 = ln2->next;
                                }
 
                        undo_inslines( ed->curline, k, p );
                        bmrk_inslines( edcurno, ed->curcrds.y, k );
                        tree_inslines( ed->hTreeItem, ed->curcrds.y, k );
                        }
                }
        cmnd_change();
        scrn_adjust();
        return(-1);
        }


blck_pasteblck( int clpbnr )  {
        int n, l, x, p;
        struct linerec *ln1, *ln2;

        //if( clpbnr > clpbcnt )  return( 0 );
        if (! clpbord[ clpbnr ].type)  return(0);
        if( p = blck )  blck_del( );
        if( clpbord[ clpbnr ].type == 2 )  {
                undo_ovrlines( ed->curline, 1, ed->curcrds.y, p );
                l = strlen( clpbord[ clpbnr ].line->strz );
                n = strlen(ed->curline->strz);

                if( n < ed->curcrds.x )  {
                        if( ! edit_sizeline( ed->curline, ed->curcrds.x ))  return(0);
                        while (n < ed->curcrds.x)
                                strinsc(ed->curline->strz, ' ', n++);
                        }

                if( ! edit_sizeline( ed->curline, l + strlen( ed->curline->strz ) ))  return(0);
                for (n = 0; n < l; ++n)
                        strinsc(ed->curline->strz, clpbord[ clpbnr ].line->strz[n], ed->curcrds.x + n);
                crma_line( ed->curline );
                }
        else    {
                if( undof )  {
                        n = 0;
                        ln1 = clpbord[ clpbnr ].line;
                        ln2 = ed->curline;
                        while (ln1 != NULL)  {
                                if( ln2->next == NULL )  break;
                                ln2 = ln2->next;
                                ln1 = ln1->next;
                                n += 1;
                                }
                        undo_ovrlines( ed->curline, n, ed->curcrds.y, p );
                        }
                ln1 = clpbord[ clpbnr ].line;
                ln2 = ed->curline;
                x = ed->curcrds.x;
                while( ln1 != NULL )  {
                        l = strlen( ln1->strz );
                        n = strlen( ln2->strz );
                        if( n < x )  {
                                if( ! edit_sizeline( ln2, x ))  return(0);
                                while( n < x )
                                        strinsc( ln2->strz, ' ', n++ );
                                }
                        if( ! edit_sizeline( ln2, l + strlen( ln2->strz ) ))  return(0);
                        for( n = 0; n < l; ++n )
                                strinsc( ln2->strz, ln1->strz[n], x + n );
                        crma_line( ln2 );
                        if( ln2->next == NULL )  break;         // Fix !!! - allow block paste past EOF
                        ln2 = ln2->next;
                        ln1 = ln1->next;
                        }
                }
        cmnd_change();
        scrn_adjust();
        return(-1);
        }


blck_case( int t )  {        /* t:  1 = upper case;  2 = lower case; */
        char r;
        int len, c, k, be;
        struct linerec *ln;

        len = strstb(ed->curline->strz);
        if (blckstr.y == blckend.y)  {
                undo_ovrlines( ed->curline, 1, ed->curcrds.y, 0 );
                if (blckstr.x < len)  {
                        if (blckend.x >= len)  blckend.x = len;
                        r = ed->curline->strz[ blckend.x ];
                        ed->curline->strz[ blckend.x ] = 0;
                        if( t == 1 )  strupc(&ed->curline->strz[blckstr.x]);
                        else  if( t == 2 )  strdnc(&ed->curline->strz[blckstr.x]);
                        else  if( t == 3 )  strinc(&ed->curline->strz[blckstr.x]);
                        ed->curline->strz[ blckend.x ] = r;
                        }
                crma_line( ed->curline );
                }
        else if( blckstr.x == blckend.x )   {
                k = blckend.y - blckstr.y;
                undo_ovrlines( blckline, k, blckstr.y, 0 );
                ln = blckline;
                for (c = 0; c < k; ++c)  {
                        if( t == 1 )  strupc(ln->strz);
                        else  if( t == 2 )  strdnc(ln->strz);
                        else  if( t == 3 )  strinc(ln->strz);
                        crma_line( ln );
                        ln = ln->next;
                        }
                }
        else    {
                k = blckend.y - blckstr.y;
                undo_ovrlines( blckline, k, blckstr.y, 0 );
                ln = blckline;
                for (c = 0; c < k; ++c)  {
                        len = strstb(ln->strz);
                        if (blckstr.x < len)  {
                                be = (blckend.x >= len ? len : blckend.x);
                                r = ln->strz[ be ];
                                ln->strz[ be ] = 0;
                                if( t == 1 )  strupc(&ln->strz[blckstr.x]);
                                else  if( t == 2 )  strdnc(&ln->strz[blckstr.x]);
                                else  if( t == 3 )  strinc(&ln->strz[blckstr.x]);
                                ln->strz[ be ] = r;
                                }
                        crma_line( ln );
                        ln = ln->next;
                        }
                }
        blck = 0;
        cmnd_change();
        scrn_adjust();
        }


