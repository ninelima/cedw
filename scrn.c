
#include "ced.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


extern struct editrec *ed;
extern struct coords cursor;
extern int insmode;
extern int edcurno;
extern int blck;
extern struct coords blckstr, blckend;

extern int crmacode;

int scrn_x = CE_DEF_SCRNX;
int scrn_y = CE_DEF_SCRNY;


scrn_ini()
        {
        vdeo_ini( scrn_x, scrn_y );
        scrn_clr();
        vdeo_cur(0, 0);
        vdeo_crs( insmode );
        //vdeo_chr(' ', 0, scrn_y, CE_CLR_STATUS, scrn_x);
        }


scrn_clr()
        {
        int y;

        for (y = 0; y <= scrn_y; y++)
                vdeo_chr(' ', 0, y, CE_CLR_NORMAL, scrn_x);
        }


scrn_resize( int x, int y )  {
        int n;

        scrn_x = x;
        scrn_y = y;
        if(( cursor.y >= y )&&( y > 0 ))  cursor.y = y - 1;
        ed->curcrds.y = cursor.y + ed->displcrds.y;
        ed->curline = ed->displline;
        for( n = 0; n < cursor.y; ++n )  {
                if( ed->curline->next == NULL )  {
                        cursor.y = n;
                        ed->curcrds.y = cursor.y + ed->displcrds.y;
                        break;
                        }
                ed->curline = ed->curline->next;
                }
        //scrn_ini( );
        vdeo_ini( scrn_x, scrn_y );
//        vdeo_crs( insmode );
        scrn_cursor( );
        }


scrn_offset()  {
        int n;

        if ((ed->curcrds.y < ed->displcrds.y) || ((ed->curcrds.y - ed->displcrds.y) > scrn_y-2))  {
                ed->displline = ed->curline;
                ed->displcrds.y = ed->curcrds.y;
                cursor.y = 0;
                for (n = 0; n < 6; ++n)  {
                        if (ed->displcrds.y == 0)  break;
                        ed->displcrds.y -= 1;
                        cursor.y += 1;
                        ed->displline = ed->displline->prev;
                        }
                }
        cursor.y = ed->curcrds.y - ed->displcrds.y;
        }


scrn_adjust()  {         /* adjusts:  cursor.x  and/or ed->displcrds.x */

        if (ed->curcrds.x == 0)
                ed->displcrds.x = cursor.x = 0;
        else    {
                if (ed->curcrds.x < ed->displcrds.x || ed->curcrds.x > ed->displcrds.x + ( scrn_x-2 ))  {
                        if (ed->curcrds.x < scrn_x-1)  {
                                ed->displcrds.x = 0;
                                cursor.x = ed->curcrds.x;
                                }
                        else    {
                                ed->displcrds.x = ed->curcrds.x - (scrn_x-2);
                                cursor.x = scrn_x-2;
                                }
                        }
                else
                        cursor.x = ed->curcrds.x - ed->displcrds.x;
                }
        scrn_rescrn();
        }


scrn_rescrn()  {
        int y, l;
        struct linerec *ln;

        if (ed->displcrds.y + scrn_y-0 < ed->nolines)
                l = scrn_y-0;
        else
                l = ed->nolines - ed->displcrds.y;
        ln = ed->displline;
        for (y = 0; y < l; y++)  {
                if (ln == NULL)  break;
                scrnline( ln, y + ed->displcrds.y );
                ln = ln->next;
                }
        for (; y < scrn_y-0; ++y)
                vdeo_chr(CE_KEY_SPACE, 0, y, CE_CLR_NORMAL, scrn_x);
        scrn_cursor();
        }


scrn_reline()  {
        scrnline( ed->curline, ed->curcrds.y );
        }


lineatrs( struct linerec *ln, int lx, int x, int y, int a )  {
        int c, cx;

        if( ! crmacode )
                return( vdeo_str( &ln->strz[ lx ], x, y, a ));

        if(( ln->cmnt & 1 )&&( !( ln->cmnt & 4 )))  {
                return( vdeo_str( &ln->strz[ lx ], x, y, CE_CLR_COMMENT ));
                }
 

        if( ! ln->atrc )
                return( vdeo_str( &ln->strz[ lx ], x, y, a ));

        for( c = 0; c < ln->atrc; ++c )  {
                if( lx <= ln->atrs[ c ].pos + ln->atrs[ c ].cnt )
                        break;
                }

        if( c < ln->atrc )  {
                if(( lx > ln->atrs[ c ].pos )&&( lx <= ln->atrs[ c ].pos + ln->atrs[ c ].cnt ))   {
                        cx = ln->atrs[ c ].cnt - ( lx - ln->atrs[ c ].pos );
                        x = vdeo_strn( &ln->strz[ lx ], x, y, ln->atrs[ c ].atr, cx );
                        lx += cx;
                        ++c;
                        }
 
                for( ; c < ln->atrc; ++c )  {
                        if( ln->atrs[ c ].pos > lx )  {
                                cx =  ln->atrs[ c ].pos - lx;
                                x = vdeo_strn( &ln->strz[ lx ], x, y, a, cx );
                                lx = ln->atrs[ c ].pos;
                                }
                        cx = ln->atrs[ c ].cnt;
                        x = vdeo_strn( &ln->strz[ lx ], x, y, ln->atrs[ c ].atr, cx );
                        lx += ln->atrs[ c ].cnt;
                        }
                }
        if( ln->strz[ lx ] )  {         // !!! Could be dangerous but is faster than strlen()
        //if( lx < strlen( ln->strz ))  {
                x = vdeo_str( &ln->strz[ lx ], x, y, a );
                }
        return( x );
        }



lineatrsmx( struct linerec *ln, int lx, int x, int y, int a, int mx )  {
        int c, cx;

        if( mx == 0 )
                return(0);

        if( ! crmacode )  {
                cx = strlen( &ln->strz[ lx ] );
                if( mx < cx + x )
                        return( vdeo_strn( &ln->strz[ lx ], x, y, a, mx - x ));
                return( vdeo_str( &ln->strz[ lx ], x, y, a ));
                }

        if(( ln->cmnt & 1 )&&( !( ln->cmnt & 4 )))  {
                cx = strlen( &ln->strz[ lx ] );
                if( mx < cx + x )
                        return( vdeo_strn( &ln->strz[ lx ], x, y, CE_CLR_COMMENT, mx - x ));
                return( vdeo_str( &ln->strz[ lx ], x, y, CE_CLR_COMMENT ));
                }


        if( ! ln->atrc )  {
                cx = strlen( &ln->strz[ lx ] );
                if( mx < cx + x )
                        return( vdeo_strn( &ln->strz[ lx ], x, y, a, mx - x ));
                return( vdeo_str( &ln->strz[ lx ], x, y, a ));
                }

        for( c = 0; c < ln->atrc; ++c )  {
                if( lx <= ln->atrs[ c ].pos + ln->atrs[ c ].cnt )
                        break;
                }
        if( c < ln->atrc )  {
                if(( lx > ln->atrs[ c ].pos )&&( lx <= ln->atrs[ c ].pos + ln->atrs[ c ].cnt ))   {
                        cx = ln->atrs[ c ].cnt - ( lx - ln->atrs[ c ].pos );
                        if( mx < cx + x )
                                return( vdeo_strn( &ln->strz[ lx ], x, y, ln->atrs[ c ].atr, mx - x ));
                        x = vdeo_strn( &ln->strz[ lx ], x, y, ln->atrs[ c ].atr, cx );
                        lx += cx;
                        ++c;
                        }
 
                for( ; c < ln->atrc; ++c )  {
                        if( ln->atrs[ c ].pos > lx )  {
                                cx =  ln->atrs[ c ].pos - lx;
                                if( mx < cx + x )
                                        return( vdeo_strn( &ln->strz[ lx ], x, y, a, mx - x ));
                                x = vdeo_strn( &ln->strz[ lx ], x, y, a, cx );
                                lx = ln->atrs[ c ].pos;
                                }
                        cx = ln->atrs[ c ].cnt;
                        if( mx < cx + x )
                                return( vdeo_strn( &ln->strz[ lx ], x, y, ln->atrs[ c ].atr, mx - x ));
                        x = vdeo_strn( &ln->strz[ lx ], x, y, ln->atrs[ c ].atr, cx );
                        lx += ln->atrs[ c ].cnt;
                        }
                }
        if( ln->strz[ lx ] )  {
                cx = strlen( &ln->strz[ lx ] );
                if( mx < cx + x )
                        return( vdeo_strn( &ln->strz[ lx ], x, y, a, mx - x ));
                x = vdeo_str( &ln->strz[ lx ], x, y, a );
                }
        return( x );
        }



scrnline( struct linerec *ln, int dy )  {
        int x, y, hx, hc, l;

        l = strlen( ln->strz );     // Some sort of crash, ln->strz pointed somewhere weird, maybe check?
        y = dy - ed->displcrds.y;
        if( ! blck )  {
                if (l > ed->displcrds.x)
                        //x = vdeo_str(&ln->strz[ed->displcrds.x], 0, y, CE_CLR_NORMAL);
                        x = lineatrs( ln, ed->displcrds.x, 0, y, CE_CLR_NORMAL);
                else
                        x = 0;
                vdeo_chr(CE_KEY_SPACE, x, y, CE_CLR_NORMAL, scrn_x-x);
                }
        else    {
                if(( dy >= blckstr.y )&&( dy <= blckend.y ))  {
                        if (blckstr.y == blckend.y)  {
                                if (ed->displcrds.x > blckstr.x)  {
                                        hx = 0;
                                        hc = blckend.x - ed->displcrds.x;
                                        }
                                else    {
                                        hx = blckstr.x - ed->displcrds.x;
                                        hc = blckend.x - blckstr.x;
                                        }
                                }
                        else  if( blckstr.x == blckend.x )  {
                                if ((blckstr.y > dy) || (blckend.y <= dy))  {
                                        hx = hc = 0;
                                        }
                                else    {
                                        hx = 0;
                                        hc = scrn_x;
                                        }
                                }
                        else    {
                                if( dy < blckend.y )  {
                                        if (ed->displcrds.x > blckstr.x)  {
                                                hx = 0;
                                                hc = blckend.x - ed->displcrds.x;
                                                }
                                        else    {
                                                hx = blckstr.x - ed->displcrds.x;
                                                hc = blckend.x - blckstr.x;
                                                }
                                        }
                                else    {
                                        hx = hc = 0;
                                        }
                                }
                        if( ed->displcrds.x >= l)  {
                                x = 0;
                                x = vdeo_chr( ' ', x, y, CE_CLR_NORMAL, hx - x );
                                x = vdeo_chr( ' ', x, y, CE_CLR_SELECT, hc );
                                }
                        else    {
                                //x = vdeo_strn(&ln->strz[ed->displcrds.x], 0, y, CE_CLR_NORMAL, hx);
                                x = lineatrsmx( ln, ed->displcrds.x, 0, y, CE_CLR_NORMAL, hx );
                                if( x < hx )  {
                                        x = vdeo_chr( ' ', x, y, CE_CLR_NORMAL, hx - x );
                                        x = vdeo_chr( ' ', x, y, CE_CLR_SELECT, hc );
                                        }
                                else    {
                                        x = vdeo_strn(&ln->strz[ed->displcrds.x + x], x, y, CE_CLR_SELECT, hc);
                                        if( x < hx + hc )  {
                                                x = vdeo_chr( ' ', x, y, CE_CLR_SELECT, hc + hx - x );
                                                }
                                        else    {
                                                //x = vdeo_str(&ln->strz[x], x, y, CE_CLR_NORMAL);
                                                x = lineatrs( ln, ed->displcrds.x + x, x, y, CE_CLR_NORMAL );
                                                }
                                        }
                                }
                        vdeo_chr(' ', x, y, CE_CLR_NORMAL, scrn_x - x);
                        }
                else    {
                        if (l > ed->displcrds.x)
                                //x = vdeo_str(&ln->strz[ed->displcrds.x], 0, y, CE_CLR_NORMAL);
                                x = lineatrs( ln, ed->displcrds.x, 0, y, CE_CLR_NORMAL );
                        else
                                x = 0;
                        vdeo_chr(CE_KEY_SPACE, x, y, CE_CLR_NORMAL, scrn_x-x);
                        }
                }
        vdeo_cur(cursor.x, cursor.y);
        }



/*lineblck( struct linerec *ln, int sx, int y, int hx, int hc )  {
        int x, l;
        char *s;

        l = strlen( s );
        x = vdeo_strn(s, 0, y, CE_CLR_NORMAL, hx);
        if( x < hx )  {
                x = vdeo_chr( ' ', x, y, CE_CLR_NORMAL, hx - x );
                x = vdeo_chr( ' ', x, y, CE_CLR_SELECT, hc );
                }
        else    {
                x = vdeo_strn(&s[x], x, y, CE_CLR_SELECT, hc);

                if( x < hx + hc )  {
                        x = vdeo_chr( ' ', x, y, CE_CLR_SELECT, hc + hx - x );
                        }
                else    {
                        x = vdeo_str(&s[x], x, y, CE_CLR_NORMAL);
                        }
                }
        vdeo_chr(' ', x, y, CE_CLR_NORMAL, scrn_x - x);
        }*/





scrn_statsl()  {
        int z;
        char s[ 5 ] = { "0000" };

        if( edcurno < 9999 )  {
                z = edcurno;
 
                s[0] = z / 1000 + 48;
                z = z % 1000;
                s[1] = z / 100 + 48;
                z = z % 100;
                s[2] = z / 10 + 48;
                z = z % 10;
                s[3] = z + 48;
                s[4] = '\0';
                }
        else
                strcpy( s, "0000" );
 
        stsl_fname( ed->fname, s );
        stsl_insmode( insmode );
        stsl_ostype( ed->ostype );
        stsl_fmod( ed->changef );
        return( 0 );


/*#if defined( _WIN )|| defined( _WIN32 )
        //char *t;

        //if(( t = strrchr( ed->fname, '\\' )) == NULL )  t = strrchr( ed->fname, ':' );
        //if( t == NULL )  main_title( ed->fname );
        //else    main_title( t+1 );
        main_title( ed->fname );

        if( scrn_x > 22 )
                vdeo_chr(' ', 0, scrn_y, CE_CLR_STATUS, (scrn_x - 22));
#else*/
/*
        if( scrn_x > 22 )  {
                vdeo_chr(' ', 0, scrn_y, CE_CLR_STATUS, 1);
                if( strlen( ed->fname ) > scrn_x - 23 )  {
                        //c = ed->fname[ scrn_x - 22 ];
                        //ed->fname[ scrn_x - 22 ] = 0;
                        //x = vdeo_str(ed->fname, 1, scrn_y, (ed->changef == 2 ? CE_CLR_STATIN : CE_CLR_STATUS));
                        //ed->fname[ scrn_x - 22 ] = c;
                        x = vdeo_str(&ed->fname[ strlen( ed->fname ) -( scrn_x - 23 ) ], 1, scrn_y, (ed->changef == 2 ? CE_CLR_STATIN : CE_CLR_STATUS));
                        }
                else    {
                        x = vdeo_str(ed->fname, 1, scrn_y, (ed->changef == 2 ? CE_CLR_STATIN : CE_CLR_STATUS));
                        x = vdeo_chr(' ', x, scrn_y, CE_CLR_STATUS, (scrn_x - 22) - x);
                        }
                }
//#endif

        //vdeo_chr(' ', scrn_x - 22, scrn_y, CE_CLR_STATUS, 2);
        vdeo_chr( ed->ostype == 0x02 ? 'U' : ' ', scrn_x - 22, scrn_y, CE_CLR_STATIN, 1);
        vdeo_chr(' ', scrn_x - 21, scrn_y, CE_CLR_STATUS, 1);
        if (ed->changef == -1)
                vdeo_chr('M', scrn_x - 20, scrn_y, CE_CLR_STATIN, 1);
        else if (ed->changef == 2)
                vdeo_chr('X', scrn_x - 20, scrn_y, CE_CLR_STATIN, 1);
        else
                vdeo_chr(' ', scrn_x - 20, scrn_y, CE_CLR_STATIN, 1);
        vdeo_chr(' ', scrn_x - 19, scrn_y, CE_CLR_STATUS, 1);
        vdeo_chr( insmode ? 'I' : 'O', scrn_x - 18, scrn_y, CE_CLR_STATIN, 1);
        vdeo_chr(' ', scrn_x - 17, scrn_y, CE_CLR_STATUS, 1);
        (kb_shf() & 0x20) ? vdeo_chr('N', scrn_x - 16, scrn_y, CE_CLR_STATIN, 1) : vdeo_chr(' ', scrn_x - 16, scrn_y, CE_CLR_STATIN, 1);
        vdeo_chr(' ', scrn_x - 15, scrn_y, CE_CLR_STATUS, 1);
        (kb_shf() & 0x40) ? vdeo_chr('C', scrn_x - 14, scrn_y, CE_CLR_STATIN, 1) : vdeo_chr(' ', scrn_x - 14, scrn_y, CE_CLR_STATIN, 1);
        vdeo_chr(' ', scrn_x - 13, scrn_y, CE_CLR_STATUS, 1);
        vdeo_chr(' ', scrn_x - 7, scrn_y, CE_CLR_STATUS, 1);
        vdeo_chr(' ', scrn_x - 3, scrn_y, CE_CLR_STATUS, 1);

        z = edcurno;
        s[0] = z / 10 + 48;
        z = z % 10;
        s[1] = z + 48;
        s[2] = '\0';
        vdeo_str( s, scrn_x - 2, scrn_y, CE_CLR_STATUS );

        //scrn_cursor();
*/
        }


scrn_cursor()  {
        char l[7], c[5];
        int x, y;

        vdeo_cur(cursor.x, cursor.y);

        y = ed->curcrds.y + 1;
        x = ed->curcrds.x + 1;

                                 // !!!!!!!! Error 16 bit int: Check for _WIN32
        if( y < 999999 )  {
                l[0] = y / 100000 + 48;
                y = y % 100000;
                l[1] = y / 10000 + 48;
                y = y % 10000;
                l[2] = y / 1000 + 48;
                y = y % 1000;
                l[3] = y / 100 + 48;
                y = y % 100;
                l[4] = y / 10 + 48;
                y = y % 10;
                l[5] = y + 48;
                l[6] = '\0';
                }
        else
                strcpy( l, "000000" );

        if( x < 9999 )  {
                c[0] = x / 1000 + 48;
                x = x % 1000;
                c[1] = x / 100 + 48;
                x = x % 100;
                c[2] = x / 10 + 48;
                x = x % 10;
                c[3] = x + 48;
                c[4] = '\0';
                }
        else
                strcpy( c, "0000" );
 
        stsl_crsrpos( l, c );
        stsl_fmod( ed->changef );

        /*vdeo_str(l, scrn_x - 12, scrn_y, CE_CLR_STATUS);
        vdeo_str(c, scrn_x - 6, scrn_y, CE_CLR_STATUS);
        if (ed->changef == -1)
                vdeo_str("M", scrn_x - 20, scrn_y, CE_CLR_STATIN);
        */
        }

/*
wrnolines()  {
        char l[6];
        int n;

        n = ed->nolines;
        l[0] = n / 10000 + 48;
        n = n % 10000;
        l[1] = n / 1000 + 48;
        n = n % 1000;
        l[2] = n / 100 + 48;
        n = n % 100;
        l[3] = n / 10 + 48;
        n = n % 10;
        l[4] = n + 48;
        l[5] = '\0';
        vdeo_str(l, 50, scrn_y, CE_CLR_STATIN);
        }


wrshift()  {
        int c;
        char l[16];
        unsigned s;

        for (c = 0; c < 16; ++c)
                l[c] = '0';
        l[16] = '\0';

        s = kb_shf();
        if (s & 0x1) l[15] = '1';
        if (s & 0x2) l[14] = '1';
        if (s & 0x4) l[13] = '1';
        if (s & 0x8) l[12] = '1';
        if (s & 16)  l[11] = '1';
        if (s & 32)  l[10] = '1';
        if (s & 64)  l[9]  = '1';
        if (s & 128) l[8]  = '1';
        if (s & 256) l[7]  = '1';
        vdeo_str(l, 30, scrn_y, CE_CLR_SELECT);
        }
*/





