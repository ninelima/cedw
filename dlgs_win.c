

#include "windows.h"

#include "string.h"
#include "stdlib.h"

#include "commdlg.h"

#include "ced.h"
#include "cedw.h"
#include "resource.h"



extern HINSTANCE hInst;
extern HWND hwndMain, hwndClient;
extern HWND hwndStatus;
extern HFONT hFont, hFontBold;

extern struct editrec *ed;
extern int tablen;

extern int cxChar, cyChar;

extern char szIniFile[ ];

extern int crmacode;
extern int ostype;

extern int fExplorerFileDlg;

extern char findstr[CE_LEN_FINDSTR];
extern char chngstr[CE_LEN_FINDSTR];


char szDlgMessage[ 1000 ];
char szDlgTitle[ 80 ];
char szDlgFileName[ CE_LEN_FILEPATH ];

int gotolinenr = 0;
int gotofilenr = 0;


char filename[ CE_LEN_MULTIFILE ];          // ??? allow for multi select files from file browse dialogue

int fnhcnt = 0;
char fnhlst[ CE_MAX_FILENHIST ][ CE_LEN_FILEPATH ];

char dlgtitle[100];
 
int fBrowse = 0;
int fMerge = 0;
int fMultiFile = 0;

int ostype;

char fillchar[ CE_LEN_FILLSTR + 1 ];


dlg_yn( char *szMsg, char *szTle )  {
        int yn;

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        strcat( szDlgMessage, " ?" );
        yn =  DialogBox( hInst, "YESNODLG", hwndClient, YesNoDlg );
        if( yn == IDC_YESNO_YES )  return( 1 );
        if( yn == IDC_YESNO_NO )  return( 0 );
        return( 0 );
        }




dlg_ync( char *szMsg, char *szTle )  {
        int yn;

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        strcat( szDlgMessage, " ?" );
        yn =  DialogBox( hInst, "YESNOCANDLG", hwndClient, ( DLGPROC )MakeProcInstance( YesNoCanDlg, hInst ));
        if( yn == IDC_YESNO_YES )  return( 1 );
        if( yn == IDC_YESNO_NO )  return( -1 );
        return( 0 );
        }




dlg_ynfile( char *szMsg, char *szTle, char *szFName )  {
        int yn;
        static f = 0;

        if( f )  return( 0 );       // don't show the dialog more than once on regaining focus 
        f = -1;
        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        strcat( szDlgMessage, " ?" );
        strcpy( szDlgFileName, "[" );
        strcat( szDlgFileName, szFName );
        strcat( szDlgFileName, "]" );
        yn =  DialogBox( hInst, "YESNOFILEDLG", hwndClient, ( DLGPROC )MakeProcInstance( YesNoCanFileDlg, hInst ));
        f = 0;
        if( yn == IDC_YESNO_YES )  return( 1 );
        return( 0 );
        }


dlg_yncfile( char *szMsg, char *szTle, char *szFName )  {
        int yn;

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        strcat( szDlgMessage, " ?" );
        strcpy( szDlgFileName, "[" );
        strcat( szDlgFileName, szFName );
        strcat( szDlgFileName, "]" );
        yn =  DialogBox( hInst, "YESNOCANFILEDLG", hwndClient, ( DLGPROC )MakeProcInstance( YesNoCanFileDlg, hInst ));
        if( yn == IDC_YESNO_YES )  return( 1 );
        if( yn == IDC_YESNO_NO )  return( -1 );
        return( 0 );
        }



dlg_yna( char *szMsg, char *szTle )  {
        int yn;

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        strcat( szDlgMessage, " ?" );
        yn =  DialogBox( hInst, "YESNOALLDLG", hwndClient, YesNoAllDlg );
        if( yn == IDC_YESNOALL_YES )  return( 1 );
        if( yn == IDC_YESNOALL_NO )  return( -1 );
        if( yn == IDC_YESNOALL_ALL )  return( 2 );
        return( 0 );
        }

dlg_ynafile( char *szMsg, char *szTle )  {
        int yn;

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        strcat( szDlgMessage, " ?" );
        yn =  DialogBox( hInst, "YESNOALLFILEDLG", hwndClient, YesNoAllDlg );
        if( yn == IDC_YESNOALL_YES )  return( 1 );
        if( yn == IDC_YESNOALL_NO )  return( -1 );
        if( yn == IDC_YESNOALL_ALL )  return( 2 );
        return( 0 );
        }



dlg_oc( char *szMsg, char *szTle )  {
        int oc;

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        strcat( szDlgMessage, " ?" );
        oc =  DialogBox( hInst, "OKCANDLG", hwndClient, OkCanDlg );
        if( oc == DID_OK )  return( 1 );
        return( 0 );
        }



dlg_ocfile( char *szMsg, char *szTle, char *szFName )  {
        int oc;

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgFileName, "[" );
        strcat( szDlgFileName, szFName );
        strcat( szDlgFileName, "]" );
        strcpy( szDlgMessage, szMsg );
        strcat( szDlgMessage, " ?" );
        oc =  DialogBox( hInst, "OKCANFILEDLG", hwndClient, OkCanFileDlg );
        if( oc == DID_OK )  return( 1 );
        return( 0 );
        }



dlg_oca( char *szMsg, char *szTle )  {
        int oca;

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        strcat( szDlgMessage, " ?" );
        oca =  DialogBox( hInst, "OKCANALLDLG", hwndClient, OkCanAllDlg );
        if( oca == DID_OK )  return( 1 );
        if( oca == IDC_OKCANALL_ALL )  return( 2 );
        return( 0 );
        }


dlg_okstop( char *szMsg, char *szTle )  {

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        DialogBox( hInst, "OKSTOPDLG", hwndClient, OkDlg );
        return( 0 );
        }


dlg_ok( char *szMsg, char *szTle )  {

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        DialogBox( hInst, "OKDLG", hwndClient, OkDlg );
        return( 0 );
        }


dlg_okinfo( char *szMsg, char *szTle )  {

        strcpy( szDlgTitle, szTle );
        strcpy( szDlgMessage, szMsg );
        DialogBox( hInst, "OKINFODLG", hwndClient, OkDlg );
        return( 0 );
        }


#ifdef _DEBUG
dlg_debug( char *sfile, int sline, char *desc )  {
        char szLine[ 20 ];

        strcpy( szDlgTitle, "Debug" );

        strcpy( szDlgMessage, sfile );
        strcat( szDlgMessage, "(" );
        _itoa( sline, szLine, 10 );
        strcat( szDlgMessage, szLine );
        strcat( szDlgMessage, ") : " );
        strcat( szDlgMessage, desc );

        DialogBox( hInst, "OKSTOPDLG", hwndClient, OkDlg );
        return( 0 );
        }
#endif




dlg_err( char *errstr )  {

        strcpy( szDlgMessage, errstr );
        strcat( szDlgMessage, " !" );
        DialogBox( hInst, "ERRDLG", hwndClient, ErrDlg );
        return( 0 );
        }


dlg_errfile( char *errstr, char *fname )  {
        LPVOID lpMsgBuf;

        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );
        strcpy( szDlgMessage, lpMsgBuf );
        LocalFree( lpMsgBuf );

        strcpy( szDlgFileName, "[" );
        strcat( szDlgFileName, fname );
        strcat( szDlgFileName, "]" );

        strcat( szDlgMessage, " !" );
        DialogBox( hInst, "ERRFILEDLG", hwndClient, ErrFileDlg );
        return( 0 );
        }


dlg_prodinfo( )  {

        DialogBox( hInst, "PRODINFO", hwndClient, ProdInfoDlg );
        return( 0 );
        }


dlg_bookmarks()  {
        if( DialogBox( hInst, "BOOKMARKS", hwndClient, BookMarksDlg ))  {
                }
        scrn_cursor( );
        }



int CALLBACK PropSheetProc( HWND hwndDlg, UINT uMsg, LPARAM lParam )  {

          
        if( uMsg == PSCB_INITIALIZED  )  {
                LONG lWindowStyle;

                lWindowStyle = GetWindowLong( hwndDlg, GWL_EXSTYLE );
                lWindowStyle &= ~( WS_EX_CONTEXTHELP );
                SetWindowLong( hwndDlg, GWL_EXSTYLE, lWindowStyle );
                }
        return 0;
}


dlg_options()  {

    PROPSHEETPAGE psp[4];
    PROPSHEETHEADER psh;

    psp[0].dwSize = sizeof(PROPSHEETPAGE);
    psp[0].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[0].hInstance = hInst;
    psp[0].pszTemplate = "FILEOPTIONS";
    psp[0].pszIcon = NULL;
    psp[0].pfnDlgProc = FileOptionsDlg;
    psp[0].pszTitle = "     File     ";
    psp[0].lParam = 0;
    psp[0].pfnCallback = NULL;

    psp[1].dwSize = sizeof(PROPSHEETPAGE);
    psp[1].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[1].hInstance = hInst;
    psp[1].pszTemplate = "EDITOPTIONS";
    psp[1].pszIcon = NULL;
    psp[1].pfnDlgProc = EditOptionsDlg;
    psp[1].pszTitle = " Windows ";
    psp[1].lParam = 0;
    psp[1].pfnCallback = NULL;

    psp[2].dwSize = sizeof(PROPSHEETPAGE);
    psp[2].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[2].hInstance = hInst;
    psp[2].pszTemplate = "FORMATOPTIONSDLG";
    psp[2].pszIcon = NULL;
    psp[2].pfnDlgProc = FormatOptionsDlg;
    psp[2].pszTitle = "  Format   ";
    psp[2].lParam = 0;
    psp[2].pfnCallback = NULL;

    psp[3].dwSize = sizeof(PROPSHEETPAGE);
    psp[3].dwFlags = PSP_USEICONID | PSP_USETITLE;
    psp[3].hInstance = hInst;
    psp[3].pszTemplate = "HELPFILESDLG";
    psp[3].pszIcon = NULL;
    psp[3].pfnDlgProc = HelpFilesDlg;
    psp[3].pszTitle = " Help Files ";
    psp[3].lParam = 0;
    psp[3].pfnCallback = NULL;

    psh.dwSize = sizeof(PROPSHEETHEADER);
    psh.dwFlags = PSH_USEICONID | PSH_PROPSHEETPAGE | PSH_NOAPPLYNOW | PSH_USECALLBACK;
    psh.hwndParent = hwndMain;
    psh.hInstance = hInst;
    psh.pszIcon = NULL;
    psh.pszCaption = (LPSTR) "Options";
    psh.nPages = 4; //sizeof(psp) / sizeof(PROPSHEETPAGE);
    psh.nStartPage = 0;
    psh.ppsp = (LPCPROPSHEETPAGE) &psp;
    psh.pfnCallback = (PFNPROPSHEETCALLBACK)PropSheetProc;

    PropertySheet(&psh);

    scrn_cursor();
    return;
}



dlg_properties( int *ft )  {
        int as;

        as = ed->assoc;
        ostype = *ft;
        if( DialogBox( hInst, "FILEPROPERTIES", hwndClient, MakeProcInstance( FilePropertiesDlg, hInst )) )  {
                if( as != ed->assoc ) {
                        crma_lines( ed->firstline );
                        scrn_rescrn();
                        if( *ed->fname )  {
                                TreeFileDel( ed->hTreeItem );
                                TreeFileIns( file_getfname( ed->fname ));
                                TreeItemSel( ed->hTreeItem, -1 );
                                }
                        }
                if( *ft != ostype )  {
                        *ft = ostype;
                        cmnd_change();
                        }
                scrn_cursor();
                return( -1 );
                }
        scrn_cursor();
        return( 0 );
        }


dlg_goto()  {
        if( DialogBox( hInst, "GOTODLG", hwndClient, GotoDlg ))  {
                if( gotofilenr )  {
                        if( cmnd_goto_file( gotofilenr, gotolinenr, 0 ))
                                TreeItemSel( ed->hTreeItem, 0 );
                        }
                else  cmnd_goto_line( gotolinenr, 0 );
                }
        scrn_cursor( );
        }


dlg_gotofile()  {
        if( DialogBox( hInst, "GOTOFILEDLG", hwndClient, GotoFileDlg ))  {
                if( gotofilenr )  {
                        cmnd_goto_file( gotofilenr, 0, 0 );
                        }
                }
        scrn_cursor( );
        }



dlg_pagesetup( )  {
 
        /*if( DialogBox( hInst, "PAGESETUP", hwndClient, PrintSetupDlg ) )  {
                scrn_cursor();
                return(-1);
                }*/
        PrinterFont( hwndMain, GetDC( hwndMain ));
        scrn_cursor();
        return( 0 );
        }


dlg_find( char *s )  {

        if( DialogBox( hInst, "FINDDLG", hwndClient, FindDlg ))  {
                ffhistory( findstr );
                //scrn_cursor( );
                return( -1 );
                }
        scrn_cursor( );
        return( 0 );
        }



dlg_find_replace()  {

        if( DialogBox( hInst, "REPLACEDLG", hwndClient, ReplaceDlg ))  {
                ffhistory( findstr );
                frhistory( chngstr );
                return(-1);
                }
        scrn_cursor( );
        return( 0 );
        }



dlg_blck_fill()  {
        if( DialogBox( hInst, "FILLDLG", hwndClient, FillDlg ))  {
                if( fillchar[ 0 ] )  {
                        blck_fill( fillchar );
                        }
                }
        scrn_cursor( );
        }


dlg_filesave( char *fname, int *ostype )  {

        if( ! dlg_fn( "File Save", fname, ostype ))  {
                fname[ 0 ] = '\0';
                return( 0 );
                }
        return(-1);
        }


dlg_filemerge( char *fname, int *ostype )  {

        fMerge = -1;
        if( ! dlg_fn( "File Insert", fname, ostype ))  {
                fMerge = 0;
                fname[ 0 ] = '\0';
                return( 0 );
                }
        fMerge = 0;
        return(-1);
        }


dlg_filesaveas( char *title, char *fname, int *ostype )  {

        if( ! dlg_fn( title, fname, ostype ))  {
                fname[ 0 ] = '\0';
                return( 0 );
                }
        return(-1);
        }



dlg_fileopen( char *fname, int *ostype, int *fncnt, char *fpath, char fnlist[CE_MAX_FILES+1][CE_LEN_FILENAME] )  {
        char *s, *t;

        fMultiFile = -1;
        if( ! dlg_fn( "File Open", fname, ostype ))  {
                fMultiFile = 0;
                return( 0 );
                }
        fMultiFile = 0;
        *fncnt = 0;

#ifdef _WIN32
        if( fExplorerFileDlg )  {
                if( filename[ strlen( filename ) + 1 ] != '\0' )  {
                        s = strchr( filename, '\0' );
                        s++;
                        strcpy( fpath, filename );
                        if( *(s-2) != '\\' )  {
                                strcat( fpath, "\\" );
                                }
                        while( 1 )  {
                                t = strchr( s, '\0' );
                                if( *++t == '\0' )  break;
                                strcpy( fnlist[ (*fncnt)++ ], s );
                                s = t;
                                }
                        strcpy( fnlist[ (*fncnt)++ ], s );
                        }
                }
        else    {
                if(( s = strchr( filename, ' ' )) != NULL )  {
                        *s++ = '\0';
                        strcpy( fpath, filename );
                        if( *(s-2) != '\\' )  {
                                strcat( fpath, "\\" );
                                }
                        while(( t = strchr( s, ' ' )) != NULL )  {
                                *t++ = '\0';
                                strcpy( fnlist[ (*fncnt)++ ], s );
                                s = t;
                                }
                        strcpy( fnlist[ (*fncnt)++ ], s );
                        }
                }
#else
        if(( s = strchr( filename, ' ' )) != NULL )  {
                *s++ = '\0';
                strcpy( fpath, filename );
                if( *(s-2) != '\\' )  {
                        strcat( fpath, "\\" );
                        }
                while(( t = strchr( s, ' ' )) != NULL )  {
                        *t++ = '\0';
                        strcpy( fnlist[ (*fncnt)++ ], s );
                        s = t;
                        }
                strcpy( fnlist[ (*fncnt)++ ], s );
                }
#endif
        return( -1 );
        }




dlg_fn( char *title, char *fname, int *ft )  {

        ostype = *ft;
        fBrowse = -1;
        strcpy(filename, fname);
        strcpy(dlgtitle, title);
        if (filename[0] == '\0')  {
                file_getcwd(filename);
                }
        strcpy( fnhlst[ fnhcnt ], filename );
        if( ! DialogBox( hInst, "FILEDLG", hwndClient, FileDlg ))  {
                scrn_cursor( );
                return(0);
                }
        *ft = ostype;
        filename[ strlen( filename ) + 1 ] = '\0';
        if(( fBrowse )||( filename[ strlen( filename )-( *filename ? 1 : 0 ) ] == '\\' ))  {
                if( ! FileBrowse( hwndMain, filename, dlgtitle, "All Files (*.*)\0*.*\0", -1 ))  {
                        scrn_cursor( );
                        return(0);
                        }
                }
        fname[ 0 ] = '\0';
#ifdef _WIN32
        if( fExplorerFileDlg )  {
                /*if( filename[ strlen( filename )+ 1 ] == '\0' )  {
                        strcpy( fname, filename );
                        fnhistory( fname );
                        }*/
                strcpy( fname, filename );
                fnhistory( fname );
                }
        else    {
                if( strchr( filename, ' ' ) == NULL )  {
                        strcpy( fname, filename );
                        fnhistory( fname );
                        }
                }
#else
        if( strchr( filename, ' ' ) == NULL )  {
                strcpy( fname, filename );
                fnhistory( fname );
                }
#endif
        return(-1);
        }



fnhistory( char *filename )  {
        int n;

        if( filename[ 0 ] == '\0' )  return(0);
        if( fnhcnt )  {
                for( n = 0; n < fnhcnt; ++n )  {
                        if( strcmp( filename, fnhlst[ n ]) == 0 )  {
                                for( ; n < fnhcnt; ++n )  {
                                        strcpy( fnhlst[ n ], fnhlst[ n + 1 ]);
                                        fnhlst[ n + 1 ][ 0 ] = '\0';
                                        }
                                fnhcnt -= 1;
                                break;
                                }
                        }

                strcpy( fnhlst[ fnhcnt ], filename );
                if( fnhcnt < CE_MAX_FILENHIST - 1 )  {
                        fnhcnt += 1;
                        }
                else    {
                        for( n = 0; n < fnhcnt; ++n )  {
                                strcpy( fnhlst[ n ], fnhlst[ n + 1 ]);
                                fnhlst[ n + 1 ][ 0 ] = '\0';
                                }
                        }
                }
        else    {
                strcpy( fnhlst[ fnhcnt ], filename );
                fnhcnt += 1;
                }
        return(-1);
        }






 
 

