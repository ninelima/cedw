
#include "windows.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "resource.h"

#ifdef __cplusplus 
extern "C"{ 
#endif   

#include "ced.h"
#include "cedw.h"


extern HINSTANCE hInst;
extern HWND hwndMain, hwndClient;
extern HWND hwndDlgActive;

extern int fExplorerFileDlg;

extern char filename[ ]; 
extern char dlgtitle[ ];

extern int fBrowse;
extern int fMerge;
extern int fMultiFile;

extern int ostype;

extern int fnhcnt;
extern char fnhlst[ CE_MAX_FILENHIST ][ CE_LEN_FILEPATH ];

extern struct editrec *ed;

extern char asstype[ ][ CE_LEN_ASSTYPE ];
extern int asscnt;
struct assrec asslst[];
extern int ostype;
extern int assocdef;


/*
typedef struct tagOFN { 
  DWORD         lStructSize; 
  HWND          hwndOwner; 
  HINSTANCE     hInstance; 
  LPCTSTR       lpstrFilter; 
  LPTSTR        lpstrCustomFilter; 
  DWORD         nMaxCustFilter; 
  DWORD         nFilterIndex; 
  LPTSTR        lpstrFile; 
  DWORD         nMaxFile; 
  LPTSTR        lpstrFileTitle; 
  DWORD         nMaxFileTitle; 
  LPCTSTR       lpstrInitialDir; 
  LPCTSTR       lpstrTitle; 
  DWORD         Flags; 
  WORD          nFileOffset; 
  WORD          nFileExtension; 
  LPCTSTR       lpstrDefExt; 
  LPARAM        lCustData; 
  LPOFNHOOKPROC lpfnHook; 
  LPCTSTR       lpTemplateName; 
//#if (_WIN32_WINNT >= 0x0500)
  void *        pvReserved;
  DWORD         dwReserved;
  DWORD         FlagsEx;
//#endif // (_WIN32_WINNT >= 0x0500)
} OPENFILENAMEXX, *LPOPENFILENAMEXX;
*/


LONG FAR PASCAL ComboEditSubClassFunc( HWND hwnd, WORD Message, WPARAM wParam, LPARAM lParam );
WNDPROC lpfnOldComboEditWndProc;
HWND hwndCombo, hwndComboEdit;


LONG FAR PASCAL ComboEditSubClassFunc( HWND hwnd, WORD message, WPARAM wParam, LPARAM lParam)  {

    if ( message == WM_GETDLGCODE )  {
        //return( DLGC_WANTCHARS | DLGC_WANTARROWS );
    }
    if( message == WM_CHAR )  {
        if( wParam == VK_SPACE )  {
            if( GetKeyState( VK_SHIFT ) & 0x80 )  {
                int sl;
 
                GetWindowText( GetParent( hwnd ), filename, CE_LEN_FILEPATH );
                sl = strlen( filename );
                if( sl )  {
                    file_nxpand( filename );
                    sl = strlen( filename );
                    SetWindowText( GetParent( hwnd ), filename );
                    SendMessage( GetParent( hwnd ), CB_SETEDITSEL, 0, MAKELONG( sl, sl ));
                    return( TRUE );
                }

            }
        }
    }
    return CallWindowProc( lpfnOldComboEditWndProc, hwnd, message, wParam, lParam );
}



BOOL CALLBACK FileDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    int n;
    POINT pt;
    int iSel;


    switch (message) {
        case WM_INITDIALOG:
            hwndDlgActive = hwndDlg;
            SetWindowText( hwndDlg, dlgtitle);
            SetWindowText( GetDlgItem( hwndDlg, IDC_FILE_NAME ), filename );
            SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFAULT ), CB_ADDSTRING, 0, ( LPARAM )( "DOS" ));
            SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFAULT ), CB_ADDSTRING, 0, ( LPARAM )( "UNIX" ));

            SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFAULT ), CB_SETCURSEL, ( ostype == 0x03 ? 0 : 1 ), ( LPARAM )( 0 ));

            if( ! fBrowse )  {
                SetWindowPos( GetDlgItem( hwndDlg, IDC_FILE_LIST ), NULL, 0,0,0,0, SWP_HIDEWINDOW );
                SetWindowPos( GetDlgItem( hwndDlg, IDC_FILE_TYPE ), NULL, 0,0,0,0, SWP_HIDEWINDOW );

                SetWindowPos( GetDlgItem( hwndDlg, IDC_ASSOC_DEFAULT ), NULL, 0,0,0,0, SWP_HIDEWINDOW );
                SetWindowPos( GetDlgItem( hwndDlg, IDC_FIND_WORD ), NULL, 0,0,0,0, SWP_HIDEWINDOW );
            }
            if( fMerge )  {
                SetWindowPos( GetDlgItem( hwndDlg, IDC_FILE_TYPE ), NULL, 0,0,0,0, SWP_HIDEWINDOW );
            }

            for( n = fnhcnt; n > 0; n-- )  {
                SendMessage( GetDlgItem( hwndDlg, IDC_FILE_NAME ), CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( fnhlst[ n - 1] ));
            }
            pt.x = pt.y = 4;
            hwndCombo = GetDlgItem( hwndDlg, IDC_FILE_NAME );
            hwndComboEdit = ChildWindowFromPoint( hwndCombo, pt );
            lpfnOldComboEditWndProc = ( WNDPROC )SetWindowLong( hwndComboEdit, GWL_WNDPROC, ( DWORD )ComboEditSubClassFunc );
 
            fBrowse = 0;

            /*hDC = GetDC( hwndDlg );
            MoveToEx( hDC, 10, 10, NULL );
            LineTo( hDC, 100, 10 );*/

            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    GetWindowText( GetDlgItem( hwndDlg, IDC_FILE_NAME ), filename, CE_LEN_FILEPATH );
                    hwndDlgActive = NULL;
                    SetWindowLong( hwndComboEdit, GWL_WNDPROC, (DWORD) lpfnOldComboEditWndProc );

                    iSel = SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFAULT ), CB_GETCURSEL, 0, ( LPARAM )( 0 ));
                    if( iSel == 0 ) ostype = 0x03;
                    else ostype = 0x02;

                    EndDialog( hwndDlg, TRUE );
                    return( TRUE );

                case DID_CANCEL:
                    hwndDlgActive = NULL;
                    SetWindowLong( hwndComboEdit, GWL_WNDPROC, (DWORD) lpfnOldComboEditWndProc );
                    EndDialog( hwndDlg, FALSE );
                    return( TRUE );

                case IDC_FILE_LIST:
                    fBrowse = TRUE;
                    GetWindowText( GetDlgItem( hwndDlg, IDC_FILE_NAME ), filename, CE_LEN_FILEPATH );
                    hwndDlgActive = NULL;
                    SetWindowLong( hwndComboEdit, GWL_WNDPROC, (DWORD) lpfnOldComboEditWndProc );

                    iSel = SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFAULT ), CB_GETCURSEL, 0, ( LPARAM )( 0 ));
                    if( iSel == 0 ) ostype = 0x03;
                    else ostype = 0x02;

                    EndDialog( hwndDlg, TRUE );
                    //SetWindowText( GetDlgItem( hwndDlg, IDC_FILE_NAME ), filename );
                    return( TRUE );

                /*case IDC_FILE_TYPE:
                    DialogBox( hInst, "FILETYPE", hwndDlg, MakeProcInstance( FileTypeDlg, hInst ));
                    return( TRUE );*/

                default:
                    /*GetWindowText( GetDlgItem( hwndDlg, IDC_FILE_NAME ), filename, CE_LEN_FILEPATH );
                    sl = strlen( filename );
                    if( sl )  {
                        if( filename[ sl - 1 ] == ' ' )  {
                            filename[ sl - 1 ] = '\0';
                            if( file_nxpand( filename ))  ;
                            sl = strlen( filename );
                            SetWindowText( GetDlgItem( hwndDlg, IDC_FILE_NAME ), filename );
                            SendMessage( GetDlgItem( hwndDlg, IDC_FILE_NAME ), CB_SETEDITSEL, 0, MAKELONG( sl, sl ));
                        }
                    }*/
                    return( TRUE );
            }
            break;

        default:
            break;
    }
    return( FALSE );
}



UINT CALLBACK OFNHookProc( HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )  {

    switch (uiMsg) {
        case WM_INITDIALOG:
            RECT rect;
            HWND hwndParent, hwndSetPos;
            char szTitle[ 2 ];
 
            hwndSetPos = hwndDlg;
            hwndParent = GetParent( hwndDlg );
            GetWindowText( hwndDlg, szTitle, 2 );
            if( !( *szTitle ))  {       // For Explorer Style this hwndDlg is NOT the dialog
                    hwndSetPos = hwndParent;
                    hwndParent = GetParent( hwndParent );
                    }

            GetWindowRect( hwndParent, &rect );
            SetWindowPos( hwndSetPos, HWND_TOP, rect.left + 64, rect.top + 64, 0, 0, SWP_NOSIZE );
            break;

        default:
            break;
    }
    return 0;
}
 
 



FileBrowse( HWND hwnd, char *fn, char *ft, char *szFilter, int fChangeDir )  {
    OPENFILENAME ofn;
    //char szFilter[ ] = "All Files\0*.*\0";
    int sl;
    char fp[ CE_LEN_FILEPATH ] = "0";
    char *s;
 

    sl = strlen( fn );
    /*if(( sl == 0 )||( fn[ sl-1 ] == '\\' )||( fn[ sl-1 ] == ':' ))
            strcat( fn, "*.*" );*/

    memset(&ofn, 0, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    //ofn.lStructSize = sizeof(OPENFILENAME_SIZE_VERSION_400);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
 
    fp[ 0 ] = '\0';
    if( sl )  {
        if( ( fn[ sl-1 ] == '\\' )||( fn[ sl-1 ] == ':' ))  {
            strncpy( fp, fn, CE_LEN_FILEPATH - 1 );
            fn[ 0 ] = '\0';
        }
        else  {
            s = strrchr( fn, '\\' );
            if( s == NULL )
                s = strrchr( fn, ':' );
            if( s != NULL )  {
                s += 1;
                strncpy( fp, fn, s - fn );
                strcpy( fn, s );
            }
        }
    }
    ofn.lpstrFile = fn;
    ofn.lpstrInitialDir = fp;
    ofn.nMaxFile = CE_LEN_MULTIFILE;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrTitle = ft;

    ofn.Flags = OFN_HIDEREADONLY;
    if( !fChangeDir )  ofn.Flags |= OFN_NOCHANGEDIR;
    if( fMultiFile )  ofn.Flags |= OFN_ALLOWMULTISELECT;
 
#ifdef _WIN32
    if( fExplorerFileDlg )  {
        ofn.Flags |= OFN_EXPLORER;
        //ofn.Flags |= OFN_ENABLEHOOK;    // Force WIN32_WINNT >= 0500 to remove the Places Bar
        //ofn.FlagsEx |= 1;   // = OFN_EX_NOPLACESBAR;
    }
    else  {
        ofn.Flags |= OFN_ALLOWMULTISELECT;      // Always for old style dialog
        if( strchr( fn, ' ' ))                  // use default dir if any spaces in the dir name
                ofn.lpstrFile = NULL;
    }

    ofn.Flags |= OFN_LONGNAMES;

    ofn.lpfnHook = OFNHookProc;
    ofn.Flags |= OFN_ENABLEHOOK;

#endif
    //ofn.Flags |= OFN_OVERWRITEPROMPT;
    if (GetOpenFileName( ( struct tagOFNA *)&ofn)) {
        InvalidateRect( hwndMain, NULL, TRUE );
        UpdateWindow( hwndMain );
        return( TRUE );
    }
    else  {
        return( FALSE );
    }
}




BOOL CALLBACK FilePropertiesDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    int n;
    char *s;
    int iSel;
    char szDt[ 100 ];
    char szSize[ 100 ];
    FILETIME ft;
    SYSTEMTIME st;


    switch (message) {

        case WM_INITDIALOG:
            for( n = 0; n < CE_CNT_ASSTYPE; ++n )  {
                SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFAULT ), CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( asstype[ n ] ));
                //if(( n >= 2 )&&( n <= 4 ))
                //        SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFSQL ), CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( assoc[ n ] ));
            }
            SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFAULT ), CB_SETCURSEL, ed->assoc, ( LPARAM )( 0 ));
            //SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFSQL ), CB_SETCURSEL, assocsql-2, ( LPARAM )( 0 ));

            SendMessage( GetDlgItem( hwndDlg, IDC_PROPS_FILETYPE ), CB_ADDSTRING, 0, ( LPARAM )( "DOS" ));
            SendMessage( GetDlgItem( hwndDlg, IDC_PROPS_FILETYPE ), CB_ADDSTRING, 0, ( LPARAM )( "UNIX" ));

            SendMessage( GetDlgItem( hwndDlg, IDC_PROPS_FILETYPE ), CB_SETCURSEL, ( ostype == 0x03 ? 0 : 1 ), ( LPARAM )( 0 ));

            if(( ed->ftime.hi != 0 )||( ed->ftime.lo != 0 ))  {
                ft.dwHighDateTime = ed->ftime.hi;
                ft.dwLowDateTime = ed->ftime.lo;

                FileTimeToLocalFileTime( &ft, &ft );
                FileTimeToSystemTime( &ft, &st );

                sprintf( szDt, "%04d/%02d/%02d %02d:%02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
                SetWindowText( GetDlgItem( hwndDlg, IDC_ASSOC_SAVED ), szDt );
            }
            sprintf( szSize, "%d Lines", ed->nolines );
            SetWindowText( GetDlgItem( hwndDlg, IDC_ASSOC_SIZE ), szSize );

            SetWindowText( GetDlgItem( hwndDlg, IDC_ASSOC_FILENAME ), ed->fname );

            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    iSel = SendMessage( GetDlgItem( hwndDlg, IDC_PROPS_FILETYPE ), CB_GETCURSEL, 0, ( LPARAM )( 0 ));

                    if( iSel == 0 ) ostype = 0x03;
                    else ostype = 0x02;

                    ed->assoc = SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_DEFAULT ), CB_GETCURSEL, 0, ( LPARAM )( 0 ));
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_ASSOC_SAVE ), BM_GETCHECK, 0, 0 ))  {
                        if( ! *ed->fname )  {
                            assocdef = ed->assoc;
                            asslst[ 0 ].lng = ed->assoc;
                        }
                        else if(( s = strrchr( ed->fname, '.' )) != NULL )  {
                            ++s;
                            for( n = 2; n < asscnt; ++n )  {
 
                                if( _stricmp( s, asslst[ n ].fnx ) == 0 )  {
                                    asslst[ n ].lng = ed->assoc;
                                    break;
                                }
                            }
                            if( n == asscnt )  {
                                if( asscnt < CE_MAX_ASSOCIATIONS )  {
                                    asslst[ n ].lng = ed->assoc;
                                    strncpy( asslst[ n ].fnx, s, CE_LEN_FILENEXT );
                                    asscnt += 1;
                                }
                            }
                        }
                        else  {
                            asslst[ 1 ].lng = ed->assoc;
                        }
                        IniSetAssosiations( asscnt, asslst );
                    }
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



#ifdef __cplusplus 
} 
#endif  
