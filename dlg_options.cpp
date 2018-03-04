


#include "windows.h"

#include "string.h"
#include "stdlib.h"

#include "resource.h"

#include "htmlhelp.h"

#include "string2.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"

int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam );


extern HINSTANCE hInst;
extern HWND hwndMain, hwndClient;
extern HWND hwndStatus;


extern int filetab;
extern int fileos;
extern int filemodchkf;
extern int restoredirf;

extern HFONT hFont, hFontBold;

extern struct editrec *ed;
extern int tablen;
extern int crmacode;

extern int fMatchBracket;

extern int fScrnFont;
extern LOGFONT lfScrnFont;

extern struct colorset clrset[ ];

extern int fMultiInstance;
extern int fTitleFileName;
extern int fExplorerFileDlg;
extern int fSaveWinPos;
extern int fFocusMousePos;
extern int fFileTree;
extern int fFileTree;

extern char asstype[ ][ CE_LEN_ASSTYPE ];

extern char szMainTitle[ ];     // ????

static char szRegKey[ ] = "SOFTWARE\\C-edit\\HelpFiles";

typedef HWND (WINAPI *FPHH) (HWND, LPCSTR, UINT, DWORD);
FPHH HTMLHelp;


static char selcolor[ CE_CNT_COLORSET ][ 40 ] = {
    "Normal",
    "Selected",
    "Keyword",
    "String",
    "Number",
    "Comment",
    "ASP Script delimiter",
    "HTML Element name",
    "HTML Tag delimiter",
    "HTML Tag text",
    "HTML Tag operator",
    //"HTML Comment",
    "XML Element name",
    "XML Tag delimiter",
    "XML Tag text",
    "XML Tag operator",
    //"XML Comment",  
    "Matching Bracket",
};






BOOL CALLBACK FileOptionsDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    char tss[ 9 ];
    int ts;
    int iSel;
    char szDir[ 1000 ];  // Fix !!!!!!!
    static fRePos = 0;

    switch (message) {
        case WM_INITDIALOG:
            _itoa( filetab, tss, 10 );
            SetWindowText( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ), tss );

            SendMessage( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_FILETYPE ), CB_ADDSTRING, 0, ( LPARAM )( "DOS" ));
            SendMessage( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_FILETYPE ), CB_ADDSTRING, 0, ( LPARAM )( "UNIX" ));

            SendMessage( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_FILETYPE ), CB_SETCURSEL, ( fileos == 0x03 ? 0 : 1 ), ( LPARAM )( 0 ));

            SendMessage( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_CHECKMOD ), BM_SETCHECK, filemodchkf, 0 );

            IniGetOptionsStartupDir( szDir );
            SetWindowText( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_DIRECTORY ), szDir );
            SendMessage( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_RESTOREDIR ), BM_SETCHECK, restoredirf, 0 );

            return( TRUE );

        case WM_NOTIFY:
            switch (((NMHDR FAR *) lParam)->code) {

                case PSN_SETACTIVE:
                    if( ! fRePos )  {       // repositions the dialogue the first time this page is activated
                        RECT rect;
 
                        GetWindowRect( hwndMain, &rect );
                        //SetWindowPos( GetParent( hwndDlg ), HWND_TOP, rect.left + 58, rect.top + 94, 0, 0, SWP_NOSIZE );
                        SetWindowPos( GetParent( hwndDlg ), HWND_TOP, rect.left + 64, rect.top + 64, 0, 0, SWP_NOSIZE );
                    }
                    break;

                case PSN_KILLACTIVE:
                    GetWindowText( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ), tss, 8 );
                    if( ts = atoi( tss ))  {
                        if( ts <= 20 )  {
                        }
                        else  {
                            SetFocus( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ));
                            MessageBeep( -1 );
                            SetWindowLong(hwndDlg,DWL_MSGRESULT, TRUE);
                            return( TRUE );
                        }
                    }
                    else  {
                        SetFocus( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ));
                        MessageBeep( -1 );
                        SetWindowLong(hwndDlg,DWL_MSGRESULT, TRUE);
                        return( TRUE );
                    }
                    break;

                case PSN_APPLY:

                    iSel = SendMessage( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_FILETYPE ), CB_GETCURSEL, 0, ( LPARAM )( 0 ));

                    if( iSel == 0 ) fileos = 0x03;
                    else fileos = 0x02;

                    filemodchkf = SendMessage( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_CHECKMOD ), BM_GETCHECK, 0, 0 );
                    restoredirf = SendMessage( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_RESTOREDIR ), BM_GETCHECK, 0, 0 );

                    GetWindowText( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ), tss, 8 );
                    if( ts = atoi( tss ))  {
                        if( ts <= 20 )  {
                            filetab = ts;
                            IniSetOptions( );
                        }
                        else  {
                            MessageBeep( -1 );
                            return( TRUE );
                        }
                    }
                    else    {
                        MessageBeep( -1 );
                        return( TRUE );
                    }

                    GetWindowText( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_DIRECTORY ), szDir, 1000 );
                    IniSetOptionsStartupDir( szDir );
                    SetWindowLong(hwndDlg,DWL_MSGRESULT, FALSE);
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




UINT CALLBACK CCHookProc( HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )  {

    switch (uiMsg) {
        case WM_INITDIALOG: 
            RECT rect;

            GetWindowRect( GetParent( hwndDlg ), &rect );
            SetWindowPos( hwndDlg, HWND_TOP, rect.left + 64, rect.top + 64, 0, 0, SWP_NOSIZE );
            break;
    }
    return 0;
}
 



 
int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam )  {
    DWORD dwFontFlag = CF_SCREENFONTS | CF_FIXEDPITCHONLY | CF_NOVECTORFONTS;
 
    if(( SendMessage((HWND) lParam, CB_FINDSTRINGEXACT, (WPARAM) 0, (LONG) (LPTSTR) lpelfe->elfLogFont.lfFaceName )) != LB_ERR )
        return -1; 
 
    if ((dwFontFlag & CF_TTONLY) && (!(FontType & TRUETYPE_FONTTYPE))) 
        return -1; 
    if ((dwFontFlag & CF_SCALABLEONLY) && (FontType & RASTER_FONTTYPE)) 
        return -1; 
    if ((dwFontFlag & CF_ANSIONLY) && (lpelfe->elfLogFont.lfCharSet != ANSI_CHARSET)) 
        return -1; 
    if ((dwFontFlag & CF_FIXEDPITCHONLY) && (lpelfe->elfLogFont.lfPitchAndFamily & VARIABLE_PITCH)) 
        return -1; 
    if ((dwFontFlag & CF_NOVECTORFONTS) && (lpelfe->elfLogFont.lfCharSet == OEM_CHARSET)) 
        return -1; 

    if (*(lpelfe->elfLogFont.lfFaceName))  {
        SendMessage( (HWND)lParam, CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )lpelfe->elfLogFont.lfFaceName );
    }
    return -1;
} 



int CALLBACK EnumFontSizeExProc( ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam )  {
    HDC hdc; 
    long vr, ps;
    char szNo[ 10 ];
    int n, i = -1;

    hdc = GetDC( hwndClient ); 
    vr = GetDeviceCaps(hdc, LOGPIXELSY);

    if( FontType & TRUETYPE_FONTTYPE )  {
        //int aiHeight[ ] = { 11, 12, 13, 15, 16, 19, 21, 24, 27, 29, 32, 35, 37, 48, 64, 96 };
        int aiHeight[ 26 ]= { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36 };
        for( n = 0; n < 22; n++ )  {
            ps =  MulDiv( aiHeight[ n ], 72, vr );
            _itoa( ps, szNo, 10 );
            
            if(( SendMessage((HWND) lParam, CB_FINDSTRINGEXACT, (WPARAM) 0, (LONG) (LPTSTR)szNo )) == LB_ERR )  {
                i = SendMessage( (HWND)lParam, CB_INSERTSTRING, -1, ( LPARAM )( LPCSTR )szNo );
                SendMessage( (HWND)lParam, CB_SETITEMDATA, i, (LPARAM) (DWORD) aiHeight[ n ] );
            }
        }
        return 0;
    }
    //ps = (( lpntme->ntmTm.tmHeight - lpntme->ntmTm.tmInternalLeading ) *72) / vr;
    //ps = MulDiv( lpntme->ntmTm.tmHeight - lpntme->ntmTm.tmInternalLeading, 72, vr );
    ps = MulDiv( lpelfe->elfLogFont.lfHeight, 72, vr );
    _itoa( ps, szNo, 10 );

    if(( SendMessage((HWND) lParam, CB_FINDSTRINGEXACT, (WPARAM) 0, (LONG) (LPTSTR)szNo )) != LB_ERR )
        return 1 ; 

    i = SendMessage( (HWND)lParam, CB_GETCOUNT, 0, 0 );
    for( n = 0; n < i; ++n )  {
        if( lpelfe->elfLogFont.lfHeight == SendMessage( (HWND)lParam, CB_GETITEMDATA, n, 0 ))  {
            return -1;
        }
        if( lpelfe->elfLogFont.lfHeight < SendMessage( (HWND)lParam, CB_GETITEMDATA, n, 0 ))  {
            break;
        }
    }
    i = SendMessage( (HWND)lParam, CB_INSERTSTRING, n, ( LPARAM )( LPCSTR )szNo );
    SendMessage( (HWND)lParam, CB_SETITEMDATA, i, (LPARAM) (DWORD) lpelfe->elfLogFont.lfHeight );
    return -1;
} 




BOOL CALLBACK FormatOptionsDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    char tss[ 9 ];
    int ts, n;
    int i;
    static int aiDefault[ CE_CNT_COLORSET + 1 ];
    static int fInit = 0;
    static COLORREF acrCustClr[16];
    static struct colorset csbak[ CE_CNT_COLORSET + 1 ];

    static HFONT hFontBak;
    static LOGFONT lfBak;


    switch (message) {
        case WM_INITDIALOG:  {
            LOGFONT lf;

            _itoa( tablen, tss, 10 );
            SetWindowText( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ), tss );

            for( n = 0; n < CE_CNT_COLORSET + 1; ++n )  {
                csbak[ n ].fore = clrset[ n ].fore;
                csbak[ n ].back = clrset[ n ].back;
            }
            for( n = 0; n < CE_CNT_COLORSET; ++n )  {
                    SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_SELECT ), CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( selcolor[ n ]));
            }
            SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_SELECT ), CB_SETCURSEL, 0, ( LPARAM )( 0 ));

            if( ! fInit )  {
                aiDefault[ 1 ] = 0;
                for( n = 2; n < CE_CNT_COLORSET + 1; ++n )  {
                    if( clrset[ 1 ].back == clrset[ n ].back )  
                        aiDefault[ n ] = -1;
                }
                for( n = 0; n < 16; ++n )
                    acrCustClr[ n ] = 0x00FFFFFF;
                fInit = -1;
            }
            SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_FOREGROUND ), WM_USER, 0, ( LPARAM )clrset[ 1 ].fore );
            SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_BACKGROUND ), WM_USER, 0, ( LPARAM )clrset[ 1 ].back );
            if( crmacode )  SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_SYNTAX ), BM_SETCHECK, TRUE, 0 );
            if( fMatchBracket )  SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_MATCHBRACKET ), BM_SETCHECK, TRUE, 0 );
            EnableWindow( GetDlgItem( hwndDlg, IDC_COLOR_DEFAULT ), FALSE );

            hFontBak = hFont;
            memcpy( &lfBak, &lfScrnFont, sizeof( LOGFONT ));
            memset(&lf, 0, sizeof(LOGFONT));
            lf.lfCharSet = ANSI_CHARSET;
            strcpy(lf.lfFaceName, "" );
            EnumFontFamiliesEx( GetDC( hwndClient ), &lf,  ( FONTENUMPROC )EnumFontFamExProc, (LPARAM)GetDlgItem( hwndDlg, IDC_SETINGS_FONTS ), 0 );

            if(( i = SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTS ), CB_FINDSTRINGEXACT, (WPARAM) 0, (LONG)(LPTSTR)lfScrnFont.lfFaceName )) != LB_ERR )
                SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTS ), CB_SETCURSEL, i, 0L );                  
    
            SendMessage( hwndDlg, WM_COMMAND, MAKEWPARAM( IDC_SETINGS_FONTS, CBN_SELCHANGE ), 0 );
            return( TRUE );
        }
        case WM_USER:  {
            CHOOSECOLOR cc;                
            HBRUSH hbrush;                  
            DWORD rgbCurrent;        

            SetFocus( GetDlgItem( hwndDlg, IDC_COLOR_SELECT ));

            i = SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_SELECT ), CB_GETCURSEL, 0L, 0L );
            i += 1;
            if( ( HWND )wParam == GetDlgItem( hwndDlg, IDC_COLOR_FOREGROUND ))
                rgbCurrent = clrset[ i ].fore;
            else
                rgbCurrent = clrset[ i ].back;

            ZeroMemory(&cc, sizeof(CHOOSECOLOR));
            cc.lStructSize = sizeof(CHOOSECOLOR);
            cc.hwndOwner = hwndDlg;
            cc.lpCustColors = (LPDWORD) acrCustClr;
            cc.rgbResult = rgbCurrent;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ENABLEHOOK;
            cc.lpfnHook = CCHookProc;

            if (ChooseColor(&cc)==TRUE) {
                hbrush = CreateSolidBrush(cc.rgbResult);
                rgbCurrent = cc.rgbResult; 
                if( ( HWND )wParam == GetDlgItem( hwndDlg, IDC_COLOR_FOREGROUND ))  {
                    clrset[ i ].fore = rgbCurrent;
                    SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_FOREGROUND ), WM_USER, 0, ( LPARAM )clrset[ i ].fore );
                }
                else  {
                    if( i == 1 )  {
                        for( n = 2; n < CE_CNT_COLORSET + 1; ++n )  {
                            if( aiDefault[ n ] )  {
                                clrset[ n ].back = rgbCurrent;
                            }
                        }
                    }
                    else    {
                        SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_DEFAULT ), BM_SETCHECK, ( rgbCurrent == clrset[ 1 ].back ), 0 );
                    }
                    clrset[ i ].back = rgbCurrent;
                    SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_BACKGROUND ), WM_USER, 0, ( LPARAM )clrset[ i ].back );
                }
                //SendMessage( GetParent( hwndDlg ), PSM_CHANGED, ( WPARAM )hwndDlg, 0 );
            }
            break;
        }
        case WM_COMMAND:
            switch( LOWORD( wParam ))  {

                case IDC_COLOR_DEFAULT:
                    i = SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_SELECT ), CB_GETCURSEL, 0L, 0L );
                    i += 1;
                    aiDefault[ i ] = SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_DEFAULT ), BM_GETCHECK, 0, 0 );
                    if( aiDefault[ i ] )  {
                        clrset[ i ].back = clrset[ 1 ].back;
                        SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_BACKGROUND ), WM_USER, 0, ( LPARAM )clrset[ i ].back );
                    }
                    break;

                case IDC_COLOR_APPLY:  {
                    long lHeight;
                    char szFaceName[ LF_FACESIZE ];
                    HFONT hFontNew, hFontDel;
                    
                    i = SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_GETCURSEL, 0L, 0L );
                    lHeight = SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_GETITEMDATA, i, 0 );
                    i = SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTS ), CB_GETCURSEL, 0L, 0L );
                    SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTS ), CB_GETLBTEXT, i, ( LPARAM )szFaceName );

                    if(( strcmp( szFaceName, lfScrnFont.lfFaceName ))||( lHeight != lfScrnFont.lfHeight ))  {
                        if(( hFontNew = CreateFont( lHeight, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, szFaceName )) != NULL )  {
                            hFontDel = ( HFONT )SelectObject( GetDC( hwndClient ), hFontNew );
                            if( hFontDel != hFontBak )
                                DeleteObject( hFontDel );
                            hFont =  hFontNew;
                            lfScrnFont.lfHeight = lHeight;
                            strcpy( lfScrnFont.lfFaceName, szFaceName );
                            SendMessage( hwndClient, WM_USER+5, 0L, 0L );
                        }
                    }
                    InvalidateRect( hwndClient, NULL, TRUE );
                    scrn_statsl();
                    scrn_rescrn();
                    break;
                }
                case IDC_COLOR_SELECT:
#ifdef _WIN32
                    if( HIWORD( wParam ) == LBN_SELCHANGE )  {
#else
                    if( HIWORD( lParam ) == LBN_SELCHANGE )  {
#endif
                        i = SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_SELECT ), CB_GETCURSEL, 0L, 0L );
                        i += 1;
                        SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_FOREGROUND ), WM_USER, 0, ( LPARAM )clrset[ i ].fore );
                        SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_BACKGROUND ), WM_USER, 0, ( LPARAM )clrset[ i ].back );
 
                        if( i == 1 )  {
                            EnableWindow( GetDlgItem( hwndDlg, IDC_COLOR_DEFAULT ), FALSE );
                        }
                        else  {
                            EnableWindow( GetDlgItem( hwndDlg, IDC_COLOR_DEFAULT ), TRUE );
                        }
                        SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_DEFAULT ), BM_SETCHECK, aiDefault[ i ], 0 );
                    }
                    break;

                case IDC_SETINGS_FONTS:
                    if( HIWORD( wParam )== CBN_SELCHANGE )  { //  ????? doesn't work as suppose to, maybe because of property page dialog
                        char szFontName[ LF_FACESIZE ];
                        LOGFONT lf;

                        i = SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTS ), CB_GETCURSEL, (WORD)0, 0L );
                        SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTS ), CB_GETLBTEXT, (WORD)i, (LONG)szFontName );
                        SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_RESETCONTENT, 0L, 0L );
 
                        memset(&lf, 0, sizeof(LOGFONT));
                        lf.lfCharSet = ANSI_CHARSET;
                        strcpy(lf.lfFaceName, szFontName );
                        EnumFontFamiliesEx( GetDC( hwndClient ), &lf, ( FONTENUMPROC )EnumFontSizeExProc, (LPARAM)GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), 0 );

                        i = SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_GETCOUNT, 0, 0 );
                        for( n = 0; n < i; ++n )  {
                            if( abs( lfScrnFont.lfHeight ) >= SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_GETITEMDATA, n, 0 ))  {
                                SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_SETCURSEL, n, 0L );
                            }
                        }
                        if( SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_GETCURSEL, 0L, 0L )== -1 )  {
                            SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_SETCURSEL, 0L, 0L );
                        }
                        //SendMessage( GetParent( hwndDlg ), PSM_CHANGED, ( WPARAM )hwndDlg, 0 );
                    }
                    break;

                case IDC_EDITOPTIONS_FONT:
                    if( SetFont( hwndDlg, GetDC( hwndDlg )))  {
                        SendMessage( hwndClient, WM_USER+5, 0L, 0L );
                    }
                    SetFocus( hwndDlg );
                    return( TRUE );

                default:
                    break;
            }
            break;

        case WM_NOTIFY:
            switch (((NMHDR FAR *) lParam)->code) {

                case PSN_QUERYCANCEL:
                    for( n = 0; n < CE_CNT_COLORSET + 1; ++n )  {
                        clrset[ n ].fore = csbak[ n ].fore;
                        clrset[ n ].back = csbak[ n ].back;
                    }
                    if( hFont != hFontBak )  {
                        HFONT hFontDel;

                        hFontDel = ( HFONT )SelectObject( GetDC( hwndClient ), hFontBak );
                        DeleteObject( hFontDel );
                        hFont = hFontBak;
                        memcpy( &lfScrnFont, &lfBak, sizeof( LOGFONT ));
                        SendMessage( hwndClient, WM_USER+5, 0L, 0L );
                    }
                    InvalidateRect( hwndClient, NULL, TRUE );
                    scrn_statsl();
                    scrn_rescrn();
                    break;

                case PSN_KILLACTIVE:
                    GetWindowText( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ), tss, 8 );
                    if( ts = atoi( tss ))  {
                        if( ts <= 20 )  {
                        }
                        else  {
                            SetFocus( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ));
                            MessageBeep( -1 );
                            SetWindowLong(hwndDlg,DWL_MSGRESULT, TRUE);
                            return( TRUE );
                        }
                    }
                    else  {
                        SetFocus( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ));
                        MessageBeep( -1 );
                        SetWindowLong(hwndDlg,DWL_MSGRESULT, TRUE);
                        return( TRUE );
                    }
                    break;

                case PSN_APPLY:
                    GetWindowText( GetDlgItem( hwndDlg, IDC_FILEOPTIONS_TABSIZE ), tss, 8 );
                    if( ts = atoi( tss ))  {
                        if( ts <= 20 )  {
                            tablen = ts;
                            IniSetOptions( );
                        }
                    }
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_SYNTAX ), BM_GETCHECK, 0, 0 ))
                            crmacode = -1;
                    else
                            crmacode = 0;

                    if( SendMessage( GetDlgItem( hwndDlg, IDC_COLOR_MATCHBRACKET ), BM_GETCHECK, 0, 0 ))
                            fMatchBracket = -1;
                    else
                            fMatchBracket = 0;

                    if( crmacode )
                        crma_lines( ed->firstline );

                    {
                        long lHeight;
                        char szFaceName[ LF_FACESIZE ];
                        HFONT hFontNew, hFontDel;

                        i = SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_GETCURSEL, 0L, 0L );
                        lHeight = SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTSIZE ), CB_GETITEMDATA, i, 0 );
                        i = SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTS ), CB_GETCURSEL, 0L, 0L );
                        SendMessage( GetDlgItem( hwndDlg, IDC_SETINGS_FONTS ), CB_GETLBTEXT, i, ( LPARAM )szFaceName );

                        if(( strcmp( szFaceName, lfScrnFont.lfFaceName ))||( lHeight != lfScrnFont.lfHeight ))  {
                            if(( hFontNew = CreateFont( lHeight, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, szFaceName )) != NULL )  {
                                hFontDel = ( HFONT )SelectObject( GetDC( hwndClient ), hFontNew );
                                if( hFontDel != hFontBak )
                                    DeleteObject( hFontDel );
                                hFont =  hFontNew;
                                lfScrnFont.lfHeight = lHeight;
                                strcpy( lfScrnFont.lfFaceName, szFaceName );
                                SendMessage( hwndClient, WM_USER+5, 0L, 0L );
                            }
                        }
                    }
                    DeleteObject( hFontBak );

                    InvalidateRect( hwndClient, NULL, TRUE );
                    scrn_statsl();
                    scrn_rescrn();
                    IniSetColor( );
                    IniSetScrnFont( lfScrnFont );
 
                    SetWindowLong(hwndDlg,DWL_MSGRESULT, FALSE);
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



UINT CALLBACK CFHookProc( HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )  {

    switch (uiMsg) {
        case WM_INITDIALOG:
            RECT rect;

            GetWindowRect( GetParent( hwndDlg ), &rect );
            SetWindowPos( hwndDlg, HWND_TOP, rect.left + 64, rect.top + 64, 0, 0, SWP_NOSIZE );
            break;
    }
    return 0;
}
 


SetFont( HWND hwnd, HDC hDC )  {
    CHOOSEFONT cf;
    HFONT hFontDel;
    LOGFONT lf;


    memset(&cf, 0, sizeof(CHOOSEFONT));
    if( fScrnFont )  memcpy( &lf, &lfScrnFont, sizeof( LOGFONT ));

    cf.hInstance = ( HINSTANCE )hInst;
    cf.lpTemplateName = "FORMATDLGORD31XX";


    cf.lStructSize = sizeof(CHOOSEFONT);
    cf.hwndOwner = hwnd;
    cf.lpLogFont = &lf;
    cf.Flags = CF_SCREENFONTS | CF_FIXEDPITCHONLY | CF_NOVECTORFONTS | CF_ENABLETEMPLATE ;
#ifdef _WIN32
    cf.Flags |= CF_NOSCRIPTSEL;
#endif
    cf.Flags |= CF_ENABLEHOOK;
    cf.lpfnHook = CFHookProc;

    if( fScrnFont ) cf.Flags |= CF_INITTOLOGFONTSTRUCT;
    cf.nFontType = SCREEN_FONTTYPE;
 
    if( ChooseFont(&cf))  {
        /*if(( cf.lpLogFont->lfOutPrecision == OUT_STROKE_PRECIS )||( cf.nFontType & ITALIC_FONTTYPE ))  {
                dlg_err( "Unable to Change Font" );
                return( FALSE);  // dont allow tt fonts or italics
                }*/
        cf.lpLogFont->lfPitchAndFamily |= FIXED_PITCH;
        hFont = CreateFontIndirect( cf.lpLogFont );
        hFontDel = ( HFONT )SelectObject( hDC, hFont );
        DeleteObject( hFontDel );
        cf.lpLogFont->lfWeight = FW_BOLD;
        hFontBold = CreateFontIndirect( cf.lpLogFont );
        cf.lpLogFont->lfWeight = FW_LIGHT;
        fScrnFont = -1;
        memcpy( &lfScrnFont, cf.lpLogFont, sizeof( LOGFONT ));
        IniSetScrnFont( lfScrnFont );
        return( TRUE );
    }
    return( FALSE );
}



BOOL CALLBACK EditOptionsDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {

    switch (message) {
        case WM_INITDIALOG:
            SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_HSCROLLBAR ), BM_SETCHECK, TRUE, 0 );
            SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_VSCROLLBAR ), BM_SETCHECK, TRUE, 0 );

            if( fSaveWinPos )  SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_POSITION ), BM_SETCHECK, TRUE, 0 );
            if( fFocusMousePos )  SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_MOUSEPOS ), BM_SETCHECK, TRUE, 0 );
            if( fFileTree )  SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_FILETREE ), BM_SETCHECK, TRUE, 0 );
#ifdef _WIN
            EnableWindow( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_INSTANCE ), FALSE );
#else
            if( fMultiInstance )  SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_INSTANCE ), BM_SETCHECK, TRUE, 0 );
#endif

            if( fExplorerFileDlg )  SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_EXPLORER ), BM_SETCHECK, TRUE, 0 );
            if( fTitleFileName )  SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_FNTITLE ), BM_SETCHECK, TRUE, 0 );
            else  SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_FNSTATUS ), BM_SETCHECK, TRUE, 0 );

            return( TRUE );

        case WM_NOTIFY:
            switch (((NMHDR FAR *) lParam)->code) {

                case PSN_APPLY:
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_POSITION ), BM_GETCHECK, 0, 0 ))
                            fSaveWinPos = 1;
                    else
                            fSaveWinPos = 0;
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_INSTANCE ), BM_GETCHECK, 0, 0 ))
                            fMultiInstance = 1;
                    else
                            fMultiInstance = 0;
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_EXPLORER ), BM_GETCHECK, 0, 0 ))
                            fExplorerFileDlg = 1;
                    else
                            fExplorerFileDlg = 0;
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_FNTITLE ), BM_GETCHECK, 0, 0 ))
                            fTitleFileName = 1;
                    else
                            fTitleFileName = 0;

                    /*if( fSaveWinPos )
                            IniSetWinPos( );   ??????????? */
 
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_MOUSEPOS ), BM_GETCHECK, 0, 0 ))
                            fFocusMousePos = 1;
                    else
                            fFocusMousePos = 0;

                    if( SendMessage( GetDlgItem( hwndDlg, IDC_EDITOPTIONS_FILETREE ), BM_GETCHECK, 0, 0 ))  {
                        if( ! fFileTree )  {
                            fFileTree = 1;
                            fFileTree = 1;
                            ShowFileTree( TRUE );
                        }
                    }
                    else  {
                        if( fFileTree )  {
                            fFileTree = 0;
                            fFileTree = 0;
                            ShowFileTree( FALSE );
                        }
                    }

                    scrn_statsl( );
                    if( fTitleFileName )
                            InvalidateRect( hwndStatus, NULL, TRUE );
                    else
                            SetWindowText( hwndMain, szMainTitle );
                    IniSetWinOptions( fMultiInstance, fTitleFileName, fExplorerFileDlg, fSaveWinPos, fFocusMousePos, fFileTree );
                    SetWindowLong(hwndDlg,DWL_MSGRESULT, FALSE);
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



BOOL CALLBACK HelpFilesDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    int n;
    char szFileName[ CE_LEN_FILEPATH + 1 ];
    static String strHelpFiles[ CE_CNT_ASSTYPE ];

    switch (message) {

        case WM_INITDIALOG:  {
            HKEY hKey;
            LONG lRc;
            DWORD cbValue;
            DWORD dwType;

            lRc = RegOpenKeyEx( HKEY_LOCAL_MACHINE, szRegKey, 0, KEY_READ, &hKey );

            for( n = 0; n < CE_CNT_ASSTYPE; ++n )  {
                SendMessage( GetDlgItem( hwndDlg, IDC_HELPFILES_LANGUAGE ), CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( asstype[ n ] ));
                //strHelpFiles[ n ] = new String;
                cbValue = CE_LEN_FILEPATH;
                szFileName[ 0 ] = 0;
                lRc = RegQueryValueEx( hKey, ( LPCSTR )asstype[ n ], 0, &dwType, ( LPBYTE )szFileName, &cbValue );

                strHelpFiles[ n ] = szFileName;
            }
            RegCloseKey( hKey );
            SendMessage( GetDlgItem( hwndDlg, IDC_HELPFILES_LANGUAGE ), CB_SETCURSEL, 0, ( LPARAM )( 0 ));
            SetWindowText( GetDlgItem( hwndDlg, IDC_HELPFILES_FILENAME ), strHelpFiles[ 0 ] );
            return( TRUE );
        }
        case WM_COMMAND:

            switch( LOWORD( wParam ))  {
                case IDC_HELPFILES_LIST:
                    GetWindowText( GetDlgItem( hwndDlg, IDC_HELPFILES_FILENAME ), szFileName, CE_LEN_FILEPATH );
                    if( ! *szFileName )
                        strcpy( szFileName, "C:\\" );
                    if( FileBrowse( hwndDlg, szFileName, "File Select", "Help Files (*.hlp; *.chm; *.col)\0*.hlp;*.chm;*.col\0", 0 ))  {
                        SetWindowText( GetDlgItem( hwndDlg, IDC_HELPFILES_FILENAME ), szFileName );
                        int i = SendMessage( GetDlgItem( hwndDlg, IDC_HELPFILES_LANGUAGE ), CB_GETCURSEL, 0L, 0L );
                        strHelpFiles[ i ] = szFileName;
                    }
                    break;

                case IDC_HELPFILES_LANGUAGE:
#ifdef _WIN32
                    if( HIWORD( wParam ) == LBN_SELCHANGE )  {
#else
                    if( HIWORD( lParam ) == LBN_SELCHANGE )  
#endif
                        int i = SendMessage( GetDlgItem( hwndDlg, IDC_HELPFILES_LANGUAGE ), CB_GETCURSEL, 0L, 0L );
                        SetWindowText( GetDlgItem( hwndDlg, IDC_HELPFILES_FILENAME ), strHelpFiles[ i ] );
                    }
                    break;

                case IDC_HELPFILES_FILENAME:
#ifdef _WIN32
                    if( HIWORD( wParam ) == EN_KILLFOCUS )  { 
#else
                    if( HIWORD( lParam ) == EN_KILLFOCUS )  
#endif
                        int i = SendMessage( GetDlgItem( hwndDlg, IDC_HELPFILES_LANGUAGE ), CB_GETCURSEL, 0L, 0L );
                        GetWindowText( GetDlgItem( hwndDlg, IDC_HELPFILES_FILENAME ), szFileName,  CE_LEN_FILEPATH );
                        strHelpFiles[ i ] = szFileName;
                    }
                    break;

                default:
                    break;
            }
            break;
                
        case WM_NOTIFY:

            switch (((NMHDR *) lParam)->code) {
                case PSN_APPLY:  {
                    HKEY hKey;
                    LONG lRc;
                    DWORD cbValue;
                    DWORD dwDisposition;

                    int i = SendMessage( GetDlgItem( hwndDlg, IDC_HELPFILES_LANGUAGE ), CB_GETCURSEL, 0L, 0L );
                    GetWindowText( GetDlgItem( hwndDlg, IDC_HELPFILES_FILENAME ), szFileName,  CE_LEN_FILEPATH );
                    strHelpFiles[ i ] = szFileName;

                    //lRc = RegOpenKeyEx( HKEY_LOCAL_MACHINE, szRegKey, 0, KEY_ALL_ACCESS, &hKey );
                    lRc = RegCreateKeyEx( HKEY_LOCAL_MACHINE, szRegKey, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );
                    for( n = 0; n < CE_CNT_ASSTYPE; ++n )  {
                        lRc = RegSetValueEx( hKey, ( LPCSTR )asstype[ n ], 0, REG_SZ, ( CONST BYTE *)( const char * )strHelpFiles[ n ], ( cbValue = strHelpFiles[ n ].len() + 1 ) );
                    }
                    RegCloseKey( hKey );
                    break;
                }
                default:
                    break;
            }
            break;

        default:
            break;
    }
    return( FALSE );
}



void KeywordHelp()  {
    char szSelect[CE_LEN_FINDSTR];
    HKEY hKey;
    CHAR szFileName[ CE_LEN_FILEPATH + 1 ];
    LONG lRc;
    DWORD cbValue;
    DWORD dwType;
    char *s;


    find_getcurselword( szSelect );
    if( ! *szSelect )  {
        MessageBeep(-1);
        return;
    }
    if( lRc = RegOpenKey( HKEY_LOCAL_MACHINE, szRegKey, &hKey ))  {
        MessageBeep(-1);
        return;
    }
    cbValue = CE_LEN_FILEPATH;
    szFileName[ 0 ] = 0;
    lRc = RegQueryValueEx( hKey, ( LPCSTR )asstype[ ed->assoc ], 0, &dwType, ( LPBYTE )szFileName, &cbValue );
    RegCloseKey( hKey );

    if( ! *szFileName )  {
        MessageBeep(-1);
        return;
    }
    if(( s = strrchr( szFileName, '.' )) == NULL )  {
        MessageBeep(-1);
        return;
    }
    s += 1;
    if(( _stricmp( s, "chm" ) == 0 )||( _stricmp( s, "col" ) == 0 ))  {
        HH_AKLINK link;
        link.cbStruct =     sizeof(HH_AKLINK) ;
        link.fReserved =    FALSE ;
        link.pszKeywords =  szSelect ;
        link.pszUrl =       NULL ;
        link.pszMsgText =   NULL ;
        link.pszMsgTitle =  NULL ;
        link.pszWindow =    NULL ;
        link.fIndexOnFail = TRUE ;

        HINSTANCE inst = LoadLibrary("HHCTRL.OCX");
        (FARPROC&) HTMLHelp = GetProcAddress(inst, "HtmlHelpA");

        HTMLHelp( NULL, szFileName, HH_DISPLAY_TOPIC, 0 );
        HTMLHelp( NULL, szFileName, HH_KEYWORD_LOOKUP, ( DWORD )&link );
    }
    else if( _stricmp( s, "hlp" ) == 0 )  {
        if( ! WinHelp(hwndMain, szFileName, HELP_KEY, (DWORD)szSelect ))
                MessageBeep( MB_OK );
    }
    else  {
        MessageBeep(-1);
    }
}




#ifdef __cplusplus
}
#endif
