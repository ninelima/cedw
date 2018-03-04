
#include "windows.h"

#include "string.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"

extern HWND hwndMain, hwndStatus;

extern HANDLE hPenBlack, hPenWhite, hPenGray, hBrushWhite, hBrushLtGray, hPenSysGray, hBrushSysGray;

extern char szMainTitle[ ];

extern int fTitleFileName;


char szStatusFileName[ CE_LEN_FILEPATH ];
char szStatusFileNo[ 5 ] = "0000";
char szStatusLineNo[ 7 ] = "000000";
char szStatusColNo[ 5 ] = "0000";
int fStatusInsmode = -1;
int fStatusModified = 0;
int fStatusFiletype = 0x03;     // ?? fileos
 
 
stsl_fname( char *fname, char *fno )  {
    RECT r;
    char szTitle[ CE_LEN_FILEPATH + 20 ];

    szStatusFileName[ 0 ] = '\0';

    GetClientRect( hwndStatus, &r );

    strcpy( szStatusFileNo, fno );

    r.top = 5;
    r.left = r.right - 46;
    InvalidateRect( hwndStatus, &r, TRUE );

    if( fTitleFileName )  {
        strcpy( szTitle, szMainTitle );
        if( *fname )  {
            strcat( szTitle, "   [" );
            strcat( szTitle, fname );
            strcat( szTitle, "]" );
        }
        SetWindowText( hwndMain, szTitle );
    }
    else  {
        strcpy( szStatusFileName, fname );
        r.top = 5;
        r.left = 4;
        r.right -= 240;
        InvalidateRect( hwndStatus, &r, TRUE );
    }
    UpdateWindow( hwndStatus );
}


stsl_crsrpos( char *l, char *c )  {
    RECT r;

    strcpy( szStatusLineNo, l );
    strcpy( szStatusColNo, c );
    GetClientRect( hwndStatus, &r );
    r.top = 5;
    r.left = r.right - 128;
    r.right -= 56;
    InvalidateRect( hwndStatus, &r, FALSE );
}


stsl_insmode( int im )  {
    RECT r;

    if( fStatusInsmode != im )  {
        fStatusInsmode = im;
        GetClientRect( hwndStatus, &r );
        r.top = 5;
        r.left = r.right - 165;
        r.right -= 139;
        InvalidateRect( hwndStatus, &r, TRUE );
    }
}


stsl_fmod( int fm )  {
    RECT r;

    if( fStatusModified != fm )  {
        fStatusModified = fm;
        GetClientRect( hwndStatus, &r );
        r.top = 5;
        r.left = r.right - 199;
        r.right -= 173;
        InvalidateRect( hwndStatus, &r, TRUE );
    }
}


stsl_ostype( int ft )  {
    RECT r;

    if( fStatusFiletype != ft )  {
        fStatusFiletype = ft;
        GetClientRect( hwndStatus, &r );
        r.top = 5;
        r.left = r.right - 233;
        r.right -= 207;
        InvalidateRect( hwndStatus, &r, TRUE );
    }
}



long FAR PASCAL StatusWndProc( HWND hwnd, unsigned message, WPARAM wParam, LPARAM lParam )  {
    HANDLE hOld;
    RECT rectl, rectlDT;
    HDC hdc;
    PAINTSTRUCT ps;
    HFONT hFontStatus;
    SIZE size;


    switch (message) {
        case WM_CREATE:
            hdc = GetDC( hwnd );
            SetTextColor( hdc, PALETTERGB( 0, 0, 0 ));
            //SetBkColor( hdc, PALETTERGB( 192, 192, 192 ));

            SetBkColor( hdc, GetSysColor( COLOR_MENU ));

            hFontStatus = CreateFont( 8, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Helv" );
            //hFontStatus = CreateFont( 8, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif" );
            SelectObject( hdc, hFontStatus );

            return( 0 );

        case WM_PAINT:

            hdc = BeginPaint( hwnd, &ps );
            GetClientRect( hwnd, &rectl );

            hOld = SelectObject( hdc, hPenSysGray );
            MoveToEx( hdc, rectl.left, rectl.top, NULL );
            LineTo( hdc, rectl.right - 2, rectl.top );

            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.left, rectl.top + 1, NULL );
            LineTo( hdc, rectl.right, rectl.top + 1 );

            rectl.top += 2;
            //hPen = GetStockObject( GRAY_BRUSH );
            //FillRect( hdc, &rectl, hBrushSysGray );
            //DeleteObject( hPen );
            rectl.top -= 2;


 
            /*SelectObject( hdc, hPenGray );
            MoveToEx( hdc, rectl.left + 2, rectl.bottom, NULL );
            LineTo( hdc, rectl.left + 2, rectl.top + 4  );
            LineTo( hdc, rectl.right - 240, rectl.top + 4  );
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.left + 2, rectl.bottom - 1, NULL );
            LineTo( hdc, rectl.right - 240, rectl.bottom - 1 );
            LineTo( hdc, rectl.right - 240, rectl.top + 4 );*/




            SelectObject( hdc, hPenGray );
            MoveToEx( hdc, rectl.right - 168, rectl.bottom, NULL );
            LineTo( hdc, rectl.right - 168, rectl.top + 4  );
            LineTo( hdc, rectl.right - 138, rectl.top + 4  );
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.right - 168, rectl.bottom - 1, NULL );
            LineTo( hdc, rectl.right - 138, rectl.bottom - 1 );
            LineTo( hdc, rectl.right - 138, rectl.top + 4 );

            SelectObject( hdc, hPenGray );
            MoveToEx( hdc, rectl.right - 202, rectl.bottom, NULL );
            LineTo( hdc, rectl.right - 202, rectl.top + 4  );
            LineTo( hdc, rectl.right - 172, rectl.top + 4  );
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.right - 202, rectl.bottom - 1, NULL );
            LineTo( hdc, rectl.right - 172, rectl.bottom - 1 );
            LineTo( hdc, rectl.right - 172, rectl.top + 4 );

            SelectObject( hdc, hPenGray );
            MoveToEx( hdc, rectl.right - 236, rectl.bottom, NULL );
            LineTo( hdc, rectl.right - 236, rectl.top + 4  );
            LineTo( hdc, rectl.right - 206, rectl.top + 4  );
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.right - 236, rectl.bottom - 1, NULL );
            LineTo( hdc, rectl.right - 206, rectl.bottom - 1 );
            LineTo( hdc, rectl.right - 206, rectl.top + 4 );


            SelectObject( hdc, hPenGray );
            MoveToEx( hdc, rectl.right - 50, rectl.bottom, NULL );
            LineTo( hdc, rectl.right - 50, rectl.top + 4  );
            LineTo( hdc, rectl.right - 18, rectl.top + 4  );
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.right - 50, rectl.bottom - 1, NULL );
            LineTo( hdc, rectl.right - 18, rectl.bottom - 1 );
            LineTo( hdc, rectl.right - 18, rectl.top + 4 );

            /*=====
            SelectObject( hdc, hPenGray );
            MoveToEx( hdc, rectl.right - 86, rectl.bottom, NULL );
            LineTo( hdc, rectl.right - 86, rectl.top + 4  );
            LineTo( hdc, rectl.right - 54, rectl.top + 4  );
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.right - 86, rectl.bottom - 1, NULL );
            LineTo( hdc, rectl.right - 54, rectl.bottom - 1 );
            LineTo( hdc, rectl.right - 54, rectl.top + 4 );

            SelectObject( hdc, hPenGray );
            MoveToEx( hdc, rectl.right - 134, rectl.bottom, NULL );
            LineTo( hdc, rectl.right - 134, rectl.top + 4  );
            LineTo( hdc, rectl.right - 90, rectl.top + 4  );
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.right - 134, rectl.bottom - 1, NULL );
            LineTo( hdc, rectl.right - 90, rectl.bottom - 1 );
            LineTo( hdc, rectl.right - 90, rectl.top + 4 );
            =====*/

            SelectObject( hdc, hPenGray );
            MoveToEx( hdc, rectl.right - 134, rectl.bottom, NULL );
            LineTo( hdc, rectl.right - 134, rectl.top + 4  );
            LineTo( hdc, rectl.right - 54, rectl.top + 4  );
            SelectObject( hdc, hPenWhite );
            MoveToEx( hdc, rectl.right - 134, rectl.bottom - 1, NULL );
            LineTo( hdc, rectl.right - 54, rectl.bottom - 1 );
            LineTo( hdc, rectl.right - 54, rectl.top + 4 );


            SelectObject( hdc, hOld );

            TextOut( hdc, rectl.right - 46, 5, szStatusFileNo, strlen( szStatusFileNo ));

            GetTextExtentPoint( hdc, szStatusFileName, strlen( szStatusFileName ), &size );

            rectlDT.top = 4;
            rectlDT.left = 4;
            rectlDT.right = rectl.right - 252;
            rectlDT.bottom = rectl.bottom;
            DrawText( hdc, szStatusFileName, strlen( szStatusFileName ), &rectlDT, ( size.cx > rectl.right - 240 ) ? DT_RIGHT : DT_LEFT );
            //TextOut( hdc, rectl.left + 4, 4, szStatusFileName, strlen( szStatusFileName ));

            TextOut( hdc, rectl.right - 130, 5, szStatusLineNo, strlen( szStatusLineNo ));
            TextOut( hdc, rectl.right - 82, 5, szStatusColNo, strlen( szStatusColNo ));

            TextOut( hdc, rectl.right - 165, 5, fStatusInsmode ? "Ins  " : "Ovr  ", 5 );

            if( fStatusModified == 1 )
                    TextOut( hdc, rectl.right - 199, 5, "Read ", 5 );
            else
                TextOut( hdc, rectl.right - 199, 5, fStatusModified ? "Mod  ": "     ", 5 );

            TextOut( hdc, rectl.right - 233, 5, fStatusFiletype == 0x03 ? "Dos  " : "Unix ", 5 );

            EndPaint( hwnd, &ps );
            return( 0 );

        default:
            return( DefWindowProc( hwnd, message, wParam, lParam ));
        }

    return( DefWindowProc( hwnd, message, wParam, lParam ));
}




#ifdef __cplusplus
}
#endif
