 
#include "windows.h"

#include "stdio.h"
#include "string.h"

#include "ced.h"
#include "cedw.h"


extern HDC hDC;
extern HFONT hFont, hFontBold;
extern HWND hwndClient;

extern int cyChar, cxChar;

extern int fCaret;


int vdeo_x = 0;
int vdeo_y = 0;

char ss[ 256 ];


struct colorset clrset[ ] = {
        0, 0,
        0x00FFFFFFL, 0x00800000L,    // CE_CLR_NORMAL - White on Dark Blue
        0x00000000L, 0x00C0C0C0L,    // CE_CLR_SELECT - Black on Palegrey
        0x00FF0000L, 0x00FFFFFFL,    // CE_CLR_KEYWORD
        0x00008000L, 0x00FFFFFFL,    // CE_CLR_STRING
        0x00000080L, 0x00FFFFFFL,    // CE_CLR_NUMBER
        0x00C0C0C0L, 0x00FFFFFFL,    // CE_CLR_COMMENT

        0x000000FFL, 0x00FFFFFFL,    // CE_CLR_ASP_SCRIPTDEL
        0x00800080L, 0x00FFFFFFL,    // CE_CLR_HTML_TAGNAME
        0x00FF0000L, 0x00FFFFFFL,    // CE_CLR_HTML_TAGDEL
        0x00800000L, 0x00FFFFFFL,    // CE_CLR_HTML_TAGTEXT
        0x00FF0000L, 0x00FFFFFFL,    // CE_CLR_HTML_TAGOPER
        //0x00C0C0C0L, 0x00FFFFFFL,    // CE_CLR_HTML_COMMENT

        0x00000080L, 0x00FFFFFFL,    // CE_CLR_XML_TAGNAME
        0x00FF0000L, 0x00FFFFFFL,    // CE_CLR_XML_TAGDEL
        0x00800000L, 0x00FFFFFFL,    // CE_CLR_XML_TAGTEXT
        0x00FF0000L, 0x00FFFFFFL,    // CE_CLR_XML_TAGOPER
        //0x00C0C0C0L, 0x00FFFFFFL,    // CE_CLR_XML_COMMENT

        0x00000000L, 0x0000FFFFL,    // CE_CLR_MATCHBRACKET


        0x00000000L, 0x00C0C0C0L,    // CE_CLR_STATUS - Black on Palegrey
        0x00FFFFFFL, 0x00C0C0C0L,    // CE_CLR_STATIN - White on Palegrey
        0x00FFFFFFL, 0x00000000L,    //                 White on Black
        0x000000FFL, 0x00FFFFFFL,    // CE_CLR_UNCOMMENT
        };



vdeo_ini( int x, int y )  {
        vdeo_x = x;
        vdeo_y = y;
        memset( ss, ' ', 255 );
        }


vdeo_str( char *s, int x, int y, int a )  {
        int l;

        l = strlen(s);
        if (x + l >= vdeo_x)  l = vdeo_x - x;

        SetTextColor( hDC, clrset[ a ].fore );
        SetBkColor( hDC, clrset[ a ].back );


        //if( a == 3 )  SelectObject( hDC, hFontBold );

        TextOut( hDC, ( int )( x*cxChar + 3 ), ( int )( y*cyChar + 1 ), s, ( int )l );
        //for( i = 0; i < l; ++i )
        //    TextOut( hDC, ( int )( ( x + i )* cxChar + 3 ), ( int )( y*cyChar + 1 ), &s[ i ], ( int )1);

        //vdeo_log( s );

        //if( a == 3 )  SelectObject( hDC, hFont );


        return(x + l);
        }


vdeo_strn( char *s, int x, int y, int a, int n )  {
        int l;

        l = strlen(s);
        if (x + l >= vdeo_x)  l = vdeo_x - x;
        if (n < l)  l = n;

        SetTextColor( hDC, clrset[ a ].fore );
        SetBkColor( hDC, clrset[ a ].back );

        //if( a == 3 )  SelectObject( hDC, hFontBold );

        TextOut( hDC, ( int )( x*cxChar + 3 ), ( int )( y*cyChar + 1 ), s, ( int )l );
        //for( i = 0; i < l; ++i )
        //    TextOut( hDC, ( int )( ( x + i )* cxChar + 3 ), ( int )( y*cyChar + 1 ), &s[ i ], ( int )1);
        
        //vdeo_log( s );

        //if( a == 3 )  SelectObject( hDC, hFont );

        return(x + l);
        }


vdeo_chr( int c, int x, int y, int a, int n )  {
        char cell[256];
        int j;

        if (x + n >= vdeo_x)  n = vdeo_x - x;

        SetTextColor( hDC, clrset[ a ].fore );
        SetBkColor( hDC, clrset[ a ].back );

        if( c == ' ' )  {
                TextOut( hDC, ( int )( x*cxChar + 3 ), ( int )( y*cyChar + 1 ), ss, ( int )n );
                //for( i = 0; i < n; ++i )
                //    TextOut( hDC, ( int )( ( x + i )* cxChar + 3 ), ( int )( y*cyChar + 1 ), &ss[ i ], ( int )4);


                }
        else    {
                for( j = 0; j < n; ++j )  cell[j] = c;
                cell[j+1] = '\0';

                TextOut( hDC, ( int )( x*cxChar + 3 ), ( int )( y*cyChar + 1 ), cell, ( int )n );
                }
        return(x + n);
        }

/*
vdeo_chrb( int c, int x, int y, int a, int n )  {
        char cell[256];
        int j;

        if (x + n >= vdeo_x)  n = vdeo_x - x;

        SetTextColor( hDC, clrset[ a ].fore );
        SetBkColor( hDC, clrset[ a ].back );
        SelectObject( hDC, hFontBold );

        if( c == ' ' )  {
                TextOut( hDC, ( int )( x*cxChar + 3 ), ( int )( y*cyChar + 1 ), ss, ( int )n );
                }
        else    {
                for( j = 0; j < n; ++j )  cell[j] = c;
                cell[j+1] = '\0';

                TextOut( hDC, ( int )( x*cxChar + 3 ), ( int )( y*cyChar + 1 ), cell, ( int )n );
                }
        SelectObject( hDC, hFont );
        return(x + n);
        }
*/


vdeo_atr( int x, int y, int a, int n )  {
        //if (x + n >= vdeo_x)  n = vdeo_x - x;
        }


vdeo_cur( int x, int y )  {
        POINT p;
        int xs, ys;

        //char zzz[ 100 ];

        xs = x * cxChar + 3;
        ys = y * cyChar + 1;

        //wsprintf( zzz, "x = %d  y = %d  cx = %d  cy = %d", x, y, cxChar, cyChar );
        //MessageBox( NULL, zzz, "", MB_OK );

        GetCaretPos( &p );

        if(( p.x != xs )||( p.y != ys ))  {
                SetCaretPos( xs, ys );
                }
        }



/*vdeo_crs(s, e)
int s, e;
        {

        CreateCaret( hwndClient, 0, e-s+1, 14);
        ShowCaret( hwndClient );
        }*/


vdeo_crs( int i )  {

        CreateCaret( hwndClient, 0, i ? 2 : 8, cyChar + 2);
        if( fCaret ) 
                ShowCaret( hwndClient );
        }



vdeo_log( char *s )  {

        FILE *f;
        if(( f = fopen( "C:\\cedwvdeo.log", "a" )) != NULL )  {
            fputs( s, f );
            }
        fputs( "\n", f );
        fclose( f );
        }
