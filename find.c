
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
 
#include "ced.h"

extern struct editrec *edfirst;
extern struct editrec *ed;
extern struct linerec *blckline;
extern struct coords cursor;
extern int insmode;
extern int edcurno;
extern int edcount;
extern int blck;
extern struct coords blckstr, blckend;
extern int scrn_x, scrn_y;
extern int undof;
extern int crmacode;


int ww = 0;
int mc = 0;
int wh = 0;
int vf = 0;
int fs = 0;
int rf = 0;
int af = 0;

char findstr[CE_LEN_FINDSTR] = { "\0" };
char chngstr[CE_LEN_FINDSTR] = { "\0" };

char fshlst[ CE_MAX_FINDHIST ][ CE_LEN_FINDSTR ];
char cshlst[ CE_MAX_FINDHIST ][ CE_LEN_FINDSTR ];
int cshcnt = 0;
int fshcnt = 0;



ffhistory( char *findstr )  {
        int n;

        if( ! findstr[ 0 ])  return( 0 );
        if( fshcnt )  {
                for( n = 0; n < fshcnt; ++n )  {
                        if( strcmp( findstr, fshlst[ n ]) == 0 )  {
                                for( ; n < fshcnt; ++n )  {
                                        strcpy( fshlst[ n ], fshlst[ n + 1 ]);
                                        fshlst[ n + 1 ][ 0 ] = '\0';
                                        }
                                fshcnt -= 1;
                                break;
                                }
                        }
                strcpy( fshlst[ fshcnt ], findstr );
                if( fshcnt < CE_MAX_FINDHIST - 1)  {
                        fshcnt += 1;
                        }
                else    {
                        for( n = 0; n < fshcnt; ++n )  {
                                strcpy( fshlst[ n ], fshlst[ n + 1 ]);
                                fshlst[ n + 1 ][ 0 ] = '\0';
                                }
                        }
                }
        else    {
                strcpy( fshlst[ fshcnt ], findstr );
                fshcnt += 1;
                }
        return(-1);
        }


frhistory( char *chngstr )  {
        int n;

        if( ! chngstr[ 0 ])  return( 0 );
        if( cshcnt )  {
                for( n = 0; n < cshcnt; ++n )  {
                        if( strcmp( chngstr, cshlst[ n ]) == 0 )  {
                                for( ; n < cshcnt; ++n )  {
                                        strcpy( cshlst[ n ], cshlst[ n + 1 ]);
                                        cshlst[ n + 1 ][ 0 ] = '\0';
                                        }
                                cshcnt -= 1;
                                break;
                                }
                        }
                strcpy( cshlst[ cshcnt ], chngstr );
                if( cshcnt < CE_MAX_FINDHIST -1)  {
                        cshcnt += 1;
                        }
                else    {
                        for( n = 0; n < cshcnt; ++n )  {
                                strcpy( cshlst[ n ], cshlst[ n + 1 ]);
                                cshlst[ n + 1 ][ 0 ] = '\0';
                                }
                        }
                }
        else    {
                strcpy( cshlst[ cshcnt ], chngstr );
                cshcnt += 1;
                }
        return(-1);
        }



find_getcurselword( char *word )  {     // get selected or first set current word select if no selection
        int t, c, l, s;

        word[ 0 ] = '\0';
        if(( blck )&&(blckstr.y == blckend.y))  {
                l = strstb(ed->curline->strz);
                if (blckstr.x < l)  {
                        if (blckend.x >= l)  blckend.x = l;
                        l = blckend.x - blckstr.x;
                        if( l < CE_LEN_FINDSTR )  {
                                strncpy( word, &ed->curline->strz[ blckstr.x ], l+1 );
                                word[ l ] = '\0';
                                }
                        }
                }
        else    {
                c = 0;
                t = ed->curcrds.x;
                if( t > 0)  {
                        while((isalphan(ed->curline->strz[t-1]))||(ed->curline->strz[t-1] == '_')) {
                                --t;
                                if (t == 0)  break;
                                }
                        }
                s = t;
                l = strlen(ed->curline->strz);
                if (t < l)  {
                        while((isalphan(ed->curline->strz[t]))||(ed->curline->strz[t] == '_'))  {
                                word[ c ] = ed->curline->strz[t];
                                ++c;
                                ++t;
                                if( c >= CE_LEN_FINDSTR - 1 )  {
                                        c = 0;
                                        break;
                                        }
                                if (t == l)  break;
                                }
                        }
                word[ c ] = '\0';
                if( ! blck && c )  {
                        blck = -1;
                        blckstr.x = s;
                        blckend.x = t;
                        blckstr.y = blckend.y = ed->curcrds.y;
                        }
                }
        }



find_dlg( )  {
        char selstr[CE_LEN_FINDSTR];

        find_getcurselword( selstr );
        if( *selstr )  strcpy( findstr, selstr );
        while( 1 )  {
                if( dlg_find( ))  {
                        if ( ! find_find( findstr, ww, mc, wh, vf, fs, rf, af ))  {
                                dlg_ok( "Search String Not Found !", "Find" );
                                scrn_cursor( );
                                return( 0 );
                                }
                        else    {
                                //if( wh != 2 )  wh = 0;
                                scrn_statsl();
                                scrn_adjust();
                                return(-1);
                                }
                        }
                else    {
                        blck_unmark();
                        scrn_reline();
                        return(0);
                        }
                }
        }



find_replace_dlg( )  {
        char selstr[CE_LEN_FINDSTR];

        find_getcurselword( selstr );
        if( *selstr )  strcpy( findstr, selstr );
        while( 1 )  {
                if( dlg_find_replace( ))  {
                        if( ! find_replace( findstr, chngstr, ww, mc, wh, vf, fs, af ))  {
                                dlg_okstop( "Search String Not Found !", "Find and Replace" );
                                scrn_cursor();
                                }
                        else    {
                                //wh = 0;
                                return(-1);
                                }
                        }
                else    {
                        blck_unmark();
                        scrn_reline();
                        return(0);
                        }
                }
        }



find_sel( int w )  {
        char selstr[CE_LEN_FINDSTR];

        find_getcurselword( selstr );
        strcpy( findstr, selstr );
        wh = w;

        find__( );
        }


find_repeat()  {
        rf = -1;
        wh = 0;

        find__( );
        }


find_prev()  {
        rf = -1;
        wh = 2;

        find__( );
        }


find__( )  {

        if( ! *findstr )  {
                //dlg_okstop( "No Search String Specified !", "Find" );
                edit_beep();
                return( 0 );
                }
        if (! find_find( findstr, ww, mc, wh, vf, fs, rf, af ))  {
                dlg_ok( "Search String Not Found !", "Find" );
                scrn_reline();
                scrn_cursor();
                return(0);
                }
        else    {
                scrn_statsl();
                scrn_adjust();
                return(-1);
                }
        }



char *strstrm( char *d, char *s )  {
        char *u, *v;

        if (! *s)  return(NULL);

        while (1)  {
                while (*d != *s)
                        if (! *d++)  return(NULL);
                u = d;
                v = s;
                while (*u++ == *v++)
                        if (! *v)  return(d);
                ++d;
                }
        }


char *strstrmi( char *d, char *s )  {
        int n;
        char *u, *v, t[CE_LEN_FINDSTR], *w;

        if (! *s)  return(NULL);

        for (n = 0; s[n]; ++n)  {
                if (isalpha(s[n]))
                        t[n] = s[n] ^ 0x20;
                else
                        t[n] = s[n];
                }
        while (1)  {
                while ((*d != *s) && (*d != *t))
                        if (! *d++)  return(NULL);
                u = d;
                v = s;
                w = t;
                while ((*u == *v) || (*u == *w))  {
                        ++v;
                        if (! *v)  return(d);
                        ++u;
                        ++w;
                        }
                ++d;
                }
        }



char *strstrmb( char *ls, char *ws, int k )  {
        int ll, wl, wi, li;

        if( ! k )  return( NULL );
        if( ! *ls )  return( NULL );
        if( ! ( wl = strlen( ws )))  return( NULL );

        ll = k;
        while( 1 )  {
                while( ls[ ll - 1 ] != ws[ wl - 1 ])
                        if( ! ll-- )  return( NULL );
                wi = wl;
                li = ll;
                while( ls[ --li ] == ws[ --wi ])
                        if( ! wi )  return( &ls[ li ]);
                --ll;
                }
        }



char *strstrmbi( char *ls, char *ws, int k )  {
        int n;
        char ts[CE_LEN_FINDSTR];
        int ll, wl, wi, li;

        if( ! k )  return( NULL );
        if( ! *ls )  return( NULL );
        if( ! ( wl = strlen( ws )))  return( NULL );

        for (n = 0; ws[n]; ++n)  {
                if (isalpha(ws[n]))
                        ts[n] = ws[n] ^ 0x20;
                else
                        ts[n] = ws[n];
                }
        ll = k;
        while( 1 )  {
                while(( ls[ ll - 1 ] != ws[ wl - 1 ])&&( ls[ ll - 1 ] != ts[ wl - 1 ]))
                        if( ! ll-- )  return( NULL );
                wi = wl;
                li = ll;
                while(( ls[ li-1 ] == ws[ wi-1 ]) || ( ls[ li-1 ] == ts[ wi-1 ]))  {
                        --li;
                        --wi;
                        if( ! wi )  return( &ls[ li ]);
                        }
                --ll;
                }
        }





int rbf = 0;


find_replace( char *findstr, char *chngstr, int ww, int mc, int wh, int vf, int sf, int af )  {
        int chngcnt, fnd, yn, l, n, rf;
        char m[40];

        rf = 0;
        chngcnt = 0;
        fnd = 0;
        rbf = 0;
        if( blck )  {
                if( blckstr.y != blckend.y )
                        wh = 3;
                else
                        rbf = -1;
                }
        while( find_find( findstr, ww, mc, wh, vf, sf, rf, af ))  {
                rf = -1;
                fnd = -1;
                if( vf == 0 )  {
                        scrn_adjust();
                        scrn_statsl();
                        if(( yn = dlg_yna( "Replace this Occurence", "Find and Replace" )) == 0)
                                break;
                        if( yn == 2 )  {
                                vf = -1;
                                yn = 1;
                                edit_wait( -1 );
                                }
                        }
                else    yn = 1;
                if( yn == 1 )  {
                        ed->curcrds.x = blckstr.x;
                        if( undof )  {
                                if( ! undo_ovrlines( ed->curline, 1, ed->curcrds.y, 0 ))  return(0);
                                }
                        l = strstb(ed->curline->strz);
                        if (blckstr.x < l)  {
                                if (blckend.x >= l)
                                        strdel(ed->curline->strz, blckstr.x, l - blckstr.x);
                                else
                                        strdel(ed->curline->strz, blckstr.x, blckend.x - blckstr.x);
                                }
                        l = strlen(chngstr);
                        if( ! edit_sizeline( ed->curline, l + strlen( ed->curline->strz ) ))  return(0);
                        n = strlen(ed->curline->strz);
                        while (n < ed->curcrds.x)
                                strinsc(ed->curline->strz, ' ', n++);
                        for (n = 0; n < l; ++n)
                                strinsc(ed->curline->strz, chngstr[n], ed->curcrds.x + n);
                        ed->curcrds.x += n;
                        ++chngcnt;
                        cmnd_change();
                        crma_line( ed->curline );
                        blck = 0;
                        /*scrn_statsl();
                        cursor.x = ed->curcrds.x - ed->displcrds.x;
                        if( vf == 0 )
                                scrn_reline();
                        else
                                scrn_adjust();
                        */
                        }
                }
        edit_wait( 0 );
        blck = 0;
        if( yn == 1 )  {
                scrn_statsl();
                cursor.x = ed->curcrds.x - ed->displcrds.x;
                scrn_adjust();
                }
        scrn_rescrn();
        if (! fnd)
                return(0);
        else    {
                //blck = 0;
                //scrn_rescrn();
                itostr(m, chngcnt);           /* Fix This ?? */
                strcat(m, "  Occurence(s) Replaced");
                dlg_okinfo( m, "Find and Replace" );
                return(-1);
                }
        }


find_find( char *findstr, int ww, int mc, int wh, int vf, int sf, int rf, int af )  {    // wh: 0 = Forward, 1 = From First, 2 = Backwards, 3 = Selected
        int y, k, fnd, ecn;
        char *f;
        struct linerec *ln;
        struct editrec *edt;
        static int blckendy = 0;


        if( ! *findstr )
                return(0);
        if( wh == 2 )
                return( findprev( findstr, ww, mc, wh, vf, sf, rf, af ));
        fnd = 0;
        edt = ed;
        ecn = edcurno;
        if( ! rf )  {
                if( wh == 1 )  {
                        if( af )  {
                                while (ed->prev != NULL)  {
                                        edcurno -= 1;
                                        ed = ed->prev;
                                        }
                                }
                        ln = ed->firstline;
                        y = 0;
                        k = 0;
                        }
                else if( wh == 3 )  {
                        ln = blckline;
                        y = blckstr.y;
                        k = 0;
                        blckendy = blckend.y;
                        }
                else    {
                        ln = ed->curline;
                        y = ed->curcrds.y;
                        if( rbf )  k = blckstr.x;
                        else  k = ed->curcrds.x + 1;
                        rbf = 0;
                        }
                }
        else    {
                ln = ed->curline;
                y = ed->curcrds.y;
                k = ed->curcrds.x + 1;
                }
        blck = 0;

        while( 1 )  {
                if(( sf && ( ln->strz[ 0 ] != ' ')) || ( ! sf ))  {
                        if( k < strlen( ln->strz ))  {
                                if( f = ( mc ? strstrm( &ln->strz[ k ], findstr ) : strstrmi( &ln->strz[ k ], findstr )))  {
                                        if( ww )  {
                                                if( ! isalphau( f[ strlen( findstr ) ] ))  {
                                                        if( f == ln->strz )  {
                                                                fnd = -1;
                                                                break;
                                                                }
                                                        else if( ! isalphau( *( f-1 )))  {
                                                                fnd = -1;
                                                                break;
                                                                }
                                                        }
                                                k = f - ln->strz + 1;
                                                continue;
                                                }
                                        else    {
                                                fnd = -1;
                                                break;
                                                }
                                        }
                                }
                        }
                k = 0;
                ++y;
                ln = ln->next;
                if( wh == 3  )
                        if( y >= blckendy )
                                break;
                if( ln == NULL )  {
                        if( af )  {
                                if (ed->next != NULL)  {
                                        edcurno += 1;
                                        ed = ed->next;
                                        ln = ed->firstline;
                                        y = 0;
                                        k = 0;
                                        }
                                else
                                        break;
                                }
                        else
                                break;
                        }
                }
        if (! fnd)  {
                ed = edt;
                edcurno = ecn;
                return(0);
                }
        else    {
                ed->curline = ln;
                ed->curcrds.x = f - ln->strz;
                ed->curcrds.y = y;
                scrn_offset();
                cursor.x = ed->curcrds.x - ed->displcrds.x;
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x + strlen(findstr);
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                if( blckend.x - blckstr.x > (scrn_x-1 ))  {
                        ed->curcrds.x = blckstr.x;
                        ed->displcrds.x = blckstr.x;
                        cursor.x = 0;
                        }
                else  if(( blckend.x > ed->displcrds.x + (scrn_x-1)) || ( ed->displcrds.x > blckstr.x ))  {
                        ed->curcrds.x = blckend.x;
                        if (ed->curcrds.x < ed->displcrds.x || ed->curcrds.x > ed->displcrds.x + (scrn_x-1))  {
                                if (ed->curcrds.x < scrn_x)  {
                                        ed->displcrds.x = 0;
                                        cursor.x = ed->curcrds.x;
                                        }
                                else    {
                                        ed->displcrds.x = ed->curcrds.x - (scrn_x-1);
                                        cursor.x = scrn_x-1;
                                        }
                                }
                        else
                                cursor.x = ed->curcrds.x - ed->displcrds.x;
                        ed->curcrds.x = blckstr.x;
                        cursor.x = ed->curcrds.x - ed->displcrds.x;
                        }
                fmru_setcur();
                }
        return(-1);
        }



findprev( char *findstr, int ww, int mc, int wh, int vf, int sf, int rf, int af )  {
        int y, k, fnd, ecn;
        char *f;
        struct linerec *ln;
        struct editrec *edt;
        static int blckendy = 0;

        fnd = 0;
        edt = ed;
        ecn = edcurno;
        if( ! rf )  {
                ln = ed->curline;
                y = ed->curcrds.y;
                if( rbf )  k = blckend.x;
                else  k = ed->curcrds.x;
                if( k > strlen( ln->strz ))
                        k = strlen( ln->strz );
                rbf = 0;
                }
        else    {
                ln = ed->curline;
                y = ed->curcrds.y;
                k = ed->curcrds.x;
                if( k > strlen( ln->strz ))
                        k = strlen( ln->strz );
                }
        blck = 0;

        while( 1 )  {
                if(( sf && ( ln->strz[ 0 ] != ' ')) || ( ! sf ))  {
                        if( k <= strlen( ln->strz ))  {
                                if( f = ( mc ? strstrmb( ln->strz, findstr, k ) : strstrmbi( ln->strz, findstr, k )))  {
                                        if( ww )  {
                                                if( ! isalphau( f[ strlen( findstr ) ] ))  {
                                                        if( f == ln->strz )  {
                                                                fnd = -1;
                                                                break;
                                                                }
                                                        else if( ! isalphau( *( f-1 )))  {
                                                                fnd = -1;
                                                                break;
                                                                }
                                                        }
                                                k = ( f - ln->strz ) - 1;
                                                continue;
                                                }
                                        else    {
                                                fnd = -1;
                                                break;
                                                }
                                        }
                                }
                        }
                ln = ln->prev;
                if( ln == NULL )  {
                        if( af )  {
                                if (ed->prev != NULL)  {
                                        edcurno -= 1;
                                        ed = ed->prev;
                                        ln = ed->firstline;
                                        y = 0;
                                        while( ln->next != NULL )  {
                                                ++y;
                                                ln = ln->next;
                                                }
                                        k = strlen( ln->strz );
                                        }
                                else
                                        break;
                                }
                        else
                                break;
                        }
                else    {
                        --y;
                        k = strlen( ln->strz );
                        }
                }
        if (! fnd)  {
                ed = edt;
                edcurno = ecn;
                return(0);
                }
        else    {
                ed->curline = ln;
                ed->curcrds.x = f - ln->strz;
                ed->curcrds.y = y;
                scrn_offset();
                cursor.x = ed->curcrds.x - ed->displcrds.x;
                blck = -1;
                blckstr.x = ed->curcrds.x;
                blckend.x = ed->curcrds.x + strlen(findstr);
                blckstr.y = ed->curcrds.y;
                blckend.y = ed->curcrds.y;
                blckline = ed->curline;
                if( blckend.x - blckstr.x > (scrn_x-1 ))  {
                        ed->curcrds.x = blckstr.x;
                        ed->displcrds.x = blckstr.x;
                        cursor.x = 0;
                        }
                else  if(( blckend.x > ed->displcrds.x + (scrn_x-1)) || ( ed->displcrds.x > blckstr.x ))  {
                        ed->curcrds.x = blckend.x;
                        if (ed->curcrds.x < ed->displcrds.x || ed->curcrds.x > ed->displcrds.x + (scrn_x-1))  {
                                if (ed->curcrds.x < scrn_x)  {
                                        ed->displcrds.x = 0;
                                        cursor.x = ed->curcrds.x;
                                        }
                                else    {
                                        ed->displcrds.x = ed->curcrds.x - (scrn_x-1);
                                        cursor.x = scrn_x-1;
                                        }
                                }
                        else
                                cursor.x = ed->curcrds.x - ed->displcrds.x;
                        ed->curcrds.x = blckstr.x;
                        cursor.x = ed->curcrds.x - ed->displcrds.x;
                        }
                fmru_setcur();
                }
        return(-1);
        }



find_bracket()  {
        int x, y;
        struct linerec *ln;

        x = ed->curcrds.x;
        y = ed->curcrds.y;
        ln = ed->curline;

        if((( ed->assoc == CE_ASC_C )||( ed->assoc == CE_ASC_JAVA ))&& crmacode )  {
                if( ! find_bracket_c( &x, &y, &ln ))  {
                        edit_beep();
                        }
                else    {
                        ed->curline = ln;
                        ed->curcrds.x = x;
                        ed->curcrds.y = y;
                        scrn_offset();
                        scrn_statsl();
                        scrn_adjust();
                        }
                }
        else if((( ed->assoc == CE_ASC_MSSQL )||( ed->assoc == CE_ASC_ORASQL )||( ed->assoc == CE_ASC_SYBSQL ))&& crmacode )  {
                //if( ! find_bracket_sql( &x, &y, &ln ))  {
                if( ! findbracket( ))  {
                        edit_beep();
                        }
                else    {
                        //ed->curline = ln;
                        //ed->curcrds.x = x;
                        //ed->curcrds.y = y;
                        //scrn_offset();
                        scrn_statsl();
                        scrn_adjust();
                        }
                }
        else    {
                if( ! findbracket( ))  {
                        //dlg_ok( "No Matching Brace !", "Find Brace" );
                        edit_beep();
                        }
                else    {
                        scrn_statsl();
                        scrn_adjust();
                        }
                }
        }



findbracket(  )  {
        char br;
        int x, y, bc, ff = -1;
        char rb;
        struct linerec *ln;
        int fc = 0;

        br = ed->curline->strz[ ed->curcrds.x ];
        switch( br )  {
                case '{' :
                        rb = '}';
                        break;
                case '(' :
                        rb = ')';
                        break;
                case '[' :
                        rb = ']';
                        break;
                case '<' :
                        rb = '>';
                        break;
                case '}' :
                        rb = '{';
                        ff = 0;
                        break;
                case ')' :
                        rb = '(';
                        ff = 0;
                        break;
                case ']' :
                        rb = '[';
                        ff = 0;
                        break;
                case '>' :
                        rb = '<';
                        ff = 0;
                        break;

                default:
                        return(0);
                }

        bc = 1;
        ln = ed->curline;
        y = ed->curcrds.y;
        x = ed->curcrds.x;
        if( ff )  {
                while( 1 )  {
                        if( ! ln->strz[ x ])  {
                                if(( ln = ln->next) == NULL )  return(0);
                                y += 1;
                                x = 0;
                                }
                        else    {
                                x += 1;
                                }
                        if( ln->strz[ x ] == br )  bc += 1;
                        if( ln->strz[ x ] == rb )  bc -= 1;
                        if( ! bc )  break;
                        }
                }
        else    {
                while( 1 )  {
                        if( x == 0 )  {
                                if(( ln = ln->prev) == NULL )  return(0);
                                y -= 1;
                                x = strlen( ln->strz );
                                }
                        else    {
                                x -= 1;
                                }
                        if( ln->strz[ x ] == br )  bc += 1;
                        if( ln->strz[ x ] == rb )  bc -= 1;
                        if( ! bc )  break;
                        }
                }
        ed->curline = ln;
        ed->curcrds.x = x;
        ed->curcrds.y = y;
        scrn_offset();
        return(-1);
        }


find_bracket_c( int *xx, int *yy, struct linerec **lnb  )  {
    char br;
    int x, y, bc, n, ff = -1;
    char rb;
    struct linerec *ln;
    char *s = NULL;
    int fc = 0;

    br = (*lnb)->strz[ *xx ];
    switch( br )  {
        case '{' :
                rb = '}';
                break;
        case '(' :
                rb = ')';
                break;
        case '[' :
                rb = ']';
                break;
        case '<' :
                rb = '>';
                break;
        case '}' :
                rb = '{';
                ff = 0;
                break;
        case ')' :
                rb = '(';
                ff = 0;
                break;
        case ']' :
                rb = '[';
                ff = 0;
                break;
        case '>' :
                rb = '<';
                ff = 0;
                break;

        default:
                return(0);
    }

    bc = 1;
    ln = *lnb;
    y = *yy;
    x = *xx;
    if( ff )  {
        s = ln->strz;
        if( ln->cmnt & 1 )
            fc = -1;
        else
            fc = 0;
        while( *s )  {      // first check if the open bracket is not within a comment
            if( fc )  {
                char *p;

                if(( p = strstr( s, "*/" ))== NULL )  {
                       return 0;
                }
                else  {
                    if( p > &(*lnb)->strz[ *xx ] )  return 0;  
                    fc = 0;
                    s = p + 2;
                    continue;
                }
            }
            if( *s == '/' )  {
                if( *( s + 1 )== '/' )  {
                    return 0;
                }
                else if( *( s + 1 )== '*' )  {
                    char *p;

                    if(( p = strstr( s, "*/" ))== NULL )  {
                        return 0;
                    }
                    else  {
                        if( p > &(*lnb)->strz[ *xx ] )  return 0;  
                        s = p + 2;
                        continue;
                    }
                }
            }
            if( s == &ln->strz[ x ] )
                break;
            ++s;
        }
        bc = 0;
        fc = 0;
        s = &ln->strz[ x ];
        while( 1 )  {
            while( *s )  {
                if( fc )  {
                    char *p;

                    if(( p = strstr( s, "*/" ))== NULL )  {
                        break; 
                    }
                    else  {
                        s = p + 2;
                        fc = 0;
                        continue;
                    }
                }
                if( *s == '/' )  {
                    if( *( s + 1 )== '/' )  {
                        break;
                    }
                    else if( *( s + 1 )== '*' )  {
                        char *p;

                        if(( p = strstr( s, "*/" ))== NULL )  {
                            fc = -1;
                            break;
                        }
                        else  {
                            s = p + 2;
                            continue;
                        }
                    }
                }
                if( *s == br )  bc += 1;
                else if( *s == rb )  bc -= 1;
                if( ! bc )  break;
                ++s;
            }
            if( ! bc )  break;
            if(( ln = ln->next) == NULL )  return(0);
            s = ln->strz;
            y += 1;
        }
    x = s - ln->strz;
    }
    else    {
        int bp[ 200 ];
        char bb[ 200 ];
        int cb;

        bc = 0;
        fc = 0;
        s = ln->strz;
        while( 1 )  {
            cb = 0;
            if( ln->cmnt & 1 )
                fc = -1;
            else
                fc = 0;
            while( *s )  {
                //if(( ln->cmnt & 1 )&&( ln->cmnt & 4 ))  {
                if( fc )  {
                    char *p;
 
                    if(( p = strstr( s, "*/" ))== NULL )  {
                        if( ln == *lnb )  return 0;     // bracket to match inside comment
                        break;
                        //return 0;
                    }
                    else  {
                        if(( ln == *lnb )&&( p > &(*lnb)->strz[ *xx ] ))  return 0;  // bracket to match inside comment
                        fc = 0;
                        s = p + 2;
                        continue;
                    }
                }
                /*else if( ln->cmnt & 1 )  {
                    if( ln == *lnb )  return 0;     // bracket to match inside comment
                    break;
                }*/
                if( *s == '/' )  {
                    if( *( s + 1 )== '/' )  {
                        if( ln == *lnb )  return 0;     // bracket to match inside comment
                        break;
                    }
                    else if( *( s + 1 )== '*' )  {
                        char *p;

                        if(( p = strstr( s, "*/" ))== NULL )  {
                            break;      // ?????????????????????
                        }
                        else  {
                            if(( ln == *lnb )&&( p > &(*lnb)->strz[ *xx ] ))  return 0;  // bracket to match inside comment
                            s = p + 2;
                            continue;
                        }
                    }
                }
                else if( *s == br )  {
                    x = s - ln->strz;
                    bp[ cb ] = x;
                    bb[ cb ] = 1;
                    if( cb == 199 )  return 0;
                    cb += 1;
                    if(( ln == *lnb )&&( x == *xx ))
                        break;
                }
                else if( *s == rb )  {
                    x = s - ln->strz;
                    bp[ cb ] = x;
                    bb[ cb ] = -1;
                    if( cb == 199 )  return 0;
                    cb += 1;
                }
            ++s;
            }
            for( n = cb; n > 0;  --n )  {
                bc += bb[ n - 1 ];
                if( ! bc )  {
                    x = bp[ n - 1 ];
                    break;
                }
            }

            if( ! bc )  break;
            if(( ln = ln->prev) == NULL )  return(0);
            s = ln->strz;
            y -= 1;
        }
    }
    *xx = x;
    *yy = y;
    *lnb = ln;
    return(-1);
}


find_bracket_sql( int *xx, int *yy, struct linerec **lnb  )  {
    char br;
    int x, y, bc, n, ff = -1;
    char rb;
    struct linerec *ln;
    char *s = NULL;
    int fc = 0;

    br = (*lnb)->strz[ *xx ];
    switch( br )  {
        case '{' :
                rb = '}';
                break;
        case '(' :
                rb = ')';
                break;
        case '[' :
                rb = ']';
                break;
        case '<' :
                rb = '>';
                break;
        case '}' :
                rb = '{';
                ff = 0;
                break;
        case ')' :
                rb = '(';
                ff = 0;
                break;
        case ']' :
                rb = '[';
                ff = 0;
                break;
        case '>' :
                rb = '<';
                ff = 0;
                break;

        default:
                return(0);
    }

    bc = 1;
    ln = *lnb;
    y = *yy;
    x = *xx;
    if( ff )  {
        bc = 0;
        s = &ln->strz[ x ];
        while( 1 )  {
            while( *s )  {
                if( fc )  {
                    char *p;

                    if(( p = strstr( s, "*/" ))== NULL )  {
                        break; 
                    }
                    else  {
                        s = p + 2;
                        fc = 0;
                        continue;
                    }
                }
                if( *s == '-' )  { 
                    if( *( s + 1 )== '-' )  {
                        break;
                        }
                    }
                else if( *s == '/' )  {
                    if( *( s + 1 )== '*' )  {
                        char *p;

                        if(( p = strstr( s, "*/" ))== NULL )  {
                            fc = -1;
                            break;
                        }
                        else  {
                            s = p + 2;
                            continue;
                        }
                    }
                }
                if( *s == br )  bc += 1;
                else if( *s == rb )  bc -= 1;
                if( ! bc )  break;
                ++s;
            }
            if( ! bc )  break;
            if(( ln = ln->next) == NULL )  return(0);
            s = ln->strz;
            y += 1;
        }
    x = s - ln->strz;
    }
    else    {
        char bp[ 200 ];
        char bb[ 200 ];
        int cb;

        bc = 0;
        fc = 0;
        s = ln->strz;
        while( 1 )  {
            cb = 0;
            while( *s )  {
                if(( ln->cmnt & 1 )&&( ln->cmnt & 4 ))  {
                    char *p;
 
                    if(( p = strstr( s, "*/" ))== NULL )  {
                           //  ????
                    }
                    else  {
                        s = p + 2;
                    }
                }
                else if( ln->cmnt & 1 )  {
                    break;
                }
                if( *s == '-' )  {
                    if( *( s + 1 )== '-' )  {
                        break;
                    }
                }
                else if( *s == '/' )  {
                    if( *( s + 1 )== '*' )  {
                        char *p;

                        if(( p = strstr( s, "*/" ))== NULL )  {
                            break;      // ?????????????????????
                        }
                        else  {
                            s = p + 2;
                            continue;
                        }
                    }
                }
                else if( *s == br )  {
                    x = s - ln->strz;
                    bp[ cb ] = x;
                    bb[ cb ] = 1;
                    if( cb == 199 )  return 0;
                    cb += 1;
                    if(( ln == *lnb )&&( x == *xx ))
                        break;
                }
                else if( *s == rb )  {
                    x = s - ln->strz;
                    bp[ cb ] = x;
                    bb[ cb ] = -1;
                    if( cb == 199 )  return 0;
                    cb += 1;
                }
            ++s;
            }
            for( n = cb; n > 0;  --n )  {
                bc += bb[ n - 1 ];
                if( ! bc )  {
                    x = bp[ n - 1 ];
                    break;
                }
            }

            if( ! bc )  break;
            if(( ln = ln->prev) == NULL )  return(0);
            s = ln->strz;
            y -= 1;
        }
    }
    *xx = x;
    *yy = y;
    *lnb = ln;
    return(-1);
}


