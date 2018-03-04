
#include "windows.h"

#include "string.h"
#include "stdlib.h"

#include "resource.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"



extern HINSTANCE hInst;
extern HWND hwndMain, hwndClient;

extern int blck;
extern struct coords blckstr, blckend;


extern int ww;
extern int mc;
extern int wh;
extern int vf;
extern int fs;
extern int rf;
extern int af;

extern char fshlst[ CE_MAX_FINDHIST ][ CE_LEN_FINDSTR ];
extern char cshlst[ CE_MAX_FINDHIST ][ CE_LEN_FINDSTR ];
extern int cshcnt;
extern int fshcnt;

extern char findstr[CE_LEN_FINDSTR];
extern char chngstr[CE_LEN_FINDSTR];

extern char fillchar[ CE_LEN_FILLSTR + 1 ];

extern int gotolinenr;
extern int gotofilenr;

HWND hwndDlgActive = NULL;



BOOL CALLBACK FindDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    int n;
    static int wht = 0;

    switch (message) {
        case WM_INITDIALOG:
            for( n = fshcnt; n > 0; n-- )  {
                SendMessage( GetDlgItem( hwndDlg, IDC_FIND_FIND ), CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( fshlst[ n - 1] ));
            }
            SetWindowText( GetDlgItem( hwndDlg, IDC_FIND_FIND ), findstr );
            //SendMessage( GetDlgItem( hwndDlg, IDC_FIND_FIND ), CB_SELECTSTRING, -1, ( LPARAM )( LPCSTR )findstr );

            if( ww )  SendMessage( GetDlgItem( hwndDlg, IDC_FIND_WORD ), BM_SETCHECK, TRUE, 0 );
            if( mc )  SendMessage( GetDlgItem( hwndDlg, IDC_FIND_CASE ), BM_SETCHECK, TRUE, 0 );
            if( fs )  SendMessage( GetDlgItem( hwndDlg, IDC_FIND_FUNC ), BM_SETCHECK, TRUE, 0 );
            if( wh == 3 )  wh = 0;
            //if( wh == 2 )  wh = 0;
            if( wht == 0 )  SendMessage( GetDlgItem( hwndDlg, IDC_FIND_FORWARD ), BM_SETCHECK, TRUE, 0 );
            else if( wht == 1 )  SendMessage( GetDlgItem( hwndDlg, IDC_FIND_ALL ), BM_SETCHECK, TRUE, 0 );
            else if( wht == 2 )  SendMessage( GetDlgItem( hwndDlg, IDC_FIND_BACKWARD ), BM_SETCHECK, TRUE, 0 );
            if( af )  SendMessage( GetDlgItem( hwndDlg, IDC_FIND_ALLFILES ), BM_SETCHECK, TRUE, 0 );
            //ww = mc = fs = 0;
            //af = 0;
            vf = rf = 0;

            //EnableWindow( GetDlgItem( hwndDlg, IDC_FIND_BACKWARD ), FALSE );
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    GetWindowText( GetDlgItem( hwndDlg, IDC_FIND_FIND ), findstr, CE_LEN_FINDSTR );
                    if( ! *findstr )  {
                        MessageBeep( MB_OK );
                        return( 0 );
                    }

                    if( SendMessage( GetDlgItem( hwndDlg, IDC_FIND_WORD), BM_GETCHECK, 0, 0 ))
                            ww = -1;
                    else    ww = 0;
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_FIND_CASE), BM_GETCHECK, 0, 0 ))
                            mc = -1;
                    else    mc = 0;
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_FIND_FUNC), BM_GETCHECK, 0, 0 ))
                            fs = -1;
                    else    fs = 0;
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_FIND_BACKWARD), BM_GETCHECK, 0, 0 ))
                            wh = wht = 2;
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_FIND_ALL), BM_GETCHECK, 0, 0 ))
                            wh = wht = 1;
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_FIND_FORWARD), BM_GETCHECK, 0, 0 ))
                            wh = wht = 0;
                    if( SendMessage( GetDlgItem( hwndDlg, IDC_FIND_ALLFILES), BM_GETCHECK, 0, 0 ))
                            af = -1;
                    else    af = 0;

                    EndDialog( hwndDlg, TRUE );
                    hwndDlgActive = NULL;
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



BOOL CALLBACK ReplaceDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    int n;
    static int wht = 0;

    switch (message) {
        case WM_INITDIALOG:
            for( n = fshcnt; n > 0; n-- )  {
                SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_FIND ), CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( fshlst[ n - 1] ));
            }
            SetWindowText( GetDlgItem( hwndDlg, IDC_REPLACE_FIND ), findstr );

            for( n = cshcnt; n > 0; n-- )  {
                SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_REPLACE ), CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( cshlst[ n - 1] ));
            }
            SetWindowText( GetDlgItem( hwndDlg, IDC_REPLACE_REPLACE ), chngstr );

            if( ww )  SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_WORD ), BM_SETCHECK, TRUE, 0 );
            if( mc )  SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_CASE ), BM_SETCHECK, TRUE, 0 );
            if( fs )  SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_FUNC ), BM_SETCHECK, TRUE, 0 );

            EnableWindow( GetDlgItem( hwndDlg, IDC_REPLACE_MARK ), FALSE );
            if( blck &&( blckstr.y != blckend.y )&&( blckstr.x == blckend.x ))  {
                //EnableWindow( GetDlgItem( hwndDlg, IDC_REPLACE_FORWARD ), FALSE );
                EnableWindow( GetDlgItem( hwndDlg, IDC_REPLACE_MARK ), TRUE );
                //EnableWindow( GetDlgItem( hwndDlg, IDC_REPLACE_ALL ), FALSE );
                //EnableWindow( GetDlgItem( hwndDlg, IDC_REPLACE_ALLFILES ), FALSE );
                //SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_MARK ), BM_SETCHECK, TRUE, 0 );
                //SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_FORWARD ), BM_SETCHECK, FALSE, 0 );
            }
            else  {
                if( wh == 3 )  wh = 0;
                if( wht == 3 )  wht = 0;
            }

            if( wh == 2 )  wh = 0;
            if( wht == 0 )  SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_FORWARD ), BM_SETCHECK, TRUE, 0 );
            else if( wht == 1 )  SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_ALL ), BM_SETCHECK, TRUE, 0 );
            else if( wht == 3 )  SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_MARK ), BM_SETCHECK, TRUE, 0 );
            if( af )  SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_ALLFILES ), BM_SETCHECK, TRUE, 0 );
            //ww = mc = fs = 0;
            //af = 0;
            vf = rf = 0;
 
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                    case DID_OK:
                        GetWindowText( GetDlgItem( hwndDlg, IDC_REPLACE_FIND ), findstr, CE_LEN_FINDSTR );
                        GetWindowText( GetDlgItem( hwndDlg, IDC_REPLACE_REPLACE ), chngstr, CE_LEN_FINDSTR );
                        if( ! *findstr )  {
                            MessageBeep( MB_OK );
                            return( 0 );
                        }

                        if( SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_WORD), BM_GETCHECK, 0, 0 ))
                                ww = -1;
                        else    ww = 0;
                        if( SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_CASE), BM_GETCHECK, 0, 0 ))
                                mc = -1;
                        else    mc = 0;
                        if( SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_FUNC), BM_GETCHECK, 0, 0 ))
                                fs = -1;
                        else    fs = 0;
                        if( SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_REPLACEALL), BM_GETCHECK, 0, 0 ))
                                vf = -1;
                        else    vf = 0;
                        if( SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_MARK), BM_GETCHECK, 0, 0 ));
// !!!!!!!!!!  Not part of if ?????????????????
                                wh = wht = 3;

                        if( SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_ALL), BM_GETCHECK, 0, 0 ))
                                wh = wht = 1;
                        if( SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_FORWARD), BM_GETCHECK, 0, 0 ))
                                wh = wht = 0;
                        if( SendMessage( GetDlgItem( hwndDlg, IDC_REPLACE_ALLFILES), BM_GETCHECK, 0, 0 ))
                                af = -1;
                        else    af = 0;

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



BOOL CALLBACK FillDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {

    switch (message) {
        case WM_INITDIALOG:
            if( ! blck ||( blckstr.x == blckend.x ))  {
                EnableWindow( GetDlgItem( hwndDlg, DID_OK ), FALSE );
            }
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    GetWindowText( GetDlgItem( hwndDlg, IDC_FILL_CHAR ), fillchar, CE_LEN_FILLSTR );
                    if( fillchar[ 0 ] )  {
                        EndDialog( hwndDlg, TRUE );
                        return( TRUE );
                    }
                    else    {
                        MessageBeep( -1 );
                    }
                    break;

                case DID_CANCEL:
                    EndDialog( hwndDlg, FALSE );
                    return( TRUE );
            }
            break;

        default:
            break;
    }
    return( FALSE );
}



BOOL CALLBACK GotoDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    int n;
    char ln[20];
    char fn[ CE_LEN_FILEPATH ];


    switch (message) {
        case WM_INITDIALOG:
            //EnableWindow( GetDlgItem( hwndDlg, IDC_GOTO_FILE ), FALSE );
            n = 1;
            while( 1 )  {
                _itoa( n, fn, 10 );
                strcat( fn, "      ");
                if( ! edit_get_fname( n, &fn[ 6 ] ))  break;
                SendMessage( GetDlgItem( hwndDlg, IDC_GOTO_FILE ), CB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( fn ));
                n += 1;
            }
            return( TRUE );

        case WM_COMMAND:
            switch( wParam )  {
                case DID_OK:
                    GetWindowText( GetDlgItem( hwndDlg, IDC_GOTO_LINE ), ln, 20 );
                    if(( gotofilenr = SendMessage( GetDlgItem( hwndDlg, IDC_GOTO_FILE ), CB_GETCURSEL, 0, ( LPARAM )( 0 ))) == CB_ERR )
                        gotofilenr = 0;
                    else
                        gotofilenr += 1;
                    gotolinenr = atoi( ln );
                    if(( ! gotofilenr )&&( ! gotolinenr ))  return( TRUE );
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




BOOL CALLBACK GotoFileDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    int n, fnr;
    char fn[ CE_LEN_FILEPATH ];
#ifdef _WIN32
    long alTabStops[ 1 ];
#else
    int alTabStops[ 1 ];
#endif


    switch (message) {
            case WM_INITDIALOG:
                alTabStops[ 0 ] = 24;
                SendMessage( GetDlgItem( hwndDlg, IDC_GOTOFILE_LIST ), LB_SETTABSTOPS, 1, ( LPARAM )alTabStops );
                SendMessage( GetDlgItem( hwndDlg, IDC_GOTOFILE_LIST ), LB_SETHORIZONTALEXTENT, 2000, 0 );
                for( n = 1; fnr = fmru_getfno( n ); ++n )  {
                    _itoa( fnr, fn, 10 );
                    strcat( fn, "\t");
                    if( ! edit_get_fname( fnr, &fn[ strlen( fn ) ] ))  break;
                    SendMessage( GetDlgItem( hwndDlg, IDC_GOTOFILE_LIST ), LB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( fn ));
                }
                return( TRUE );

            case WM_COMMAND:
                switch( LOWORD( wParam ))  {
                    case IDC_GOTOFILE_LIST:
#ifdef _WIN32
                        if( HIWORD( wParam ) != LBN_DBLCLK )  break;
#else
                        if( HIWORD( lParam ) != LBN_DBLCLK )  break;
#endif
                    case DID_OK:
                        if(( fnr = SendMessage( GetDlgItem( hwndDlg, IDC_GOTOFILE_LIST ), LB_GETCURSEL, 0, 0 )) == LB_ERR )  {
                            gotofilenr = 0;
                            return( TRUE );
                        }
                        gotofilenr = fmru_getfno( fnr + 1 );
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



BOOL CALLBACK BookMarksDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam )  {
    char szBookMark[ 240 ];        // nr + linenr + filename
    char szLineNr[ 20 ];
    char szFileNr[ 20 ];
    char szDlgFileName[ CE_LEN_FILEPATH ];
    int c, sSel, sSelFirst, iFileNr, iLineNr;
#ifdef _WIN32
    long alTabStops[ 2 ];
#else
    int alTabStops[ 2 ];
#endif

    switch (message) {
        case WM_INITDIALOG:
            alTabStops[ 0 ] = 40;
            alTabStops[ 1 ] = 80;
            //alTabStops[ 2 ] = 72;

            SendMessage( GetDlgItem( hwndDlg, IDC_BOOKMARKS_LIST ), LB_SETTABSTOPS, 2, ( LPARAM )alTabStops );
            SendMessage( GetDlgItem( hwndDlg, IDC_BOOKMARKS_LIST ), LB_SETHORIZONTALEXTENT, 2000, 0 );
            sSelFirst = -1;
            for( c = 0; c < CE_MAX_BOOKMARKS; ++c )  {
                //_itoa( c + 1, szBookMark, 10 );
                szBookMark[ 0 ] = '\0';
                bmrk_get( c, &iLineNr, &iFileNr );
                if( iFileNr )  {
                    //strcat( szBookMark, "\t" );
                    _itoa( iLineNr + 1, szLineNr, 10 );
                    strcat( szBookMark, szLineNr );
                    strcat( szBookMark, "\t" );
                    _itoa( iFileNr, szFileNr, 10 );
                    strcat( szBookMark, szFileNr );
                    strcat( szBookMark, "\t" );
                    edit_get_fname( iFileNr, szDlgFileName );
                    strcat( szBookMark, szDlgFileName );
                    SendMessage( GetDlgItem( hwndDlg, IDC_BOOKMARKS_LIST ), LB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( szBookMark ));
                }
                else  {
                    if( sSelFirst == -1 )  sSelFirst = c ;
                }
                //SendMessage( GetDlgItem( hwndDlg, IDC_BOOKMARKS_LIST ), LB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( szBookMark ));
            }
            SendMessage( GetDlgItem( hwndDlg, IDC_BOOKMARKS_LIST ), LB_ADDSTRING, 0, ( LPARAM )( LPCSTR )( "" ));
            if( sSelFirst == -1 )  sSelFirst = 0;
            SendMessage( GetDlgItem( hwndDlg, IDC_BOOKMARKS_LIST ), LB_SETCURSEL, ( WPARAM )sSelFirst, 0 );
            return( TRUE );

        case WM_COMMAND:
            switch( LOWORD( wParam ))  {
                case IDC_BOOKMARKS_LIST:
#ifdef _WIN32
                    if( HIWORD( wParam ) != LBN_DBLCLK )  break;
#else
                    if( HIWORD( lParam ) != LBN_DBLCLK )  break;
#endif
                case IDC_BOOKMARKS_GOTO:
                    if(( sSel = SendMessage( GetDlgItem( hwndDlg, IDC_BOOKMARKS_LIST ), LB_GETCURSEL, 0, 0 )) == LB_ERR )  return( TRUE );
                    if( ! bmrk_goto( sSel ))  return( TRUE );
                    EndDialog( hwndDlg, TRUE );
                    return( TRUE );

                case IDC_BOOKMARKS_SET:
                    if(( sSel = SendMessage( GetDlgItem( hwndDlg, IDC_BOOKMARKS_LIST ), LB_GETCURSEL, 0, 0 )) == LB_ERR )  return( TRUE );
                    bmrk_set( sSel );
                    EndDialog( hwndDlg, TRUE );
                    return( TRUE );

                case IDC_BOOKMARKS_CLEAR:
                    if(( sSel = SendMessage( GetDlgItem( hwndDlg, IDC_BOOKMARKS_LIST ), LB_GETCURSEL, 0, 0 )) == LB_ERR )  return( TRUE );
                    bmrk_clear( sSel );
                    EndDialog( hwndDlg, TRUE );
                    return( TRUE );

                case IDC_BOOKMARKS_CLEARALL:
                    bmrk_clearall( );
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
