

#include "windows.h"
#include "winbase.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"


extern int tablen;
extern int filetab;
extern int fileos;
extern int crmacode;
extern int filemodchkf;
extern int restoredirf;
extern int fMatchBracket;

extern int fnhcnt;
extern char fnhlst[ CE_MAX_FILENHIST ][ CE_LEN_FILEPATH ];

extern int fshcnt;
extern char fshlst[ CE_MAX_FINDHIST ][ CE_LEN_FINDSTR ];

//extern int cshcnt;
//extern char cshlst[ CE_MAX_FINDHIST ][ CE_LEN_FINDSTR ];


extern struct colorset clrset[ ];       // defined in vdeo_win.c

extern char asstype[ CE_CNT_ASSTYPE ][ CE_LEN_ASSTYPE ];

char szIniFile[ ] = "cedw.ini";


IniGetOptions(  )  {
    char szIniSec[ ] = "Options";
    char szFileType[ 5 ];

    tablen = GetPrivateProfileInt( szIniSec, "TabSize", 8, szIniFile );
    if(( tablen > 20 )||( tablen < 0 ))  tablen = 8;
    filetab = GetPrivateProfileInt( szIniSec, "FileTabs", 8, szIniFile );
    if(( filetab > 20 )||( filetab < 0 ))  filetab = 8;

    GetPrivateProfileString( szIniSec, "FileType", "DOS", szFileType, 5, szIniFile );
    if( _stricmp( szFileType, "DOS" ) == 0 )  fileos = 0x03;
    else if( _stricmp( szFileType, "UNIX" ) == 0 )  fileos = 0x02;
    else fileos = 0x03;

    filemodchkf = GetPrivateProfileInt( szIniSec, "FileModCheck", 0, szIniFile );
    restoredirf = GetPrivateProfileInt( szIniSec, "RestoreDir", 0, szIniFile );
}


IniSetOptions( )  {
    char szIniSec[ ] = "Options";
    char szLong[ 20 ];

    _itoa( tablen, szLong, 10 );
    WritePrivateProfileString( szIniSec, "TabSize", szLong, szIniFile );
    _itoa( filetab, szLong, 10 );
    WritePrivateProfileString( szIniSec, "FileTabs", szLong, szIniFile );

    WritePrivateProfileString( szIniSec, "FileType", ( fileos == 0x03 ? "DOS" : "UNIX" ), szIniFile );

    _itoa( filemodchkf, szLong, 10 );
    WritePrivateProfileString( szIniSec, "FileModCheck", szLong, szIniFile );

    WritePrivateProfileString( szIniSec, "RestoreDir", ( restoredirf ? "1" : "0" ), szIniFile );
}


IniGetOptionsStartupDir( char *szDir )  {
    char szIniSec[ ] = "Options";

    GetPrivateProfileString( szIniSec, "StartupDir", "", szDir, 1000, szIniFile );
}

IniSetOptionsStartupDir( char *szDir )  {
    char szIniSec[ ] = "Options";

    WritePrivateProfileString( szIniSec, "StartupDir", szDir, szIniFile );
}


IniGetPrint( int *iLeftMargin, int *fSyntaxColor )  {
    char szIniSec[ ] = "Print";

    *iLeftMargin = GetPrivateProfileInt( szIniSec, "LeftMargin", 0, szIniFile );
    *fSyntaxColor = GetPrivateProfileInt( szIniSec, "SyntaxColor", 0, szIniFile );
}

IniSetPrint( int iLeftMargin, int fSyntaxColor )  {
    char szIniSec[ ] = "Print";
    char szLong[ 20 ];

    _itoa( iLeftMargin, szLong, 10 );
    WritePrivateProfileString( szIniSec, "LeftMargin", szLong, szIniFile );

    WritePrivateProfileString( szIniSec, "SyntaxColor", ( fSyntaxColor ? "1" : "0" ), szIniFile );
}



IniGetWinOptions( int *fMultiInstance, int *fTitleFileName, int *fExplorerFileDlg, int *fSaveWinPos, int *fFocusMousePos, int *fFileTree )  {
    char szIniSec[ ] = "Options";

    *fMultiInstance = GetPrivateProfileInt( szIniSec, "MultiInstance", 0, szIniFile );
    *fTitleFileName = GetPrivateProfileInt( szIniSec, "TitleFilename", 0, szIniFile );
    *fExplorerFileDlg  = GetPrivateProfileInt( szIniSec, "ExplorerDialog", 1, szIniFile );
    *fSaveWinPos  = GetPrivateProfileInt( szIniSec, "SaveWinPos", 0, szIniFile );
    *fFocusMousePos  = GetPrivateProfileInt( szIniSec, "FocusMousePos", 0, szIniFile );
    *fFileTree  = GetPrivateProfileInt( szIniSec, "FileTree", 1, szIniFile );
}


IniSetWinOptions( int fMultiInstance, int fTitleFileName, int fExplorerFileDlg, int fSaveWinPos, int fFocusMousePos, int fFileTree )  {
    char szIniSec[ ] = "Options";

    WritePrivateProfileString( szIniSec, "MultiInstance", ( fMultiInstance ? "1" : "0" ), szIniFile );
    WritePrivateProfileString( szIniSec, "TitleFileName", ( fTitleFileName ? "1" : "0" ), szIniFile );
    WritePrivateProfileString( szIniSec, "ExplorerDialog", ( fExplorerFileDlg ? "1" : "0" ), szIniFile );
    WritePrivateProfileString( szIniSec, "SaveWinPos", ( fSaveWinPos ? "1" : "0" ), szIniFile );
    WritePrivateProfileString( szIniSec, "FocusMousePos", ( fFocusMousePos ? "1" : "0" ), szIniFile );
    WritePrivateProfileString( szIniSec, "FileTree", ( fFileTree ? "1" : "0" ), szIniFile );
}


 
IniGetScrnFont( int *pfScrnFont, LOGFONT *plfScrnFont )  {
    char szIniSec[ ] = "Font";

    *pfScrnFont = FALSE;
    memset( plfScrnFont, 0, sizeof( LOGFONT ));
    plfScrnFont->lfWeight = FW_NORMAL;
    plfScrnFont->lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

    GetPrivateProfileString( szIniSec, "FontName", "", plfScrnFont->lfFaceName, 32, szIniFile );
    plfScrnFont->lfHeight = GetPrivateProfileInt( szIniSec, "FontSize", 0, szIniFile );
    if( plfScrnFont->lfFaceName && plfScrnFont->lfHeight )  *pfScrnFont = TRUE;
}


IniSetScrnFont( LOGFONT lfScrnFont )  {
    char szLong[ 20 ];
    char szIniSec[ ] = "Font";
 
    WritePrivateProfileString( szIniSec, "FontName", lfScrnFont.lfFaceName, szIniFile );
    _itoa( lfScrnFont.lfHeight, szLong, 10 );
    WritePrivateProfileString( szIniSec, "FontSize", szLong, szIniFile );
}


IniGetColor( )  {
    char szIniSec[ ] = "Color";
    char szLong[ 20 ];
    char szBack[ 20 ];

    GetPrivateProfileString( szIniSec, "NormalFore", "0", szLong, 19, szIniFile );
    clrset[ CE_CLR_NORMAL ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "NormalBack", "16777215", szLong, 19, szIniFile );
    clrset[ CE_CLR_NORMAL ].back = atol( szLong );
    GetPrivateProfileString( szIniSec, "SelectFore", "0", szLong, 19, szIniFile );
    clrset[ CE_CLR_SELECT ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "SelectBack", "12632256", szLong, 19, szIniFile );
    clrset[ CE_CLR_SELECT ].back = atol( szLong );

    /*GetPrivateProfileString( szIniSec, "StatusBarFore", "0", szLong, 11, szIniFile );
    clrset[ CE_CLR_STATUS ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "StatusBarBack", "12632256", szLong, 11, szIniFile );
    clrset[ CE_CLR_STATUS ].back = atol( szLong );
    GetPrivateProfileString( szIniSec, "StatusIndFore", "16777215", szLong, 11, szIniFile );
    clrset[ CE_CLR_STATIN ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "StatusIndBack", "12632256", szLong, 11, szIniFile );
    clrset[ CE_CLR_STATIN ].back = atol( szLong );*/

    _ltoa( clrset[ CE_CLR_NORMAL ].back, szBack, 10 );

    GetPrivateProfileString( szIniSec, "KeywordFore", "16711680", szLong, 19, szIniFile );
    clrset[ CE_CLR_KEYWORD ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "KeywordBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_KEYWORD ].back = atol( szLong );

    GetPrivateProfileString( szIniSec, "NumberFore", "128", szLong, 19, szIniFile );
    clrset[ CE_CLR_NUMBER ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "NumberBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_NUMBER ].back = atol( szLong );
 
    GetPrivateProfileString( szIniSec, "StringFore", "32768", szLong, 19, szIniFile );
    clrset[ CE_CLR_STRING ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "StringBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_STRING ].back = atol( szLong );
 
    GetPrivateProfileString( szIniSec, "CommentFore", "12632256", szLong, 19, szIniFile );
    clrset[ CE_CLR_COMMENT ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "CommentBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_COMMENT ].back = atol( szLong );


    GetPrivateProfileString( szIniSec, "ASPScriptDelFore", "255", szLong, 19, szIniFile );
    clrset[ CE_CLR_ASP_SCRIPTDEL ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "ASPScriptDelBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_ASP_SCRIPTDEL ].back = atol( szLong );

    GetPrivateProfileString( szIniSec, "HTMLTagNameFore", "128", szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_TAGNAME ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "HTMLTagNameBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_TAGNAME ].back = atol( szLong );

    GetPrivateProfileString( szIniSec, "HTMLTagDelimitFore", "16711680", szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_TAGDEL ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "HTMLTagDelimitBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_TAGDEL ].back = atol( szLong );

    GetPrivateProfileString( szIniSec, "HTMLTagTextFore", "8388608", szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_TAGTEXT ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "HTMLTagTextBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_TAGTEXT ].back = atol( szLong );

    GetPrivateProfileString( szIniSec, "HTMLTagOperFore", "16711680", szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_TAGOPER ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "HTMLTagOperBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_TAGOPER ].back = atol( szLong );

    /*GetPrivateProfileString( szIniSec, "HTMLCommentFore", "12632256", szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_COMMENT ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "HTMLCommentBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_HTML_COMMENT ].back = atol( szLong );*/


    GetPrivateProfileString( szIniSec, "XMLTagNameFore", "128", szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_TAGNAME ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "XMLTagNameBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_TAGNAME ].back = atol( szLong );

    GetPrivateProfileString( szIniSec, "XMLTagDelimitFore", "16711680", szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_TAGDEL ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "XMLTagDelimitBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_TAGDEL ].back = atol( szLong );

    GetPrivateProfileString( szIniSec, "XMLTagTextFore", "8388608", szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_TAGTEXT ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "XMLTagTextBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_TAGTEXT ].back = atol( szLong );

    GetPrivateProfileString( szIniSec, "XMLTagOperFore", "16711680", szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_TAGOPER ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "XMLTagOperBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_TAGOPER ].back = atol( szLong );

    /*GetPrivateProfileString( szIniSec, "XMLCommentFore", "12632256", szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_COMMENT ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "XMLCommentBack", szBack, szLong, 19, szIniFile );
    clrset[ CE_CLR_XML_COMMENT ].back = atol( szLong );*/

    GetPrivateProfileString( szIniSec, "MatchBracketFore", "0", szLong, 19, szIniFile );
    clrset[ CE_CLR_MATCHBRACKET ].fore = atol( szLong );
    GetPrivateProfileString( szIniSec, "MatchBracketBack", "65535", szLong, 19, szIniFile );
    clrset[ CE_CLR_MATCHBRACKET ].back = atol( szLong );

    crmacode  = GetPrivateProfileInt( szIniSec, "SyntaxColor", 1, szIniFile );
    fMatchBracket = GetPrivateProfileInt( szIniSec, "MatchBracket", 1, szIniFile );
}


IniSetColor( )  {
    char szIniSec[ ] = "Color";
    char szLong[ 20 ];


    _ltoa( clrset[ CE_CLR_NORMAL ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "NormalFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_NORMAL ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "NormalBack", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_SELECT ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "SelectFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_SELECT ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "SelectBack", szLong, szIniFile );

    /*_ltoa( clrset[ CE_CLR_STATUS ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "StatusBarFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_STATUS ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "StatusBarBack", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_STATIN ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "StatusIndFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_STATIN ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "StatusIndBack", szLong, szIniFile );*/

    _ltoa( clrset[ CE_CLR_KEYWORD ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "KeywordFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_KEYWORD ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "KeywordBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_NUMBER ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "NumberFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_NUMBER ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "NumberBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_STRING ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "StringFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_STRING ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "StringBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_COMMENT ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "CommentFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_COMMENT ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "CommentBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_ASP_SCRIPTDEL ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "ASPScriptDelFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_ASP_SCRIPTDEL ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "ASPScriptDelBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_HTML_TAGNAME ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLTagNameFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_HTML_TAGNAME ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLTagNameBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_HTML_TAGDEL ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLTagDelimitFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_HTML_TAGDEL ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLTagDelimitBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_HTML_TAGTEXT ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLTagTextFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_HTML_TAGTEXT ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLTagTextBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_HTML_TAGOPER ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLTagOperFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_HTML_TAGOPER ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLTagOperBack", szLong, szIniFile );

    /*_ltoa( clrset[ CE_CLR_HTML_COMMENT ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLCommentFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_HTML_COMMENT ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "HTMLCommentBack", szLong, szIniFile );*/


    _ltoa( clrset[ CE_CLR_XML_TAGNAME ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLTagNameFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_XML_TAGNAME ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLTagNameBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_XML_TAGDEL ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLTagDelimitFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_XML_TAGDEL ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLTagDelimitBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_XML_TAGTEXT ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLTagTextFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_XML_TAGTEXT ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLTagTextBack", szLong, szIniFile );

    _ltoa( clrset[ CE_CLR_XML_TAGOPER ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLTagOperFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_XML_TAGOPER ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLTagOperBack", szLong, szIniFile );

    /*_ltoa( clrset[ CE_CLR_XML_COMMENT ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLCommentFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_XML_COMMENT ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "XMLCommentBack", szLong, szIniFile );*/

    _ltoa( clrset[ CE_CLR_MATCHBRACKET ].fore, szLong, 10 );
    WritePrivateProfileString( szIniSec, "MatchBracketFore", szLong, szIniFile );
    _ltoa( clrset[ CE_CLR_MATCHBRACKET ].back, szLong, 10 );
    WritePrivateProfileString( szIniSec, "MatchBracketBack", szLong, szIniFile );

    WritePrivateProfileString( szIniSec, "SyntaxColor", ( crmacode ? "1" : "0" ), szIniFile );
    WritePrivateProfileString( szIniSec, "MatchBracket", ( fMatchBracket ? "1" : "0" ), szIniFile );
}



IniGetWinPos( WINDOWPLACEMENT *wp, int *iFileTree )  {
    char szIniSec[ ] = "Window";


    wp->rcNormalPosition.left = GetPrivateProfileInt( szIniSec, "Left", 0, szIniFile );
    wp->rcNormalPosition.right = GetPrivateProfileInt( szIniSec, "Right", 0, szIniFile );
    wp->rcNormalPosition.top = GetPrivateProfileInt( szIniSec, "Top", 0, szIniFile );
    wp->rcNormalPosition.bottom = GetPrivateProfileInt( szIniSec, "Bottom", 0, szIniFile );
    wp->flags = GetPrivateProfileInt( szIniSec, "ShowFlags", 0, szIniFile );
    wp->showCmd = GetPrivateProfileInt( szIniSec, "ShowCmd", 0, szIniFile );
    wp->ptMinPosition.x = GetPrivateProfileInt( szIniSec, "MinimizedLeft", 0, szIniFile );
    wp->ptMinPosition.y = GetPrivateProfileInt( szIniSec, "MinimizedTop", 0, szIniFile );
    wp->ptMaxPosition.x = GetPrivateProfileInt( szIniSec, "MaximizedLeft", 0, szIniFile );
    wp->ptMaxPosition.y = GetPrivateProfileInt( szIniSec, "MaximizedTop", 0, szIniFile );

    if( !( wp->rcNormalPosition.left || wp->rcNormalPosition.right ))
        return( FALSE );
    if( !( wp->rcNormalPosition.top || wp->rcNormalPosition.bottom ))
        return( FALSE );

    *iFileTree = GetPrivateProfileInt( szIniSec, "FileTreeWidth", 0, szIniFile );
    return( TRUE );
}


IniSetWinPos( WINDOWPLACEMENT wp, int iFileTree )  {
    char szIniSec[ ] = "Window";
    char szNo[ 20 ];


    _ltoa( wp.rcNormalPosition.left, szNo, 10 );
    WritePrivateProfileString( szIniSec, "Left", szNo, szIniFile );

    _ltoa( wp.rcNormalPosition.right, szNo, 10 );
    WritePrivateProfileString( szIniSec, "Right", szNo, szIniFile );

    _ltoa( wp.rcNormalPosition.top, szNo, 10 );
    WritePrivateProfileString( szIniSec, "Top", szNo, szIniFile );

    _ltoa( wp.rcNormalPosition.bottom, szNo, 10 );
    WritePrivateProfileString( szIniSec, "Bottom", szNo, szIniFile );

    _itoa( wp.flags, szNo, 10 );
    WritePrivateProfileString( szIniSec, "ShowFlags", szNo, szIniFile );

    _itoa( wp.showCmd, szNo, 10 );
    WritePrivateProfileString( szIniSec, "ShowCmd", szNo, szIniFile );

    _ltoa( wp.ptMinPosition.x, szNo, 10 );
    WritePrivateProfileString( szIniSec, "MinimizedLeft", szNo, szIniFile );

    _ltoa( wp.ptMinPosition.y, szNo, 10 );
    WritePrivateProfileString( szIniSec, "MinimizedTop", szNo, szIniFile );

    _ltoa( wp.ptMaxPosition.x, szNo, 10 );
    WritePrivateProfileString( szIniSec, "MaximizedLeft", szNo, szIniFile );

    _ltoa( wp.ptMaxPosition.y, szNo, 10 );
    WritePrivateProfileString( szIniSec, "MaximizedTop", szNo, szIniFile );

    _itoa( iFileTree, szNo, 10 );
    WritePrivateProfileString( szIniSec, "FileTreeWidth", szNo, szIniFile );

    return( TRUE );
}


IniSetFiles( )  {
    char szIniSec[ ] = "Files";
    char szFileNr[ 3 ];
    int cFileNr;

    for( cFileNr = 0; cFileNr < fnhcnt; ++cFileNr )  {
        _ltoa( cFileNr, szFileNr, 10 );
        WritePrivateProfileString( szIniSec, szFileNr, fnhlst[ cFileNr ], szIniFile );
    }
}


IniGetFiles( )  {
    char szIniSec[ ] = "Files";
    char szFileNr[ 3 ];
    int cFileNr;


    for( cFileNr = 0; cFileNr < CE_MAX_FILENHIST; ++cFileNr )  {
        _ltoa( cFileNr, szFileNr, 10 );

        GetPrivateProfileString( szIniSec, szFileNr, "", fnhlst[ cFileNr ], CE_LEN_FILEPATH, szIniFile );
        if( fnhlst[ cFileNr ][ 0 ] == '\0' )
            break;

    }
    fnhcnt = cFileNr;
}


IniSetFind( )  {
    char szIniSec[ ] = "Find";
    char szFindNr[ 3 ];
    int cFindNr;


    for( cFindNr = 0; cFindNr < fshcnt; ++cFindNr )  {
        _ltoa( cFindNr, szFindNr, 10 );
        WritePrivateProfileString( szIniSec, szFindNr, fshlst[ cFindNr ], szIniFile );
    }
}

IniGetFind( )  {
    char szIniSec[ ] = "Find";
    char szFindNr[ 3 ];
    int cFindNr;


    for( cFindNr = 0; cFindNr < CE_MAX_FINDHIST; ++cFindNr )  {
        _ltoa( cFindNr, szFindNr, 10 );

        GetPrivateProfileString( szIniSec, szFindNr, "", fshlst[ cFindNr ], CE_LEN_FINDSTR, szIniFile );
        if( fshlst[ cFindNr ][ 0 ] == '\0' )
            break;

    }
    fshcnt = cFindNr;
}



IniGetPrntFont( LOGFONT *plfPrntFont, INT *piPointSize )  {
    char szIniSec[ ] = "PrinterFont";


    GetPrivateProfileString( szIniSec, "FontName", "", plfPrntFont->lfFaceName, 32, szIniFile );

    *piPointSize = GetPrivateProfileInt( szIniSec, "FontSize", 0, szIniFile );

    plfPrntFont->lfHeight = GetPrivateProfileInt( szIniSec, "FontHeight", 0, szIniFile );
    plfPrntFont->lfItalic = GetPrivateProfileInt( szIniSec, "Italic", 0, szIniFile );
    plfPrntFont->lfWeight = GetPrivateProfileInt( szIniSec, "Weight", 0, szIniFile );

    if( *plfPrntFont->lfFaceName && plfPrntFont->lfHeight && *piPointSize )
        return( TRUE );

    return( FALSE );
}



IniSetPrntFont( LOGFONT lfPrntFont, INT iPointSize )  {
    char szLong[ 20 ];
    char szIniSec[ ] = "PrinterFont";
 
    WritePrivateProfileString( szIniSec, "FontName", lfPrntFont.lfFaceName, szIniFile );
    _itoa( lfPrntFont.lfHeight, szLong, 10 );
    WritePrivateProfileString( szIniSec, "FontHeight", szLong, szIniFile );
    _itoa( iPointSize, szLong, 10 );
    WritePrivateProfileString( szIniSec, "FontSize", szLong, szIniFile );
    _itoa( lfPrntFont.lfItalic, szLong, 10 );
    WritePrivateProfileString( szIniSec, "Italic", szLong, szIniFile );
    _itoa( lfPrntFont.lfWeight, szLong, 10 );
    WritePrivateProfileString( szIniSec, "Weight", szLong, szIniFile );
}



IniSetAssosiations( int asscnt, struct assrec asslst[] )  {
    char szIniSec[ ] = "Associations";
    char szNr[ 3 ];
    char szLng[ CE_LEN_ASSTYPE ];
    char szLst[ 200 ];
    int n, q;
    char *s;

    _itoa( asslst[ 0 ].lng, szNr, 10 );
    WritePrivateProfileString( szIniSec, "NoName", szNr, szIniFile );
    _itoa( asslst[ 1 ].lng, szNr, 10 );
    WritePrivateProfileString( szIniSec, "NoExtn", szNr, szIniFile );

    for( n = 0; n < CE_CNT_ASSTYPE; ++n )  {
        szLst[ 0 ] = '\0';
        for( q = 2; q < asscnt; ++q )  {
            if( asslst[ q ].lng == n )  {
                if(( strlen( szLst ) + strlen( asslst[ q ].fnx ) + 1 ) >= 200 )
                        break;
                if(( n == 0 )&&( q >= CE_CNT_ASSFNEXT ))        // None only for internal defined types
                        break;
                if( *szLst )
                        strcat( szLst, "," );
                strcat( szLst, asslst[ q ].fnx );
            }
        }
        strcpy( szLng, asstype[ n ] );
        while(( s = strchr( szLng, ' ' )) != NULL )
            *s = '_';

        WritePrivateProfileString( szIniSec, szLng, szLst, szIniFile );
    }
}



IniGetAssosiations( int *asscnt, struct assrec asslst[] )  {
    char szIniSec[ ] = "Associations";
    char szLng[ CE_LEN_ASSTYPE ];
    char szLst[ 200 ];
    int n, q;
    char *s, *t;

 
    asslst[ 0 ].lng = GetPrivateProfileInt( szIniSec, "NoName", 0, szIniFile );
    asslst[ 1 ].lng = GetPrivateProfileInt( szIniSec, "NoExtn", 0, szIniFile );

    for( n = 0; n < CE_CNT_ASSTYPE; ++n )  {
        szLst[ 0 ] = '\0';
        strcpy( szLng, asstype[ n ] );
        while(( s = strchr( szLng, ' ' )) != NULL )
            *s = '_';
 
        GetPrivateProfileString( szIniSec, szLng, "", szLst, 200, szIniFile );

        s = szLst;
        while( *s )  {
            if(( t = strchr( s, ',' ))  == NULL )
                t = strchr( s, '\0' );
            for( q = 2; q < *asscnt; ++q )  {
                if( _strnicmp( asslst[ q ].fnx, s, t - s ) == 0 )  {
                    asslst[ q ].lng = n;
                    break;
                }
            }
            if( q == *asscnt )  {
                if( *asscnt < CE_MAX_ASSOCIATIONS )  {
                    asslst[ q ].lng = n;
                    strncpy( asslst[ q ].fnx, s, t - s );
                    *asscnt += 1;
                }
            }
            if( ! *t )
                break;
            s = t + 1;
        }
    }
}




//IniGetKeyword_C( )  {

/*init_getkeyword_c( int *ckw, struct keywordrec kwl[ ] )  {

    char szIniSec[ ] = "Keyword_C";
    char szNr[ 3 ];
    int c;

    for( c = 0; c < CE_MAX_KEYWORD; ++c )  {
        _ltoa( c, szNr, 10 );
 
        GetPrivateProfileString( szIniSec, szNr, "", kwl[ c ].word, CE_LEN_KEYWORD, szIniFile );
        if( *kwl[ c ].word == '\0' )
            break;
    }
    *ckw = c;
}*/




#ifdef __cplusplus
}
#endif
