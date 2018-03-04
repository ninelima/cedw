
#include "windows.h"

#include "string.h"

#include "resource.h"


#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"


extern struct editrec *ed;
extern int blck;


extern HINSTANCE hInst;
extern HWND hwndClient, hwndVScroll, hwndMain, hwndStatus, hwndHScroll, hwndTree;
extern HDC hDC;
char  szMainTitle[ ];


extern struct linerec *blckline;
extern struct coords blckstr, blckend;
extern int fFileTree;
extern int fClipbrd;
extern int scrn_x, scrn_y;

int fMatchBracket = -1;
int iClpbSel = 0;

int fCaret = 0;
int fCtrl = 0;
int fShift = 0;
int fAlt = 0;
int fMru = 0;


struct kh {
    int fCtrl;
    int fShift;
    int fChar;
    int vkey;
    LONG dkey;
};


int ckh = 0, frkh = 0, fpkh = 0, crkh;
struct kh akh[ 200 ];


int fbc = 0;
char b1, b2;
int b1x, b1y, b2x, b2y;



PlayMacro( HWND hwnd )  {
    int fC;
 
    if( frkh)  {
        return(0);
    }
    fC = fCtrl;
    fpkh = -1;
    for( crkh = 0; crkh < ckh; ++crkh )  {

        if(( akh[ crkh ].fCtrl )&&( akh[ crkh ].vkey == 'A' ))  continue;

        fCtrl = akh[ crkh ].fCtrl;
        fShift = akh[ crkh ].fShift;

        if( akh[ crkh ].fChar )  {
            WMChar( hwnd, akh[ crkh ].vkey );
        }
        else  {
            WMKeyDn( hwnd, akh[ crkh ].vkey, akh[ crkh ].dkey );
        }
    }
    fpkh = 0;
    fCtrl = fC;
    fShift = 0;
}



WMSysKeyDn( HWND hwnd, WORD vkey, LONG dkey )  {

    if( vkey == VK_MENU )  {
        fAlt = -1;
    }
    return(0);
}


WMSysKeyUp( HWND hwnd, WORD vkey, LONG dkey )  {

    if( vkey == VK_F3 )  {
        HideCaret( NULL );
        fCaret = 0;

        find_dlg();
        fAlt = 0;

        ShowCaret( NULL );
        fCaret = -1;

        return( -1 );
    }

    if( vkey == VK_MENU )  {
        fAlt = 0;
    }
    return( 0 );
}


WMSysChar( HWND hwnd, char chr )  {
    int fsbu = FALSE;

    HideCaret( NULL );
    fCaret = 0;

    switch( chr )  {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            blck_unmark();
            if( ! cmnd_goto_file( chr - '1' + 1, 0, 0 ))
                edit_beep( );
            else
                fsbu = TRUE;
            break;

        case VK_BACK:
            undo_undo( );
            break;

        case VK_RETURN:
            dlg_properties( &ed->ostype );
            scrn_statsl();
            break;

        default:
            ShowCaret( NULL );
            fCaret = -1;
            return( 0 );
    }
    ShowCaret( NULL );
    fCaret = -1;

    if( fsbu )  {
        SetScrollRange( hwndVScroll, SB_CTL, 0, ( int )( ed->nolines ), FALSE );
        SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );
    }
    SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );
    return( -1 );
}


WMKeyUp( HWND hwnd, WORD vkey, LONG dkey )  {

    switch( vkey )  {
        case VK_CONTROL:
            fCtrl = 0;
            iClpbSel = 0;
            if( fMru )
                fmru_setcur();
            break;

        case VK_SHIFT:
            fShift = 0;
            break;
    }
}



WMKeyDn( HWND hwnd, WORD vkey, LONG dkey )  {
    int dispy;
    int fsbu = FALSE;
    int rc;
    HTREEITEM hTreeItem;
    int fKeyProcessed = TRUE;


    if( frkh )  {
        if(( vkey != VK_CONTROL )&&( vkey != VK_SHIFT ))  {
            if( ckh < 200 )  {
                if( fCtrl && ( vkey == 'Q' ))  ;
                else  {
                    edit_beep();
                    akh[ ckh ].fCtrl = fCtrl;
                    akh[ ckh ].fShift = fShift;
                    akh[ ckh ].fChar = 0;
                    akh[ ckh ].vkey = vkey;
                    akh[ ckh ].dkey = dkey;
                    ckh += 1;
                }
            }
            else  {
                // Key History Buffer Full
            }
        }
    }
 
    dispy = ed->displcrds.y;

    HideCaret( NULL );
    fCaret = 0;
 
    if(( vkey != VK_CONTROL )&&( vkey != VK_SHIFT ))  {
        if( fMatchBracket )  HideMatchBracket();
    }

    if( fCtrl )  {
        switch( vkey )  {
            case VK_ESCAPE:
                blck_unmark();
                break;

            case VK_BACK:
                blck_unmark();
                cmnd_del_line();
                break;

            case VK_RETURN:
                blck_unmark();
                cmnd_ctrlcr();
                break;

            case VK_UP:
                blck_unmark();
                cmnd_scroll_up();
                break;

            case VK_DOWN:
                blck_unmark();
                cmnd_scroll_down();
                break;

            case VK_PRIOR:
                blck_unmark();
                cmnd_file_prev();
                fMru = TRUE;
                fsbu = TRUE;
                TreeItemSel( ed->hTreeItem, 0 );
                break;

            case VK_NEXT:
                blck_unmark();
                cmnd_file_next();
                fMru = TRUE;
                fsbu = TRUE;
                TreeItemSel( ed->hTreeItem, 0 );
                break;

            case VK_HOME:
                if( fShift )  blck_file_home();
                else  {
                    blck_unmark();
                    cmnd_file_home();
                }
                break;

            case VK_END:
                if( fShift )  blck_file_end();
                else  {
                    blck_unmark();
                    cmnd_file_end();
                }
                break;

            case VK_RIGHT:
                if( fShift )  blck_word_right();
                else  {
                    blck_unmark();
                    cmnd_word_right();
                }
                break;

            case VK_LEFT:
                if( fShift )  blck_word_left();
                else  {
                    blck_unmark();
                    cmnd_word_left();
                }
                break;

            case VK_INSERT:
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

            case VK_TAB:
            case VK_F6:
                blck_unmark();
                if( fShift )  {
                    if( !fFileTree )  {
                        fFileTree = !fFileTree;
                        ShowFileTree( fFileTree );
                    }
                    SetFocus( hwndTree );
                    fKeyProcessed = FALSE;
                }
                else  {
                    fmru_goto();
                    fMru = TRUE;
                    fsbu = TRUE;
                    TreeItemSel( ed->hTreeItem, 0 );
                }
                break;

            case VK_F2:
                bmrk_setnext();
                break;

            case VK_F3:
                if( fShift )  find_sel( 2 );
                else  find_sel( 0 );
                break;

            case VK_F4:
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
                break;

            /*case '0':
                if( fShift )  {
                    blck_unmark();
                    if( ! blck_paste( vkey - '0' ))  edit_beep();
                }
                break;*/
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                /*if( fShift )  {
                    blck_unmark();
                    if( ! blck_paste( vkey - '0' ))  edit_beep();
                }
                else    {*/
                    //==blck_unmark();
                    //==if( ! bmrk_goto( vkey - '1' ))  edit_beep();
                //}
                iClpbSel = vkey - '1' + 1;
                break;

            case 'A':
                blck_all();
                break;

            case 'B':
                blck_unmark();
                dlg_bookmarks( );
                break;

            case 'C':
                if( ! blck )  edit_beep( );
                else  {
                    blck_copy( iClpbSel );
                    if( iClpbSel == 0 )  {
                        fClipbrd = 0;
                        OpenClipboard( hwnd );
                        EmptyClipboard( );
                        SetClipboardData( CF_TEXT, NULL );
                        CloseClipboard( );
                    }
                }
                break;

            case 'D':
                 if( blck )  blck_case(2);
                 else  edit_beep( );
                 break;

            case 'E':
                fCtrl = 0;
                dlg_blck_fill();
                break;

            case 'F':
                //blck_unmark();
                fCtrl = 0;
                find_dlg();
                break;

            case 'G':
                blck_unmark();
                fCtrl = 0;
                dlg_goto();
                break;

            case 'I':
                blck_case(3);
                break;

            /*case 'J':
                cmnd_line_end();
                cmnd_del();
                break;*/

            /*case 'K':
                break;*/

            case 'L':
                if( ! TreeItemExpand( ed->hTreeItem ))  {
                    tree_file( ed, -1 );
                }
                break;

            case 'M':
                undo_goto();
                break;

            case 'N':
                blck_unmark();
                cmnd_file_last();
                break;

            case 'O':
                blck_unmark();
                if( file_open(""))
                    TreeItemSel( ed->hTreeItem, -1 );
                scrn_statsl();
                scrn_rescrn();
                fsbu = TRUE;
                break;

            case 'P':
                FilePrint( hwndMain );
                blck_unmark();
                scrn_rescrn();
                break;

            case 'Q':
                blck_unmark();
                if( fShift )  {
                    if( frkh )  {
                        frkh = 0;
                        //ckh -= 1;
                    }
                    else  {
                        ckh = 0;
                        frkh = -1;
                    }
                }
                else
                    PlayMacro( hwnd );
                break;

            case 'H':
            case 'R':
                fCtrl = 0;
                find_replace_dlg();
                break;

            case 'S':
                blck_unmark();
                file_save();
                scrn_statsl();
                scrn_rescrn();
                if ( TreeItemIsExpanded( ed->hTreeItem ))
                    tree_file( ed, -1 );
                break;

            case 'T':
                blck_unmark();
                fFileTree = !fFileTree;
                ShowFileTree( fFileTree );
                break;

            case 'U':
                 if( blck )  blck_case(1);
                 else  edit_beep( );
                 break;

            case 'V':
                //blck_unmark();
                if( iClpbSel == 0 )  {
                    if( fClipbrd )  {
                        fClipbrd = 0;
                        ClipbrdPaste( hwnd );
                    }
                }
                if( ! blck_paste( iClpbSel ))  edit_beep( );
                break;

            case 'W':
                //blck_unmark();
                if( iClpbSel == 0 )  {
                    if( fClipbrd )  {
                        fClipbrd = 0;
                        ClipbrdPaste( hwnd );
                    }
                }
                if( ! blck_pasteblck( iClpbSel ))  edit_beep( );
                break;

            case 'X':
                if( ! blck )  edit_beep( );
                else  {
                    blck_cut( iClpbSel );
                    if( iClpbSel == 0 )  {
                        fClipbrd = 0;
                        OpenClipboard( hwnd );
                        EmptyClipboard( );
                        SetClipboardData( CF_TEXT, NULL );
                        CloseClipboard( );
                    }
                }
                break;

            case 'Y':
                undo_redo( );
                break;

            case 'Z':
                undo_undo( );
                break;

            case 219:   // '['
            case 221:   // ']'
                blck_unmark();
                find_bracket();
                break;

            case 220:        // '\'
                break;

            default:
                fKeyProcessed = FALSE;
        }
    }
    else if( fShift )  {
        switch( vkey )  {
            case VK_SPACE:
                if( blck )  {
                    if( ! blck_revspace())  edit_beep( );
                }
                else    {
                    cmnd_char(( char )vkey );
                }
                break;

            case VK_PRIOR:
                //blck_beg();
                blck_page_up();
                break;

            case VK_NEXT:
                //blck_end();
                blck_page_down();
                break;

            case VK_DOWN:
                blck_down();
                break;

            case VK_UP:
                blck_up();
                break;

            case VK_RIGHT:
                blck_right();
                break;

            case VK_LEFT:
                blck_left();
                break;

            case VK_HOME:
                blck_line_home();
                break;

            case VK_END:
                blck_line_end();
                break;

            case VK_DELETE:
                if( ! blck )  edit_beep( );
                else  {
                    blck_cut( 0 );
                    fClipbrd = 0;
                    OpenClipboard( hwnd );
                    EmptyClipboard( );
                    SetClipboardData( CF_TEXT, NULL );
                    CloseClipboard( );
                }
                break;

            case VK_INSERT:
                //blck_unmark();
                if( fClipbrd )  {
                    fClipbrd = 0;
                    ClipbrdPaste( hwnd );
                }
                if( ! blck_paste( 0 ))  edit_beep( );
                break;

            case VK_TAB:
                if( blck )  {  if( ! blck_revtab())  edit_beep( );  }
                else    {  edit_beep( );  }
                break;

            case VK_F2:
                blck_unmark();
                if( ! bmrk_prev( ))  edit_beep( );
                break;

            case VK_F3:
                find_prev();
                break;

            case VK_F1:
                //DialogBox( hInst, "HELPFILE", hwndClient, KeywordHelpDlg );
                break;

            case VK_F12:
                file_saveas();
                blck_unmark();
                scrn_statsl();
                scrn_rescrn();
                if ( TreeItemIsExpanded( ed->hTreeItem ))
                    tree_file( ed, -1 );
                break;

            default:
                fKeyProcessed = FALSE;
        }
    }
    else  {
        switch( vkey )  {
            case VK_SPACE:
                //blck_unmark();
                //if( blck )  {
                if(( ! blck )||( blckstr.y == blckend.y ))  {
                    cmnd_char(( char )vkey );
                }
                else  {
                    if( ! blck_space())  edit_beep( );
                }
                break;

            case VK_ESCAPE:
                blck_unmark();
                break;

            case VK_UP:
                blck_unmark();
                cmnd_up();
                break;

            case VK_DOWN:
                blck_unmark();
                cmnd_down();
                break;

            case VK_PRIOR:
                blck_unmark();
                cmnd_page_up();
                break;

            case VK_NEXT:
                blck_unmark();
                cmnd_page_down();
                break;

            case VK_HOME:
                blck_unmark();
                cmnd_line_home();
                break;

            case VK_END:
                blck_unmark();
                cmnd_line_end();
                break;

            case VK_RIGHT:
                blck_unmark();
                cmnd_right();
                break;

            case VK_LEFT:
                blck_unmark();
                cmnd_left();
                break;

            case VK_INSERT:
                if( blck )  {
                    /*blck_copy( 0 );
                    fClipbrd = 0;
                    OpenClipboard( hwnd );
                    EmptyClipboard( );
                    SetClipboardData( CF_TEXT, NULL );
                    CloseClipboard( );*/
                }
                else  {
                    cmnd_ins();
                }
                break;

            case VK_DELETE:
                if( blck )  blck_del();
                else    cmnd_del();
                break;

            case VK_BACK:
                if( blck )  blck_del();
                else    cmnd_bs();
                break;

            case VK_RETURN:
                //blck_unmark();
                cmnd_cr();
                break;

            case VK_TAB:
                if( blck )  {  if( ! blck_tab())  edit_beep( );  }
                else    {  cmnd_tab();  }
                break;

            case VK_F2:
                blck_unmark();
                if( ! bmrk_next())  edit_beep();
                break;

            case VK_F3:
                find_repeat();
                break;

            case VK_F4 :
                blck_unmark();
                if( file_open(""))
                    TreeItemSel( ed->hTreeItem, -1 );
                scrn_statsl();
                scrn_rescrn();
                fsbu = TRUE;
                break;

            case VK_F7 :
                break;

            case VK_F1:
                KeywordHelp();
                blck_unmark( );
                scrn_cursor();
                break;

            case VK_F12:
                blck_unmark();
                file_save();
                scrn_statsl();
                scrn_rescrn();
                if ( TreeItemIsExpanded( ed->hTreeItem ))
                    tree_file( ed, -1 );
                break;

            default:
                fKeyProcessed = FALSE;

        }
    }
    switch( vkey )  {
        case VK_CONTROL:
                fCtrl = -1;
                break;

        case VK_SHIFT:
                fShift = -1;
                break;
    }
    if( fKeyProcessed )  {
        if(( ! blck )&&( fMatchBracket ))  ShowMatchBracket();
    }

    ShowCaret( NULL );
    fCaret = -1;
    if( dispy != ed->displcrds.y )  fsbu = TRUE;
    if( fsbu )  {
        SetScrollRange( hwndVScroll, SB_CTL, 0, ( int )( ed->nolines ), FALSE );
        SetScrollPos( hwndVScroll, SB_CTL, ( ( int )ed->displcrds.y ), TRUE );
    }
    SetScrollPos( hwndHScroll, SB_CTL, ( ( int )ed->displcrds.x ), TRUE );
}


WMChar( HWND hwnd, char chr )  {

    if(( chr > 32 )&&( chr < 127 ))  {
        if( frkh )  {
            akh[ ckh - 1 ].fChar = -1;
            akh[ ckh - 1 ].vkey = chr;
        }

        HideCaret( NULL );
        fCaret = 0;
        //blck_unmark();
        cmnd_char(( char )chr );
        if(( ! blck )&&( fMatchBracket ))  ShowMatchBracket();
        ShowCaret( NULL );
        fCaret = -1;
    }
}



HideMatchBracket()  {

    if( fbc )  {
        //scrn_reline();
        //scrn_rescrn();
        vdeo_chr( b1, b1x - ed->displcrds.x, b1y - ed->displcrds.y, CE_CLR_NORMAL, 1 );
        if( b2 )  {
            vdeo_chr( b2, b2x - ed->displcrds.x, b2y - ed->displcrds.y, CE_CLR_NORMAL, 1 );
        }
        fbc = 0;
    }
}


ShowMatchBracket()  {

    fbc = 0;
    if( ed->curcrds.x <= strlen( ed->curline->strz ))  {
        if(( ed->assoc == CE_ASC_C )||( ed->assoc == CE_ASC_JAVA ))  {
            int xb;
            char c;
            int f = 0;
 
            f = 0;

            c = ed->curline->strz[ ed->curcrds.x ];

            if(( c == '(' )||( c == ')' )||( c == '{' )||( c == '}' ))  {
                f = -1;
                xb = ed->curcrds.x;
            }
            if( !f )  {
                if( ed->curcrds.x > 0 )  {
                    c = ed->curline->strz[ ed->curcrds.x - 1 ];
                    if(( c == '(' )||( c == ')' )||( c == '{' )||( c == '}' ))  {
                        f = -1;
                        xb = ed->curcrds.x - 1;
                    }
                }
            }

            if( f )  {
                int yb, xc;
                struct linerec *lnb;

                xc = xb;
                yb = ed->curcrds.y;
                lnb = ed->curline;
                if( find_bracket_c( &xb, &yb, &lnb ))  {
                    b1 = ed->curline->strz[ xc ];
                    b1x = xc;
                    b1y = ed->curcrds.y;
 
                    vdeo_chr( ed->curline->strz[ xc ], xc - ed->displcrds.x, ed->curcrds.y - ed->displcrds.y, CE_CLR_MATCHBRACKET, 1 );
                    /*vdeo_chrb( ed->curline->strz[ xc ], xc - ed->displcrds.x, ed->curcrds.y - ed->displcrds.y, CE_CLR_NORMAL, 1 );
                    xc += 1;
                    if( ed->curline->strz[ xc ] )
                        vdeo_chr( ed->curline->strz[ xc  ], xc - ed->displcrds.x, ed->curcrds.y - ed->displcrds.y, CE_CLR_NORMAL, 1 );*/
 
                    if(( xb >= ed->displcrds.x )&&( xb < ed->displcrds.x + ( scrn_x-2 ))&&( yb >= ed->displcrds.y )&&( yb < ed->displcrds.y + ( scrn_y ))) {
                        b2 = lnb->strz[ xb ];
                        b2x = xb;
                        b2y = yb;
 
                        vdeo_chr( lnb->strz[ xb ], xb - ed->displcrds.x, yb - ed->displcrds.y, CE_CLR_MATCHBRACKET, 1 );
                        /*vdeo_chrb( lnb->strz[ xb ], xb - ed->displcrds.x, yb - ed->displcrds.y, CE_CLR_NORMAL, 1 );
                        if( xc != xb + 2 )  {
                            xc = xb + 1;
                            if( lnb->strz[ xc ] )
                                vdeo_chr( lnb->strz[ xc ], xc - ed->displcrds.x, yb - ed->displcrds.y, CE_CLR_NORMAL, 1 );
                        }*/
                    }
                    else  {
                        b2 = 0;
                    }
                    fbc = -1;
                }
            }
        }
    }
}


#ifdef __cplusplus
}
#endif
