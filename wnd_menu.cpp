

#include "windows.h"

#include "string.h"

#include "resource.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"


extern HINSTANCE hInst;
extern HWND hwndClient, hwndVScroll, hwndMain, hwndStatus, hwndHScroll, hwndTree;
extern HDC hDC;
char  szMainTitle[ ];

extern struct editrec *ed;
extern int blck;
extern struct coords blckstr, blckend;

extern int fMatchBracket;
extern int iClpbSel;

extern int fCaret;
extern int fCtrl;
extern int fShift;
extern int fAlt;
extern int fMru;
extern int fClipbrd;
extern int fFileTree;

extern int ckh, frkh;

extern int edcurno;
extern int edcount;
extern char findstr[CE_LEN_FINDSTR];
extern int bmrkcur;
extern struct clpbrec clpbord[ ];


WMInitMenu( HWND hwnd, WPARAM wParam )  {
    HMENU hMenu;

    hMenu = GetMenu( hwnd );

    EnableMenuItem(( HMENU )wParam, IDM_FILE_NEW, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_CLOSE, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_CLOSEALL, ((edcount > 1) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_SAVE, (ed->changef ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_SAVEAS, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_SAVEALL, ((edcount > 1) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_MERGE, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_RELOAD, ((ed->fname[0] ) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    //EnableMenuItem(( HMENU )wParam, IDM_FILE_TYPE, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    //EnableMenuItem(( HMENU )wParam, IDM_FILE_PAGESETUP, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_PRINT, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    //EnableMenuItem(( HMENU )wParam, IDM_FILE_PROPERTIES, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

    EnableMenuItem(( HMENU )wParam, IDM_FILE_GOTO, (((edcount > 1)|| ed->fname[0] || ed->changef ) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

    EnableMenuItem(( HMENU )wParam, IDM_FILE_RECENT, ((edcount > 1) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_PREV, (( edcurno != 1 ) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_FILE_NEXT, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

    //CheckMenuItem(( HMENU )wParam, IDM_FILE_SETINGS_CRLF, ( ed->ostype == 0x03 ? MF_UNCHECKED : MF_CHECKED )| MF_BYCOMMAND );

    //EnableMenuItem(( HMENU )wParam, IDM_SETINGS_ASSOCIATIONS, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    //EnableMenuItem(( HMENU )wParam, IDM_FILE_TYPE, ((ed->fname[0] || ed->changef) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

    EnableMenuItem(( HMENU )wParam, IDM_SEARCH_NEXT, ( *findstr ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_SEARCH_PREV, ( *findstr ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

    EnableMenuItem(( HMENU )wParam, IDM_SEARCH_LASTEDIT, ( ed->undocnt ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

    EnableMenuItem(( HMENU )wParam, IDM_SEARCH_NEXTBOOKMARK, ( bmrkcur != -1 ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_SEARCH_PREVBOOKMARK, ( bmrkcur != -1 ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    EnableMenuItem(( HMENU )wParam, IDM_SEARCH_CLEARBOOKMARKS, ( bmrkcur != -1 ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

    CheckMenuItem( hMenu, IDM_VIEW_FILELIST, ( fFileTree ? MF_CHECKED : MF_UNCHECKED )| MF_BYCOMMAND );
    if( GetFocus() == hwndTree )  {
        EnableMenuItem(( HMENU )wParam, IDM_FILE_CLOSE, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_FILE_SAVE, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_FILE_SAVEAS, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_FILE_MERGE, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_FILE_RELOAD, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_FILE_PRINT, MF_GRAYED | MF_BYCOMMAND );

        EnableMenuItem(( HMENU )wParam, IDM_EDIT_UNDO, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_REDO, MF_GRAYED | MF_BYCOMMAND );

        EnableMenuItem(( HMENU )wParam, IDM_EDIT_CUT, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_COPY, MF_GRAYED | MF_BYCOMMAND );

        EnableMenuItem(( HMENU )wParam, IDM_EDIT_PASTE, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_PASTEBLOCK, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_DELETE, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_SELALL, MF_GRAYED | MF_BYCOMMAND );

        /*EnableMenuItem(( HMENU )wParam, IDM_EDIT_FILL, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_UPCASE, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_LOCASE, MF_GRAYED | MF_BYCOMMAND );*/

        EnableMenuItem( GetSubMenu(( HMENU )wParam, 1 ), 10, MF_GRAYED | MF_BYPOSITION );

        CheckMenuItem(( HMENU )wParam, IDM_EDIT_RECMACRO, ( frkh ? MF_CHECKED : MF_UNCHECKED )| MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_PLAYMACRO, (( frkh ? 0 : ckh ) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );


        EnableMenuItem(( HMENU )wParam, IDM_VIEW_PROPERTIES, MF_GRAYED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_VIEW_MEMBERS, MF_GRAYED | MF_BYCOMMAND );
    }
    else  {
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_UNDO, ( ed->undocnt ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_REDO, ( ed->redocnt ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

        EnableMenuItem(( HMENU )wParam, IDM_EDIT_CUT, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_COPY, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

        EnableMenuItem(( HMENU )wParam, IDM_EDIT_PASTE, (( clpbord[ 0 ].type || fClipbrd ) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_PASTEBLOCK, (( clpbord[ 0 ].type || fClipbrd ) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_DELETE, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_SELALL, MF_ENABLED | MF_BYCOMMAND );
        
        EnableMenuItem( GetSubMenu(( HMENU )wParam, 1 ), 10, MF_ENABLED | MF_BYPOSITION );

        EnableMenuItem(( HMENU )wParam, IDM_EDIT_FILL, (( blck &&( !( blckstr.x == blckend.x ))) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_UPCASE, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_LOCASE, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_INVERTCASE, ( blck ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

        EnableMenuItem(( HMENU )wParam, IDM_EDIT_TAB, (( blck &&( ( blckstr.x == blckend.x ))) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_UNTAB, (( blck &&( ( blckstr.x == blckend.x ))) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_SPACE, (( blck &&( ( blckstr.y != blckend.y ))) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_UNSPACE, (( blck &&( ( blckstr.y != blckend.y ))) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

        CheckMenuItem(( HMENU )wParam, IDM_EDIT_RECMACRO, ( frkh ? MF_CHECKED : MF_UNCHECKED )| MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_EDIT_PLAYMACRO, (( frkh ? 0 : ckh ) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );

        EnableMenuItem(( HMENU )wParam, IDM_VIEW_PROPERTIES, MF_ENABLED | MF_BYCOMMAND );
        EnableMenuItem(( HMENU )wParam, IDM_VIEW_MEMBERS, (( fFileTree && *ed->fname && (( ed->assoc == CE_ASC_C )||( ed->assoc == CE_ASC_JAVA )||( ed->assoc == CE_ASC_ORASQL )||( ed->assoc == CE_ASC_MSSQL )||( ed->assoc == CE_ASC_JVS )||( ed->assoc == CE_ASC_VBS ))) ? MF_ENABLED : MF_GRAYED ) | MF_BYCOMMAND );
    }
}



WMCommand( HWND hwnd, WPARAM wParam )  {
    int dispy;
    int fsbu = FALSE;
    int rc;
    HTREEITEM hTreeItem;

    dispy = ed->displcrds.y;

    HideCaret( NULL );
    fCaret = 0;
    if( fMatchBracket )  HideMatchBracket();

    switch (wParam) {
        case IDM_EDIT_RECMACRO:
            if( frkh )  {
                frkh = 0;
                //ckh -= 1;
            }
            else  {
                ckh = 0;
                frkh = -1;
            }
            break;

        case IDM_EDIT_PLAYMACRO:
            PlayMacro( hwnd );
            break;

        case IDM_HELP_PRODINFO:
            dlg_prodinfo();
            break;

        case IDM_HELP_KEYWORD:
            //DialogBox( hInst, "HELPFILE", hwndClient, KeywordHelpDlg );
            KeywordHelp();
            blck_unmark( );
            scrn_cursor();
            break;

        case IDM_HELP_KEYS:
            DialogBox( ( HINSTANCE )hInst, "HELPKEYS", hwndClient, HelpKeysDlg );
            blck_unmark( );
            scrn_cursor();
            break;

        case IDM_FILE_NEW:
            blck_unmark();
            cmnd_file_last();
            break;

        case IDM_FILE_OPEN:
            blck_unmark();
            if( file_open(""))
                TreeItemSel( ed->hTreeItem, 1 );
            scrn_statsl();
            scrn_rescrn();
            fsbu = TRUE;
            break;

        case IDM_FILE_MERGE:
            blck_unmark();
            file_merge();
            scrn_statsl();
            scrn_adjust();
            fsbu = TRUE;
            break;

        case IDM_FILE_CLOSE:  {
            //TVITEM tvi;
            //HTREEITEM hti;

            /*if( GetFocus() == hwndTree )  {
                hTreeItem = TreeView_GetSelection( hwndTree );
                tvi.hItem = hTreeItem;
                tvi.mask = TVIF_PARAM;
                TreeView_GetItem( hwndTree, &tvi );
                if( tvi.lParam )
                    cmnd_file_select( tvi.lParam );
                }*/

            blck_unmark();
            hTreeItem = ed->hTreeItem;
            rc = file_close();
            //fmru_goto();
            scrn_statsl();
            scrn_rescrn();
            fsbu = TRUE;
            if( rc )  {
                TreeFileDel( hTreeItem );
                TreeItemSel( ed->hTreeItem, 0 );
                }
            }
            break;

        case IDM_FILE_SAVE:
            blck_unmark();
            file_save();
            scrn_statsl();
            scrn_rescrn();
            if ( TreeItemIsExpanded( ed->hTreeItem ))
                tree_file( ed, -1 );
            break;

        case IDM_FILE_SAVEAS:
            file_saveas();
            blck_unmark();
            scrn_statsl();
            scrn_rescrn();
            if ( TreeItemIsExpanded( ed->hTreeItem ))
                tree_file( ed, -1 );
            break;

        case IDM_FILE_SAVEALL:
            blck_unmark();
            file_saveall();
            scrn_statsl();
            scrn_rescrn();
            break;

        case IDM_FILE_RECENT:
            blck_unmark();
            fmru_goto();
            fmru_setcur();
            fsbu = TRUE;
            TreeItemSel( ed->hTreeItem, 0 );
            break;

        case IDM_FILE_PREV:
            blck_unmark();
            cmnd_file_prev();
            fmru_setcur();
            fsbu = TRUE;
            TreeItemSel( ed->hTreeItem, 0 );
            break;

        case IDM_FILE_NEXT:
            blck_unmark();
            cmnd_file_next();
            fmru_setcur();
            fsbu = TRUE;
            TreeItemSel( ed->hTreeItem, 0 );
            break;

        case IDM_FILE_EXIT:
            SendMessage( hwnd, WM_CLOSE, ( WPARAM )NULL, ( LPARAM )NULL );
            break;

        case IDM_FILE_CLOSEALL:
            rc = file_closeall();
            scrn_statsl();
            scrn_rescrn();
            if( rc )  TreeItemsDel( );
            break;

        case IDM_FILE_RELOAD:  {
                int x, y;
    
                blck_unmark();
                x = ed->curcrds.x;
                y = ed->curcrds.y;
                if( file_reload( ))  {
                    ed->displline = ed->firstline;
                    ed->displcrds.x = 0;
                    ed->displcrds.y = 0;
                    cmnd_goto_line( y+1, x );
                }
                else  {
                    scrn_rescrn();
                    scrn_statsl();
                }
            break;
            }

        case IDM_FILE_PAGESETUP:
            dlg_pagesetup();
            break;

        case IDM_FILE_PRINT:
            FilePrint( hwndMain );
            blck_unmark();
            scrn_rescrn();
            break;

        case IDM_FILE_PRINTSETUP:
            PrintSetup( hwndMain );
            blck_unmark();
            scrn_rescrn();
            break;

        case IDM_VIEW_PROPERTIES:
            dlg_properties( &ed->ostype );
            scrn_statsl();
            break;

        case IDM_VIEW_FILELIST:
            fFileTree = !fFileTree;
            ShowFileTree( fFileTree );
            break;
 
        case IDM_VIEW_MEMBERS:
            if( ! TreeItemExpand( ed->hTreeItem ))  {
                tree_file( ed, -1 );
            }
            break;

        case IDM_FILE_GOTO:
            dlg_gotofile();
            break;

        case IDM_SEARCH_FIND:
            find_dlg();
            break;

        case IDM_SEARCH_REPLACE:
            find_replace_dlg();
            break;

        case IDM_SEARCH_NEXT:
            find_repeat();
            break;

        case IDM_SEARCH_PREV:
            find_prev();
            break;

        case IDM_SEARCH_GOTOLINE:
            dlg_goto();
            break;

        case IDM_SEARCH_LASTEDIT:
            undo_goto();
            break;

        case IDM_SEARCH_BRACE:
            find_bracket();
            break;

        case IDM_SEARCH_SETBOOKMARK:
            bmrk_setnext();
            break;

        case IDM_SEARCH_BOOKMARKS:
            dlg_bookmarks();
            break;

        case IDM_SEARCH_NEXTBOOKMARK:
            if( ! bmrk_next())  edit_beep();
            break;

        case IDM_SEARCH_PREVBOOKMARK:
            if( ! bmrk_prev())  edit_beep();
            break;

        case IDM_SEARCH_CLEARBOOKMARKS:
            bmrk_clearall();
            break;

        case IDM_EDIT_CUT:
            if( ! blck )  edit_beep( );
            else  {
                blck_cut( 0 );
                fClipbrd = 0;
                OpenClipboard( hwnd );
                EmptyClipboard( );
                SetClipboardData( CF_TEXT, NULL );
                CloseClipboard( );
            }
            fsbu = TRUE;
            break;

        case IDM_EDIT_COPY:
            if( ! blck )  edit_beep( );
            else  {
                blck_copy( 0 );
                fClipbrd = 0;
                OpenClipboard( hwnd );
                EmptyClipboard( );
                SetClipboardData( CF_TEXT, NULL );
                CloseClipboard( );
            }
            break;

        case IDM_EDIT_PASTE:
            //blck_unmark();
            if( fClipbrd )  {
                fClipbrd = 0;
                ClipbrdPaste( hwnd );
            }
            if( ! blck_paste( 0 ))  edit_beep( );
            fsbu = TRUE;
            break;

        case IDM_EDIT_DELETE:
            if( ! blck )  edit_beep( );
            else  blck_del();
            fsbu = TRUE;
            break;

        case IDM_EDIT_SELALL:
            blck_all();
            break;

        case IDM_EDIT_PASTEBLOCK:
            //blck_unmark();
            if( fClipbrd )  {
                    fClipbrd = 0;
                    ClipbrdPaste( hwnd );
                    }
            if( ! blck_pasteblck( 0 ))  edit_beep( );
            break;

        case IDM_EDIT_FILL:
            dlg_blck_fill();
            break;

        case IDM_EDIT_UPCASE:
            blck_case(1);
            break;

        case IDM_EDIT_LOCASE:
            blck_case(2);
            break;

        case IDM_EDIT_INVERTCASE:
            blck_case(3);
            break;

        case IDM_EDIT_TAB:
            if( blck )  {  if( ! blck_tab())  edit_beep( );  }
            else    {  cmnd_tab();  }
            break;

        case IDM_EDIT_UNTAB:
            if( blck )  {  if( ! blck_revtab())  edit_beep( );  }
            else    {  edit_beep( );  }
            break;
 
        case IDM_EDIT_SPACE:
            if(( ! blck )||( blckstr.y == blckend.y ))  {  edit_beep( );  }
            else    {  if( ! blck_space())  edit_beep( );  }
            break;

        case IDM_EDIT_UNSPACE:
            if( blck )  {  if( ! blck_revspace())  edit_beep( );  }
            else    {  edit_beep( );  }
            break;

        case IDM_SETINGS_PRINTERFONT:
            PrinterFont( hwnd, hDC );
            scrn_cursor();
            break;

        /*case IDM_SETINGS_COLOR:
            if( dlg_color())  {
                SetTextColor( hDC, clrset[ CE_CLR_NORMAL ].fore );
                SetBkColor( hDC, clrset[ CE_CLR_NORMAL ].back );
                InvalidateRect( hwndClient, NULL, FALSE );
            }
            break;

        case IDM_SETINGS_TABSIZE:
            dlg_tabsize();
            break;

        case IDM_SETINGS_FILEOPTIONS:
            dlg_fileoptions();
            break;

        case IDM_SETINGS_EDITOPTIONS:
            dlg_editoptions();
            break;*/

        /*case IDM_SETINGS_ASSOCIATIONS:
            dlg_associations();
            break;*/

        case IDM_OPTIONS:
            dlg_options();
            break;
 
        case IDM_EDIT_UNDO:
            undo_undo( );
            break;

        case IDM_EDIT_REDO:
            undo_redo( );
            break;

        default:
            return( 0 );
    }
    if(( ! blck )&&( fMatchBracket ))  ShowMatchBracket();
    ShowCaret( NULL );
    fCaret = -1;
    if( dispy != ed->displcrds.y )  fsbu = TRUE;
    if( fsbu )  {
        SetScrollRange( hwndVScroll, SB_CTL, 0, ( int )( ed->nolines ), FALSE );
        SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );
    }
    SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );
}



#ifdef __cplusplus
}
#endif
