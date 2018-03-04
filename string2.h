

/* String class

    Function naming convention:

        val:    assign new value to the string

        set:    sets the value of the string overwriting its existing value

        ins:    insert char(s) to the existing string

        cat:    adds char(s) to the end of the existing string

        cpy:    extracts a copy of the string

        buf:    related to internal buffer

    Most functions are based on the original crt str functions, buffer management is with malloc and free.

    !!!! Must still add malloc error checks !!!!

    There is no direct support for extracting a BSTR, but can be accomplished with an intermediate _bstr_t
    COM support class defined in COMDEF.H:

    _bstr_t( (char *)String ).copy()

*/

//#define STRING2_BSTR_SUPPORT   // comment out if no BSTR support is required

#ifdef STRING2_BSTR_SUPPORT
#include <atlbase.h>
#endif

#if !defined(AFX_STRING_H__9B9128E1_F8BF_11D3_9506_444553540000__INCLUDED_)
#define AFX_STRING_H__9B9128E1_F8BF_11D3_9506_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class String
{
public:
    String();
    String( const char *s );                            // constructs with char *.
    String( const String &str );                        // Copy constructor
    virtual ~String();
#ifdef STRING2_BSTR_SUPPORT
    String( const BSTR &bstr );                         // constructs with BSTR.

    String & operator=( const BSTR &bstr );             // assigns new to copy from BSTR, and returns String.
#endif

    String & operator=( const String &str );            // assigns new to copy from String, and returns String, checks for self-assignment!
    String & operator=( const char *s );                // assigns new to copy from char *s, and returns String.

    //String & operator+=( const String &str );           // concatenates String at the end (strcat).
    String & operator+=( const char *s );               // concatenates s at the end (strcat).

    //int operator==( const String &str );                // returns TRUE or FALSE from case sensitive equality String compare (strcmp).
    int operator==( const char *s );                    // returns TRUE or FALSE from case sensitive equality char * compare (strcmp).
    //int operator!=( const String &str );                // returns TRUE or FALSE from case sensitive inequality String compare (strcmp).
    int operator!=( const char *s );                    // returns TRUE or FALSE from case sensitive inequality char * compare (strcmp).

    char operator[]( int i )  const;                    // returns character at index.

    //operator char*() const;   { return bfr; };        // same as buf, but not obvious enough to be safe.
    operator const char*() const  { return bfr; };

    const char *strz( ) const  { return bfr; };          // returns const char * to the string, probably the best way to reference the string.
    //const char *str( int i ) const  { return &bfr[ i ]; };  !!! check i > ln

    char *buf() const { return bfr; };                  // returns char* to internal buffer, dangerous but one should realise that this is the internal buffer.
    char *buf( int l );                                 // adjusts the size of the buffer to length l, and returns char* to internal buffer.

    int len() const { return ln; }                     // returns length of the string.

    void val( const char *s );                          // assigns new to copy from s (strcpy), the same as = operator but returns void.
    void val( const char *s, int n );                   // assigns new to copy of n chars from s (strncpy).

    int ins( int i, const char *s );                    // inserts copy from s at index i.
    int ins( int i, const char *s, int n );
    int ins( int i, char c, int n );                    // inserts n chars of c starting at index i

    //!!void set( int i, char *s );
    //!!void set( int i, char *s, int n );
    void set( char c );
    void set( int i, char c );                          // set the char at index i to c within length of String.
    void set( int i, char c, int n );                   // set n chars at index i to c within length of String.

    int String::cat( const char *s );
    int String::cat( const char *s, int n );

    void cpy( char *s ) const;                          // copies the string to s (strcpy), s must be big enough.
    void cpy( char *s, int n ) const;
    char *cpy() const;                                  // returns newly allocated char* copy of string (strcpy), that must be free'd afterwards.

    int del();
    int del( int i, int n );                            // deletes n chars starting at index i if within the length of the string, returns the length of the string.

    int replace( char *f, char *r );                    // replace all occurenses of chars f with char r

    String sub( int i, int n ) const;                   // returns String containing n middle chars starting from i, using copy constructor

    int cmp( const char *s );                           // returns strcmp
    //int cmp( const String &str );                       // returns strcmp
    int cmp( const char *s, int n );                    // returns strncmp
    int icmp( const char *s );                          // returns stricmp
    //int icmp( const String &str );                      // returns stricmp
    int icmp( const char *s, int n );                   // returns strnicmp
    
    int chr( char c );
    //int chr( int i, char c );   ????

    //void lwr();
    //void lwr( int i );          ????  to eos or 1 chr only ?
    //void lwr( int i, int n );   ????



    int format( const char *fmt, ... );                       // printf format


private:
    int ln;
    char *bfr;

};

#endif // !defined(AFX_STRING_H__9B9128E1_F8BF_11D3_9506_444553540000__INCLUDED_)
