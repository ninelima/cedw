

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

extern struct editrec *ed;
extern struct clpbrec clpbord[ ];



ClipbrdPaste( HWND hwnd )  {
    HANDLE hClipText;
    LPSTR clipbufr;
    int linecnt, lcrlf;
    unsigned bufrlen, linepos;
    struct linerec *linecur, *linedel;

    if( ! OpenClipboard( hwnd ))  {
        return( 0 );
    }
    if( !( hClipText = GetClipboardData( CF_TEXT )))  {
        CloseClipboard();
        return( 0 );
    }
    if( !( clipbufr = ( char * )GlobalLock( hClipText )))  {
        CloseClipboard( );
        return( 0 );
    }

    clpb_free( 0 );
    linecur = clpbord[ 0 ].line;

    linepos = 0;
    linecnt = 0;
    bufrlen = strlen( clipbufr );
    if( edit_get( &linecur, &linecnt, &linepos, &lcrlf, clipbufr, bufrlen ))  {
        linecur->strz[linepos] = '\0';
        ++linecnt;
        if( linecnt > 1 )  {
            clpbord[ 0 ].type = 1;
            //linecur->prev->next = NULL;
        }
        else  clpbord[ 0 ].type = 2;
    }
    GlobalUnlock( hClipText );
    CloseClipboard( );
}


ClipbrdCopy( HWND hwnd )  {
    HANDLE hClipText;
    LPSTR cliptxt;
    int bufrlen, bufrpos;
    struct linerec *linecur;


    if (! clpbord[ 0 ].type)  return(0);

    if (clpbord[ 0 ].type == 2)  {
        bufrlen = strlen( clpbord[ 0 ].line->strz );
    }
    else  {
        bufrlen = 0;
        linecur = clpbord[ 0 ].line;
        while( linecur != NULL )  {
            //bufrlen += strlen( linecur->strz ) + ( ed->ostype == 0x03 ? 2 : 1 );
            bufrlen += strlen( linecur->strz ) + 2;
            linecur = linecur->next;
        }
    }

    if( !( hClipText = GlobalAlloc( GMEM_MOVEABLE, ( DWORD )( bufrlen + 1 ))))  ;
    if( !( cliptxt = ( char * )GlobalLock( hClipText )))  ;

    if (clpbord[ 0 ].type == 2)  {
        strcpy( cliptxt, clpbord[ 0 ].line->strz );
    }
    else  {
        bufrpos = 0;
        linecur = clpbord[ 0 ].line;
        while( linecur != NULL )  {
            strcpy( &cliptxt[ bufrpos ], linecur->strz );
            bufrpos += strlen( linecur->strz );

            //if( ed->ostype == 0x03 )  cliptxt[ bufrpos++ ] = CE_KEY_CR;
            cliptxt[ bufrpos++ ] = CE_KEY_CR;
            cliptxt[ bufrpos++ ] = CE_KEY_LF;

            linecur = linecur->next;
        }
        cliptxt[ bufrpos ] = '\0';
    }
    GlobalUnlock( hClipText );
    SetClipboardData( CF_TEXT, hClipText );
}



#ifdef __cplusplus
}
#endif
