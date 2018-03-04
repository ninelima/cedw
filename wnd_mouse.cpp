


#include "windows.h"

#include "string.h"
#include "stdlib.h"

#include "resource.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"


extern struct editrec *ed;
extern int blck;
extern struct coords blckstr, blckend;
extern struct coords cursor;


extern HINSTANCE hInst;
extern HWND hwndMain, hwndClient;
extern HWND hwndVScroll, hwndHScroll;

extern int cxScrn, cyScrn;
extern int cxChar, cyChar;
extern int fMatchBracket;
extern short fMouseRepeat;

WORD cxMouChar, cyMouChar;


WMLButtonDown( HWND hwnd, WORD cxMou, WORD cyMou )  {
    int n;
    RECT rect;
 
    GetClientRect( hwndClient, &rect );
    if( cxMou > rect.right - CE_SZE_HSCROLL )  return;

    HideCaret( NULL );
    HideMatchBracket();

    cursor.x = cxMouChar = ( cxMou - 2 )/ cxChar;
    ed->curcrds.x  = cursor.x + ed->displcrds.x;
    cursor.y = cyMouChar = ( cyMou - 1 )/ cyChar;

    if( cursor.y >= cyScrn - 1 )  cursor.y = cyScrn - 2;
    ed->curcrds.y = cursor.y + ed->displcrds.y;

    ed->curline = ed->displline;
    for( n = 0; n < cursor.y; ++n )  {
        if( ed->curline->next == NULL )  {
            cursor.y = n;
            ed->curcrds.y = cursor.y + ed->displcrds.y;
            cursor.x = 0;
            ed->curcrds.x  = ed->displcrds.x;
            break;
        }
        ed->curline = ed->curline->next;
    }
    //HideCaret( NULL );
    if( blck )  {
        blck = 0;
        scrn_adjust();
    }
    else  {
        scrn_reline();
        scrn_cursor();
    }
    if(( ! blck )&&( fMatchBracket ))  ShowMatchBracket();
    ShowCaret( NULL );
}


WMLButtonDblclk( HWND hwnd, WORD cxMou, WORD cyMou )  {
    int n;
    char selstr[CE_LEN_FINDSTR];

    cursor.x = cxMouChar = ( cxMou - 2 )/ cxChar;
    ed->curcrds.x  = cursor.x + ed->displcrds.x;
    cursor.y = cyMouChar = ( cyMou - 1 )/ cyChar;

    if( cursor.y >= cyScrn - 1 )  cursor.y = cyScrn - 2;
    ed->curcrds.y = cursor.y + ed->displcrds.y;

    ed->curline = ed->displline;
    for( n = 0; n < cursor.y; ++n )  {
        if( ed->curline->next == NULL )  {
            cursor.y = n;
            ed->curcrds.y = cursor.y + ed->displcrds.y;
            break;
        }
        ed->curline = ed->curline->next;
    }
    HideCaret( NULL );
    blck = 0;
    find_getcurselword( selstr );
    if( blck )  ed->curcrds.x = blckstr.x;
    scrn_adjust();
    if(( ! blck )&&( fMatchBracket ))  ShowMatchBracket();
    ShowCaret( NULL );
}



WMMouseMove( HWND hwnd, WPARAM fwKeys, WORD cxMou, WORD cyMou )  {
    WORD cy, cx;
    static DWORD dwMouMove = 1;
    //static DWORD dwMouScrollPause = 400;
 
    MSG msg;

    fMouseRepeat = 0;

    if( fwKeys == MK_LBUTTON )  {
        cy = ( cyMou - 1 )/ cyChar;
        cx = ( cxMou - 2 )/ cxChar;

        HideCaret( NULL );
 
        if(( cy > cyScrn - 2 )&&( blckstr.y != blckend.y ))  {

            fMouseRepeat = -1;
            while( fMouseRepeat )  {
                while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
                    GetMessage(&msg, NULL, 0, 0);
                    DispatchMessage(&msg);
                }
                Sleep( 10 );
                //Sleep( dwMouScrollPause );
                //dwMouScrollPause /= 2;

                blck_down();
                SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );

                if( dwMouMove == 1 )  dwMouMove = -1;
                else  dwMouMove = 1;
                //mouse_event( MOUSEEVENTF_MOVE, dwMouMove, 0, 0, 0 );
                //WMMouseMove( hwnd, fwKeys, cxMou, cyMou, -1 );
                //WMMouseMove( hwnd, wParam, LOWORD( lParam ), HIWORD( lParam ), 0 );
                //SendMessage( hwnd, SM_MOUSEREPEAT, fwKeys, MAKELPARAM( cxMou, cyMou ));
            }
            ShowCaret( NULL );
            return(-1);
        }
        if(( cy == 0 )&&( blckstr.y != blckend.y ))  {

            fMouseRepeat = -1;
            while( fMouseRepeat )  {
                while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
                    GetMessage(&msg, NULL, 0, 0);
                    DispatchMessage(&msg);
                }
                Sleep( 10 );
                //Sleep( dwMouScrollPause );
                //dwMouScrollPause /= 2;

                blck_up();
                SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );

                if( dwMouMove == 1 )  dwMouMove = -1;
                else  dwMouMove = 1;
            }
            ShowCaret( NULL );
            return(-1);
        }
        if(( cx > cxScrn - 3 )&&( blckstr.x != blckend.x ))  {

            fMouseRepeat = -1;
            while( fMouseRepeat )  {
                while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
                    GetMessage(&msg, NULL, 0, 0);
                    DispatchMessage(&msg);
                }
                Sleep( 10 );
                //Sleep( dwMouScrollPause );
                //dwMouScrollPause /= 2;

                blck_right();
                SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );

                if( dwMouMove == 1 )  dwMouMove = -1;
                else  dwMouMove = 1;
            }
            ShowCaret( NULL );
            return(-1);
        }
        if(( cx == 0 )&&( blckstr.x != blckend.x ))  {

            fMouseRepeat = -1;
            while( fMouseRepeat )  {
                while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
                    GetMessage(&msg, NULL, 0, 0);
                    DispatchMessage(&msg);
                }
                Sleep( 10 );
                //Sleep( dwMouScrollPause );
                //dwMouScrollPause /= 2;

                blck_left();
                SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );

                if( dwMouMove == 1 )  dwMouMove = -1;
                else  dwMouMove = 1;
            }
            ShowCaret( NULL );
            return(-1);
        }
        //dwMouScrollPause = 400;

        if( blck )  {
            if( blckstr.y == blckend.y )  {
                while( cxMouChar < cx )  {
                    cxMouChar += 1;
                    blck_right( );
                }
                while( cxMouChar > cx )  {
                    cxMouChar -= 1;
                    blck_left( );
                }
            }
            else  {
                while( cyMouChar < cy )  {
                    cyMouChar += 1;
                    blck_down( );
                }
                while( cyMouChar > cy )  {
                    cyMouChar -= 1;
                    blck_up( );
                }
            }
        }
        else  {
            if( cx != cxMouChar )  {
                while( cxMouChar < cx )  {
                    cxMouChar += 1;
                    blck_right( );
                }
                while( cxMouChar > cx )  {
                    cxMouChar -= 1;
                    blck_left( );
                }
            }
            else  {
                while( cyMouChar < cy )  {
                    cyMouChar += 1;
                    blck_down( );
                }
                while( cyMouChar > cy )  {
                    cyMouChar -= 1;
                    blck_up( );
                }
            }
        }
        ShowCaret( NULL );
    }
    if( fwKeys == ( MK_LBUTTON | MK_SHIFT ))  {
        cy = ( cyMou - 1 )/ cyChar;
        cx = ( cxMou - 2 )/ cxChar;

        HideCaret( NULL );

        if( cy >= cyScrn - 2 )  {
            fMouseRepeat = -1;
            while( fMouseRepeat )  {
                while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
                    GetMessage(&msg, NULL, 0, 0);
                    DispatchMessage(&msg);
                }
                Sleep( 10 );
                //Sleep( dwMouScrollPause );
                //dwMouScrollPause /= 2;

                blck_down();
                SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );

                if( dwMouMove == 1 )  dwMouMove = -1;
                else  dwMouMove = 1;
            }
            ShowCaret( NULL );
            return(-1);
        }
        if( cy == 0 )  {
           fMouseRepeat = -1;
            while( fMouseRepeat )  {
                while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
                    GetMessage(&msg, NULL, 0, 0);
                    DispatchMessage(&msg);
                }
                 Sleep( 10 );
                //Sleep( dwMouScrollPause );
                //dwMouScrollPause /= 2;
                blck_up();

                SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );

                if( dwMouMove == 1 )  dwMouMove = -1;
                else  dwMouMove = 1;
            }
            ShowCaret( NULL );
            return(-1);
        }
        if( cx > cxScrn - 3 )  {
            fMouseRepeat = -1;
            while( fMouseRepeat )  {
            while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
                    GetMessage(&msg, NULL, 0, 0);
                    DispatchMessage(&msg);
                }
                Sleep( 10 );
                //Sleep( dwMouScrollPause );
                //dwMouScrollPause /= 2;

                blck_right();
                SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );

                if( dwMouMove == 1 )  dwMouMove = -1;
                else  dwMouMove = 1;
            }
            ShowCaret( NULL );
            return(-1);
        }
        if( cx == 0 )  {
           fMouseRepeat = -1;
            while( fMouseRepeat )  {
                while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
                    GetMessage(&msg, NULL, 0, 0);
                    DispatchMessage(&msg);
                }
                Sleep( 10 );
                //Sleep( dwMouScrollPause );
                //dwMouScrollPause /= 2;

                blck_left();
                SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );

                if( dwMouMove == 1 )  dwMouMove = -1;
                else  dwMouMove = 1;
            }
            ShowCaret( NULL );
            return(-1);
        }

        //dwMouScrollPause = 400;

        while( cxMouChar < cx )  {
            blck_rightn( cx - cxMouChar );
            cxMouChar = cx;
        }
        while( cxMouChar > cx )  {
            blck_leftn( cxMouChar - cx );
            cxMouChar = cx;
        }
        while( cyMouChar < cy )  {
            cyMouChar += 1;
            blck_down( );
        }
        while( cyMouChar > cy )  {
            cyMouChar -= 1;
            blck_up( );
        }
        ShowCaret( NULL );
    }
}


#ifdef __cplusplus
}
#endif
