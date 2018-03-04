


#include "windows.h"

#include "string.h"

#include "commctrl.h"

#include "resource.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"


extern HINSTANCE hInst;
extern HDC hDC;
extern HWND hwndMain, hwndClient, hwndTree;
extern HWND hwndVScroll, hwndHScroll, hwndScrollF;

extern int cxScrn, cyScrn;
extern int cxChar, cyChar;

extern int scrn_x;
extern int scrn_y;


extern int fScrnFont;
extern LOGFONT lfScrnFont;

extern int fClipbrd;
extern int fStart;
extern int fCaret;
extern int fCtrl;
extern int fShift;
extern int fAlt;
extern int fMatchBracket;

extern HANDLE hPenBlack, hPenWhite, hPenGray, hBrushWhite, hBrushLtGray, hPenSysGray, hBrushSysGray;

extern struct editrec *ed;

extern struct colorset clrset[ ];       // defined in vdeo_win.c

extern int insmode;
extern int blck;
extern struct coords blckstr, blckend;

extern struct clpbrec clpbord[ ];


int fButtonDown = 0;
short fMouseRepeat = 0;

HFONT hFont, hFontBold;


long FAR PASCAL EditWndProc( HWND hwnd, unsigned message, WPARAM wParam, LPARAM lParam )  {
    RECT rectl;
    PAINTSTRUCT ps;
    HDC hdc;
    TEXTMETRIC tm;
    HBRUSH hBrush;
    int cx, cy;
    static HWND hwndNextClipboardViewer;


    //hwndClient = hwnd;
    switch (message) {

        case WM_USER+5:
            hDC = GetDC( hwnd );

            GetTextMetrics( hDC, &tm );
            cyChar = tm.tmHeight - tm.tmExternalLeading;
            cxChar = tm.tmAveCharWidth;

            GetClientRect( hwndClient, &rectl );

            cx = rectl.right - rectl.left;
            cy = rectl.bottom - rectl.top;

            cyScrn = ( cy - 1 - CE_SZE_VSCROLL )/ cyChar;
            cyScrn += ((( cy - 1 - CE_SZE_VSCROLL ) - ( cyScrn * cyChar )) > cyChar / 2 ? 1 : 0 );

            cxScrn = ( cx - CE_SZE_HSCROLL )/ cxChar + (( cx - CE_SZE_HSCROLL )% cxChar ? 1 : 0 );

            scrn_resize( cxScrn, cyScrn );
            InvalidateRect( hwndClient, NULL, TRUE );
            return 0;

        case WM_CHANGECBCHAIN:
            if(( HWND )wParam == hwndNextClipboardViewer )
                hwndNextClipboardViewer = ( HWND )lParam;       //  If the next window is closing, repair the chain.
            else if( hwndNextClipboardViewer != NULL )
                SendMessage( hwndNextClipboardViewer, message, wParam, lParam );
            break;

        case WM_DRAWCLIPBOARD:
            if( GetFocus( ) != hwnd )  {
                if( IsClipboardFormatAvailable( CF_TEXT )|| IsClipboardFormatAvailable( CF_OEMTEXT ))  {
                    fClipbrd = -1;
                }
            }
            SendMessage( hwndNextClipboardViewer, message, wParam, lParam );
            break;

        case WM_CREATE:
            hwndNextClipboardViewer = SetClipboardViewer( hwnd );
            hDC = GetDC( hwnd );
            hFont = NULL;
            if( fScrnFont )  {
                hFont = CreateFont( lfScrnFont.lfHeight, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, lfScrnFont.lfFaceName );
                hFontBold = CreateFont( lfScrnFont.lfHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, lfScrnFont.lfFaceName );
            }
            if( hFont == NULL )  {
                hFont = CreateFont( -12, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, "Courier" );
                hFontBold = CreateFont( -12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, "Courier" );
            }
            if( hFont == NULL )  {
                hFont = (HFONT)GetStockObject( SYSTEM_FIXED_FONT );
                hFontBold = hFont;
            }
            SelectObject( hDC, hFont );
 
            GetTextMetrics( hDC, &tm );
            cyChar = ( int )( tm.tmHeight - tm.tmExternalLeading );
            cxChar = ( int )( tm.tmAveCharWidth );

            SetTextColor( hDC, clrset[ CE_CLR_NORMAL ].fore );
            SetBkColor( hDC, clrset[ CE_CLR_NORMAL ].back );

            hwndVScroll = CreateWindow( "scrollbar", "", WS_CHILD | SBS_VERT, 0, 0, 0, 0, hwnd, ( HMENU )IDC_VSCROLL, hInst, NULL );
            hwndHScroll = CreateWindow( "scrollbar", "", WS_CHILD | SBS_HORZ, 0, 0, 0, 0, hwnd, ( HMENU )IDC_VSCROLL, hInst, NULL );
            hwndScrollF = CreateWindow( "ceWinScrollFill", "", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hwnd, ( HMENU )4568, hInst, NULL );
            SetScrollRange( hwndHScroll, SB_CTL, 0, 2000, FALSE );

            return( 0 );

        case WM_SIZE:
            if( wParam == SIZE_MINIMIZED )  return( 0 );

            cx = ( int )LOWORD( lParam );
            cy = ( int )HIWORD( lParam );

            if( cx == 0 )  return 0;

            cyScrn = ( cy - 1 - CE_SZE_VSCROLL )/ cyChar;
            cyScrn += ((( cy - 1 - CE_SZE_VSCROLL ) - ( cyScrn * cyChar )) > cyChar / 2 ? 1 : 0 );

            cxScrn = ( cx - CE_SZE_HSCROLL )/ cxChar + (( cx - CE_SZE_HSCROLL )% cxChar ? 1 : 0 );

            SetWindowPos( hwndVScroll, NULL, cx - CE_SZE_HSCROLL - 2, 1, CE_SZE_HSCROLL, cy - CE_SZE_VSCROLL - 1, SWP_SHOWWINDOW | SWP_NOZORDER );

            SetWindowPos( hwndHScroll, NULL, 1, cy - CE_SZE_VSCROLL, cx - CE_SZE_HSCROLL - 2, CE_SZE_VSCROLL, SWP_SHOWWINDOW | SWP_NOZORDER );

            SetWindowPos( hwndScrollF, NULL, cx - CE_SZE_HSCROLL - 1, cy - CE_SZE_VSCROLL, CE_SZE_HSCROLL, CE_SZE_VSCROLL, SWP_NOZORDER | SWP_DEFERERASE | SWP_NOACTIVATE );

            if( fStart )  {
                scrn_resize( cxScrn, cyScrn );
            }
            else  {
                scrn_x = cxScrn;
            }
            return( 0 );

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect( hwnd, &rectl );

#ifdef _WIN32
            SelectObject( hdc, hPenBlack );
#else
            SelectObject( hdc, hPenWhite );
#endif
            MoveToEx( hdc, rectl.left, rectl.top, NULL );
            LineTo( hdc, rectl.right - 2, rectl.top );
            MoveToEx( hdc, rectl.left, rectl.top, NULL );
            LineTo( hdc, rectl.left, rectl.bottom );

            rectl.right -= 2;
            rectl.left = rectl.right - CE_SZE_HSCROLL;
            rectl.top = rectl.bottom - CE_SZE_VSCROLL;
            FillRect( hdc, &rectl, ( HBRUSH )hBrushSysGray );
 
            GetClientRect( hwnd, &rectl );   // !!!! Store rectl then do not make this call
            if( fStart )  {
                scrn_statsl();
                scrn_rescrn();
            }

            hBrush = ( HBRUSH )( CreateSolidBrush( clrset[ CE_CLR_NORMAL ].back));
            rectl.top = cyScrn * cyChar + 1;
            rectl.left += 1;
            FillRect( hdc, &rectl, hBrush );

            GetClientRect( hwnd, &rectl );   // !!!! Store rectl then do not make this call
            rectl.top += 1;
            rectl.left += 1;
            rectl.right = rectl.left + 2;
            FillRect( hdc, &rectl, hBrush );

            GetClientRect( hwnd, &rectl );   // !!!! Store rectl then do not make this call
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.right - 1, rectl.top, NULL );
            LineTo( hdc, rectl.right - 1, rectl.bottom );

            SelectObject( hdc, hPenSysGray );
            MoveToEx( hdc, rectl.right - 2, rectl.top, NULL );
            LineTo( hdc, rectl.right - 2, rectl.bottom );

            EndPaint( hwnd, &ps );
            return( 0 );

        case WM_SETFOCUS:
            CreateCaret( hwnd, 0, insmode ? 2 : 8, cyChar + 2 );
            if( fCaret )  {
            //if( ! wParam )  {
                if(( ! blck )&&( fMatchBracket ))  ShowMatchBracket();
                ShowCaret( hwnd );
            }
            if( ed )  {
                HTREEITEM hTreeItem;
                TVITEM tvi;
                TVITEMDATA *tvidata;

                if( hTreeItem = TreeView_GetSelection( hwndTree ))  {
                    tvi.hItem = hTreeItem;
                    tvi.mask = TVIF_PARAM;
                    TreeView_GetItem( hwndTree, &tvi );
                    tvidata = ( TVITEMDATA * )tvi.lParam;
 
                    if( tvidata->iLevel > 1 )  {
                        if( ed != tvidata->ed )
                            TreeItemSel( ed->hTreeItem, 0 );
                    }
                    else  {
                        TreeItemSel( ed->hTreeItem, 0 );
                    }
                }
            }
            return( 0 );

        case WM_KILLFOCUS:
            fCtrl = 0;
            fShift = 0;
            DestroyCaret( );
            HideMatchBracket();
            return( 0 );

        case WM_SYSCOMMAND:
            return( DefWindowProc( hwnd, message, wParam, lParam ));

        case WM_CHAR:
            WMChar( hwnd, ( char )wParam );
            return( 0 );

        case WM_KEYDOWN:
            WMKeyDn( hwnd, ( WORD )wParam, ( LONG )lParam );
            return( 0 );

        case WM_KEYUP:
            WMKeyUp( hwnd, ( WORD )wParam, ( LONG )lParam );
            return( 0 );

        case WM_SYSCHAR:
            if( WMSysChar(  hwnd, ( char )wParam ))  return( 0 );
            break;

        case WM_SYSKEYDOWN:
            if( WMSysKeyDn( hwnd, ( WORD )wParam, ( LONG )lParam ))  return(0);
            break;

        case WM_SYSKEYUP:
            if( WMSysKeyUp( hwnd, ( WORD )wParam, ( LONG )lParam ))  return( 0 );
            break;

        case WM_RBUTTONDOWN:
            SetFocus(hwnd);
            break;

        case WM_LBUTTONDOWN:
            SetFocus(hwnd);
            fButtonDown = -1;
            fMouseRepeat = 0;
            WMLButtonDown( hwnd, LOWORD( lParam ), HIWORD( lParam ));
            break;

        case WM_LBUTTONUP:
            fButtonDown = 0;
            fMouseRepeat = 0;
            break;

        case WM_MOUSEMOVE:
            if( fButtonDown )  WMMouseMove( hwnd, wParam, LOWORD( lParam ), HIWORD( lParam ));
            break;

        case WM_LBUTTONDBLCLK:
            fButtonDown = -1;
            WMLButtonDblclk( hwnd, LOWORD( lParam ), HIWORD( lParam ));
            break;
 
        case WM_VSCROLL:
            WMScroll( hwnd, wParam, lParam );
            return( 0 );

        case WM_HSCROLL:
            WMHScroll( hwnd, wParam, lParam );
            return( 0 );

        case WM_CONTEXTMENU:
            if ((HWND)wParam ==  hwnd )  {
                HMENU hMenuCtx;
                HMENU hMenu;

                if( !( hMenu = LoadMenu ( hInst, "CtxMenu")))
                    break;
 
                hMenuCtx = GetSubMenu( hMenu, 0 );
 
                TrackPopupMenu( hMenuCtx, TPM_LEFTALIGN | TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam), 0, hwnd, NULL);
 
                DestroyMenu( hMenu );
            }
            break;

        case WM_INITMENU:
            //hMenu = GetMenu( hwnd );

            EnableMenuItem(( HMENU )wParam, IDM_EDIT_UNDO, ( ed->undocnt ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
            EnableMenuItem(( HMENU )wParam, IDM_EDIT_REDO, ( ed->redocnt ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
 
            EnableMenuItem(( HMENU )wParam, IDM_EDIT_CUT, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
            EnableMenuItem(( HMENU )wParam, IDM_EDIT_COPY, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

            EnableMenuItem(( HMENU )wParam, IDM_EDIT_PASTE, (( clpbord[ 0 ].type || fClipbrd ) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
            EnableMenuItem(( HMENU )wParam, IDM_EDIT_PASTEBLOCK, (( clpbord[ 0 ].type || fClipbrd ) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
            EnableMenuItem(( HMENU )wParam, IDM_EDIT_DELETE, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
            EnableMenuItem(( HMENU )wParam, IDM_EDIT_FILL, (( blck &( !( blckstr.x == blckend.x ))) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
            EnableMenuItem(( HMENU )wParam, IDM_EDIT_UPCASE, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
            EnableMenuItem(( HMENU )wParam, IDM_EDIT_LOCASE, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
            EnableMenuItem(( HMENU )wParam, IDM_EDIT_INVERTCASE, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

            return( DefWindowProc( hwnd, message, wParam, lParam ));

        case WM_COMMAND:
            WMCommand( hwnd, wParam );
            return( 0 );

        case WM_RENDERALLFORMATS:       // I don't understand why these messages WAS send here, rather than to hwndMain which WAS set as the clioboard viewer
            OpenClipboard( hwnd );
            EmptyClipboard( );
            ClipbrdCopy( hwnd );
            CloseClipboard( );
            break;

        case WM_RENDERFORMAT:
            ClipbrdCopy( hwnd );
            break;
 
        case WM_DESTROY:
            ChangeClipboardChain( hwnd, hwndNextClipboardViewer );
            clpb_freeall();
            break;

        default:
            return( DefWindowProc( hwnd, message, wParam, lParam ));
    }
    return( DefWindowProc( hwnd, message, wParam, lParam ));
}



WMScroll( HWND hwnd, WPARAM wPrm, LPARAM lParam )  {
    int rc;

#ifdef WIN32
    switch(( int ) LOWORD( wPrm ))  {
#else
    switch( wPrm )  {
#endif
        case SB_LINEUP:
            blck_unmark();
            HideCaret( NULL );
            cmnd_scroll_up();
            ShowCaret( NULL );
            break;

        case SB_LINEDOWN:
            blck_unmark();
            HideCaret( NULL );
            cmnd_scroll_down();
            ShowCaret( NULL );
            break;

        case SB_PAGEUP:
            blck_unmark();
            HideCaret( NULL );
            cmnd_page_up();
            ShowCaret( NULL );
            break;

        case SB_PAGEDOWN:
            blck_unmark();
            HideCaret( NULL );
            cmnd_page_down();
            ShowCaret( NULL );
            break;

        //case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            blck_unmark();
            HideCaret( NULL );
#ifdef WIN32
            cmnd_goto_displ(( long )HIWORD( wPrm ));
#else
            cmnd_goto_displ(( long )LOWORD( lParam ));
#endif
            ShowCaret( NULL );
            break;

        default:
            return( 0 );
    }
    SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );
}


WMHScroll( HWND hwnd, WPARAM wPrm, LPARAM lParam )  {

#ifdef WIN32
    switch(( int ) LOWORD( wPrm ))  {
#else
    switch( wPrm )  {
#endif
        case SB_LINELEFT:
            blck_unmark();
            HideCaret( NULL );
            cmnd_scroll_left();
            ShowCaret( NULL );
            break;

        case SB_LINERIGHT:
            blck_unmark();
            HideCaret( NULL );
            cmnd_scroll_right();
            ShowCaret( NULL );
            break;

        case SB_PAGELEFT:
            blck_unmark();
            HideCaret( NULL );
            cmnd_page_left();
            ShowCaret( NULL );
            break;

        case SB_PAGERIGHT:
            blck_unmark();
            HideCaret( NULL );
            cmnd_page_right();
            ShowCaret( NULL );
            break;

        //case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            blck_unmark();
            HideCaret( NULL );
#ifdef WIN32
            cmnd_goto_dispx(( long )HIWORD( wPrm ));
#else
            cmnd_goto_dispx(( long )LOWORD( lParam ));
#endif
            ShowCaret( NULL );
            break;

        default:
            return( 0 );
    }
    SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );
}


#ifdef __cplusplus
}
#endif
