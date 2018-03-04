
#include "ced.h"

#include "stdlib.h"
#include "stdio.h"


extern struct editrec *ed;
extern struct linerec *blckline;
extern struct linerec *clpbline;
extern struct coords cursor;
extern int insmode;
extern int edcurno;
extern int edcount;
extern int blck;
extern struct coords blckstr, blckend;
extern int tablen;


int undof = -1;



undo_goto( )  {     // move cursor to last edit position
        if( ! undof )  return( 0 );
        if( ! ed->undocnt )  {
                edit_beep( );
                return( 0 );
                }
        blck_unmark( );
        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->undo[ ed->undocur ].linepos );
        }


undo_undo( )  {
        int n, im, l, p;
        struct linerec *ln;
        char c;

        if( ! undof )  return( 0 );
        if( ! ed->undocnt )  {
                edit_beep( );
                return( 0 );
                }
        blck_unmark( );
        p = ed->undo[ ed->undocur ].prev;
        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->undo[ ed->undocur ].linepos );
        undof = 0;
        switch( ed->undo[ ed->undocur ].type )  {

                case 1: //  char
                        cmnd_del( );
                        break;

                case 2: //  CR
                        cmnd_del( );
                        edit_insspaces( ed->curline, ed->curcrds.x, ed->undo[ ed->undocur ].cnt );
                        break;

                case 3: //  Tab
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )
                                cmnd_del( );
                        break;

                case 4: //  BS
                        im = insmode;
                        insmode = 1;
                        cmnd_char( ed->undo[ ed->undocur ].chr );
                        insmode = im;
                        break;

                case 5: //  BS CR
                        //cmnd_cr( );
                        edit_splitline( ed->curline, ed->curcrds.x );
                        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, 0 );
                        break;

                case 6: // Del Tab
                        edit_insspaces( ed->curline, ed->curcrds.x, ed->undo[ ed->undocur ].cnt );
                        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->curcrds.x + ed->undo[ ed->undocur ].cnt );
                        break;

                case 7: //  Del
                        im = insmode;
                        insmode = 1;
                        cmnd_char( ed->undo[ ed->undocur ].chr );
                        insmode = im;
                        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->undo[ ed->undocur ].linepos );
                        break;

                case 8: //  Del CR
                        //cmnd_cr( );
                        edit_splitline( ed->curline, ed->curcrds.x );
                        edit_insspaces( ed->curline->next, 0, ed->undo[ ed->undocur ].cnt );
                        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->undo[ ed->undocur ].linepos );
                        break;

                case 9: // Char Ovr
                        im = insmode;
                        insmode = 0;
                        c = ed->curline->strz[ ed->curcrds.x ];;
                        cmnd_char( ed->undo[ ed->undocur ].chr );
                        ed->undo[ ed->undocur ].chr = c;
                        insmode = im;
                        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->undo[ ed->undocur ].linepos );
                        break;

                /*case 10:
                        im = insmode;
                        insmode = 0;
                        if( ed->undo[ ed->undocur ].line != NULL )  {
                                for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )      // Fix ?
                                        cmnd_char( ed->undo[ ed->undocur ].line->strz[ n ] );
                                }
                        insmode = im;
                        break;

                case 11:
                        if( ed->undo[ ed->undocur ].line == NULL )  {
                                if( edit_newline( &ed->undo[ ed->undocur ].line ))  {  // setup for redo
                                        if( ! edit_sizeline( ed->undo[ ed->undocur ].line->strz, ed->undo[ ed->undocur ].cnt ))  return(0);
                                        strncpy( ed->undo[ ed->undocur ].line->strz, &ed->curline->strz[ ed->undo[ ed->undocur ].linepos ], ed->undo[ ed->undocur ].cnt );
                                        }
                                }
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )
                                cmnd_del( );
                        break;*/

                case 12:
                        edit_inslines( ed->undo[ ed->undocur ].line, ed->undo[ ed->undocur ].cnt );
                        ed->undo[ ed->undocur ].line = NULL;
                        break;

                case 13:
                        ln = ed->curline;
                        edit_cutlines( ln, ed->undo[ ed->undocur ].cnt );
                        edit_inslines( ed->undo[ ed->undocur ].line, ed->undo[ ed->undocur ].cnt );
                        ed->undo[ ed->undocur ].line = ln;
                        break;

                case 14:
                        ln = ed->curline;
                        edit_cutlines( ln, ed->undo[ ed->undocur ].cnt );
                        ed->undo[ ed->undocur ].line = ln;
                        ed->undolns += ed->undo[ ed->undocur ].cnt;
                        break;

                case 20:
                        ln = ed->curline;
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )  {
                                l = strlen(ln->strz);
                                if( ed->undo[ ed->undocur ].linepos < l )
                                        strdel( ln->strz, ed->undo[ ed->undocur ].linepos, 1 );
                                crma_line( ln );
                                if ((ln = ln->next) == NULL)
                                        break;          // just in case

                                }
                        break;

                case 21:
                        ln = ed->curline;
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )  {
                                if (strchra(ln->strz) >= tablen)
                                        strdel(ln->strz, 0, tablen);
                                crma_line( ln );
                                if ((ln = ln->next) == NULL)
                                        break;          // just in case
                                }
                        break;

                case 22:
                        ln = ed->curline;
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )  {
                                l = strlen(ln->strz);
                                for (; l >= 0; --l)
                                        ln->strz[l + tablen] = ln->strz[l];
                                for (l = 0; l < tablen; ++l)
                                        ln->strz[l] = CE_KEY_SPACE;
                                crma_line( ln );
                                if ((ln = ln->next) == NULL)
                                        break;          /* just in case */

                                }

                        break;

                case 23:
                        ln = ed->curline;
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )  {
                                l = strlen(ln->strz);
                                for (; l >= ed->undo[ ed->undocur ].linepos; --l)
                                        ln->strz[l+1] = ln->strz[l];
                                ln->strz[ ed->undo[ ed->undocur ].linepos ] = CE_KEY_SPACE;
                                crma_line( ln );
                                if ((ln = ln->next) == NULL)
                                        break;          // just in case
                                }
                        break;

                }
        --ed->undocnt;
        ++ed->redocnt;
        if( ed->undocnt == 0 )  ed->undocur = 0;
        else if( ed->undocur == 0 )  ed->undocur = CE_MAX_UNDO - 1;
        else --ed->undocur;
        undof = -1;
        if( p )  undo_undo();
        scrn_rescrn();
        }



undo_redo( )  {
        int n, im, l;
        char c;
        struct linerec *ln;
 
        if( ! undof )  return( 0 );
        if( ! ed->redocnt )  {
                edit_beep( );
                return( 0 );
                }
        blck_unmark( );
        undof = 0;
        if( ed->undocnt == 0 )  ed->undocur = 0;
        else if( ed->undocur == CE_MAX_UNDO - 1 )  ed->undocur = 0;
        else  ++ed->undocur;

        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->undo[ ed->undocur ].linepos );
        switch( ed->undo[ ed->undocur ].type )  {
                case 1: // char
                        im = insmode;
                        insmode = 1;
                        cmnd_char( ed->undo[ ed->undocur ].chr );
                        insmode = im;
                        break;

                case 2: //  CR
                        cmnd_cr( );
                        break;

                case 3: //  Tab
                        //cmnd_tab( );  tabsize could have been changed in the meantime
                        edit_insspaces( ed->curline, ed->curcrds.x, ed->undo[ ed->undocur ].cnt );
                        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->curcrds.x + ed->undo[ ed->undocur ].cnt );
                        break;

                case 4: //  BS
                        cmnd_del( );
                        break;

                case 5: //  BS CR
                        cmnd_del( );
                        break;

                case 6: // Del Tab
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )
                                cmnd_del( );
                        break;

                case 7: //  Del
                        cmnd_del( );
                        break;

                case 8: //  Del CR
                        cmnd_del( );
                        break;

                case 9: // Char Ovr
                        im = insmode;
                        insmode = 0;
                        c = ed->curline->strz[ ed->curcrds.x ];;
                        cmnd_char( ed->undo[ ed->undocur ].chr );
                        ed->undo[ ed->undocur ].chr = c;
                        insmode = im;
                        break;

                /*case 10:
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )
                                cmnd_del( );
                        break;

                case 11:
                        if( ed->undo[ ed->undocur ].line != NULL )  {
                                for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )
                                        cmnd_char( ed->undo[ ed->undocur ].line->strz[ n ] );    // Fix !!!!
                                }
                        break;*/

                case 12:
                        ln = ed->curline;
                        edit_cutlines( ln, ed->undo[ ed->undocur ].cnt );
                        ed->undo[ ed->undocur ].line = ln;
                        break;

                case 13:
                        ln = ed->curline;
                        edit_cutlines( ln, ed->undo[ ed->undocur ].cnt );
                        edit_inslines( ed->undo[ ed->undocur ].line, ed->undo[ ed->undocur ].cnt );
                        ed->undo[ ed->undocur ].line = ln;
                        break;

                case 14:
                        edit_inslines( ed->undo[ ed->undocur ].line, ed->undo[ ed->undocur ].cnt );
                        ed->undo[ ed->undocur ].line = NULL;
                        break;



                case 20:
                        ln = ed->curline;
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )  {
                                l = strlen(ln->strz);
                                for (; l >= ed->undo[ ed->undocur ].linepos; --l)
                                        ln->strz[l+1] = ln->strz[l];
                                ln->strz[ ed->undo[ ed->undocur ].linepos ] = CE_KEY_SPACE;
                                crma_line( ln );
                                if ((ln = ln->next) == NULL)
                                        break;          // just in case
                                }
                        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->undo[ ed->undocur ].linepos );
                        break;

                case 21:
                        ln = ed->curline;
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )  {
                                l = strlen(ln->strz);
                                for (; l >= 0; --l)
                                        ln->strz[l + tablen] = ln->strz[l];
                                for (l = 0; l < tablen; ++l)
                                        ln->strz[l] = CE_KEY_SPACE;
                                crma_line( ln );
                                if ((ln = ln->next) == NULL)
                                        break;          // just in case
                                }
                        break;

                case 22:
                        ln = ed->curline;
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )  {
                                l = strlen(ln->strz);
                                if( ed->undo[ ed->undocur ].linepos < l )
                                        strdel( ln->strz, 0, tablen );
                                crma_line( ln );
                                if ((ln = ln->next) == NULL)
                                        break;          // just in case

                                }
                        break;

                case 23:
                        ln = ed->curline;
                        for( n = 0; n < ed->undo[ ed->undocur ].cnt; ++n )  {
                                l = strlen(ln->strz);
                                if( ed->undo[ ed->undocur ].linepos < l )
                                        strdel( ln->strz, ed->undo[ ed->undocur ].linepos, 1 );
                                crma_line( ln );
                                if ((ln = ln->next) == NULL)
                                        break;          // just in case

                                }
                        cmnd_goto_line( ed->undo[ ed->undocur ].linenr+1, ed->undo[ ed->undocur ].linepos );
                        break;

                }
        --ed->redocnt;
        ++ed->undocnt;
        undof = -1;
        if(( ed->undo[ ed->undocur == CE_MAX_UNDO - 1 ? 0 : ed->undocur + 1 ].prev )&&( ed->redocnt ))  undo_redo();
        scrn_rescrn();
        }




undo_clear( )  {

        while( ed->redocnt )  {
                --ed->redocnt;
                ++ed->undocnt;
                if( ed->undocur == CE_MAX_UNDO - 1 )  ed->undocur = 0;
                else  ++ed->undocur;
                }
        while( ed->undocnt )  {
                if( ed->undo[ ed->undocur ].line != NULL )
                        edit_freelines( ed->undo[ ed->undocur ].line );
                ed->undo[ ed->undocur ].line = NULL;
                ed->undo[ ed->undocur ].cnt = 0;
                if( ed->undocur == 0 )  ed->undocur = CE_MAX_UNDO - 1;
                else --ed->undocur;
                --ed->undocnt;
                }
        //ed->redocnt = 0;
        //ed->undocnt = 0;
        ed->undolns = 0;
        }



undo_char( char c, int p )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 1;
        ed->undo[ ed->undocur ].chr = c;
        ed->undo[ ed->undocur ].prev = p;
        return(-1);
        }


undo_charovr( char c, int p )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 9;
        ed->undo[ ed->undocur ].chr = ed->curline->strz[ ed->curcrds.x ];
        ed->undo[ ed->undocur ].prev = p;
        return(-1);
        }


undo_cr( int p )  {
        int a;

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 2;
        ed->undo[ ed->undocur ].chr = CE_KEY_CR;
        /*a = strchra( ed->curline->strz );
        //ed->undo[ ed->undocur ].cnt = a;
        if( a > ed->curcrds.x )
                ed->undo[ ed->undocur ].cnt = a - ed->curcrds.x;*/

        a = strchra( &ed->curline->strz[ ed->curcrds.x ] );
        if( a != -1 )
                ed->undo[ ed->undocur ].cnt = a;

        ed->undo[ ed->undocur ].prev = p;
        return(-1);
        }


undo_tab( int d )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 3;
        ed->undo[ ed->undocur ].chr = CE_KEY_TAB;
        ed->undo[ ed->undocur ].cnt = d;
        return(-1);
        }


undo_bs( )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 4;
        ed->undo[ ed->undocur ].chr = ed->curline->strz[ ed->curcrds.x ];
        return(-1);
        }


undo_del( )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 7;
        ed->undo[ ed->undocur ].chr = ed->curline->strz[ ed->curcrds.x ];
        return(-1);
        }


undo_bscr( )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 5;
        ed->undo[ ed->undocur ].chr = 0;
        return(-1);
        }


undo_delcr( int a )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 8;
        ed->undo[ ed->undocur ].chr = 0;
        ed->undo[ ed->undocur ].cnt = a;
        return(-1);
        }


undo_deltab( int d )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 6;
        ed->undo[ ed->undocur ].chr = 0;
        ed->undo[ ed->undocur ].cnt = d;
        return(-1);
        }


undo_blckspace( int x, int y, int c )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 20;
        ed->undo[ ed->undocur ].chr = 0;
        ed->undo[ ed->undocur ].cnt = c;
        ed->undo[ ed->undocur ].linepos = x;
        ed->undo[ ed->undocur ].linenr = y;
        return(-1);
        }


undo_blckrevspace( int x, int y, int c )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 23;
        ed->undo[ ed->undocur ].chr = 0;
        ed->undo[ ed->undocur ].cnt = c;
        ed->undo[ ed->undocur ].linepos = x;
        ed->undo[ ed->undocur ].linenr = y;
        return(-1);
        }


undo_blcktab( int y, int c )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 21;
        ed->undo[ ed->undocur ].chr = 0;
        ed->undo[ ed->undocur ].cnt = c;
        ed->undo[ ed->undocur ].linepos = 0;
        ed->undo[ ed->undocur ].linenr = y;
        return(-1);
        }


undo_blckrevtab( int y, int c )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 22;
        ed->undo[ ed->undocur ].chr = 0;
        ed->undo[ ed->undocur ].cnt = c;
        ed->undo[ ed->undocur ].linepos = 0;
        ed->undo[ ed->undocur ].linenr = y;
        return(-1);
        }



undo_dellines( struct linerec *ln, int c )  {
        int n;

        if( ! undof )  return(0);
        if( ! undoadd( c ))  return(0);
        ed->undo[ ed->undocur ].type = 12;
        ed->undo[ ed->undocur ].cnt = c;
        ed->undo[ ed->undocur ].line = ln;
        for( n = 1; n < c; ++n )
                ln = ln->next;
        ln->next = NULL;
        return(-1);
        }


undo_cutlines( struct linerec *ln, int c )  {

        if( ! undof )  return(0);
        if( ! undoadd( c ))  return(0);
        ed->undo[ ed->undocur ].type = 12;
        ed->undo[ ed->undocur ].cnt = c;
        if( ! edit_copylines( &ed->undo[ ed->undocur ].line, ln, c ))  return(0);
        return(-1);
        }



undo_ovrlines( struct linerec *ln, int c, int y, int p )  {

        if( ! undof )  return(0);
        if( ! undoadd( c ))  return(0);
        ed->undo[ ed->undocur ].linenr = y;
        ed->undo[ ed->undocur ].type = 13;
        ed->undo[ ed->undocur ].cnt = c;
        ed->undo[ ed->undocur ].prev = p;
        if( ! edit_copylines( &ed->undo[ ed->undocur ].line, ln, c ))  return(0);
        return(-1);
        }



undo_inslines( struct linerec *ln, int c, int p )  {

        if( ! undof )  return(0);
        if( ! undoadd( 0 ))  return(0);
        ed->undo[ ed->undocur ].type = 14;
        ed->undo[ ed->undocur ].cnt = c;
        ed->undo[ ed->undocur ].line = NULL;
        ed->undo[ ed->undocur ].prev = p;
        return(-1);
        }




undoadd( int lc )  {
        int uc, ut, dn, dt;

        if( lc )  {
                if( lc > CE_MAX_UNDOLINES )  {
                        undo_clear( );
                        /*if( dlg_yn( "Undo Exceeds Maximum Lines !  Discard", "Undo" ))  {
                                return( 0 );
                                }*/
                        }
                if(( ed->undocnt )&&( ed->undolns + lc > CE_MAX_UNDOLINES ))  {
                        uc = ed->undocur;
                        ut = ed->undocnt;
                        while( --ut )  {
                                if( uc == 0 )  uc = CE_MAX_UNDO - 1;
                                else --uc;
                                }
                        dt = 0;
                        dn = ed->undolns + lc - CE_MAX_UNDOLINES;
                        while( ed->undocnt )  {
                                dt += ed->undo[ uc ].cnt;
                                if( ed->undo[ uc ].line != NULL )
                                        edit_freelines( ed->undo[ uc ].line );
                                ed->undo[ uc ].line = NULL;
                                ed->undo[ uc ].cnt = 0;
                                --ed->undocnt;
                                if( dt >= dn )  break;
                                if( uc == CE_MAX_UNDO - 1 )  uc = 0;
                                else  ++uc;
                                }
                        ed->undolns -= dt;
                        }
                }

        if( ed->undocnt == 0 )  {
                ++ed->undocnt;
                ed->undocur = 0;
                }
        else if( ed->undocur == CE_MAX_UNDO - 1 )  {
                ed->undocur = 0;
                }
        else    {
                if( ed->undocnt < CE_MAX_UNDO )
                        ++ed->undocnt;
                ++ed->undocur;
                }

        if( ed->redocnt )  {
                uc = ed->undocur;
                while( ed->redocnt )  {
                        if( ed->undo[ uc ].line != NULL )  {
                                edit_freelines( ed->undo[ uc ].line );
                                ed->undolns -= ed->undo[ ed->undocur ].cnt;
                                }
                        ed->undo[ uc ].line = NULL;
                        ed->undo[ uc ].cnt = 0;
                        --ed->redocnt;
                        if( uc == CE_MAX_UNDO - 1 )  uc = 0;
                        else  ++uc;
                        }
                }
        else    {
                if( ed->undo[ ed->undocur ].line != NULL )  {
                        edit_freelines( ed->undo[ ed->undocur ].line );
                        ed->undolns -= ed->undo[ ed->undocur ].cnt;
                        }
                }

        ed->undo[ ed->undocur ].filenr = edcurno;
        ed->undo[ ed->undocur ].linenr = ed->curcrds.y;
        ed->undo[ ed->undocur ].linepos = ed->curcrds.x;
        ed->undo[ ed->undocur ].line = NULL;
        ed->undo[ ed->undocur ].cnt = 0;
        ed->undo[ ed->undocur ].prev = 0;

        ed->undolns += lc;
        return(-1);
        }

