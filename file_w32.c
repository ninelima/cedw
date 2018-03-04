
#include "windows.h"

#include "ced.h"
#include "string.h"
#include "stdio.h"

extern struct editrec *ed;
extern struct coords cursor;
extern struct linerec *blckline;
extern struct coords blckstr, blckend;
//extern int tablen;
extern char *editbufr;


file_exist( char *fn, struct ftimerec *ft )  {         /* returns: 0 = error,  1 = exist,  -1 = not exist */
        HANDLE hF;
        DWORD err;
        FILETIME ftWrite;

        if(( hF = CreateFile( fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL )) == INVALID_HANDLE_VALUE )  {
                err = GetLastError( );
                if( err == ERROR_FILE_NOT_FOUND )  return( -1 );
                file_err( err, fn );
                return( 0 );
                }
        if( ! GetFileTime( hF, NULL, NULL, &ftWrite ))  {
                file_err( GetLastError( ), fn);
                }
        ft->lo = ftWrite.dwLowDateTime;
        ft->hi = ftWrite.dwHighDateTime;
        if( ! CloseHandle( hF ))  {
                file_err( GetLastError( ), fn );
                return( 0 );
                }
        return(1);
        }



file_time( char *fn, struct ftimerec *ft )  {
        HANDLE hF;
        DWORD err;
        FILETIME ftWrite;

        ft->lo = 0;
        ft->hi = 0;
        if(( hF = CreateFile( fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL )) == INVALID_HANDLE_VALUE )  {
                err = GetLastError( );
                if( err == ERROR_FILE_NOT_FOUND )  return( -1 );
                file_err( err, fn );
                return( 0 );
                }
        if( ! GetFileTime( hF, NULL, NULL, &ftWrite ))  {
                file_err( GetLastError( ), fn);
                }
        ft->lo = ftWrite.dwLowDateTime;
        ft->hi = ftWrite.dwHighDateTime;
        if( ! CloseHandle( hF ))  {
                file_err( GetLastError( ), fn );
                return( 0 );
                }
        return(-1);
        }

file_time_noerr( char *fn, struct ftimerec *ft )  {
        HANDLE hF;
        FILETIME ftWrite;

        ft->lo = 0;
        ft->hi = 0;
        if(( hF = CreateFile( fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL )) == INVALID_HANDLE_VALUE )  {
                //err = GetLastError( );
                //if( err == ERROR_FILE_NOT_FOUND )  return( -1 );
                //file_err( err, fn );
                return( 0 );
                }
        if( ! GetFileTime( hF, NULL, NULL, &ftWrite ))  {
                //file_err( GetLastError( ), fn);
                return 0;
                }
        ft->lo = ftWrite.dwLowDateTime;
        ft->hi = ftWrite.dwHighDateTime;
        if( ! CloseHandle( hF ))  {
                //file_err( GetLastError( ), fn );
                //return( 0 );
                }
        return(-1);
        }




file_tcmp( struct ftimerec *ft1, struct ftimerec *ft2 )  {
        /*char xxx[ 40 ];

        wsprintf( xxx, "%lX  %lX  -  %lX  %lX", ft1->lo, ft1->hi, ft2->lo, ft2->hi );
        MessageBox( NULL, xxx, "", MB_OK );*/

        if(( ft1->lo == 0 )&&( ft1->hi == 0 ))  return(-1);
        if(( ft2->lo == 0 )&&( ft2->hi == 0 ))  return(-1);

        if(( ft1->lo != ft2->lo )||( ft1->hi != ft2->hi ))  return( 0 );
        return( -1 );
        }



file_read( char *fn, struct linerec *line, int *linecnt, int *lcrlf, struct ftimerec *ft )  {
        HANDLE hF;
        DWORD err;
        FILETIME ftWrite;

        ft->lo = 0;
        ft->hi = 0;
        if(( hF = CreateFile( fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL )) == INVALID_HANDLE_VALUE )  {
                err = GetLastError( );
                if( err != ERROR_FILE_NOT_FOUND )  {
                        file_err( err, fn );
                        return( 0 );
                        }
                if(( hF = CreateFile( fn, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL )) == INVALID_HANDLE_VALUE )  {
                        file_err( GetLastError( ), fn );
                        return( 0 );
                        }
                }
        else    {
                if( ! getfile(fn, hF, line, linecnt, lcrlf))  {
                        CloseHandle( hF );
                        return( 0 );
                        }
                }
        if( ! GetFileTime( hF, NULL, NULL, &ftWrite ))  {
                file_err( GetLastError( ), fn );
                }
        ft->lo = ftWrite.dwLowDateTime;
        ft->hi = ftWrite.dwHighDateTime;
        if( ! CloseHandle( hF ))  {
                file_err( GetLastError( ), fn );
                return( 0 );
                }

        err = GetFileAttributes( fn );
        if( err == 0xFFFFFFFF )  {
                file_err( GetLastError( ), fn );
                return( 0 );
                }
        if( err & FILE_ATTRIBUTE_READONLY )
                ed->changef = 1;

        return(-1);
        }


/*
long WINAPI NotifyThread( long lParam )  {
        struct editrec *ed;
        HANDLE hChange;

        ed = ( struct editrec *)lParam;

        //MessageBox( NULL, ed->fname, "", MB_OK );

        hChange = FindFirstChangeNotification ((LPCTSTR) "C:\\\\Projects\\ced7\\1.c", FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);

        if (hChange == INVALID_HANDLE_VALUE)
                return 1;

        MessageBeep(-1);

        while( 1 )  {
                if( WaitForSingleObject( hChange, INFINITE ))  {
                        MessageBeep(-1);
                        }
                else
                        break;
                }
        FindCloseChangeNotification (hChange);
        return 0;
        }




file_notify( struct editrec *ed )  {
        int iID;
        HANDLE hThread;

        hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)NotifyThread, ( LPVOID )ed, 0, &iID );
}
*/



file_write( char *fn, struct linerec *line, int linecnt, int lcrlf, struct ftimerec *ft  )  {
        HANDLE hF;

        //ft->lo = 0;
        //ft->hi = 0;
        if(( hF = CreateFile( fn, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL )) == INVALID_HANDLE_VALUE )  {
                file_err( GetLastError( ), fn );
                return(0);
                }
        if( ! putfile(fn, hF, line, linecnt, lcrlf))  {
                CloseHandle( hF );
                return(0);
                }
        if( ! CloseHandle( hF ))  {
                file_err( GetLastError( ), fn );
                return( 0 );
                }
        file_time( fn, ft );
        return(-1);
        }



getfile( char *fn, HANDLE hF, struct linerec *line, int *linecnt, int *lcrlf)  {
        unsigned linepos;
        DWORD bufrlen;
        int crlf;

        crlf = 0;
        linepos = 0;
        *linecnt = 0;
        while (1)  {
                if( ! ReadFile( hF, (char far *)editbufr, CE_LEN_FILEBUFR, &bufrlen, NULL ))  {
                        file_err( GetLastError( ), fn );
                        return(0);
                        }
                if( bufrlen == 0 )  break;
                if( ! edit_get( &line, linecnt, &linepos, &crlf, editbufr, bufrlen ))  {
                        return(0);
                        }
                }
        line->strz[linepos] = '\0';
        ++(*linecnt);
        if( crlf )  *lcrlf = crlf;
        return(-1);
        }



putfile( char *fn, HANDLE hF, struct linerec *line, int linecnt, int lcrlf )  {
        unsigned linelen, linepos, lcnt;
        DWORD bufrpos, bufrcnt;

        lcnt = 0;
        linepos = 0;
        bufrpos = 0;
        linelen = strlen(line->strz);
        while (line)  {
                while (bufrpos != CE_LEN_FILEBUFR)  {
                        if (linepos == linelen)  {
                                if( lcrlf == 0x03 )  {
                                        editbufr[bufrpos++] = CE_KEY_CR;
                                        if (bufrpos == CE_LEN_FILEBUFR)  {
                                                if( ! WriteFile( hF, (char far *)editbufr, bufrpos, &bufrcnt, NULL ))  {
                                                        file_err( GetLastError( ), fn );
                                                        return(0);
                                                        }
                                                if (bufrcnt != bufrpos)  {
                                                        edit_err(3);   // weird err ?
                                                        return(0);
                                                        }
                                                bufrpos = 0;
                                                }
                                        }
                                editbufr[bufrpos++] = CE_KEY_LF;
                                if (! (line = line->next))  break;
                                if (((line->next == NULL) && (line->strz[0] == '\0')) || (++lcnt == linecnt))  {
                                        if( ! WriteFile( hF, (char far *)editbufr, bufrpos, &bufrcnt, NULL ))  {
                                                file_err( GetLastError( ), fn );
                                                return(0);
                                                }
                                        if (bufrcnt != bufrpos)  {
                                                edit_err(3);   // weird err ?
                                                return(0);
                                                }
                                        return(-1);
                                        }
                                linepos = 0;
                                linelen = strlen(line->strz);
                                }
                        else
                                editbufr[bufrpos++] = line->strz[linepos++];
                        }
                if( ! WriteFile( hF, (char far *)editbufr, bufrpos, &bufrcnt, NULL ))  {
                        file_err( GetLastError( ), fn );
                        return(0);
                        }
                if (bufrcnt != bufrpos)  {
                        edit_err(3);   // weird err ?
                        return(0);
                        }
                bufrpos = 0;
                }
        return(-1);
        }



file_nxpand( char *fn )  {
        WIN32_FIND_DATA find;
        int fc, l, n;
        char sn[CE_LEN_FILEPATH], tn[CE_LEN_FILEPATH];
        HANDLE hF;

        l = strlen(fn);
        tn[ 0 ] = '\0';
        for (n = l; n > 0; --n)  {
                if ((fn[n-1] == '\\') || (fn[n-1] == ':'))  {
                        strncpy( tn, fn, n);
                        tn[ n ] = '\0';
                        break;
                        }
                }

        strcpy( sn, fn );
        strcat( sn, "*" );
        fc = 0;

        if(( hF = FindFirstFile( sn, &find )) == INVALID_HANDLE_VALUE )  {
                edit_beep();
                }

        else    {
                do      {
                        fc += 1;
                        }  while( FindNextFile( hF, &find ));
                }
        if( fc == 1 )  {
                strcpy( fn, tn );
                //if( strchr( find.cFileName, ' ' ) == NULL )  {
                        strcat( fn, find.cFileName );
                        //}
                /*else    {
                        strcat( fn, find.cAlternateFileName );
                        }*/
                //strupr( fn );
                if( find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )  strcat( fn, "\\" );
                return( -1 );
                }
        return( 0 );
        }


file_nxplode( char *fn, char *pn, char fns[CE_MAX_FILES+1][CE_LEN_FILENAME] )  {
        int wc, fc, n, l;
        WIN32_FIND_DATA find;
        DWORD err;
        HANDLE hF;

        wc = 0;
        fc = 0;
        l = strlen(fn);
        for (n = l; n > 0; --n)  {
                if ((fn[n-1] == '*') || (fn[n-1] == '?'))  wc = -1;
                if ((fn[n-1] == '\\') || (fn[n-1] == ':'))  break;
                }

        if( n > 0 )  {
                strncpy( pn, fn, n );
                pn[ n ] = '\0';
                }
        else    {
                file_getcwd(pn);
                }
        if( ! wc )  {
                strcpy(fns[0], &fn[ n ]);
                //strupc(fns[0]);
                fc = 1;
                }
        else    {
                //edit_err( 4 );  // no wildcards yet
 
                fc = 0;
                if(( hF = FindFirstFile( fn, &find )) == INVALID_HANDLE_VALUE )  {
                        file_err( GetLastError( ), fn );
                        }
                else    {
                        do      {
                                if( !( find.dwFileAttributes &= FILE_ATTRIBUTE_DIRECTORY ))  {
                                        strcpy( fns[fc], find.cFileName );
                                        if( fc++ == CE_MAX_FILES )  break;
                                        }
                                }  while( FindNextFile( hF, &find ));

                        if( err = GetLastError( ) != ERROR_NO_MORE_FILES )  {
                                file_err( err, fn  );
                                }
                        if( ! FindClose( hF ))  {
                                file_err( GetLastError( ), fn );
                                }
                        }
                }
        return(fc);
        }


file_getcwd( char *dn )  {

        if( ! GetCurrentDirectory( CE_LEN_FILEPATH, dn ))  file_err( GetLastError( ), dn );
        if (dn[strlen(dn)-1] != '\\')  strcat(dn, "\\");
        }


file_setcwd( char *dn )  {

        if( ! SetCurrentDirectory( dn ))  return( file_err( GetLastError( ), dn ));
        return -1;
        }


char* file_getfname( char *fn )  {
        int n, l;

        l = strlen(fn);
        for (n = l; n > 0; --n)  {
                if ((fn[n-1] == '\\') || (fn[n-1] == ':'))  {
                        return( &fn[ n ] );
                        }
                }
        return( fn );
        }