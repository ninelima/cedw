
//#include "stdafx.h"



#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "malloc.h"


#include "String2.h"

#ifdef __cplusplus 
extern "C"{ 
int edit_err( int errno );
}
#endif   



String::String() {

    ln = 0;
    if(( bfr = ( char *)malloc( 1 ))== NULL )  return;
    bfr[ 0 ] = '\0';
}

String::String( const char *s )  {

    ln = ::strlen( s );
    if(( bfr = ( char *)malloc( ln + 1 ))== NULL )  return;
    strcpy( bfr, s );
}

String::String( const String &str )  {

    ln = str.ln;
    if(( bfr = ( char *)malloc( ln + 1 ))== NULL )  return;
    if( ln )
        strcpy( bfr, str.bfr );
    else
        bfr[ 0 ] = '\0';
}

#ifdef STRING2_BSTR_SUPPORT
String::String( const BSTR &bstr )  {

    if( bstr != NULL )  {
        ln = wcstombs(NULL, bstr, SysStringLen( bstr ));
        if(( bfr = ( char *)malloc( ln + 1 ))== NULL )  return;
        wcstombs( bfr, bstr, ln );
        bfr[ ln ] = '\0';
    }
}
#endif

String::~String()  {
    free( bfr );
}


String &String::operator=( const String &str )  {

    if ( &str == this )  return *this;
    ln = str.ln;
    if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  {
        edit_err(1);
        return( *this );
    }
    if( ln )
        strcpy( bfr, str.bfr );
    else
        bfr[ 0 ] = '\0';
    return *this;
}

String &String::operator=( const char *s )  {

    ln = ::strlen( s );
    if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  {
        edit_err(1);
        return( *this );
    }
    strcpy( bfr, s );
    return *this;
}

#ifdef STRING2_BSTR_SUPPORT
String &String::operator=( const BSTR &bstr )  {

    if( bstr != NULL )  {
        ln = wcstombs( NULL, bstr, SysStringLen( bstr ) );
        if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  {
            edit_err(1);
            return( *this );
        }
        wcstombs( bfr, bstr, ln );
        bfr[ ln ] = '\0';
    }
    return *this;
}
#endif


/*String &String::operator+=( const String &str )  {

    ln += str.ln;
    bfr = ( char *)realloc( bfr, ln + 1 );
    strcat( bfr, str.bfr );
    return *this;
}*/

String &String::operator+=( const char *s )  {

    ln += ::strlen( s );
    if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  {
        edit_err(1);
        return( *this );
    }
    strcat( bfr, s );
    return *this;
}


int String::operator==( const char *s )  {

    if( bfr == NULL )  return 0;
    if( strcmp( bfr, s )== 0 )  return -1;
    return 0;
}

/*int String::operator==( const String &str )  {

    if( strcmp( bfr, str.buf() )== 0 )  return -1;
    return 0;
}*/


int String::operator!=( const char *s )  {

    if( bfr == NULL )  return -1;
    if( strcmp( bfr, s )== 0 )  return 0;
    return -1;
}

/*int String::operator!=( const String &str )  {

    if( strcmp( bfr, str.buf() )== 0 )  return 0;
    return -1;
}*/


char String::operator[]( int i ) const {

    if( bfr == NULL )  return 0;
    if(( i > 0 )&&( i < ln ))
        return( bfr[ i ]);
    else
        return( '\0' );
}


char *String::buf( int l )  {
 
    ln = l;
    if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  {
        edit_err(1);
        return( NULL );
    }
    bfr[ ln ] = 0;
    return bfr;
}


void String::val( const char *s )  {

    ln = ::strlen( s );
    if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  {
        edit_err(1);
        return;
    }
    strcpy( bfr, s );
}

void String::val( const char *s, int n )  {

    ln = ::strlen( s );
    if( n < ln )
        ln = n;
    if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  {
        edit_err(1);
        return;
    }
    strncpy( bfr, s, ln );
    bfr[ ln ] = 0;
}


void String::set( char c )  {

    if( bfr == NULL )  return;
    strset( bfr, c );
}

void String::set( int i, char c )  {       // ambigeous ??

    if( bfr == NULL )  return;
    if( i >= ln )  return;
    bfr[ i ] = c;
}

void String::set( int i, char c, int n )  {

    if( bfr == NULL )  return;
    if( i >= ln )  return;
    strnset( &bfr[ i ], c, n );
    /*if(( i + n ) > ln )
        n = ln - i;
    for( int j = 0; j < n; ++j )
        bfr[ i++ ] = c;*/
}


void String::cpy( char *s ) const {
 
    if( bfr == NULL )  return;
    strcpy( s, bfr );
}

void String::cpy( char *s, int n ) const {
 
    if( bfr == NULL )  return;
    if( n > ln )
        n = ln;
    strncpy( s, bfr, n );
}

char *String::cpy() const {
    char *s;

    if( bfr == NULL )  return NULL;
    if(( s = ( char *)malloc( ln + 1 ))== NULL )  {
        edit_err(1);
        return( NULL );
    }
    strcpy( s, bfr );
    return s;
}


int String::del( )  {

    if(( bfr = ( char *)realloc( bfr, 1 ))== NULL )  return( edit_err(1) );
    bfr[ 0 ] = '\0';
    ln = 0;
    return ln;
}

int String::del( int i, int n )  {
 
    if( bfr == NULL )  return 0;
    if( i < ln )  {
        strcpy( bfr + i, bfr + i + n );
        ln = ::strlen( bfr );
        if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  return( edit_err(1) );
    }
    return ln;
}


int String::ins( int i, const char *s )  {
    char *t;
    int l;

    if( i > ln )  return ln;
    l = ::strlen( s );
    if(( bfr = ( char *)realloc( bfr, ln + l + 1 ))== NULL )  return( edit_err(1) );
    t = new char[ ln - i + 1 ];
    ::strcpy( t, bfr + i );
    *( bfr + i ) = '\0';
    ::strcat( bfr, s );
    ::strcat( bfr, t );
    delete [] t;
    ln += l;
    return ln;
}

int String::ins( int i, const char *s, int n )  {
    char *t;
    int l;

    if( i > ln )  return ln;
    l = ::strlen( s );
    if( n < l )
        l = n;
    if(( bfr = ( char *)realloc( bfr, ln + l + 1 ))== NULL )  return( edit_err(1) );
    t = new char[ ln - i + 1 ];
    ::strcpy( t, bfr + i );
    *( bfr + i ) = '\0';
    ::strcat( bfr, s );
    ::strcat( bfr, t );
    delete [] t;
    ln += l;
    return ln;
}

int String::ins( int i, char c, int n )  {
    char *t;

    if( i > ln )  return ln;
    if(( bfr = ( char *)realloc( bfr, ln + n + 1 ))== NULL )  return( edit_err(1) );
    t = new char[ ln - i + 1 ];
    ::strcpy( t, bfr + i );
    for( int j = 0; j < n; ++j )
        bfr[ i++ ] = c;
    bfr[ i ] = '\0';
    ::strcat( bfr, t );
    delete [] t;
    ln += n;
    return ln;
}


int String::cat( const char *s )  {

    ln += ::strlen( s );
    if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  return( edit_err(1) );
    strcat( bfr, s );
    return ln;
}

int String::cat( const char *s, int n )  {
    int l;

    l = ::strlen( s );
    if( n > l )
        n = l;
    ln += n;
    if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  return( edit_err(1) );
    strncat( bfr, s, n );
    return ln;
}


int String::replace( char *f, char *r )  {
    char *p;
    int i, l;

    if( bfr == NULL )  return 0;
    l = ::strlen( r ) - ::strlen( f );
    while(( p = strstr( bfr, f )) != NULL )  {
        i = p - bfr;
        ln += l;
        if( l > 0 )
            if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  return( edit_err(1) );
        del( i, ::strlen( f ));
        ins( i, r );
        if( l < 0 )
            if(( bfr = ( char *)realloc( bfr, ln + 1 ))== NULL )  return( edit_err(1) );
    }
    return ln;
}


/*void String::lwr()  {
 
    if( bfr == NULL )  return;
    strlwr( bfr );
}*/


/*int String::find( char *s, int i ) const {

}*/

int String::chr( char c )  {
    char *s;

    if( bfr == NULL )  return -1;
    if(( s = strchr( bfr, c ) )!= NULL )
        return( s - bfr );
    else
        return -1;
}



int String::cmp( const char *s )  {

    if( bfr == NULL )  return 0;
    return( ::strcmp( bfr, s ));
    }

/*int String::cmp( const String &str )  {

    return( ::strcmp( bfr, str.buf() ));
}*/

int String::cmp( const char *s, int n )  {

    if( bfr == NULL )  return 0;
    return( ::strncmp( bfr, s, n ));
    }

int String::icmp( const char *s )  {

    if( bfr == NULL )  return 0;
    return( ::stricmp( bfr, s ));
    }

/*int String::icmp( const String &str )  {

    return( ::stricmp( bfr, str.buf() ));
}*/

int String::icmp( const char *s, int n )  {

    if( bfr == NULL )  return 0;
    return( ::strnicmp( bfr, s, n ));
    }



String String::sub( int i, int n ) const  {
    String str;
    int l;

    if( bfr == NULL )  return   str;
    if( i >= ln )  return str;
    //l = ::strlen( bfr + i );
    l = ln - i;
    if( n < l ) l = n;
    str.buf( l );
    strncpy( str.buf(), bfr + i, l );
    return str;     // calls copy constructor
}



int String::format( const char *fmt, ...)  {
    va_list arg;
    char tmp[ 2000 ];                       // malloc ????

    va_start( arg, fmt );

    ln = _vsnprintf( tmp, 2000, fmt, arg ); // get the size max 2000 - there must be a better way !
    bfr = ( char *)realloc( bfr, ln + 1 );
    vsprintf( bfr, fmt, arg );
    va_end( arg );
    return ln;
}


