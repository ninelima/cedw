 
#include "windows.h"
#include "winbase.h"

//#include "ctl3d.h"
PASCAL Ctl3dRegister( HANDLE );
PASCAL Ctl3dAutoSubClass( HANDLE );
PASCAL Ctl3dUnRegister( HANDLE );

//#include "shellapi.h"
//#include "commdlg.h"
#include "commctrl.h"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "resource.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"


extern struct editrec *ed;
extern int blck;
extern int restoredirf;
extern int scrn_x;
extern int scrn_y;

extern PRINTDLG pd;

extern int fCaret;


HINSTANCE hInst;
HWND hwndMain, hwndClient;
HWND hwndVScroll, hwndHScroll, hwndScrollF, hwndStatus, hwndTree, hwndSizeBar;

char  szMainClass[ ] = "ceWin" ;
char  szMainTitle[ ]  = "C-edit" ;

HPEN hPenBlack, hPenWhite, hPenGray, hPenSysGray;
HBRUSH hBrushWhite, hBrushLtGray,  hBrushSysGray;

int argc;
char far *argv[ CE_MAX_FILES ];

int cxScrn = CE_DEF_SCRNX, cyScrn = CE_DEF_SCRNY;
int cxChar, cyChar;
int cxFileTree = 0;
int cxFileTreeBak = 200;
int cxSizeBar = 0; //CE_SZE_SIZEBAR;
int fFileTree = 0;
int fFileTreeIni = 0;

int fMultiInstance = 0;
int fTitleFileName = 0;
int fExplorerFileDlg = 0;
int fSaveWinPos = 0;
int fFocusMousePos = 0;

int fActive = 0;
int fClipbrd = 0;

int fScrnFont = 0;
LOGFONT lfScrnFont;

HDC hDC;
int fStart = 0;

WNDPROC lpfnOldTreeViewWndProc;


main_flags(int argc, char **argv);



int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )  {
    MSG msg;
    WNDCLASS  wc;
    HWND hwndPrev, hwndPrevDlg;
#ifdef _WIN32
    COPYDATASTRUCT cds;
#endif
    RECT rect;
    WINDOWPLACEMENT wp;
    int cyWin = 0, cxWin = 0;

    IniGetOptions( );
    IniGetWinOptions( &fMultiInstance, &fTitleFileName, &fExplorerFileDlg, &fSaveWinPos, &fFocusMousePos, &fFileTreeIni );

    //if( hPrevInstance )  return( FALSE );
    hInst = hInstance;
    if( ! fMultiInstance )  {
        if( hwndPrev = FindWindow( szMainClass, NULL ))  {      // Posible race condition !!!
            if( IsIconic( hwndPrev )) {
                ShowWindow( hwndPrev, SW_RESTORE );
            }
#ifdef _WIN
            hwndPrevDlg = GetLastActivePopup( hwndPrev );
            BringWindowToTop( hwndPrev );
            if( hwndPrevDlg != hwndPrev )  {
                BringWindowToTop( hwndPrevDlg );
            }
#else
            hwndPrevDlg = GetLastActivePopup( hwndPrev );
            SetForegroundWindow( hwndPrev );
            if( hwndPrevDlg != hwndPrev )  {
                SetForegroundWindow( hwndPrevDlg );
            }
 
            if( lpCmdLine[ 0 ] )  {
                cds.dwData = 0;
                cds.cbData = strlen( lpCmdLine ) + 1;
                cds.lpData = lpCmdLine;
                SendMessage( hwndPrev, WM_COPYDATA, (WPARAM)NULL, (LPARAM) &cds);
            }
#endif
            return( FALSE );
        }
    }

#ifndef _WIN32
    Ctl3dRegister( hInst );
    Ctl3dAutoSubClass( hInst );
#endif

    hBrushSysGray = CreateSolidBrush( GetSysColor( COLOR_MENU ));


    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = ( WNDPROC )MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInst, "cewicon" );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = NULL;
    wc.lpszMenuName = "CewMenu";
    wc.lpszClassName = szMainClass;
#ifdef _WIN
    if( ! hPrevInstance )
#endif
    if( ! RegisterClass( &wc ))
            return( FALSE );

    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = StatusWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;        //LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBrushSysGray;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ceWinStatus";
#ifdef _WIN
    if( ! hPrevInstance )
#endif
    if( ! RegisterClass( &wc ))
            return( FALSE );

    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = ScrollFillWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;        //LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBrushSysGray;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ceWinScrollFill";
#ifdef _WIN
    if( ! hPrevInstance )
#endif
    if( ! RegisterClass( &wc ))
            return( FALSE );


    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = SeperatorLineWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;        //LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = NULL;      //LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBrushSysGray;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ceWinSeperatorLine";
#ifdef _WIN
    if( ! hPrevInstance )
#endif
    if( ! RegisterClass( &wc ))
            return( FALSE );


    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = ColorSelectWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;        //LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = NULL;      //LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBrushWhite;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ceWinColorSelect";
#ifdef _WIN
    if( ! hPrevInstance )
#endif
    if( ! RegisterClass( &wc ))
            return( FALSE );



    wc.style = CS_CLASSDC | CS_DBLCLKS;
    wc.lpfnWndProc = EditWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBrushWhite;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ceWinEdit";
#ifdef _WIN
    if( ! hPrevInstance )
#endif
    if( ! RegisterClass( &wc ))
            return( FALSE );


    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = SizeBarWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;        //LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_SIZEWE);
    wc.hbrBackground = hBrushSysGray;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "ceWinSizeBar";
#ifdef _WIN
    if( ! hPrevInstance )
#endif
    if( ! RegisterClass( &wc ))
            return( FALSE );


    argc = CmdLine( lpCmdLine, argv );
    //==main_flags( argc, argv );
    //cxScrn = scrn_x;
    //cyScrn = scrn_y;

    IniGetScrnFont( &fScrnFont, &lfScrnFont );
    IniGetColor( );

    hPenBlack = ( HPEN )GetStockObject( BLACK_PEN );
    hPenWhite = ( HPEN )GetStockObject( WHITE_PEN );
    hPenGray = ( HPEN )CreatePen(PS_SOLID, 1, RGB(128, 128, 128));

    hPenSysGray = CreatePen(PS_SOLID, 1, GetSysColor( COLOR_MENU ));
 
    hBrushWhite = ( HBRUSH )GetStockObject( WHITE_BRUSH );
    hBrushLtGray = ( HBRUSH )GetStockObject( LTGRAY_BRUSH );


    hwndMain = CreateWindow( szMainClass, szMainTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            HWND_DESKTOP, NULL, hInstance, NULL );

    if( !hwndMain )  return( FALSE );

    if( IniGetWinPos( &wp, &cxFileTreeBak ))  {
        wp.length = sizeof( WINDOWPLACEMENT );
        SetWindowPlacement( hwndMain, &wp );
    }
    if( cxFileTreeBak == 0 )  cxFileTreeBak = 200;
    SetWindowPos( hwndMain, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE );   // Show the main window

    GetClientRect( hwndMain, &rect );
    cxWin = rect.right - rect.left;
    cyWin = rect.bottom - rect.top;

    cyScrn = ( cyWin - 1 - CE_SZE_STATUSBAR - CE_SZE_VSCROLL )/ cyChar;  // !!!! Do NOT Combine these lines !!!!
    cyScrn += ((( cyWin - 1 - CE_SZE_STATUSBAR - CE_SZE_VSCROLL ) - ( cyScrn * cyChar )) > cyChar / 2 ? 1 : 0 );
    scrn_y = cyScrn;

    cxScrn = ( cxWin - cxFileTree - cxSizeBar - CE_SZE_HSCROLL )/ cxChar + (( cxWin -  cxFileTree - cxSizeBar - CE_SZE_HSCROLL )% cxChar ? 1 : 0 );
    scrn_x = cxScrn;

    if( fFileTreeIni )  {
        fFileTree = -1;
        cxFileTree = cxFileTreeBak;
        cxSizeBar = CE_SZE_SIZEBAR;
        MoveWindow( hwndTree, 1, 1, cxFileTree - 1, cyWin - CE_SZE_STATUSBAR - 1, TRUE );
        MoveWindow( hwndSizeBar, cxFileTree, 0, cxSizeBar, cyWin - CE_SZE_STATUSBAR, TRUE );
    }

    // BEWARE !!!
    // cxScrn and cyScrn is required in the WM_PAINT of the edit window (hwndClient), therefore it can only be
    // enabled after these variables have been calculated which can only happen after the size of the main window
    // is set.
    SetWindowPos( hwndClient, NULL, cxFileTree + cxSizeBar, 0, cxWin - ( cxFileTree + cxSizeBar ), cyWin - CE_SZE_STATUSBAR, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOOWNERZORDER );
 

    DragAcceptFiles( hwndMain, TRUE );

    //IniGetAssosiations( );
    IniGetFiles();
    IniGetFind();

    PostMessage( hwndMain, SM_INIT, 0, 0 );

    while( GetMessage( &msg, NULL, 0, 0 ))  {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    return( msg.wParam );
}



CmdLine( char *achCmdLine, char *achArgv[ CE_MAX_FILES ] )  {
    int argc;
    char *s, *t;

    argc = 0;
    s = achCmdLine;
    if( s[ 0 ] == '\0' )  return( 0 );

    while( *s == '"' )  {
        ++s;
        if(( t = strchr( s, '"' ))!= NULL )  {
            *t++ = '\0';
            if( strlen( s ))  {
                if(( achArgv[ argc ] = ( char * )malloc( strlen( s ) + 1 )) == NULL )  continue;
                strcpy( achArgv[ argc ], s );
                ++argc;
            }
        }
        while( *t == ' ' )
            ++t;
        s = t;
    }
    if( strlen( s ))  {
        if(( achArgv[ argc ] = ( char * )malloc( strlen( s ) + 1 )) == NULL )  return( argc );
        strcpy( achArgv[ argc ], s );
        ++argc;
    }
    return( argc );
}




long FAR PASCAL MainWndProc( HWND hwnd, unsigned message, WPARAM wParam, LPARAM lParam )  {
    RECT rectl;
    HDC hdc;
    int cx, cy;
    int n, a;
    LONG lWinLong;


    //hwndClient = hwnd;
    switch (message) {

        case WM_KEYDOWN:
            WMKeyDn( hwnd, ( WORD )wParam, ( LONG )lParam );
            return( 0 );

        case WM_USER+5:
            HideCaret( NULL );

            cmnd_file_select( (struct editrec *)wParam );
            cmnd_goto_line( lParam, 0 );

            SetScrollRange( hwndVScroll, SB_CTL, 0, ( int )( ed->nolines ), FALSE );
            SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );
            SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );

            SetFocus( hwndClient );
            scrn_cursor();
            return 0;

        case WM_USER+4:
            HideCaret( NULL );

            cmnd_file_select( (struct editrec *)wParam );

            //TreeItemSel( ed->hTreeItem, 0 );
            SetScrollRange( hwndVScroll, SB_CTL, 0, ( int )( ed->nolines ), FALSE );
            SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );
            SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );

            SetFocus( hwndClient );
            //ShowCaret( NULL );
            return 0;

        case WM_NOTIFY:  {
                LPNMHDR pnmh;
                LPNMTREEVIEW pnmtv;
                TVITEM tvi;

                pnmh = (LPNMHDR) lParam;

                if(( pnmh->code == NM_DBLCLK )||( pnmh->code == NM_RETURN ))  {
                    return 0;
                }
                else if ( pnmh->code == TVN_ITEMEXPANDED )  {
                   pnmtv = (LPNMTREEVIEW) lParam;

                   tvi = pnmtv->itemNew;
                   if(( pnmtv->action == 2 )&&( tvi.iImage == 0 )) {
                        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
                        tvi.iImage = 1;
                        tvi.iSelectedImage = 1;
                        TreeView_SetItem(hwndTree, &tvi);
                   }
                   if(( pnmtv->action == 1 )&&( tvi.iImage == 1 ))  {
                        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
                        tvi.iImage = 0;
                        tvi.iSelectedImage = 0;
                        TreeView_SetItem(hwndTree, &tvi);
                   }
                }
                else if ( pnmh->code == TVN_ITEMEXPANDING )  {
                    pnmtv = (LPNMTREEVIEW) lParam;

                    if( pnmtv->action == 2 )  {
                        TVITEMDATA *tvidata;

                        tvi = pnmtv->itemNew;
                        tvidata = ( TVITEMDATA * )tvi.lParam;
                        if( tvidata->iType == CE_TVI_FILE )  {
                            if(( tvidata->iAssoc == CE_ASC_C )||( tvidata->iAssoc == CE_ASC_JAVA )||( tvidata->iAssoc == CE_ASC_ORASQL )||( tvidata->iAssoc == CE_ASC_MSSQL )||( tvidata->iAssoc == CE_ASC_JVS )||( tvidata->iAssoc == CE_ASC_VBS ))  {
                                TreeItemDelChildren( tvi.hItem );
                                tree_file( tvidata->ed, 0 );

                                if( TreeView_GetChild( hwndTree, tvi.hItem )== NULL )  {
                                    TreeItemIns( "", 3, 3, tvi.hItem, CE_TVI_FUNC, NULL, 0, 0 );
                                    return TRUE;
                                }

                            }
                            else  {
                                return TRUE ;
                            }
                        }
                    }
                    else if( pnmtv->action == 1 )  {
                        TVITEMDATA *tvidata;

                        tvi = pnmtv->itemNew;
                        tvidata = ( TVITEMDATA * )tvi.lParam;
                        if( tvidata->iType == CE_TVI_FILE )  {
                            if(( tvidata->iAssoc == CE_ASC_C )||( tvidata->iAssoc == CE_ASC_JAVA )||( tvidata->iAssoc == CE_ASC_ORASQL )||( tvidata->iAssoc == CE_ASC_MSSQL )||( tvidata->iAssoc == CE_ASC_JVS )||( tvidata->iAssoc == CE_ASC_VBS ))  {
                                TreeItemDelChildren( tvi.hItem );
                                TreeItemIns( "", 3, 3, tvi.hItem, CE_TVI_FUNC, NULL, 0, 0 );
                            }
                        }
                    }
                }
            }
            break;

#ifdef _WIN32
        case WM_COPYDATA:  {
                COPYDATASTRUCT *lpcds;

                lpcds = ( COPYDATASTRUCT * )lParam;
                argc = CmdLine( ( char * )lpcds->lpData, argv );
                PostMessage( hwndMain, SM_FILEOPEN, 0, 0 );
                return( 0 );
            }
#endif
        case SM_FILEOPEN:
            HideCaret( NULL );
            blck_unmark( );
            UpdateWindow( hwndVScroll );
 
            for (a = 0; a < argc; ++a)  {
                if( argv[ a ] == NULL )  continue;
                if( ! (( *argv[a] == '/' )||( *argv[a] == '-' )))  {
                    file_open( argv[a] );
                }
                free( argv[a] );
            }
            scrn_statsl();
            scrn_rescrn();
            SetScrollRange( hwndVScroll, SB_CTL, 0, ( int )( ed->nolines ), FALSE );
            SetScrollPos( hwndVScroll, SB_CTL, ed->displcrds.y, TRUE );
            SetScrollPos( hwndHScroll, SB_CTL, ed->displcrds.x, TRUE );
            TreeItemSel( ed->hTreeItem, -1 );
            ShowCaret( NULL );
            return( 0 );

        case WM_DROPFILES:  {
                UINT iNumFilesDropped, iPathnameSize;
                HWND hwndDlg;

                iNumFilesDropped = DragQueryFile((HDROP) wParam, -1, NULL, 0);
                for (n = 0 ; n < iNumFilesDropped; n++) {
                    iPathnameSize = DragQueryFile((HDROP) wParam, n, NULL, 0);
                    if(( argv[ n ] = ( char * )malloc( iPathnameSize + 1 )) == NULL )  {
                        continue;
                    }
                    DragQueryFile((HDROP) wParam, n, ( LPSTR )argv[ n ], iPathnameSize + 1);
                }
                DragFinish((HDROP) wParam);
                argc = n;
                hwndDlg = GetLastActivePopup( hwnd );
#ifdef _WIN
                BringWindowToTop( hwnd );
                if( hwndDlg != hwnd )  {
                    BringWindowToTop( hwndDlg );
                }
#else
                SetForegroundWindow( hwnd );
                if( hwndDlg != hwnd )  {
                    SetForegroundWindow( hwndDlg );
                }
#endif
                PostMessage( hwndMain, SM_FILEOPEN, 0, 0 );
            }
            break;

        case SM_INIT:
            scrn_ini();
            //crma_ini();
            edit_ini();

            UpdateWindow( hwndVScroll );
            {
                char szDir[ 1000 ];      // Fix !!!!!

                IniGetOptionsStartupDir( szDir );
                if( *szDir )
                    file_setcwd( szDir );
            }
            for (a = 0; a < argc; ++a)  {
                if( argv[ a ] == NULL )  continue;
                if( ! (( *argv[a] == '/' )||( *argv[a] == '-' )))  {
                    file_open( argv[a] );
                }
                free( argv[a] );
            }
            scrn_statsl();
            scrn_rescrn();
            fStart = -1;
            SetScrollRange( hwndVScroll, SB_CTL, 0, ( int )( ed->nolines ), FALSE );
            SetScrollPos( hwndVScroll, SB_CTL, ( int )ed->displcrds.y, TRUE );
            SetScrollPos( hwndHScroll, SB_CTL, ( int )ed->displcrds.x, TRUE );
            if( *ed->fname )
                TreeItemSel( ed->hTreeItem, -1 );
            fCaret = -1;
            ShowCaret( NULL );
            return( 0 );

        case SM_ACTIVATEAPP:
            InvalidateRect( hwndMain, NULL, TRUE );
            UpdateWindow( hwndMain );

            if( fStart )  {
                file_2reload();
            }
            return 0;

        case WM_CREATE:
            GetClientRect( hwnd, &rectl );
            cx = rectl.right - rectl.left;
            cy = rectl.bottom - rectl.top;

            InitCommonControls();

            hwndStatus = CreateWindow( "ceWinStatus", "", WS_CHILD | WS_VISIBLE, 0, cy - CE_SZE_STATUSBAR, cx, CE_SZE_STATUSBAR, hwnd, ( HMENU )4567, hInst, NULL );


            hwndTree = CreateWindowEx( 0, WC_TREEVIEW, "Tree View", WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_DISABLEDRAGDROP | TVS_SHOWSELALWAYS, 0, 0, 0, 0, hwnd, (HMENU)NULL, hInst, NULL);
            lWinLong = GetWindowLong( hwndTree, GWL_STYLE );
            lWinLong -= WS_BORDER;
            SetWindowLong( hwndTree, GWL_STYLE, lWinLong );
            lpfnOldTreeViewWndProc = ( WNDPROC )SetWindowLong( hwndTree, GWL_WNDPROC, ( DWORD )TreeViewSubClassFunc );
            SetWindowPos( hwndTree, NULL, 1, 1, cxFileTree - 1, cy - CE_SZE_STATUSBAR - 1, SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOOWNERZORDER );

            hwndSizeBar = CreateWindow( "ceWinSizeBar", "", WS_CHILD | WS_VISIBLE, cxFileTree, 0, cxSizeBar, cy - CE_SZE_STATUSBAR, hwnd, ( HMENU )4567, hInst, NULL );

            hwndClient = CreateWindow( "ceWinEdit", "", WS_CHILD | WS_CLIPCHILDREN, 0, 0, 0, 0, hwnd, NULL, hInst, NULL );

            crma_ini();
            InitTreeImageLists( hwndTree );
            //alHti[ 0 ] = TreeItemIns( "", 1, 0, TVI_ROOT );
            return 0;

        case WM_SIZE:
            //if(( cyScrn == 0 )||( cxScrn <= 2 ))  return( NULL );
            if( wParam == SIZE_MINIMIZED )  return( 0 );

            cx = ( int )LOWORD( lParam );
            cy = ( int )HIWORD( lParam );

            MoveWindow( hwndStatus, 0, cy - CE_SZE_STATUSBAR, cx, CE_SZE_STATUSBAR, TRUE);
            MoveWindow( hwndTree, 1, 1, cxFileTree - 1, cy - CE_SZE_STATUSBAR - 1, TRUE );
            MoveWindow( hwndSizeBar, cxFileTree, 0, cxSizeBar, cy - CE_SZE_STATUSBAR, TRUE );
            MoveWindow( hwndClient, cxFileTree + cxSizeBar, 0, cx - (cxFileTree + cxSizeBar ), cy - CE_SZE_STATUSBAR, TRUE );
            return( 0 );

        case WM_PAINT:  {
                PAINTSTRUCT ps;

                hdc = BeginPaint(hwnd, &ps);

                GetClientRect( hwnd, &rectl );
#ifdef _WIN32
                SelectObject( hdc, hPenBlack );
#else
                SelectObject( hdc, hPenWhite );
#endif

                MoveToEx( hdc, rectl.left, rectl.top, NULL );
                LineTo( hdc, cxFileTree, rectl.top );
                MoveToEx( hdc, rectl.left, rectl.top, NULL );
                LineTo( hdc, rectl.left, rectl.bottom );

                EndPaint( hwnd, &ps );
            }
            return( 0 );

        case WM_ACTIVATE:
            if( wParam == WA_INACTIVE )  fActive = 0;
            else if( wParam == WA_ACTIVE )  fActive = -1;
            break;
 
        case WM_ACTIVATEAPP:
            if( wParam == WA_ACTIVE )  {
                PostMessage( hwndMain, SM_ACTIVATEAPP, 0, 0 );
            }
            break;

        case WM_MOUSEACTIVATE:
            if( ! fActive )  {
                fActive = -1;
                if(( INT )LOWORD( lParam ) == HTCLIENT )  {
                    if( fFocusMousePos )
                        return( MA_ACTIVATEANDEAT );
                    else
                        return( MA_ACTIVATE );
                }
                else
                    return( MA_ACTIVATE );
            }
            else  {
                return( MA_ACTIVATE );
            }

        case WM_INITMENU:
            WMInitMenu( hwnd, wParam );
            return( DefWindowProc( hwnd, message, wParam, lParam ));
 
        case WM_COMMAND:
            WMCommand( hwnd, wParam );  // ????? should this still be here ????
            return( 0 );

        case WM_SETFOCUS:
            SetFocus( hwndClient );
            return 0;

        case WM_CLOSE:
            blck_unmark();
            if ( ! file_closeall())  {
                scrn_statsl();
                scrn_rescrn();
                return( 0 );
            }
            IniSetFiles();
            IniSetFind();
            if( restoredirf )  {
                char szDir[ 1000 ];     // Fix somehow !!!!!

                if( file_getcwd(szDir))
                    IniSetOptionsStartupDir( szDir );
            }
            break;

        case WM_DESTROY:
            if( fSaveWinPos )  {
                WINDOWPLACEMENT wp;

                wp.length = sizeof( WINDOWPLACEMENT );
                GetWindowPlacement( hwndMain, &wp );

                IniSetWinPos( wp, cxFileTree ? cxFileTree : cxFileTreeBak );
            }
#ifndef _WIN32
            Ctl3dUnRegister( hInst );
#endif
            //ChangeClipboardChain( hwnd, hwndNextClipboardViewer );
            DestroyImageList();
            WinHelp(hwnd, NULL, HELP_QUIT, 0 );
            if ( pd.hDevMode )  GlobalFree( pd.hDevMode );
            if ( pd.hDevNames )  GlobalFree( pd.hDevNames );
            PostQuitMessage( 0 );
            return( 0 );

        default:
            return( DefWindowProc( hwnd, message, wParam, lParam ));
    }
    return( DefWindowProc( hwnd, message, wParam, lParam ));
}



edit_beep( )  {
    MessageBeep( MB_OK );
}



edit_wait( int f )  {

    if( f )  SetCursor( LoadCursor(( HINSTANCE )NULL, IDC_WAIT ));
    else     SetCursor( LoadCursor(( HINSTANCE )NULL, IDC_ARROW ));
}
    




#ifdef __cplusplus
}
#endif
