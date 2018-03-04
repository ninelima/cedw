

//#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include "windows.h"

#include "string.h"

#include "commctrl.h"

#include "resource.h"

#ifdef __cplusplus
extern "C"{
#endif

#include "ced.h"
#include "cedw.h"

extern HINSTANCE hInst;
extern HWND hwndMain, hwndClient, hwndTree, hwndStatus, hwndSizeBar;

extern struct editrec *ed;

extern int fFileTree;
extern WNDPROC lpfnOldTreeViewWndProc;

extern int cxFileTree;
extern int cxFileTreeBak;
extern int cxSizeBar;

static HIMAGELIST himl;  
#define CE_CNT_IMAGES 21


static char abAss[ 10 ] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
static HTREEITEM alHti[ 10 ];

static int tvasscnt = 0;
static struct assrec tvasslst[ CE_MAX_ASSOCIATIONS ];

extern char asstype[ CE_CNT_ASSTYPE ][ CE_LEN_ASSTYPE ];
extern int asscnt;
extern struct assrec asslst[ CE_MAX_ASSOCIATIONS ];



tree_insfile( const char *szFileName )  {

    TreeFileIns( szFileName );
}


tree_delfile( HTREEITEM hTreeItem )  {

    TreeFileDel( hTreeItem );
}


tree_selfile( HTREEITEM hTreeItem, int fExpand )  {
    
    TreeItemSel( hTreeItem, fExpand );
}



tree_dellines( HTREEITEM hTreeItem, int l, int c )  {

    if( ! hTreeItem )  return;
    TreeDelLines( hTreeItem, l, c );
    if( TreeView_GetChild( hwndTree, hTreeItem )== NULL )  {
        TreeItemIns( "", 3, 3, hTreeItem, CE_TVI_FUNC, NULL, 0, 0 );
        TreeView_Expand( hwndTree, hTreeItem, TVE_COLLAPSE );
    }
}


tree_inslines( HTREEITEM hTreeItem, int l, int c )  {

    if( ! hTreeItem )  return;
    TreeInsLines( hTreeItem, l, c );
}



HTREEITEM TreeGetParent( HTREEITEM hTreeItem )  {

    return( TreeView_GetParent( hwndTree, hTreeItem ));
}

ShowFileTree( BOOL fShow )   {
    RECT rect;
    int cx, cy;

    GetClientRect( hwndMain, &rect );

    cx = ( int )rect.right;
    cy = ( int )rect.bottom;

    if( fShow )  {
        cxSizeBar = CE_SZE_SIZEBAR;
        cxFileTree = cxFileTreeBak;
    }
    else  {
        cxFileTreeBak = cxFileTree;
        cxSizeBar = 0;
        cxFileTree = 0;
    }

    MoveWindow( hwndStatus, 0, cy - CE_SZE_STATUSBAR, cx, CE_SZE_STATUSBAR, TRUE);
    MoveWindow( hwndTree, 1, 1, cxFileTree - 1, cy - CE_SZE_STATUSBAR - 1, TRUE );
    MoveWindow( hwndSizeBar, cxFileTree, 0, cxSizeBar, cy - CE_SZE_STATUSBAR, TRUE );
    //MoveWindow( hwndClient, cxFileTree + cxSizeBar, 0, cx - (cxFileTree + cxSizeBar ), cy - CE_SZE_STATUSBAR, TRUE );
    SetWindowPos( hwndClient, NULL, cxFileTree + cxSizeBar, 0, rect.right - ( cxFileTree + cxSizeBar ), rect.bottom - CE_SZE_STATUSBAR, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOMOVE );  // get rid of flashing scrollbar on sizing to the left
    SetWindowPos( hwndClient, NULL, cxFileTree + cxSizeBar, 0, rect.right - ( cxFileTree + cxSizeBar ), rect.bottom - CE_SZE_STATUSBAR, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER );
    InvalidateRect( hwndClient, NULL, TRUE );
    SetFocus( hwndClient );
    //scrn_resize();
    ShowCaret( NULL );
}





LONG FAR PASCAL TreeViewSubClassFunc( HWND hwnd, WORD message, WPARAM wParam, LPARAM lParam)  {
    int fCtrl, fShft;
 
    if(( message == WM_LBUTTONDBLCLK )||(( message == WM_CHAR )&&( wParam == VK_RETURN )))    {
        HTREEITEM hTreeItem;
        TVITEM tvi;
        TVITEMDATA *tvidata;

        hTreeItem = TreeView_GetSelection( hwndTree );
        tvi.hItem = hTreeItem;
        tvi.mask = TVIF_PARAM;
        TreeView_GetItem( hwndTree, &tvi );

        tvidata = ( TVITEMDATA * )tvi.lParam;
        if( tvidata->iType == CE_TVI_FUNC )  {
            PostMessage( hwndMain, WM_USER+5, ( WPARAM )tvidata->ed, ( LPARAM )tvidata->iLine );
            return 0;
        }
        else if( tvidata->iType == CE_TVI_FILE )  {
            PostMessage( hwndMain, WM_USER+4, ( WPARAM )tvidata->ed, 0 );
            return 0;
        }
        else if( tvidata->iType == CE_TVI_CLASS )  {
            if( tvidata->iLine )
                PostMessage( hwndMain, WM_USER+5, ( WPARAM )tvidata->ed, ( LPARAM )tvidata->iLine );
            else
                PostMessage( hwndMain, WM_USER+4, ( WPARAM )tvidata->ed, 0 );
            return 0;
        }
    }

    if(( message == WM_KEYDOWN )||( message == WM_CHAR ))  {
        fCtrl = ( GetKeyState( VK_CONTROL )& 0x80 );
        fShft = ( GetKeyState( VK_SHIFT )& 0x80 );
 
        if(( message == WM_CHAR )&&( wParam == VK_RETURN ))
            return 0;

        switch( wParam )  {
            case VK_F4:
                if( ! fCtrl )  {
                    blck_unmark();
                    if( file_open(""))
                        TreeItemSel( ed->hTreeItem, -1 );
                    scrn_statsl();
                    scrn_rescrn();
                    return 0;
                }
                break;
            case VK_ESCAPE:
                    SetFocus( hwndClient );
                    TreeItemSel( ed->hTreeItem, 0 );
                    return 0;
            case VK_TAB:
            case VK_F6:
                if( fCtrl && fShft )  {
                    SetFocus( hwndClient );
                    return 0;
                }
                break;
            case 'T':
                if( fCtrl )  {
                    if( fFileTree )  {
                        fFileTree = 0;
                        ShowFileTree( fFileTree );
                    }
                    return 0;
                }
        }
    }
    return CallWindowProc( lpfnOldTreeViewWndProc, hwnd, message, wParam, lParam );
}




void DestroyImageList()  {

    ImageList_Destroy( himl );
}



BOOL InitTreeImageLists(HWND hwndTree)  { 
    HBITMAP hbmp; 
    int i, n;
    char fn[ CE_LEN_FILENEXT + 2 ];
    SHFILEINFO shfi;
    //COLORREF cr;

    if ((himl = ImageList_Create( 16, 16, ILC_COLOR, asscnt + CE_CNT_IMAGES, 0)) == NULL)  
        return FALSE; 

    //TreeView_SetBkColor( hwndTree, -1 );
    //cr = TreeView_GetBkColor( hwndTree );
    //cr = GetSysColor( COLOR_WINDOW );
    //ImageList_SetBkColor( himl, cr );
    ImageList_SetBkColor( himl, 0xFFFFFF );     // White

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FOLDER_CLOSED)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 
    
    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FOLDER_OPEN)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FILE)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FUNC_C)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FUNC_C_UNSEL)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_C_CLASS)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_JAVA_CLASS)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_JAVA_FUNC_PUBLIC)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_JAVA_FUNC_PRIVATE)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_JAVA_FUNC_PROTECTED)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ORA_PACKAGE)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ORA_PROC)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ORA_FUNC)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ORA_TRIGGER)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ORA_VIEW)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ORA_TABLE)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MSSQL_PROC)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MSSQL_TRIGGER)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 
   
    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MSSQL_VIEW)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 

    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MSSQL_TABLE)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 


    hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_INTERFACE)); 
    i = ImageList_Add(himl, hbmp, (HBITMAP) NULL); 
    DeleteObject(hbmp); 


    for( n = 1; n < asscnt; ++n )  {
        strcpy( fn, "x." );
        strcat( fn, asslst[ n ].fnx );

        memset(&shfi,0,sizeof(shfi));
        SHGetFileInfo( fn, FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON );
        i = ImageList_AddIcon( himl, shfi.hIcon );
        DestroyIcon( shfi.hIcon );
    }

    if (ImageList_GetImageCount(himl) < 1) 
        return FALSE; 

    TreeView_SetImageList(hwndTree, himl, TVSIL_NORMAL); 

    return TRUE; 
} 




HTREEITEM TreeItemIns( LPCTSTR lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine )  { 
    TVITEM tvi; 
    TVINSERTSTRUCT tvins; 
    static HTREEITEM hPrev = (HTREEITEM) TVI_SORT; 
    static HTREEITEM hPrevRootItem = NULL; 
    static HTREEITEM hPrevLev2Item = NULL; 
    TVITEMDATA *tvidata;

    tvidata = ( TVITEMDATA *)malloc( sizeof( TVITEMDATA ));
    tvidata->iLevel = iLevel;
    tvidata->iType = iType;
    tvidata->iAssoc = iAssoc;
    tvidata->iLine = iLine;
    tvidata->iDelete = 0;
    tvidata->ed = ed;
 
    tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
    if( *lpszItem )
        tvi.mask |= TVIF_TEXT;
 
    tvi.pszText = ( char * )lpszItem; 
    tvi.cchTextMax = lstrlen(lpszItem); 
 
    tvi.iImage = nImage; 
    tvi.iSelectedImage = nImage; 
 
    tvi.lParam = ( LPARAM )tvidata; 

    tvins.item = tvi; 
    tvins.hInsertAfter = TVI_SORT;  //hPrev; 
 
    if (iLevel == 1) 
        tvins.hParent = TVI_ROOT; 
    else if (iLevel == 2) 
        tvins.hParent = hPrevRootItem; 
    else 
        tvins.hParent = hPrevLev2Item;
    tvins.hParent = htiParent;
 
    hPrev = (HTREEITEM) SendMessage(hwndTree, TVM_INSERTITEM, 0, (LPARAM) (LPTVINSERTSTRUCT) &tvins); 
    //TreeView_SelectItem( hwndTree, hPrev );
 
    if (iLevel == 1) 
        hPrevRootItem = hPrev; 
    else if (iLevel == 2) 
        hPrevLev2Item = hPrev; 
 
    // The new item is a child item. Give the parent item a 
    // closed folder bitmap to indicate it now has child items. 
    /*if (iLevel > 1) { 
        hti = TreeView_GetParent(hwndTree, hPrev); 
        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE; 
        tvi.hItem = hti; 
        tvi.iImage = 3;
        tvi.iSelectedImage = 3;
        TreeView_SetItem(hwndTree, &tvi); 
    } */
 
    return hPrev; 
}



HTREEITEM TreeItemUpd( LPCTSTR lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine )  { 
    HTREEITEM hChild;
    TVITEM tvi;
    TVITEMDATA *tvidata;

    if( ! htiParent )  return 0;
    hChild = TreeView_GetChild( hwndTree, htiParent );
    while( hChild )  {
        tvi.mask = TVIF_PARAM;
        tvi.hItem = hChild; 
        TreeView_GetItem(hwndTree, &tvi); 
        if( tvi.lParam )  {
            tvidata = ( TVITEMDATA * )tvi.lParam;

            if(( tvidata->iLine == iLine )&&( tvidata->iType == iType)&&( tvidata->iLevel == iLevel ))  {

                tvi.mask = TVIF_TEXT;
                tvi.hItem = hChild; 
                tvi.pszText = ( char * )lpszItem; 
                //tvi.cchTextMax = lstrlen(lpszItem); 
                TreeView_SetItem(hwndTree, &tvi); 
                tvidata->iDelete = 0;
                return hChild;
            }
        }
        hChild = TreeView_GetNextSibling( hwndTree, hChild );
    }
    return TreeItemIns( lpszItem, iLevel, nImage, htiParent, iType, ed, iAssoc, iLine );
}



TreeItemDelChildren( HTREEITEM hTreeItem )  {
    HTREEITEM hChild, hChildNext;
    TVITEM tvi;

    if( ! hTreeItem )  return 0;
    hChild = TreeView_GetChild( hwndTree, hTreeItem );
    while( hChild )  {
        TreeItemDelChildren( hChild );

        hChildNext = TreeView_GetNextSibling( hwndTree, hChild );

        tvi.mask = TVIF_PARAM;
        tvi.hItem = hChild; 
        TreeView_GetItem(hwndTree, &tvi); 
        if( tvi.lParam )
            free(( void *) tvi.lParam );

        TreeView_DeleteItem(hwndTree, hChild ); 
        hChild = hChildNext;
    }
}


void TreeItemsDel( )  {      
    int n;

    TreeItemDelChildren( TreeView_GetRoot( hwndTree ));
    TreeView_DeleteAllItems( hwndTree );
    for( n = 0; n < 10; ++n ) 
            abAss[ n ] = -1;

}


TreeItemSel( HTREEITEM hItem, int fExpand )  {
    HTREEITEM hParent;
    TVITEM tvi;

    //if( hItem == 0 )  return 0;
    if( fExpand )  {
        TreeView_SelectItem( hwndTree, hItem );
    }
    else  {
        hParent = TreeView_GetParent( hwndTree, hItem );
        tvi.hItem = hParent;
        tvi.mask = TVIF_STATE;
        tvi.stateMask = TVIS_EXPANDED;
        TreeView_GetItem( hwndTree, &tvi );
        if( tvi.state & TVIS_EXPANDED )  {
            TreeView_SelectItem( hwndTree, hItem );
        }
        else  {
            TreeView_SelectItem( hwndTree, hParent );
        }
    }
}


TreeItemExpand( HTREEITEM hTreeItem )  {
    int rc;
    TVITEM tvi;

    if( ! hTreeItem )  return 0;
    tvi.hItem = (struct _TREEITEM *)hTreeItem;
    tvi.mask = TVIF_STATE;
    tvi.stateMask = TVIS_EXPANDED;
    TreeView_GetItem( hwndTree, &tvi );
    if( !( tvi.state & TVIS_EXPANDED ))  {
        TreeView_Expand( hwndTree, ed->hTreeItem, TVE_COLLAPSE | TVE_COLLAPSERESET );
        rc = TreeView_Expand( hwndTree, ed->hTreeItem, TVE_EXPAND );
        return -1;
    }
    return 0;
}


TreeItemIsExpanded( HTREEITEM hTreeItem )  {
    TVITEM tvi;

    if( ! hTreeItem )  return 0;
    tvi.hItem = (struct _TREEITEM *)hTreeItem;
    tvi.mask = TVIF_STATE;
    tvi.stateMask = TVIS_EXPANDED;
    TreeView_GetItem( hwndTree, &tvi );
    if( tvi.state & TVIS_EXPANDED )  {
        return -1;
    }
    return 0;
}


void TreeFileDel( HTREEITEM hTreeItem )  {
    HTREEITEM hParent, hChild;
    int n;

    if( ! hTreeItem )  return;
    hParent = TreeView_GetParent( hwndTree, hTreeItem );

    SendMessage(hwndTree, TVM_DELETEITEM, 0, (LPARAM)hTreeItem ); 
    hChild = TreeView_GetChild( hwndTree, hParent );

    if( !hChild )  {
        SendMessage(hwndTree, TVM_DELETEITEM, 0, (LPARAM)hParent ); 
        for( n = 0; n < 10; ++n )  {
            if( alHti[ n ] == hParent )  {
                abAss[ n ] = -1;
                break;
            }
        }
    }
}




void TreeFileIns( LPCTSTR lpszFileName )  {
    int n, iItm;
    int f;
    int iImage;
    char *s;

    
    f = 0;
    for( iItm = 0; iItm < 10; ++iItm )  {
        if( abAss[ iItm ] == ed->assoc )  {
            f = -1;
            break;
        }
    }
    if( !f )  {        
        for( iItm = 0; iItm < 10; ++iItm )  
            if( abAss[ iItm ] == -1 )  break;

        abAss[ iItm ] = ed->assoc;
        if( ed->assoc == 0 )  
            alHti[ iItm ] = TreeItemIns( "", 1, 0, TVI_ROOT, CE_TVI_FOLDER, NULL, ed->assoc, 0 );
        else
            alHti[ iItm ] = TreeItemIns( asstype[ ed->assoc ], 1, 0, TVI_ROOT, CE_TVI_FOLDER, NULL, ed->assoc, 0 );
    }

    iImage = CE_CNT_IMAGES;
    if(( s = strrchr( lpszFileName, '.' )) != NULL )  {
        ++s;
        for( n = 2; n < asscnt; ++n )  {
            if( _stricmp( s, asslst[ n ].fnx ) == 0 )  {    // Check for CE_LEN_FILENEXT
                iImage = n + (CE_CNT_IMAGES - 1);
                break;
            }
        }

        if( n == asscnt )  {
            for( n = 0; n < tvasscnt; ++n )  {
                if( _stricmp( s, tvasslst[ n ].fnx ) == 0 )  {    // Check for CE_LEN_FILENEXT
                    iImage = n + (CE_CNT_IMAGES - 1) + asscnt;
                    break;
                }
            }
            if( n == tvasscnt )  {
                if( tvasscnt < CE_MAX_ASSOCIATIONS )  {
                    char fn[ CE_LEN_FILENEXT + 2 ];
                    SHFILEINFO shfi;
                
                    strcpy( tvasslst[ tvasscnt ].fnx, s );
                    tvasslst[ tvasscnt ].lng = 0;
                    tvasscnt += 1;
                 
                    strcpy( fn, "x." );
                    strcat( fn, s );

                    memset(&shfi,0,sizeof(shfi));
                    SHGetFileInfo( fn, FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_USEFILEATTRIBUTES | SHGFI_SMALLICON );
                    iImage = ImageList_AddIcon( himl, shfi.hIcon );
                    DestroyIcon( shfi.hIcon );
                }
            else
                iImage = 2;
            }
        }
    }
    ed->hTreeItem = TreeItemIns( lpszFileName, 2, iImage, alHti[ iItm ], CE_TVI_FILE, ed, ed->assoc, 0 );
    if(( ed->assoc == CE_ASC_C )||( ed->assoc == CE_ASC_JAVA )||( ed->assoc == CE_ASC_ORASQL )||( ed->assoc == CE_ASC_MSSQL )||( ed->assoc == CE_ASC_JVS )||( ed->assoc == CE_ASC_VBS ))
            TreeItemIns( "", 3, 3, (struct _TREEITEM *)ed->hTreeItem, CE_TVI_FUNC, NULL, 0, 0 );
}



TreeDelLines( HTREEITEM hTreeItem, int l, int c )  {
    HTREEITEM hChild;
    TVITEM tvi;  
    TVITEMDATA *tvidata;

    if( ! hTreeItem )  return;
    hChild = TreeView_GetChild( hwndTree, hTreeItem );
    while( hChild )  {
        TreeDelLines( hChild, l, c );

        tvi.mask = TVIF_PARAM;
        tvi.hItem = hChild; 
        TreeView_GetItem( hwndTree, &tvi ); 

        if( tvi.lParam )  {
            tvidata = ( TVITEMDATA * )tvi.lParam;
            if( tvidata->iType >= 3 )  {
                if( tvidata->iLine >= ( l + c ) )
                    tvidata->iLine -= c;
                else if(( tvidata->iLine >= l )&&( tvidata->iLine < ( l + c )))
                    TreeView_DeleteItem( hwndTree, tvi.hItem );
            }
        }
        hChild = TreeView_GetNextSibling( hwndTree, hChild );
    }
}



TreeInsLines( HTREEITEM hTreeItem, int l, int c )  {
    HTREEITEM hChild;
    TVITEM tvi;  
    TVITEMDATA *tvidata;

    if( ! hTreeItem )  return;
    hChild = TreeView_GetChild( hwndTree, hTreeItem );
    while( hChild )  {
        TreeInsLines( hChild, l, c );

        tvi.mask = TVIF_PARAM;
        tvi.hItem = hChild; 
        TreeView_GetItem(hwndTree, &tvi); 
        if( tvi.lParam )  {
            tvidata = ( TVITEMDATA * )tvi.lParam;
            if( tvidata->iType >= 3 )  {
                if( tvidata->iLine >= l )
                    tvidata->iLine += c;
            }
        }
        hChild = TreeView_GetNextSibling( hwndTree, hChild );
    }
}



TreeItemMark2Del( HTREEITEM hTreeItem )  {
    HTREEITEM hChild;
    TVITEM tvi;  
    TVITEMDATA *tvidata;

    if( ! hTreeItem )  return;
    hChild = TreeView_GetChild( hwndTree, hTreeItem );
    while( hChild )  {
        TreeItemMark2Del( hChild );

        tvi.mask = TVIF_PARAM;
        tvi.hItem = hChild; 
        TreeView_GetItem(hwndTree, &tvi); 
        if( tvi.lParam )  {
            tvidata = ( TVITEMDATA * )tvi.lParam;
            tvidata->iDelete = -1;
        }
        hChild = TreeView_GetNextSibling( hwndTree, hChild );
    }
}



TreeItemDelMarked( HTREEITEM hTreeItem )  {
    HTREEITEM hChild, hChildNext;
    TVITEM tvi;  
    TVITEMDATA *tvidata;

    if( ! hTreeItem )  return;
    hChild = TreeView_GetChild( hwndTree, hTreeItem );
    while( hChild )  {
        TreeItemDelMarked( hChild );
        hChildNext = TreeView_GetNextSibling( hwndTree, hChild );

        tvi.mask = TVIF_PARAM;
        tvi.hItem = hChild; 
        TreeView_GetItem(hwndTree, &tvi); 
        if( tvi.lParam )  {
            tvidata = ( TVITEMDATA * )tvi.lParam;
            if( tvidata->iDelete )  {
                TreeItemDelChildren( hChild );
                TreeView_DeleteItem( hwndTree, hChild ); 
            }
        }
    hChild = hChildNext;
    }
}


#ifdef __cplusplus
}
#endif
