

#include "stdio.h"
#include "string.h"

#include "ced.h"

TreeItemSel( long, int );    // !!!!

extern struct editrec *edfirst;
extern struct editrec *ed;
extern int edcurno;
extern int edcount;

extern int scrn_x;
extern int scrn_y;

extern int tablen;
extern int fileos;


main_flags(argc, argv)
int argc;
char **argv;
        {
        int a;

        for (a = 0; a < argc; ++a)  {
                if(( *argv[a] == '/' )||( *argv[a] == '-' ))  {
                        if( strcmp( &argv[a][1], "unix" ) == 0)  {
                                fileos = 0x02;
                                }
                        if( strncmp( &argv[a][1], "cols=", 5 ) == 0)  {
                                scrn_x = atoi( &argv[a][6] );
                                }
                        if( strncmp( &argv[a][1], "lines=", 6 ) == 0)  {
                                scrn_y = atoi( &argv[a][7] );
                                }
                        if( strncmp( &argv[a][1], "tablen=", 7 ) == 0)  {
                                tablen = atoi( &argv[a][8] );
                                }
                        }
                }
        }


main_open(argc, argv)
int argc;
char *argv[];

        {
        int a;

        for (a = 0; a < argc; ++a)  {
                if( ! (( *argv[a] == '/' )||( *argv[a] == '-' )))  {
                        file_open( argv[a] );
                        }
                }
        //ed = edfirst;
        //edcurno = 1;
        //fmru_current( edcurno );
        }


/*
ctrlkey(c)
int c;
        {
        if (c >= 32 && c < 127)  return(0);
        else  return(1);
        }
*/

chrkey(c)       /* check for ASCII key */
int c;
        {
        if (c >= 32 && c < 127)  return(1);
        else  return(0);
        }


altakey(c)      /* check for ALT-Alpha key */
int c;
        {
        if (c >= 0xC1 && c <= 0xDA)  return(1);
        else  return(0);
        }

/*int isalpha(c)
int c;
        {
        if ((c >= 'A') && (c <= 'Z'))  return(-1);
        if ((c >= 'a') && (c <= 'z'))  return(-1);
        return(0);
        }*/


int isalphau(c)
int c;
        {
        if ((c >= 'A') && (c <= 'Z'))  return(-1);
        if ((c >= 'a') && (c <= 'z'))  return(-1);
        if( c == '_' )  return(-1);
        return(0);
        }

int isalphan(c)
int c;
        {
        if ((c >= 'A') && (c <= 'Z'))  return(-1);
        if ((c >= 'a') && (c <= 'z'))  return(-1);
        if ((c >= '0') && (c <= '9'))  return(-1);
        return(0);
        }

int isalnumu(c)
int c;
        {
        if ((c >= 'A') && (c <= 'Z'))  return(-1);
        if ((c >= 'a') && (c <= 'z'))  return(-1);
        if ((c >= '0') && (c <= '9'))  return(-1);
        if( c == '_' )  return(-1);
        return(0);
        }

int ishexdigit(c)
int c;
        {
        if ((c >= '0') && (c <= '9'))  return(-1);
        if ((c >= 'A') && (c <= 'F'))  return(-1);
        if ((c >= 'a') && (c <= 'f'))  return(-1);
        return(0);
        }


int isoctdigit(c)
int c;
        {
        if ((c >= '0') && (c <= '7'))  return(-1);
        return(0);
        }


strinsc(str, chr, pos)          /* insert one char in a string */
char *str;
char chr;
int pos;
        {
        char tc;

        do  {
                tc =str[pos];
                str[pos] = chr;
                chr = tc;
                pos += 1;
                }  while (tc != '\0');
        str[pos] = tc;
        }


strdel(s, x, c)         /* delete number of characters from a string */
char s[];
int x, c;
        {
        strcpy(&s[x], &s[x+c]);
        return(strlen(s));
        }


strcut(d, s, x, c)         /* cut number of characters from source to dest string */
char d[], s[];
int x, c;
        {
        int n;

        for (n = 0; n < c; ++n)  d[n] = s[x + n];
        d[n] = '\0';
        strcpy(&s[x], &s[x+c]);
        return(strlen(s));
        }



strchra(s)      /* returns position of first non-space char in a string */
char s[];
        {
        int c;

#ifdef _DEBUG
        if( s == NULL )  {
                dlg_debug( __FILE__, __LINE__, "'strchra' NULL pointer" );
                return(0);
                }
#endif
        for (c = 0; s[c] == CE_KEY_SPACE; ++c)  ;
        if (s[c] == '\0')
                return(-1);
        else
                return(c);
        }


strslb(s)       /* strip leading blanks from a string */
char s[];
        {
        int c, d;

        if ((c = strchra(s)) == -1)  {
                s[0] = '\0';
                return(0);
                }
        for (d = 0; s[d++] = s[c++]; )  ;
        return(-1);
        }


strstb(s)       /* strip trailing blanks from a string */
char s[];
        {
        int c;

#ifdef _DEBUG
        if( s == NULL )  {
                dlg_debug( __FILE__, __LINE__, "'strstb' NULL pointer" );
                return(0);
                }
#endif
        // !!!!!!!!!!!!! Remove strlen from loop !!!!!!!!!!!!!!!!
 
        if (s[0] == '\0')  return(0);
        for (c = strlen(s) - 1; c; --c)  if( s[c] != CE_KEY_SPACE )  break;
        s[++c] = '\0';
        return(c);
        }


strupc(s)
char *s;
        {
#ifdef _DEBUG
        if( s == NULL )  {
                dlg_debug( __FILE__, __LINE__, "'strupc' NULL pointer" );
                return(0);
                }
#endif
        while( *s )  {
                if( isalpha( *s )) *s &= 0xDF;
                ++s;
                }
        }


strdnc(s)
char *s;
        {
#ifdef _DEBUG
        if( s == NULL )  {
                dlg_debug( __FILE__, __LINE__, "'strdnc' NULL pointer" );
                return(0);
                }
#endif
        while( *s )  {
                if( isalpha( *s )) *s |= 0x20;
                ++s;
                }
        }

strinc(s)
char *s;
        {
#ifdef _DEBUG
        if( s == NULL )  {
                dlg_debug( __FILE__, __LINE__, "'strinc' NULL pointer" );
                return(0);
                }
#endif
        while( *s )  {
                if( isalpha( *s ))  {
                    if( !( *s & 0x20 ))  *s |= 0x20;
                    else  *s &= 0xDF;
                    }
                ++s;
                }
        }


itostr(s, z)
char *s;
int z;
        {
        s[0] = z / 10000 + 48;
        z = z % 10000;
        s[1] = z / 1000 + 48;
        z = z % 1000;
        s[2] = z / 100 + 48;
        z = z % 100;
        s[3] = z / 10 + 48;
        z = z % 10;
        s[4] = z + 48;
        s[5] = 0;
        while(( s[0] == '0' )&&( strlen( s ) > 1 )) strdel( s, 0, 1 );
        }


/*

#ifdef _PM
        pm_main();

#elif  _UNIX
        scrn_x = COLS;
        scrn_y = LINES - 1;
        scrn_ini();
        kb_ini();

#else
        scrn_ini();

#ifdef _OS2
        kb_ini();
#endif
#endif

#ifndef _PM
        edit_ini();
        main_cmdlfiles();
        scrn_statsl();
        scrn_rescrn();

        while (1)  {
                int key;

                key = kb_key();
                if( edit(key))  break;
                }
        scrn_clr();

#ifdef _OS2
        kb_close();
#endif

#ifdef _UNIX
        vdeo_close();
#endif

#endif

*/

