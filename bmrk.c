 

#include "stdio.h"
#include "string.h"

#include "ced.h"

extern struct editrec *ed;
extern int edcurno;


int bmrkcur = -1;
struct bookmrec bookmarks[ CE_MAX_BOOKMARKS ];



bmrk_clear( int b )  {

        if( b >= CE_MAX_BOOKMARKS )  return(0);
        bookmarks[ b ].linenr = 0;
        bookmarks[ b ].linepos = 0;
        bookmarks[ b ].filenr = 0;
        return(-1);
        }


bmrk_clearall( )  {
        int b;

        for( b = 0; b < CE_MAX_BOOKMARKS; ++b )  {
                bookmarks[ b ].linenr = 0;
                bookmarks[ b ].linepos = 0;
                bookmarks[ b ].filenr = 0;
                }
        bmrkcur = -1;
        return(-1);
        }


bmrk_set( int b )  {

        if( b >= CE_MAX_BOOKMARKS )  return(0);
        bookmarks[ b ].linenr = ed->curcrds.y;
        bookmarks[ b ].linepos = ed->curcrds.x;
        bookmarks[ b ].filenr = edcurno;
        bmrkcur = b;
        return(-1);
        }


bmrk_setnext( )  {
        int b;
        
        for( b = 0; b < CE_MAX_BOOKMARKS; ++b )  {
            if( bookmarks[ b ].filenr == 0 )
                break;
        }
        if( b >= CE_MAX_BOOKMARKS )  return(0);
        bookmarks[ b ].linenr = ed->curcrds.y;
        bookmarks[ b ].linepos = ed->curcrds.x;
        bookmarks[ b ].filenr = edcurno;
        bmrkcur = b;
        return(-1);
        }


bmrk_get( int b, int *linenr, int *filenr )  {

        *linenr = 0;
        *filenr = 0;
        if( b >= CE_MAX_BOOKMARKS )  return(0);
        *linenr = bookmarks[ b ].linenr;
        *filenr = bookmarks[ b ].filenr;
        return(-1);
        }


bmrk_next( )  {
        int b;

        if( bmrkcur == -1 )  return( 0 );
        //for( b = bmrkcur + 1; b < CE_MAX_BOOKMARKS; ++b )  {  //  2001/12/26
        for( b = bmrkcur + 1; ; ++b )  {
                if( b == CE_MAX_BOOKMARKS )  b = 0;
                if( bookmarks[ b ].filenr )  {
                        if( ! cmnd_goto_file( bookmarks[ b ].filenr, bookmarks[ b ].linenr + 1, bookmarks[ b ].linepos ))  {
                                return( 0 );
                                }
                        else    {
                                bmrkcur = b;
                                tree_selfile( ed->hTreeItem, 0 );
                                return(-1);
                                }
                        }
                }
        return( 0 );
        }


bmrk_prev( )  {
        int b;

        if( bmrkcur == -1 )  return( 0 );
        //for( b = bmrkcur - 1; b >= 0; --b )  {
        for( b = bmrkcur - 1; ; --b )  {
                if( b == -1 )  b = CE_MAX_BOOKMARKS;
                if( bookmarks[ b ].filenr )  {
                        if( ! cmnd_goto_file( bookmarks[ b ].filenr, bookmarks[ b ].linenr + 1, bookmarks[ b ].linepos ))  {
                                return( 0 );
                                }
                        else    {
                                bmrkcur = b;
                                tree_selfile( ed->hTreeItem, 0 );
                                return(-1);
                                }
                        }
                }
        return( 0 );
        }



bmrk_goto( int b )  {

        if( b >= CE_MAX_BOOKMARKS )  return(0);
        if( ! bookmarks[ b ].filenr )  return( 0 );
        if( ! cmnd_goto_file( bookmarks[ b ].filenr, bookmarks[ b ].linenr + 1, bookmarks[ b ].linepos ))  {
                return( 0 );
                }
        bmrkcur = b;
        tree_selfile( ed->hTreeItem, 0 );
        return( -1 );
        }



bmrk_fclose( int f )  {
        int b;

        for( b = 0; b < CE_MAX_BOOKMARKS; ++b )  {
                if( f == bookmarks[ b ].filenr )  bookmarks[ b ].filenr = 0;
                if( f < bookmarks[ b ].filenr )  bookmarks[ b ].filenr -= 1;
                }
        }



bmrk_dellines( int f, int l, int c )  {
        int b;

        for( b = 0; b < CE_MAX_BOOKMARKS; ++b )  {
                if( f == bookmarks[ b ].filenr )  {
                        if( bookmarks[ b ].linenr < l )
                                continue;
                        if( bookmarks[ b ].linenr >= ( l + c ))
                                bookmarks[ b ].linenr -= c;
                        else if( bookmarks[ b ].linenr < ( l + c ))
                                bookmarks[ b ].linenr = l;
                        }
                }
        }



bmrk_inslines( int f, int l, int c )  {
        int b;

        for( b = 0; b < CE_MAX_BOOKMARKS; ++b )  {
                if( f == bookmarks[ b ].filenr )  {
                        if( bookmarks[ b ].linenr < l )
                                continue;
                        if( bookmarks[ b ].linenr >= l )
                                bookmarks[ b ].linenr += c;

                        }
                }
        }

