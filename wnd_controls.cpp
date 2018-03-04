

#include "windows.h"

#include "string.h"

#include "resource.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"


extern HWND hwndMain, hwndClient, hwndTree, hwndStatus, hwndSizeBar;
extern HPEN hPenBlack, hPenWhite, hPenGray, hPenSysGray;
extern HBRUSH hBrushWhite, hBrushLtGray,  hBrushSysGray;

extern int cxFileTree;



long FAR PASCAL SizeBarWndProc( HWND hwnd, unsigned message, WPARAM wParam, LPARAM lParam )  {
    HDC hdc;
    PAINTSTRUCT ps;
    static int xFocusRect = -1;
    static int xOf, yOf, iHg, fMouseDn = 0;
    RECT rect;


    switch (message) {
        case WM_CREATE:
            return( 0 );

        case WM_PAINT:  {
                RECT rectl;
                int n;

                hdc = BeginPaint( hwnd, &ps );
                GetClientRect( hwnd, &rectl );

                SelectObject( hdc, hPenSysGray );
                MoveToEx( hdc, rectl.left, rectl.top, NULL );
                LineTo( hdc, rectl.left, rectl.bottom );
                SelectObject( hdc, hPenWhite );
                MoveToEx( hdc, rectl.left + 1, rectl.top, NULL );
                LineTo( hdc, rectl.left + 1, rectl.bottom );
                SelectObject( hdc, hPenSysGray );
                for( n = 2; n < CE_SZE_SIZEBAR; ++n )  {
                    MoveToEx( hdc, rectl.left + n, rectl.top, NULL );
                    LineTo( hdc, rectl.left + n, rectl.bottom );
                }
                SelectObject( hdc, hPenBlack );
                MoveToEx( hdc, rectl.left + CE_SZE_SIZEBAR, rectl.top, NULL );
                LineTo( hdc, rectl.left + CE_SZE_SIZEBAR, rectl.bottom );

                EndPaint( hwnd, &ps );
            }
            return( 0 );
 
        case WM_LBUTTONDOWN:

            SetCapture( hwndSizeBar );
            fMouseDn = TRUE;

            GetWindowRect( hwndSizeBar, &rect );
            xOf = rect.left;
            GetWindowRect( hwndMain, &rect );
            xOf -= rect.left;
            yOf = GetSystemMetrics( SM_CYFRAME ) + GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYMENU ) + 1;

            GetClientRect( hwndSizeBar, &rect );
            iHg = rect.bottom;
            break;

        case WM_LBUTTONUP:
            if( fMouseDn )  {
                ReleaseCapture();
                fMouseDn = FALSE;
                if( xFocusRect >= 0 )  {
                    RECT rectMain;

                    hdc = GetWindowDC( hwndMain );

                    rect.top = yOf;
                    rect.bottom = yOf + iHg;
 
                    rect.left = xFocusRect;
                    rect.right = rect.left + CE_SZE_SIZEBAR;
                    DrawFocusRect( hdc, &rect );

                    GetClientRect( hwndMain, &rectMain );

                    cxFileTree = xFocusRect - GetSystemMetrics( SM_CXFRAME );
                    //MoveWindow( hwndTree, 1, 1, cxFileTree - 1, rectMain.bottom - CE_SZE_STATUSBAR - 1, TRUE );
                    SetWindowPos( hwndTree, NULL, 1, 1, cxFileTree - 1, rectMain.bottom - CE_SZE_STATUSBAR - 1, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER );
                    //MoveWindow( hwndSizeBar, cxFileTree, 0, CE_SZE_SIZEBAR, rectMain.bottom - CE_SZE_STATUSBAR, TRUE );
                    SetWindowPos( hwndSizeBar, NULL, cxFileTree, 0, CE_SZE_SIZEBAR, rectMain.bottom - CE_SZE_STATUSBAR,  SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER );
                    //MoveWindow( hwndClient, cxFileTree + CE_SZE_SIZEBAR, 0, rectMain.right - ( cxFileTree + CE_SZE_SIZEBAR), rectMain.bottom - CE_SZE_STATUSBAR, TRUE );
                    SetWindowPos( hwndClient, NULL, cxFileTree + CE_SZE_SIZEBAR, 0, rectMain.right - ( cxFileTree + CE_SZE_SIZEBAR ), rectMain.bottom - CE_SZE_STATUSBAR, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE );  // get rid of flashing scrollbar on sizing to the left
                    SetWindowPos( hwndClient, NULL, cxFileTree + CE_SZE_SIZEBAR, 0, rectMain.right - ( cxFileTree + CE_SZE_SIZEBAR ), rectMain.bottom - CE_SZE_STATUSBAR, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER );

                    InvalidateRect( hwndTree, NULL, FALSE );
                    InvalidateRect( hwndSizeBar, NULL, FALSE );
                    InvalidateRect( hwndClient, NULL, FALSE );
                    xFocusRect = -1;
                }
            }
            break;
 
        case WM_MOUSEMOVE:  {
                RECT rectMain;
 
                if(( wParam == MK_LBUTTON )&&( fMouseDn ))  {
                    short z;
 
                    z = ( short )LOWORD( lParam );
 
                    hdc = GetWindowDC( hwndMain );
 
                    GetClientRect( hwndMain, &rectMain );
 
                    rect.top = yOf;
                    rect.bottom = yOf + iHg;
                    rect.left = xOf + z;
 
                    if(( rect.left < rectMain.right - 20 )&&( rect.left > 4 ))  {
                        if( xFocusRect >= 0 )  {
                            rect.left = xFocusRect;
                            rect.right = rect.left + CE_SZE_SIZEBAR;
                            DrawFocusRect( hdc, &rect );
                        }
                        rect.left = xOf + z;
                        rect.right = rect.left + CE_SZE_SIZEBAR;
                        DrawFocusRect( hdc, &rect );
                        xFocusRect = rect.left;
                    }
                }
            }
            break;

        default:
            return( DefWindowProc( hwnd, message, wParam, lParam ));
    }
    return( DefWindowProc( hwnd, message, wParam, lParam ));
}



long FAR PASCAL ScrollFillWndProc( HWND hwnd, unsigned message, WPARAM wParam, LPARAM lParam )  {
    RECT rectl;
    HDC hdc;
    PAINTSTRUCT ps;

    switch (message) {
        case WM_CREATE:
            return( 0 );

        case WM_PAINT:
            hdc = BeginPaint( hwnd, &ps );
            GetClientRect( hwnd, &rectl );

            FillRect( hdc, &rectl, ( HBRUSH )hBrushSysGray );

            EndPaint( hwnd, &ps );
            return( 0 );

        default:
            return( DefWindowProc( hwnd, message, wParam, lParam ));
    }
    return( DefWindowProc( hwnd, message, wParam, lParam ));
}


long FAR PASCAL SeperatorLineWndProc( HWND hwnd, unsigned message, WPARAM wParam, LPARAM lParam )  {
    RECT rectl;
    HDC hdc;
    PAINTSTRUCT ps;


    switch (message) {
        case WM_CREATE:
            //hdc = GetDC( hwnd );
            //SetBkColor( hdc, GetSysColor( COLOR_MENU ));
            return( 0 );

        case WM_PAINT:
            hdc = BeginPaint( hwnd, &ps );
            GetClientRect( hwnd, &rectl );

            SelectObject( hdc, hPenGray );
            MoveToEx( hdc, rectl.left, rectl.top, NULL );
            LineTo( hdc, rectl.right, rectl.top );
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.left, rectl.top+1, NULL );
            LineTo( hdc, rectl.right, rectl.top+1 );
 
            EndPaint( hwnd, &ps );
            return( 0 );

        default:
            return( DefWindowProc( hwnd, message, wParam, lParam ));
    }
    return( DefWindowProc( hwnd, message, wParam, lParam ));
}



long FAR PASCAL ColorSelectWndProc( HWND hwnd, unsigned message, WPARAM wParam, LPARAM lParam )  {
    RECT rectl;
    HDC hdc;
    PAINTSTRUCT ps;
    HBRUSH hBrush;


    switch (message)  {
        case WM_CREATE:
            SetWindowLong( hwnd, GWL_USERDATA, 0 );
            return( 0 );

        case WM_PAINT:
            hdc = BeginPaint( hwnd, &ps );
            GetClientRect( hwnd, &rectl );
            rectl.right -= 1;
            rectl.bottom -= 1;
 
            FillRect( hdc, &rectl, ( HBRUSH )hBrushWhite );

            SelectObject( hdc, hPenBlack );
            MoveToEx( hdc, rectl.left, rectl.bottom - 1, NULL );
            LineTo( hdc, rectl.left, rectl.top );
            LineTo( hdc, rectl.right, rectl.top );
            SelectObject( hdc, hPenWhite );
            LineTo( hdc, rectl.right, rectl.bottom );
            LineTo( hdc, rectl.left, rectl.bottom );

            rectl.top += 1;
            rectl.bottom -=1;
            rectl.right -= 1;
            rectl.left += 1;
            SelectObject( hdc, hPenSysGray );
            MoveToEx( hdc, rectl.left, rectl.bottom, NULL );
            LineTo( hdc, rectl.left, rectl.top );
            LineTo( hdc, rectl.right, rectl.top );
            LineTo( hdc, rectl.right, rectl.bottom );
            LineTo( hdc, rectl.left, rectl.bottom );
 
            rectl.top += 4;
            rectl.bottom -=3;
            rectl.right -= 3;
            rectl.left += 4;
 
            hBrush = CreateSolidBrush( GetWindowLong( hwnd, GWL_USERDATA ) );

            FillRect( hdc, &rectl, hBrush );
 
            EndPaint( hwnd, &ps );
            return( 0 );

        case WM_USER:
            SetWindowLong( hwnd, GWL_USERDATA, lParam );

            InvalidateRect( hwnd, NULL, FALSE );
            return 0;

        case WM_LBUTTONDOWN:
            PostMessage( GetParent( hwnd ), WM_USER, ( WPARAM )hwnd, 0 );
            break;

        default:
            return( DefWindowProc( hwnd, message, wParam, lParam ));
    }
    return( DefWindowProc( hwnd, message, wParam, lParam ));
}




#ifdef __cplusplus
}
#endif
