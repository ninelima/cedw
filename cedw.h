
#define __TITLE__       "C-edit Programmer's Editor"
#define __COPYRIGHT__   "Copyright © 1995 - 2002  Coert van der Walt\nEmail:  coertvdw@global.co.za"

#define DID_NULL                -1
#define DID_OK                  1
#define DID_CANCEL              2

#define IDC_VSCROLL             3

#define IDD_TEXT 111111
//#define IDD_FILE 111112
#define IDD_CANCEL 1111113


#define IDM_FILE                200
#define IDM_FILE_OPEN           201
#define IDM_FILE_MERGE          202
#define IDM_FILE_CLOSE          209
#define IDM_FILE_SAVE           208
#define IDM_FILE_SAVEAS         207
#define IDM_FILE_SAVEALL        205
#define IDM_FILE_EXIT           206
#define IDM_FILE_PREV           210
#define IDM_FILE_NEXT           211
#define IDM_FILE_NEW            212
#define IDM_FILE_TYPE           213
#define IDM_FILE_SETINGS_CRLF   214
#define IDM_FILE_SETINGS_RDONLY 215
#define IDM_FILE_CLOSEALL       216
#define IDM_FILE_RELOAD         217
#define IDM_FILE_PRINT          218
#define IDM_FILE_RECENT         219
#define IDM_FILE_PAGESETUP      220
#define IDM_FILE_PROPERTIES     221


#define IDM_EDIT                300
#define IDM_EDIT_CUT            301
#define IDM_EDIT_COPY           302
#define IDM_EDIT_PASTE          303
#define IDM_EDIT_PASTEBLOCK     309
#define IDM_EDIT_DELETE         304
#define IDM_EDIT_UPCASE         307
#define IDM_EDIT_LOCASE         308
#define IDM_EDIT_FILL           305
#define IDM_EDIT_UNDO           306
#define IDM_EDIT_SYSTEM         310
#define IDM_EDIT_SYSTEM_COPY    311
#define IDM_EDIT_SYSTEM_PASTE   312
#define IDM_EDIT_RECMACRO       313
#define IDM_EDIT_PLAYMACRO      314
#define IDM_EDIT_SELALL         315
#define IDM_EDIT_REDO           316

#define IDM_SEARCH              400
#define IDM_SEARCH_FIND         401
#define IDM_SEARCH_REPLACE      402
#define IDM_SEARCH_NEXT         403
#define IDM_SEARCH_GOTOLINE     404
#define IDM_SEARCH_BRACE        405
#define IDM_SEARCH_BOOKMARKS    406
#define IDM_SEARCH_PREV         407

#define IDM_SETINGS             500
#define IDM_SETINGS_FONT        501
#define IDM_SETINGS_FILEOPTIONS 502
#define IDM_SETINGS_COLOR       503
#define IDM_SETINGS_DEFDIR      504
#define IDM_SETINGS_SAVE        505
//#define IDM_SETINGS_FILEDLG     506
#define IDM_SETINGS_SAVEPOS     507
#define IDM_SETINGS_EDITOPTIONS 508
#define IDM_SETINGS_TABSIZE     509
#define IDM_SETINGS_PRINTERFONT 510
#define IDM_SETINGS_ASSOCIATIONS 511

#define IDM_HELP                600
#define IDM_HELP_PRODINFO       601
#define IDM_HELP_KEYWORD        602
#define IDM_HELP_KEYS           603

#define IDM_FILE_GOTO           604


#define IDS_HELPKEYS_FIRST      30000
//      .
//      .
//      .
#define IDS_HELPKEYS_LAST       30070


#define IDD_OK                  8000
#define IDC_OK_MSG              8001
#define IDC_OK_ICON             8002

#define IDD_OKCAN              13000
#define IDC_OKCAN_MSG          13001
#define IDC_OKCAN_ICON         13002
#define IDC_OKCAN_FILENAME     13002

#define IDD_PRINTCAN              22000
#define IDC_PRINTCAN_FILENAME     22001


#define IDD_ERR                10000
#define IDC_ERR_MSG            10001
#define IDC_ERR_ICON           10002
#define IDC_ERR_FILENAME       10003

#define IDD_PRODINFO           11000
#define IDC_PRODINFO_NAME      11001
#define IDC_PRODINFO_COPYRIGHT 11002
#define IDC_PRODINFO_AUTHOR    11003
#define IDC_PRODINFO_ICON      11004

#define IDD_YESNO               9000
#define IDC_YESNO_MSG           9001
#define IDC_YESNO_ICON          9002
#define IDC_YESNO_YES           9003
#define IDC_YESNO_NO            9004
#define IDC_YESNO_CANCEL        9005
#define IDC_YESNO_FILENAME      9006

#define IDD_YESNOALL               15000
#define IDC_YESNOALL_MSG           15001
#define IDC_YESNOALL_ICON          15002
#define IDC_YESNOALL_YES           15003
#define IDC_YESNOALL_NO            15004
#define IDC_YESNOALL_CANCEL        15005
#define IDC_YESNOALL_ALL           15006

#define IDD_FILE                1000
#define IDC_FILE_NAME           1001
#define IDC_FILE_LIST           1002
#define IDC_FILE_TYPE           1003

#define IDD_FILE2               7000
#define IDC_FILE2_NAME          7001
#define IDC_FILE2_APPLY         7002
#define IDC_FILE2_DIR           7003
#define IDC_FILE2_FILES         7004
#define IDC_FILE2_DIRS          7005
#define IDC_FILE2_DRIVES        7006
#define IDC_FILE2_ALL           7007

#define IDD_FIND                2000
#define IDC_FIND_FIND           2001
#define IDC_FIND_WORD           2002
#define IDC_FIND_CASE           2003
#define IDC_FIND_FUNC           2004
#define IDC_FIND_FORWARD        2005
#define IDC_FIND_BACKWARD       2006
#define IDC_FIND_ALL            2007
#define IDC_FIND_ALLFILES       2008


#define IDD_REPLACE             3000
#define IDC_REPLACE_FIND        3001
#define IDC_REPLACE_REPLACE     3002
#define IDC_REPLACE_WORD        3003
#define IDC_REPLACE_CASE        3004
#define IDC_REPLACE_FUNC        3005
#define IDC_REPLACE_REPLACEALL  3006
#define IDC_REPLACE_FORWARD     3007
#define IDC_REPLACE_BACKWARD    3008
#define IDC_REPLACE_ALL         3009
#define IDC_REPLACE_ALLFILES    3010
#define IDC_REPLACE_MARK        3011

#define IDD_GOTO                4000
#define IDC_GOTO_LINE           4001
#define IDC_GOTO_FILE           4002

#define IDD_GOTOFILE           23000
#define IDC_GOTOFILE_LIST      23001
#define IDC_GOTOFILE_ACTIVATE  23002
#define IDC_GOTOFILE_CLOSE     23003


#define IDD_FONT                5000
#define IDC_FONT_SIZE           5001

#define IDD_FILL                6000
#define IDC_FILL_CHAR           6001

#define IDD_COLOR               12000
#define IDC_COLOR_NORMALFORE     12001
#define IDC_COLOR_NORMALBACK    12002
#define IDC_COLOR_SELECTEDFORE  12003
#define IDC_COLOR_SELECTEDBACK  12004
#define IDC_COLOR_KEYWORD       12005
#define IDC_COLOR_NUMBER        12006
#define IDC_COLOR_STRING        12007
#define IDC_COLOR_COMMENT       12008
#define IDC_COLOR_SYNTAX        12009

#define IDD_FILEOPTIONS           14000
#define IDC_FILEOPTIONS_TABSIZE   14001
#define IDC_FILEOPTIONS_DOS       14002
#define IDC_FILEOPTIONS_UNIX      14003
#define IDC_FILEOPTIONS_FILETYPE  14004
#define IDC_FILEOPTIONS_CHECKMOD  14005
#define IDC_FILEOPTIONS_RESTOREDIR 14006
#define IDC_FILEOPTIONS_DIRECTORY 14007

#define IDD_EDITOPTIONS           20000
#define IDC_EDITOPTIONS_INSTANCE  20001
#define IDC_EDITOPTIONS_EXPLORER  20002
#define IDC_EDITOPTIONS_FNTITLE   20003
#define IDC_EDITOPTIONS_FNSTATUS  20004
#define IDC_EDITOPTIONS_POSITION  20005
#define IDC_EDITOPTIONS_MOUSEPOS  20006
#define IDC_EDITOPTIONS_FONT      20007


#define IDD_OKCANALL            16000
#define IDC_OKCANALL_MSG        16001
#define IDC_OKCANALL_ALL        16002

#define IDD_BOOKMARKS           17000
#define IDC_BOOKMARKS_LIST      17001
#define IDC_BOOKMARKS_SET       17002
#define IDC_BOOKMARKS_GOTO      17003
#define IDC_BOOKMARKS_CLEAR     17004
#define IDC_BOOKMARKS_CLEARALL  17005

#define IDC_HELPFILE            18000
#define IDC_HELPFILE_LIST       18001
#define IDC_HELPFILE_ADD        18002
#define IDC_HELPFILE_DELETE     18003
#define IDC_HELPFILE_KEYWORD    18003


#define IDC_HELPKEYS            21000
#define IDC_HELPKEYS_LIST       21001

#define IDD_FILETYPE        19000
#define IDC_FILETYPE_DOS    19002
#define IDC_FILETYPE_UNIX   19003

#define IDD_ASSOCIATIONS        24000
#define IDC_ASSOC_DEFAULT       24001
#define IDC_ASSOC_DEFSQL        24002
#define IDC_ASSOC_SAVE          24003
#define IDC_ASSOC_RDONLY        24004

#define IDC_PAGESETUP_LEFTMARGIN 25001
#define IDC_PAGESETUP_SYNTAXCOLOR 25002
#define IDC_PAGESETUP_FONT 25003

#define IDC_PROPS_FILETYPE      26001

#define SM_INIT         (WM_USER + 1)
#define SM_FILEOPEN     (WM_USER + 2)
#define SM_ACTIVATEAPP  (WM_USER + 3)


#define CE_SZE_VSCROLL  16
#define CE_SZE_HSCROLL  16
#define CE_SZE_STATUSBAR   19
#define CE_SZE_SIZEBAR 4


#define CE_TVI_FOLDER   1
#define CE_TVI_FILE     2
#define CE_TVI_PACKAGE  3
#define CE_TVI_CLASS    4
#define CE_TVI_FUNC     5



typedef struct tagTVITEMDATA{
    int iLevel;
    int iType;                      // 1 = Association Folder, 2 = File, 3 = Package, 4 = Class, 5 = Function
    int iLine;
    int iAssoc;
    int iDelete;                    //  indicate the entry must be deleted
    struct editrec *ed;
} TVITEMDATA, FAR *LPTVITEMDATA;


struct colorset {
    long fore;
    long back;
};


BOOL CALLBACK FileDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK YesNoDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL FAR PASCAL YesNoCanDlg( HWND hwndDlg, WORD message, WORD wParam, LONG lParam );
BOOL FAR PASCAL YesNoCanFileDlg( HWND hwndDlg, WORD message, WORD wParam, LONG lParam );
BOOL FAR PASCAL YesNoFileDlg( HWND hwndDlg, WORD message, WORD wParam, LONG lParam );
BOOL CALLBACK YesNoAllDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK OkDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK OkCanDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK OkCanFileDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK OkCanAllDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK ErrDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK ErrFileDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK FindDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK ReplaceDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK GotoDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK ProdInfoDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK FileOptionsDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK EditOptionsDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK BookMarksDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK HelpFilesDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK HelpKeysDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK FileTypeDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK FormatOptionsDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK GotoFileDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK FilePropertiesDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK FillDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK PrintSetupDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK KeywordHelpDlg( HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam );

FileBrowse( HWND hwnd, char *fn, char *ft, char *szFilter, int fChangeDir );

SetFont( HWND hwnd, HDC hDC );

PrinterFont( HWND hwnd, HDC hDC );

ShowFileTree( BOOL fShow );


IniSetOptions( );
IniGetOptions( );

IniSetColor( );
IniGetColor( );

IniSetScrnFont( LOGFONT lfScrnFont );
IniGetScrnFont( int *pfScrnFont, LOGFONT *plfScrnFont );

IniSetAssosiations( int asscnt, struct assrec asslst[] );
IniGetAssosiations( int *asscnt, struct assrec asslst[] );

IniGetOptionsStartupDir( char *szDir );
IniSetOptionsStartupDir( char *szDir );

IniSetWinOptions( int fMultiInstance, int fTitleFileName, int fExplorerFileDlg, int fSaveWinPos, int fFocusMousePos, int fFileTree );
IniGetWinOptions( int *fMultiInstance, int *fTitleFileName, int *fExplorerFileDlg, int *fSaveWinPos, int *fFocusMousePos, int *fFileTree );

IniGetPrint( int *iLeftMargin, int *fSyntaxColor );
IniSetPrint( int iLeftMargin, int fSyntaxColor );

IniSetPrntFont( LOGFONT lfPrntFont, INT iPointSize );
IniGetPrntFont( LOGFONT *plfPrntFont, INT *piPointSize );

IniGetWinPos( WINDOWPLACEMENT *wp, int *iFileTree );
IniSetWinPos( WINDOWPLACEMENT wp, int iFileTree );

IniGetPrntFont( LOGFONT *plfPrntFont, INT *piPointSize );

IniSetFiles();
IniGetFiles( );

IniGetFind();
IniSetFind();


HideMatchBracket();
ShowMatchBracket();

PlayMacro( HWND hwnd );

void KeywordHelp();


int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, int FontType, LPARAM lParam );

LONG FAR PASCAL TreeViewSubClassFunc( HWND hwnd, WORD Message, WPARAM wParam, LPARAM lParam );

FilePrint( HWND hwnd );

PrintSetup( HWND hwnd );

ClipbrdCopy( HWND hwnd );
ClipbrdPaste( HWND hwnd );


int PASCAL WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
long FAR PASCAL MainWndProc(HWND, unsigned, WPARAM, LPARAM);
long FAR PASCAL EditWndProc(HWND, unsigned, WPARAM, LPARAM);
long FAR PASCAL StatusWndProc(HWND, unsigned, WPARAM, LPARAM);
long FAR PASCAL ScrollFillWndProc(HWND, unsigned, WPARAM, LPARAM);
long FAR PASCAL SeperatorLineWndProc(HWND, unsigned, WPARAM, LPARAM);
long FAR PASCAL ColorSelectWndProc(HWND, unsigned, WPARAM, LPARAM);
long FAR PASCAL SizeBarWndProc(HWND, unsigned, WPARAM, LPARAM);


WMChar( HWND hwnd, char chr );
WMSysChar( HWND hwnd, char chr );
WMSysKeyDn( HWND hwnd, WORD vkey, LONG dkey );
WMSysKeyUp( HWND hwnd, WORD vkey, LONG dkey );
WMKeyUp( HWND hwnd, WORD vkey, LONG dkey );
WMKeyDn( HWND hwnd, WORD vkey, LONG dkey );

WMLButtonDown( HWND hwnd, WORD cxMou, WORD cyMou );
WMLButtonDblclk( HWND hwnd, WORD cxMou, WORD cyMou );
WMMouseMove( HWND hwnd, WPARAM fwKeys, WORD cxMou, WORD cyMou );

WMScroll( HWND hwnd, WPARAM wPrm, LPARAM lParam );
WMHScroll( HWND hwnd, WPARAM wPrm, LPARAM lParam );

WMCommand( HWND hwnd, WPARAM wParam );
WMInitMenu( HWND hwnd, WPARAM wParam );

CmdLine( char *achCmdLine, char *achArgv[ CE_MAX_FILES ] );


#ifdef __cplusplus
extern "C"{
#endif

typedef struct _TREEITEM * HTREEITEM;

TreeItemSel( HTREEITEM hItem, int fExpand );
void TreeFileDel( HTREEITEM hTreeItem );
TreeItemIsExpanded( HTREEITEM hTreeItem );
void TreeItemsDel( );
TreeItemExpand( HTREEITEM hTreeItem );
TreeItemDelChildren( HTREEITEM hTreeItem );
HTREEITEM TreeItemIns( LPCTSTR lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine );
HTREEITEM TreeItemUpd( LPCTSTR lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine );
BOOL InitTreeImageLists(HWND hwndTree);
void DestroyImageList();
TreeDelLines( HTREEITEM hTreeItem, int l, int c );
TreeInsLines( HTREEITEM hTreeItem, int l, int c );
TreeItemMark2Del( HTREEITEM hTreeItem );
TreeItemDelMarked( HTREEITEM hTreeItem );
HTREEITEM TreeGetParent( HTREEITEM hTreeItem );
void TreeFileIns( LPCTSTR lpszFileName );;

#ifdef __cplusplus
}
#endif
