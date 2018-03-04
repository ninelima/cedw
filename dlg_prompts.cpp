

#include "windows.h"

#include "string.h"
#include "stdlib.h"

#include "resource.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"
#include "cedw.ver"


extern HINSTANCE hInst;
extern HWND hwndMain, hwndClient;
extern HWND hwndStatus;

extern int cxChar, cyChar;

extern int blck;
extern struct coords blckstr, blckend;


extern char szDlgMessage[ ];
extern char szDlgTitle[  ];
extern char szDlgFileName[  ];




BOOL CALLBACK YesNoDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {

    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( hwndDlg, szDlgTitle );
            SetWindowText( GetDlgItem( hwndDlg, IDC_YESNO_MSG ), szDlgMessage );
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case IDC_YESNO_YES:
                    EndDialog( hwndDlg, IDC_YESNO_YES );
                    return( TRUE );

                case IDC_YESNO_NO:
                    EndDialog( hwndDlg, IDC_YESNO_NO );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}





BOOL FAR PASCAL YesNoCanDlg( HWND hwndDlg, WORD message, WORD wParam, LONG lParam )  {
    POINT lpt;
    RECT lrc;

    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( hwndDlg, szDlgTitle );
            SetWindowText( GetDlgItem( hwndDlg, IDC_YESNO_MSG ), szDlgMessage );
            if(( blck )&&( blckstr.y == blckend.y ))  {
                GetCaretPos( &lpt );
                GetWindowRect( hwndClient, &lrc );
                SetWindowPos( hwndDlg, HWND_TOP,
                    //lrc.left + lpt.x,
                    lrc.top + 64,
                    lrc.top + lpt.y + ( cyChar * 2 ) + GetSystemMetrics( SM_CYFRAME ) + GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYMENU ),
                    0, 0, SWP_NOSIZE );
            }
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case IDC_YESNO_YES:
                    EndDialog( hwndDlg, IDC_YESNO_YES );
                    return( TRUE );

                case IDC_YESNO_NO:
                    EndDialog( hwndDlg, IDC_YESNO_NO );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, DID_CANCEL );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}




BOOL CALLBACK YesNoAllDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    POINT lpt;
    RECT lrc;

    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( hwndDlg, szDlgTitle );
            SetWindowText( GetDlgItem( hwndDlg, IDC_YESNOALL_MSG ), szDlgMessage );
            if(( blck )&&( blckstr.y == blckend.y ))  {
                GetCaretPos( &lpt );
                GetWindowRect( hwndClient, &lrc );
                SetWindowPos( hwndDlg, HWND_TOP,
                    //lrc.left + lpt.x,
                    lrc.left + 64,
                    lrc.top + lpt.y + ( cyChar * 2 ) + GetSystemMetrics( SM_CYFRAME ) + GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYMENU ),
                    0, 0, SWP_NOSIZE );
            }
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case IDC_YESNOALL_YES:
                    EndDialog( hwndDlg, IDC_YESNOALL_YES );
                    return( TRUE );

                case IDC_YESNOALL_NO:
                    EndDialog( hwndDlg, IDC_YESNOALL_NO );
                    return( TRUE );

                case IDC_YESNOALL_ALL:
                    EndDialog( hwndDlg, IDC_YESNOALL_ALL );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, DID_CANCEL );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}




BOOL CALLBACK OkCanDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {

    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( hwndDlg, szDlgTitle );
            SetWindowText( GetDlgItem( hwndDlg, IDC_OKCAN_MSG ), szDlgMessage );
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    EndDialog( hwndDlg, DID_OK );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, DID_CANCEL );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}




BOOL CALLBACK OkCanAllDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {

    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( hwndDlg, szDlgTitle );
            SetWindowText( GetDlgItem( hwndDlg, IDC_OKCANALL_MSG ), szDlgMessage );
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    EndDialog( hwndDlg, DID_OK );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, DID_CANCEL );
                    return( TRUE );

                case IDC_OKCANALL_ALL:
                    EndDialog( hwndDlg, IDC_OKCANALL_ALL );
                    return( TRUE );
 
                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}




BOOL CALLBACK OkDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {

    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( hwndDlg, szDlgTitle );
            SetWindowText( GetDlgItem( hwndDlg, IDC_OK_MSG ), szDlgMessage );
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    EndDialog( hwndDlg, TRUE );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, FALSE );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}




BOOL CALLBACK ErrDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    HDC hdc;
    SIZE size;
    RECT rectl, rectlb;
    int xo, xd;

    switch (message) {
            case WM_INITDIALOG:
                SetWindowText( GetDlgItem( hwndDlg, IDC_ERR_MSG ), szDlgMessage );

                if( szDlgMessage )  {
#ifdef _WIN32
                    hdc = GetDC( hwndStatus );      // Not Good !!!!!!!!!!!!
                    xo = (( 38 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#else
                    hdc = GetDC( hwndDlg );
                    xo = (( 30 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#endif
                    GetTextExtentPoint( hdc, szDlgMessage, strlen( szDlgMessage ), &size );
                    ReleaseDC( hwndDlg, hdc );
                    GetWindowRect( hwndDlg, &rectl );
                    if( size.cx + xo > rectl.right - rectl.left )  {
                        SetWindowPos( hwndDlg, HWND_TOP, 0, 0, size.cx + xo, rectl.bottom - rectl.top, SWP_NOMOVE );
                        xd = ( size.cx + xo ) - ( rectl.right - rectl.left );
                        GetWindowRect( GetDlgItem( hwndDlg, DID_OK ), &rectlb );
                        SetWindowPos(  GetDlgItem( hwndDlg, DID_OK ), HWND_TOP, rectlb.left - ( rectl.left + GetSystemMetrics( SM_CXDLGFRAME )) + xd,
                                                        rectlb.top - ( rectl.top +  GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYDLGFRAME ) ), 0, 0, SWP_NOSIZE );
                    }
                }
                return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    EndDialog( hwndDlg, TRUE );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, FALSE );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}



BOOL CALLBACK ProdInfoDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( GetDlgItem( hwndDlg,  IDC_PRODINFO_NAME), __TITLE__"  v"__VERSION__ );
            SetWindowText( GetDlgItem( hwndDlg,  IDC_PRODINFO_COPYRIGHT), __COPYRIGHT__  );
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    EndDialog( hwndDlg, TRUE );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, FALSE );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}





BOOL FAR PASCAL YesNoCanFileDlg( HWND hwndDlg, WORD message, WORD wParam, LONG lParam )  {
    HDC hdc;
    SIZE size;
    RECT rectl, rectlb;
    int xo, xd;

    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( hwndDlg, szDlgTitle );
            SetWindowText( GetDlgItem( hwndDlg, IDC_YESNO_MSG ), szDlgMessage );
            if( szDlgFileName )  {
                SetWindowText( GetDlgItem( hwndDlg, IDC_YESNO_FILENAME ), szDlgFileName );
#ifdef _WIN32
                hdc = GetDC( hwndStatus );      // Not Good !!!!!!!!!!!!
                xo = (( 38 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#else
                hdc = GetDC( hwndDlg );
                xo = (( 30 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#endif
                GetTextExtentPoint( hdc, szDlgFileName, strlen( szDlgFileName ), &size );
                ReleaseDC( hwndDlg, hdc );
                GetWindowRect( hwndDlg, &rectl );
                if( size.cx + xo > rectl.right - rectl.left )  {
                    xd = ( size.cx + xo ) - ( rectl.right - rectl.left );
                    SetWindowPos( hwndDlg, HWND_TOP, 0, 0, size.cx + xo, rectl.bottom - rectl.top, SWP_NOMOVE );
                    GetWindowRect( GetDlgItem( hwndDlg, DID_CANCEL ), &rectlb );
                    SetWindowPos(  GetDlgItem( hwndDlg, DID_CANCEL ), HWND_TOP, rectlb.left - ( rectl.left + GetSystemMetrics( SM_CXDLGFRAME )) + xd,
                                                    rectlb.top - ( rectl.top +  GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYDLGFRAME ) ), 0, 0, SWP_NOSIZE );
                    GetWindowRect( GetDlgItem( hwndDlg, IDC_YESNO_NO ), &rectlb );
                    SetWindowPos(  GetDlgItem( hwndDlg, IDC_YESNO_NO ), HWND_TOP, rectlb.left - ( rectl.left + GetSystemMetrics( SM_CXDLGFRAME )) + xd,
                                                    rectlb.top - ( rectl.top +  GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYDLGFRAME ) ), 0, 0, SWP_NOSIZE );
                    GetWindowRect( GetDlgItem( hwndDlg, IDC_YESNO_YES ), &rectlb );
                    SetWindowPos(  GetDlgItem( hwndDlg, IDC_YESNO_YES ), HWND_TOP, rectlb.left - ( rectl.left + GetSystemMetrics( SM_CXDLGFRAME )) + xd,
                                                    rectlb.top - ( rectl.top +  GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYDLGFRAME ) ), 0, 0, SWP_NOSIZE );
                }

            }
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case IDC_YESNO_YES:
                    EndDialog( hwndDlg, IDC_YESNO_YES );
                    return( TRUE );

                case IDC_YESNO_NO:
                    EndDialog( hwndDlg, IDC_YESNO_NO );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, DID_CANCEL );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}



BOOL CALLBACK OkCanFileDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    HDC hdc;
    SIZE size;
    RECT rectl, rectlb;
    int xo, xd;
 
    switch (message) {
        case WM_INITDIALOG:
                SetWindowText( hwndDlg, szDlgTitle );
                SetWindowText( GetDlgItem( hwndDlg, IDC_OKCAN_MSG ), szDlgMessage );
                if( szDlgFileName )  {
                    SetWindowText( GetDlgItem( hwndDlg, IDC_OKCAN_FILENAME ), szDlgFileName );
#ifdef _WIN32
                    hdc = GetDC( hwndStatus );      // Not Good !!!!!!!!!!!!
                    xo = (( 38 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#else
                    hdc = GetDC( hwndDlg );
                    xo = (( 30 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#endif
                    GetTextExtentPoint( hdc, szDlgFileName, strlen( szDlgFileName ), &size );
                    ReleaseDC( hwndDlg, hdc );
                    GetWindowRect( hwndDlg, &rectl );
                    if( size.cx + xo > rectl.right - rectl.left )  {
                        xd = ( size.cx + xo ) - ( rectl.right - rectl.left );
                        SetWindowPos( hwndDlg, HWND_TOP, 0, 0, size.cx + xo, rectl.bottom - rectl.top, SWP_NOMOVE );
                        GetWindowRect( GetDlgItem( hwndDlg, DID_CANCEL ), &rectlb );
                        SetWindowPos(  GetDlgItem( hwndDlg, DID_CANCEL ), HWND_TOP, rectlb.left - ( rectl.left + GetSystemMetrics( SM_CXDLGFRAME )) + xd,
                                                        rectlb.top - ( rectl.top +  GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYDLGFRAME ) ), 0, 0, SWP_NOSIZE );
                        GetWindowRect( GetDlgItem( hwndDlg, DID_OK ), &rectlb );
                        SetWindowPos(  GetDlgItem( hwndDlg, DID_OK ), HWND_TOP, rectlb.left - ( rectl.left + GetSystemMetrics( SM_CXDLGFRAME )) + xd,
                                                        rectlb.top - ( rectl.top +  GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYDLGFRAME ) ), 0, 0, SWP_NOSIZE );
                    }
                }
                return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    EndDialog( hwndDlg, DID_OK );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, DID_CANCEL );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}




BOOL CALLBACK ErrFileDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    HDC hdc;
    SIZE size, size1;
    RECT rectl, rectlb;
    int xo, xd;

    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( GetDlgItem( hwndDlg, IDC_ERR_MSG ), szDlgMessage );
            if( szDlgFileName )  {
                SetWindowText( GetDlgItem( hwndDlg, IDC_ERR_FILENAME ), szDlgFileName );
#ifdef _WIN32
                hdc = GetDC( hwndStatus );      // Not Good !!!!!!!!!!!!
                xo = (( 38 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#else
                hdc = GetDC( hwndDlg );
                xo = (( 30 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#endif
                GetTextExtentPoint( hdc, szDlgFileName, strlen( szDlgFileName ), &size );
                GetTextExtentPoint( hdc, szDlgMessage, strlen( szDlgMessage ), &size1 );
                ReleaseDC( hwndDlg, hdc );
                if( size1.cx > size.cx )  size.cx = size1.cx;
                GetWindowRect( hwndDlg, &rectl );
                if( size.cx + xo > rectl.right - rectl.left )  {
                    xd = ( size.cx + xo ) - ( rectl.right - rectl.left );
                    SetWindowPos( hwndDlg, HWND_TOP, 0, 0, size.cx + xo, rectl.bottom - rectl.top, SWP_NOMOVE );
                    GetWindowRect( GetDlgItem( hwndDlg, DID_OK ), &rectlb );
                    SetWindowPos(  GetDlgItem( hwndDlg, DID_OK ), HWND_TOP, rectlb.left - ( rectl.left + GetSystemMetrics( SM_CXDLGFRAME )) + xd,
                                                    rectlb.top - ( rectl.top +  GetSystemMetrics( SM_CYCAPTION ) + GetSystemMetrics( SM_CYDLGFRAME ) ), 0, 0, SWP_NOSIZE );
                }
            }
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    EndDialog( hwndDlg, TRUE );
                    return( TRUE );

                case DID_CANCEL:
                    EndDialog( hwndDlg, FALSE );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}




BOOL CALLBACK HelpKeysDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    char sz[ 200 ];
    int n;
#ifdef _WIN32
    long alTabStops[ 2 ];
#else
    int alTabStops[ 2 ];
#endif

    switch (message) {
        case WM_INITDIALOG:
            alTabStops[ 0 ] = 96;

            SendMessage( GetDlgItem( hwndDlg, IDC_HELPKEYS_LIST ), LB_SETTABSTOPS, 1, ( LPARAM )alTabStops );

            for( n = IDS_HELPKEYS_FIRST; n <= IDS_HELPKEYS_LAST; ++n )  {
                LoadString( ( HINSTANCE )hInst, ( UINT )n, sz, 200 );
                SendMessage( GetDlgItem( hwndDlg, IDC_HELPKEYS_LIST ), LB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( sz ));
            }
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                /*case DID_OK:
                    EndDialog( hwndDlg, TRUE );
                    return( TRUE );*/

                case DID_CANCEL:
                    EndDialog( hwndDlg, FALSE );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}




#ifdef __cplusplus
}
#endif
