

#include "windows.h"

#include "string.h"

#include "commdlg.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"

print_atrs( HDC hDC, struct linerec *ln, int y );


extern struct editrec *ed;
extern int blck;
extern struct linerec *blckline;
extern struct coords blckstr, blckend;
extern int crmacode;


extern char szMainTitle[ ];

extern HINSTANCE hInst;
extern HWND hwndClient, hwndScroll, hwndMain, hwndStatus;
extern HDC hDC;

static int fPDInit = 0;
PRINTDLG pd;

static BOOL bPrint = FALSE;

static HWND hdlgCancel = NULL;

BOOL CALLBACK AbortProc( HDC, int );

static char szPrintMsg[ 800 ] = "";

static HFONT hFontNormal, hFontBold, hFontLight;
static long lLeftMarginLc;




UINT CALLBACK PrintHookProc( HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )  {
    RECT rect;

    switch (uiMsg) {
        case WM_INITDIALOG:

        GetWindowRect( GetParent( hwndDlg ), &rect );
        SetWindowPos( hwndDlg, HWND_TOP, rect.left + 64, rect.top + 64, 0, 0, SWP_NOSIZE );
        break;
    }
    return 0;
}



BOOL CALLBACK AbortProc( HDC hdc, int nCode )  {
    MSG msg;

    while (PeekMessage((LPMSG) &msg, (HWND) NULL, 0, 0, PM_REMOVE)) {
 
        if (!IsDialogMessage( hdlgCancel, (LPMSG) &msg )) {
            TranslateMessage( (LPMSG) &msg );
            DispatchMessage( (LPMSG) &msg );
        }
    }
    return bPrint;
}



LRESULT CALLBACK PrintCanDlg( HWND hwndDlg, WORD message, WORD wParam, LONG lParam )  {
    HDC hdc;
    SIZE size;
    RECT rectl, rectlb;
    int xo, xd;
 
    switch (message) {
        case WM_INITDIALOG:
            SetWindowText( hwndDlg, "File Print" );
            if( szPrintMsg )  {
                SetWindowText( GetDlgItem( hwndDlg, IDC_PRINTCAN_FILENAME ), szPrintMsg );
#ifdef _WIN32
                hdc = GetDC( hwndStatus );      // Not Good !!!!!!!!!!!!
                xo = (( 38 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#else
                hdc = GetDC( hwndDlg );
                xo = (( 30 * LOWORD( GetDialogBaseUnits( ))) / 4 );
#endif
                GetTextExtentPoint( hdc, szPrintMsg, strlen( szPrintMsg ), &size );
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

                case DID_CANCEL:
                    bPrint = FALSE;
                    //EndDialog( hwndDlg, DID_CANCEL );
                    return( TRUE );

                default:
                    break;
            }
            break;

        default:
            break;
    }
    //UNREFERENCED_PARAMETER(lParam);
    //UNREFERENCED_PARAMETER(wParam);
    //UNREFERENCED_PARAMETER(message);
    return( FALSE );
}



PrintSetup( HWND hwnd )  {
 
    if( ! fPDInit )  {
        memset(&pd, 0, sizeof(PRINTDLG));
    }
    pd.lStructSize = sizeof(PRINTDLG);
    pd.hwndOwner = hwnd;
    pd.Flags = 0;
    pd.Flags |= PD_PRINTSETUP;
    pd.Flags |= PD_ENABLESETUPHOOK;
    pd.lpfnSetupHook = PrintHookProc;

    if( PrintDlg( &pd ) != 0 )  {
        fPDInit = -1;
    }
}



FilePrint( HWND hwnd )  {
    int fPrntFont = 0;
    LOGFONT lfPrntFont;
    INT iPointSize;
    HFONT hFontOld;
    DOCINFO di;
    struct linerec *ln;
    int yChar, nCharsPerLine, nLinesPerPage, nTotalPages, nPage, nLine, nLineNum ;
    int  nTotalLines;
    TEXTMETRIC tm ;
    char szPrintTitle[ CE_LEN_FILEPATH + 20 ];
    int        nyLogPix, nxLogPix;
    LPDEVNAMES lpDevNames;
    LPSTR      lpszDriverName;
    LPSTR      lpszDeviceName;
    LPSTR      lpszPortName;
    POINT pt[1];
    int iLeftMarginMm = 0;
    int fSyntax;
 

    if(( ed->nolines == 1 )&&( ! *ed->firstline->strz ))
        return( 0 );

    if( ! fPDInit )  {
        memset(&pd, 0, sizeof(PRINTDLG));
    }
    pd.lStructSize = sizeof(PRINTDLG);
    pd.hwndOwner = hwnd;
    pd.Flags = 0;
    pd.Flags = PD_RETURNDC | PD_HIDEPRINTTOFILE | PD_NOPAGENUMS | PD_USEDEVMODECOPIES;

    if( !(( blck )&&( blckend.y != blckstr.y )))
        pd.Flags |= PD_NOSELECTION;

    //pd.Flags |= PD_PRINTSETUP;
    pd.Flags |= PD_ENABLEPRINTHOOK;
    pd.lpfnPrintHook = PrintHookProc;

    if( PrintDlg( &pd ) != 0 )  {
        fPDInit = -1;

        lpDevNames = (LPDEVNAMES) GlobalLock(pd.hDevNames);
        lpszDriverName = (LPSTR) lpDevNames + lpDevNames->wDriverOffset;
        lpszDeviceName = (LPSTR) lpDevNames + lpDevNames->wDeviceOffset;
        lpszPortName   = (LPSTR) lpDevNames + lpDevNames->wOutputOffset;

        strcpy( szPrintMsg, "Printing to " );
        strcat( szPrintMsg, lpszDeviceName );
        strcat( szPrintMsg, " on " );
        strcat( szPrintMsg, lpszPortName );

        GlobalUnlock( pd.hDevNames );

        memset( &lfPrntFont, 0, sizeof( LOGFONT ));
        //lfPrntFont.lfWeight = FW_NORMAL;
        //lfPrntFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
 

        IniGetPrint( &iLeftMarginMm, &fSyntax );
        fPrntFont = IniGetPrntFont( &lfPrntFont, &iPointSize );

        if( fPrntFont )  {
            nyLogPix = GetDeviceCaps( pd.hDC, LOGPIXELSY );
            lfPrntFont.lfHeight = -(( iPointSize / 12 ) * nyLogPix )/ 72;
            //lf.lfHeight = -MulDiv( iPointSize, GetDeviceCaps( pd.hDC, LOGPIXELSY ), 72 );

            lfPrntFont.lfWeight = 700;
            hFontBold = CreateFontIndirect( &lfPrntFont );
            lfPrntFont.lfWeight = 400;
            hFontNormal = CreateFontIndirect( &lfPrntFont );
            lfPrntFont.lfWeight = 100;
            //lfPrntFont.lfStrikeOut  = TRUE;
            hFontLight = CreateFontIndirect( &lfPrntFont );

            if( hFontNormal != NULL )  {
                hFontOld = ( HFONT )SelectObject( pd.hDC, hFontNormal );
                if( hFontOld != NULL )  
                    DeleteObject( hFontOld );
            }
        }
 
        strcpy( szPrintTitle, szMainTitle );
 
        if( *ed->fname )  {
            strcat( szPrintTitle, " [" );
            strcat( szPrintTitle, ed->fname );
            strcat( szPrintTitle, "]" );
        }

        memset( &di, 0, sizeof( DOCINFO ) );
        di.cbSize = sizeof(DOCINFO);
        di.lpszDocName = szPrintTitle;
        di.lpszOutput = (LPTSTR) NULL;
        di.fwType = 0;

        GetTextMetrics (pd.hDC, &tm) ;
        yChar = ( int )( tm.tmHeight + tm.tmExternalLeading );
 
        if( pd.Flags &= PD_SELECTION )  {
            ln = blckline;
            nTotalLines = blckend.y - blckstr.y;
        }
        else  {
            ln = ed->firstline;
            nTotalLines = ed->nolines;
        }

        nCharsPerLine = ( int )( GetDeviceCaps (pd.hDC, HORZRES) / tm.tmAveCharWidth );
        nLinesPerPage = ( int )GetDeviceCaps (pd.hDC, VERTRES) / yChar ;
        nTotalPages   = (nTotalLines + nLinesPerPage - 1) / nLinesPerPage ;

        if( iLeftMarginMm )  {
            nxLogPix = GetDeviceCaps( pd.hDC, LOGPIXELSX );
            pt[0].x = MulDiv(iLeftMarginMm, nxLogPix, 25);
            pt[0].y = 200;
 
            if( ! DPtoLP( pd.hDC, ( POINT *)&pt, 1 ))  {
                    }
            lLeftMarginLc = pt[0].x;
        }
        else    lLeftMarginLc = 0;
 

        //edit_wait( -1 );
        bPrint = TRUE;

        //if( SetAbortProc( pd.hDC, AbortProc ) == SP_ERROR )  ;

        hdlgCancel = CreateDialog( ( HINSTANCE )hInst, (LPTSTR) "PrintCanDlg", hwnd, (DLGPROC) PrintCanDlg );
        EnableWindow(hwnd, FALSE);

        if(( StartDoc( pd.hDC, &di )) < 0 )  ;

        for (nPage = 0 ; nPage < nTotalPages ; nPage++)  {

            if(( StartPage( pd.hDC )) < 0 )  break;

            SelectObject( pd.hDC, hFontNormal );          // HP print device resets DC after StartPage

            for (nLine = 0 ; nLine < nLinesPerPage ; nLine++)  {
                nLineNum = nLinesPerPage * nPage + nLine ;
 
                if (nLineNum >= nTotalLines)
                    break ;

                if( fSyntax )
                    print_atrs( pd.hDC, ln, yChar * nLine );
                else
                    TextOut( pd.hDC, lLeftMarginLc, yChar * nLine, ln->strz, strlen( ln->strz ));
 
                if( ln->next == NULL )  break;
                if( ! bPrint )  break;
                ln = ln->next;
            }

            if(( EndPage( pd.hDC )) < 0 )  break;
            AbortProc( pd.hDC, 0 );
            if( ! bPrint )  break;
        }

        if( ! bPrint )  {
            if(( AbortDoc( pd.hDC )) < 0 )  ;
        }
        else  {
            if(( EndDoc( pd.hDC )) < 0 )  ;
        }

        DeleteDC(pd.hDC);
        //edit_wait( 0 );

        DeleteObject( hFontNormal );
        DeleteObject( hFontBold );
        DeleteObject( hFontLight );

        EnableWindow( hwnd, TRUE );
        DestroyWindow( hdlgCancel );
    }
    else  {
        // CommDlgErr
    }
    return 0;
}



print_strn( HDC hDC, char *sz, int y, int a, int l )  {
    SIZE size;
    static int x = 0;
    static yo = -1;

    if( yo != y )  {
        x = lLeftMarginLc;
        yo = y;
    }

    if( a == CE_CLR_NORMAL )
        SelectObject( hDC, hFontNormal );
    else if( a == CE_CLR_KEYWORD )
        SelectObject( hDC, hFontBold );
    else if( a == CE_CLR_COMMENT )
        SelectObject( hDC, hFontLight );
    else
        SelectObject( hDC, hFontNormal );

    if( a == CE_CLR_COMMENT )
        SetTextColor( hDC, RGB( 128, 128, 128 ));
    else
        SetTextColor( hDC, RGB( 0, 0, 0 ));

    TextOut( hDC, x, y, sz, l);

    GetTextExtentPoint32( hDC, ( LPCTSTR )sz, l, &size );
    x += size.cx;
}




print_atrs( HDC hDC, struct linerec *ln, int y )  {
    int c, cx, lx;

    lx = 0;

    if( ! crmacode )  {
        print_strn( hDC, ln->strz, y, CE_CLR_NORMAL, strlen( ln->strz ));
        return;
    }

    if(( ln->cmnt & 1 )&&( !( ln->cmnt & 4 )))  {
        print_strn( hDC, ln->strz, y, CE_CLR_COMMENT, strlen( ln->strz ));
        return;
    }
 
    if( ! ln->atrc )  {
        print_strn( hDC, ln->strz, y, CE_CLR_NORMAL, strlen( ln->strz ));
        return;
    }

    for( c = 0; c < ln->atrc; ++c )  {
        if( lx <= ln->atrs[ c ].pos + ln->atrs[ c ].cnt )
            break;
    }

    if( c < ln->atrc )  {
        if(( lx > ln->atrs[ c ].pos )&&( lx <= ln->atrs[ c ].pos + ln->atrs[ c ].cnt ))   {
            cx = ln->atrs[ c ].cnt - ( lx - ln->atrs[ c ].pos );
            print_strn( hDC, &ln->strz[ lx ], y, ln->atrs[ c ].atr, cx );
            lx += cx;
            ++c;
        }
 
        for( ; c < ln->atrc; ++c )  {
            if( ln->atrs[ c ].pos > lx )  {
                cx =  ln->atrs[ c ].pos - lx;
                print_strn( hDC, &ln->strz[ lx ], y, CE_CLR_NORMAL, cx );
                lx = ln->atrs[ c ].pos;
            }
            cx = ln->atrs[ c ].cnt;
            print_strn( hDC, &ln->strz[ lx ], y, ln->atrs[ c ].atr, cx );
            lx += ln->atrs[ c ].cnt;
        }
    }
    if( ln->strz[ lx ] )  {         // !!! Could be dangerous but is faster than strlen()
    //if( lx < strlen( ln->strz ))  {
        print_strn( hDC, &ln->strz[ lx ], y, CE_CLR_NORMAL, strlen( &ln->strz[ lx ]) );
    }
    return;
}



UINT CALLBACK CFHookProc2( HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam )  {
    RECT rect;
    int iLeftMargin, fSyntaxColor;
    char sz[ 20 ];
    LONG lWindowStyle;


    switch (uiMsg) {
        case WM_INITDIALOG:

            lWindowStyle = GetWindowLong( hwndDlg, GWL_EXSTYLE );
            lWindowStyle &= ~( WS_EX_CONTEXTHELP );
            SetWindowLong( hwndDlg, GWL_EXSTYLE, lWindowStyle );

            GetWindowRect( GetParent( hwndDlg ), &rect );
            SetWindowPos( hwndDlg, HWND_TOP, rect.left + 64, rect.top + 64, 0, 0, SWP_NOSIZE );
 
            IniGetPrint( &iLeftMargin, &fSyntaxColor );

            _itoa( iLeftMargin, sz, 10 );
            SetWindowText( GetDlgItem( hwndDlg, IDC_PAGESETUP_LEFTMARGIN ), sz );

            SendMessage( GetDlgItem( hwndDlg, IDC_PAGESETUP_SYNTAXCOLOR ), BM_SETCHECK, fSyntaxColor, 0 );
 
            SetFocus( GetDlgItem( hwndDlg, IDC_PAGESETUP_LEFTMARGIN ));
 
            break;

        case WM_COMMAND:
            switch( wParam )  {
                case IDOK:
                    GetWindowText( GetDlgItem( hwndDlg, IDC_PAGESETUP_LEFTMARGIN ), sz, 20 );
                    iLeftMargin = atoi( sz );

                    fSyntaxColor = SendMessage( GetDlgItem( hwndDlg, IDC_PAGESETUP_SYNTAXCOLOR  ), BM_GETCHECK, 0, 0 );

                    IniSetPrint( iLeftMargin, fSyntaxColor );
 
                default:
                    break;
            }
            break;

        default:
            break;

    }
    return 0;
}



PrinterFont( HWND hwnd, HDC hDC )  {
    CHOOSEFONT cf;
    int fPrntFont = FALSE;
    LOGFONT lfPrntFont;
    INT iPointSize;

    memset( &lfPrntFont, 0, sizeof( LOGFONT ));

    fPrntFont = IniGetPrntFont( &lfPrntFont, &iPointSize );

    memset(&cf, 0, sizeof(CHOOSEFONT));

    cf.hInstance = ( HINSTANCE )hInst;
    cf.lpTemplateName = "FORMATDLGORD31XX1";

    cf.lStructSize = sizeof(CHOOSEFONT);
    cf.hwndOwner = hwnd;
    cf.hDC = hDC;
    cf.lpLogFont = &lfPrntFont;
    cf.Flags = CF_PRINTERFONTS | CF_ENABLETEMPLATE ; // | CF_FIXEDPITCHONLY | CF_NOVECTORFONTS;
#ifdef _WIN32
    cf.Flags |= CF_NOSCRIPTSEL;
#endif

    cf.Flags |= CF_ENABLEHOOK;
    cf.lpfnHook = CFHookProc2;

    if( fPrntFont ) cf.Flags |= CF_INITTOLOGFONTSTRUCT;
    cf.nFontType = PRINTER_FONTTYPE;

    if( ChooseFont( &cf ))  {
        fPrntFont = -1;
        memcpy( &lfPrntFont, cf.lpLogFont, sizeof( LOGFONT ));
        IniSetPrntFont( lfPrntFont, cf.iPointSize );
    }
}




#ifdef __cplusplus
}
#endif
