

#include "windows.h"    // !!!! shouldn't actually be used here, but is needed for cedw.h, which should also not be used here
                        //      the split between Windows tree functions and the ced tree functions isn't as clean as it could be

#include "string2.h"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "resource.h"

#ifdef __cplusplus 
extern "C"{ 
#endif   

#include "ced.h"
#include "cedw.h"


extern char asstype[ CE_CNT_ASSTYPE ][ CE_LEN_ASSTYPE ];
extern struct assrec asslst[ CE_MAX_ASSOCIATIONS ];
extern int asscnt;

extern int kwc_c;
extern struct keywordrec kwl_c[ ];
extern int kwc_java;
extern struct keywordrec kwl_java[ ];


extern struct editrec *ed;

extern int cxFileTree;
extern int cxFileTreeBak;
extern int cxSizeBar;
extern int fFileList;
extern int fFileTree;



int isalnumu(int c);
int isalphau(int c);
resword( struct keywordrec kwl[], int kwc, char *s, int wl );



struct recClass {
    String *strName;
    HTREEITEM hTreeItem; 
};



void c_prms( char *szprms )  {
    char c;
    char *q, *prms;
    char s[ 1000 ];

    prms = s;
    q = szprms;
    c = *q;
    while( *q )  {
        if( *q == ' ' )  {
            char c2;

            c2 = *(q + 1);
            if(!(( c == ' ' )||( c == '(' )||( c == ',' )||( c == '[' )||( c2 == '(' )||( c2 == ')' )||( c2 == '[' )||( c2 == ']' )||( c == '*' )||( c == '&' )||( c2 == '/' )))    // ||( c2 == ' ' )
            //if( !(( ! isalphau( c ))||( ! yisalphau( c2 ))))
                *prms++ = *q;
        }
        else if(( *q == '*' )||( *q == '&' ))  {
            if(( c != ' ' )&&( c != '*' ))
                *prms++ = ' ';
            *prms++ = *q;
        }
        else  {
            *prms++ = *q;
        }
        c = *q++;
    }
    *prms = '\0';
    strcpy( szprms, s );
}


c_func( struct linerec **ln, char **s, int *lnc, char *prms )  {
    int bc = 0;
    char *q;
    int fc = 0;
    int c = 0;

    q = *s;
    while( 1 )  {
        while( *q )  {
            if( fc )  {
                char *p;

                if(( p = strstr( q, "*/" ))== NULL )  {
                    break;
                }
                else  {
                    q = p + 2;
                    fc = 0;
                    continue;
                }
            }
            if( *q == '/' )  {
                if( *( q + 1 )== '/' )  {
                    break;
                }
                else if( *( q + 1 )== '*' )  {
                    char *p;

                    if(( p = strstr( q, "*/" ))== NULL )  {
                        fc = -1;
                        break;
                    }
                    else  {
                        q = p + 2;
                        continue;
                    }
                }
            }
            else if( *q == '(' )  ++bc;
            else if( *q == ')' )  {
                if( bc )  {
                    --bc;
                    if( ! bc )  {
                        if( c < 1000 )  {
                            *prms++ = *q;
                            ++c;
                        }
                    }
                }
                else  {
                    *s = q;
                    return 0;
                }
            }
            else if( ! bc )  {
                if( strncmp( q, "const", 5 )== 0 )  {
                    q += 5;
                    continue;
                }
                if( strncmp( q, "throws", 6 )== 0 )  {
                    *s = q;
                    *prms = '\0';
                    return -1;
                }
                if( *q == '{' )  {
                    *s = q;
                    *prms = '\0';
                    return -1;
                }
                if( *q != ' ' )  {
                    *s = q;
                    return 0;
                }
                /*if(( *q == ';' )||( *q == '}' )||( *q == '#' ))  {
                    *s = q;
                    return 0;
                }*/
            }
            else if(( *q == '{' )||( *q == '}' ))  {
                *s = q;
                return 0;
            }
            if( bc )  {
                if( c < 1000 )  {
                    *prms++ = *q;
                    ++c;
                }
            }
            ++q;
        }
        *ln = (*ln)->next;
        if( *ln == NULL )  break;
        (*lnc)++;
        q = (*ln)->strz;     
    }
    *s = q;
    return 0;
}



tree_c( struct editrec *ed, int fupd )  {
    struct linerec *ln;
    char szprms[ 1000 ];     
    int lnc, lnc2;
    int wl;
    char *si, *ws;
    int wordf, spacef, classnf, classf; //cmntf
    int fc = 0;
    HTREEITEM hTreeClass = 0;
    int sqglf;
    char sopr[ 10 ];
    int oprf = 0;
    int copr;
    int pc;
    int classl;
    int classc = 0;
    String strClass, strFunction;
    struct recClass rClasses[ 100 ];    


    HTREEITEM (*TreeItemFunc)( const char * lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine );

    if( fupd )
        TreeItemFunc = TreeItemUpd;
    else
        TreeItemFunc = TreeItemIns;

    if( fupd )
        TreeItemMark2Del( (struct _TREEITEM *)ed->hTreeItem );
    
    ln = ed->firstline;
    lnc = 1;

    pc = 0;
    spacef = 0;
    wordf = 0;
    //cmntf = 0;
    sqglf = 0;
    classf = 0;
    classnf = 0;
    *sopr = 0;
    classl = 0;

    hTreeClass = (struct _TREEITEM *)ed->hTreeItem;
    while( ln != NULL )  {

        si = ln->strz;
        while( *si )  {

            if( fc )  {
                char *p;

                if(( p = strstr( si, "*/" ))== NULL )  {
                    break;
                }
                else  {
                    si = p + 2;
                    fc = 0;
                    continue;
                }
            }

            if(( spacef )||( *si == '{' ))  {
                if( classf )  {
                    //  ???? Check if ws exists anf and not keyword

                    classf = 0;
                    rClasses[ classc ].strName = new String;
                    rClasses[ classc ].strName->val( ws, wl );
                    hTreeClass = TreeItemFunc( *rClasses[ classc ].strName, 3 + classl, 5, hTreeClass, CE_TVI_CLASS, ed, ed->assoc, lnc2 );
                    rClasses[ classc ].hTreeItem = hTreeClass;
                    classl += 1;
                    if( classc < 100 - 1 )
                        classc += 1;
                }
            }
            if( wordf ? isalnumu( *si ) : isalphau( *si ))  {
                if(( wordf )&&( spacef ))  {
                    if( wl == 5 ? !strncmp( ws, "class", wl ) : 0 )
                        classf = -1;
                }
                if(( ! wordf )||( spacef ))  {
                    wordf = -1;
                    ws = si;
                    wl = 1;
                }
                else  {
                    ++wl;
                }
                spacef = 0;
                //oprf = 0;
            }
            else if( *si == '(' )   {
                if( wordf )  {
                    wordf = 0;
                    classf = 0;
                    if(( ! resword( kwl_c, kwc_c, ws, wl ))||(( strncmp( ws, "main", wl ))== 0 )||( oprf ))  {
                        lnc2 = lnc;
                        if( c_func( &ln, &si, &lnc, szprms ))  {
                            if( oprf)  {
                                oprf= 0;
                                strFunction.val( ws, wl );
                                strFunction.cat( sopr, copr );
                                wl += copr;
                            }
                            else if( sqglf )  {
                                strFunction = "~";
                                strFunction.cat( ws, wl );
                                ++wl;
                            }
                            else
                                strFunction.val( ws, wl );
                            c_prms( szprms );
                            strFunction.cat( szprms );

                            if( classnf )  {
                                classnf = 0;

                                for( int n = 0; n < classc; ++n )  {
                                    if( strClass == *rClasses[ n ].strName )  break;
                                }
                                if( n == classc )  {
                                    rClasses[ classc ].strName = new String;
                                    *rClasses[ classc ].strName = strClass;
                                    rClasses[ classc ].hTreeItem = TreeItemFunc( strClass, 3 + classl, 5, hTreeClass, CE_TVI_CLASS, ed, ed->assoc, lnc2 );
                                    if( classc < 100 - 1 )
                                        classc += 1;
                                }
                                TreeItemFunc( strFunction, 4 + classl, 3, rClasses[ n ].hTreeItem, CE_TVI_FUNC, ed, ed->assoc, lnc2 );
                            }
                            else  {
                                TreeItemFunc( strFunction, 3 + classl, 3, hTreeClass, CE_TVI_FUNC, ed, ed->assoc, lnc2 );
                            }
                        }
                        continue;
                    }
                }
            }
            else if( *si == ':' )  {
                wordf = 0;
                spacef = 0;
                sqglf = 0;
                if( *( si + 1 )== ':' )  {
                    if( ! resword( kwl_c, kwc_c, ws, wl ) )  {
                        strClass.val( ws, wl );
                        classnf = -1;
                        si += 2;
                        continue;
                    }
                }
            }
            else if( *si == ' ' )   {
                spacef = -1;
                lnc2 = lnc;
            }
            else if( *si == '{' )  {
                wordf = 0;
                classf = 0;
                classnf = 0;
                spacef = 0;
                sqglf = 0;
                pc += 1;
            }
            else if( *si == '}' )  {
                classf = 0;
                classnf = 0;
                if( pc > 0 )  {
                    if( pc == classl )  {
                        classl -= 1;
                        hTreeClass = TreeGetParent( hTreeClass ); 
                    }
                    pc -= 1;
                }
            }
            else if( *si == '~' )  {
                sqglf = -1;
                classf = 0;
            }
            else if(( *si == '/' )&&( *( si + 1 )== '/' ))  {
                break;
            }
            else if(( *si == '/' )&&( *( si + 1 )== '*' ))  {
                char *p;

                if(( p = strstr( si, "*/" ))== NULL )  {
                    fc = -1;
                    break;
                }
                else  {
                    si = p + 2;
                    continue;
                }
            }
            else if( oprf )  {
                if( copr < 10 )  
                    sopr[ copr++ ] = *si;
                else
                    oprf = 0;       //  ?????
            }
            else if (( wordf )&( classnf ))  {
                if( wl == 8 ? !strncmp( ws, "operator", 8 ) : 0 )  {
                    oprf = -1;
                    sopr[ 0 ] = ' ';
                    sopr[ 1 ] = *si;
                    copr = 2;
                    }
            }
            else  {
                sqglf = 0;
                spacef = 0;
                classnf = 0;
                classf = 0;
                wordf = 0;
            }
            ++si;
        }
        lnc2 = lnc;
        if( ln == NULL )  break;
        ln = ln->next;
        lnc += 1;
        spacef = -1;
    }
    for( int n = 0; n < classc; ++n )  
        delete rClasses[ n ].strName;

    if( fupd )
        TreeItemDelMarked( (struct _TREEITEM *)ed->hTreeItem );
}



tree_java( struct editrec *ed, int fupd )  {
    struct linerec *ln;
    char szprms[ 1000 ];       
    int lnc, lnc2;
    int wl;
    char *si, *ws;
    int wordf, spacef, classf, privf, intrf;
    int classl;
    HTREEITEM hTreeClass = 0;
    int pc;
    int fc = 0;
    String strClass, strFunction;
    HTREEITEM (*TreeItemFunc)( const char * lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine );

    if( fupd )
        TreeItemFunc = TreeItemUpd;
    else
        TreeItemFunc = TreeItemIns;

    if( fupd )
        TreeItemMark2Del( (struct _TREEITEM *)ed->hTreeItem );


    ln = ed->firstline;
    lnc = 1;

    pc = 0;
    spacef = 0;
    wordf = 0;
    privf = 0;
    classf = 0;
    classl = 0;
    intrf = 0;

    hTreeClass = (struct _TREEITEM *)ed->hTreeItem;
    while( ln != NULL )  {

        si = ln->strz;
        while( *si )  {

            if( fc )  {
                char *p;

                if(( p = strstr( si, "*/" ))== NULL )  {
                    break;
                }
                else  {
                    si = p + 2;
                    fc = 0;
                    continue;
                }
            }
            
            if(( spacef )||( *si == '{' ))  {
                if( classf )  {
                    classf = 0;
                    strClass.val( ws, wl );
                    hTreeClass = TreeItemFunc( strClass, 3 + classl, 6, hTreeClass, CE_TVI_CLASS, ed, ed->assoc, lnc2 );
                    classl += 1;
                }
                else if( intrf )  {
                    intrf = 0;
                    strClass.val( ws, wl );
                    TreeItemFunc( strClass, 3 + classl, 20, hTreeClass, CE_TVI_CLASS, ed, ed->assoc, lnc2 );
                }
            }

            if( wordf ? isalnumu( *si ) : isalphau( *si ))  {
                if(( wordf )&&( spacef ))  {
                    if( wl == 5 ? !strncmp( ws, "class", 5 ) : 0 )
                        classf = -1;
                    else if( wl == 7 ? !strncmp( ws, "private", 7 ) : 0 )
                        privf = 1;
                    else if( wl == 9 ? !strncmp( ws, "protected", 9 ) : 0 )
                        privf = 2;
                    else if( wl == 9 ? !strncmp( ws, "interface", 9 ) : 0 )
                        intrf = -1;
                }
                if(( ! wordf )||( spacef ))  {
                    wordf = -1;
                    ws = si;
                    wl = 1;
                }
                else  {
                    ++wl;
                }
                spacef = 0;
            }
            else if( *si == '(' )   {
                classf = 0;
                intrf = 0;
                if( wordf )  {
                    wordf = 0;
                    if(( ! resword( kwl_java, kwc_java, ws, wl ))||(( strncmp( ws, "main", 4 ))== 0 ))  {

                        lnc2 = lnc;
                        if( c_func( &ln, &si, &lnc, szprms ))  {
                            //strncpy( szf, ws, wl );
                            strFunction.val( ws, wl );
                            //get_c_funcparms( lnt, st, &szf[ wl ] );
                            //c_prms( &szf[ wl ], szprms );
                            c_prms( szprms );
                            strFunction.cat( szprms );
                            TreeItemFunc( strFunction, 3 + classl, 7 + privf, hTreeClass, CE_TVI_FUNC, ed, ed->assoc, lnc2 );
                        }
                        privf = 0;
                        continue;
                    }
                }
            privf = 0;
            }
            else if( *si == ' ' )   {
                spacef = -1;
                lnc2 = lnc;
            }
            else if( *si == '{' )  {
                intrf = 0;
                classf = 0;
                wordf = 0;
                spacef = 0;
                privf = 0;
                pc += 1;
            }
            else if( *si == '}' )  {
                intrf = 0;
                classf = 0;
                privf = 0;
                if( pc > 0 )  {
                    if( pc == classl )  {
                        classl -= 1;
                        hTreeClass = TreeGetParent( hTreeClass ); 
                    }
                    pc -= 1;
                }
                /*if( pc == 0 )
                    classf = 0;*/
            }
            else if( *si == '/' )  {
                if( *( si + 1 )== '/' )  {
                    break;
                }
                else if( *( si + 1 )== '*' )  {
                    char *p;

                    if(( p = strstr( si, "*/" ))== NULL )  {
                        fc = -1;
                        break;
                    }
                    else  {
                        si = p + 2;
                        continue;
                    }
                }
            }
            else  {
                intrf = 0;
                classf = 0;
                privf = 0;
                spacef = 0;
                wordf = 0;
            }
            ++si;
        }
        if( ln == NULL )  break;
        ln = ln->next;
        lnc2 = lnc;
        lnc += 1;
        spacef = -1;
    }
    if( fupd )
        TreeItemDelMarked( (struct _TREEITEM *)ed->hTreeItem );
}



tree_orasql( struct editrec *ed, int fupd )  {
    struct linerec *ln;
    int lnc, lnc2;
    int wl;
    char *si, *ws;
    int wordf, spacef, packgf, procf, bodyf, dropf;
    HTREEITEM hTreePackg = 0;
    int pc;
    int fc = 0;
    String strName;
    int packgc = 0;
    struct recClass rPackgs[ 100 ];
    HTREEITEM (*TreeItemFunc)( const char * lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine );

    if( fupd )
        TreeItemFunc = TreeItemUpd;
    else
        TreeItemFunc = TreeItemIns;

    if( fupd )
        TreeItemMark2Del( (struct _TREEITEM *)ed->hTreeItem );

    
    ln = ed->firstline;
    lnc = lnc2 = 1;

    pc = 0;
    spacef = 0;
    wordf = 0;
    packgf = 0;
    bodyf = 0;
    procf = 0;
    dropf = 0;

    hTreePackg = (struct _TREEITEM *)ed->hTreeItem;
    while( ln != NULL )  {

        si = ln->strz;
        while( *si )  {

            if( fc )  {
                char *p;

                if(( p = strstr( si, "*/" ))== NULL )  {
                    break;
                }
                else  {
                    si = p + 2;
                    fc = 0;
                    continue;
                }
            }

            if(( spacef )||( *si == '(' ))  {
                
                if(( packgf == -1 )&&( strnicmp( ws, "body", 4 ) != 0 ))  {    //  BUG ??????? i.e. bodyxyz
                    if( ! dropf )  {        
                        packgf = 1;
                        strName.val( ws, wl );
                        for( int n = 0; n < packgc; ++n )  {
                            if( strName == *rPackgs[ n ].strName )  break;
                        }
                        if( n == packgc )  {
                            rPackgs[ packgc ].strName = new String;
                            *rPackgs[ packgc ].strName = strName;
                            rPackgs[ packgc ].hTreeItem = TreeItemFunc( strName, 3, 10, (struct _TREEITEM *)ed->hTreeItem, CE_TVI_CLASS, ed, ed->assoc, lnc2 );
                            packgc += 1;
                        }
                         hTreePackg = rPackgs[ n ].hTreeItem;
                     }
                     else
                        packgf = 0;
                     dropf = 0;
                }
                if( wordf && procf )  {
                    strName.val( ws, wl );
                    if( ! dropf )  {        
                        if( packgf )  {
                             if( bodyf )  {
                                TreeItemFunc( strName, packgc ? 4 : 3, 10 + procf, hTreePackg, CE_TVI_FUNC, ed, ed->assoc, lnc2 );
                            }
                            else  {
                                //TreeItemFunc( strName, 3, 10 + procf, (struct _TREEITEM *)ed->hTreeItem, CE_TVI_FUNC, ed, ed->assoc, lnc );
                            }
                        }
                        else  {
                            TreeItemFunc( strName, 3, 10 + procf, (struct _TREEITEM *)ed->hTreeItem, CE_TVI_FUNC, ed, ed->assoc, lnc2 );
                        }
                    }
                    procf = 0;
                    dropf = 0;
                }
            }

            if( wordf ? isalnumu( *si ) : isalphau( *si ))  {
                if(( wordf )&&( spacef ))  {
                    lnc2 = lnc;
                    if( wl == 7 ? !strnicmp( ws, "package", 7 ) : 0 )  {
                        packgf = -1;
                        bodyf = 0;
                        if( strnicmp( si, "body", 4 )== 0 )  {    // BUG !!!!!
                            bodyf = -1;
                        }
                    }
                    else if( wl == 9 ? !strnicmp( ws, "procedure", 9 ) : 0 )
                        procf = 1;
                    else if( wl == 8 ? !strnicmp( ws, "function", 8 ) : 0 )
                        procf = 2;
                    else if( wl == 7 ? !strnicmp( ws, "trigger", 7 ) : 0 )
                        procf = 3;
                    else if( wl == 4 ? !strnicmp( ws, "view", 4 ) : 0 )
                        procf = 4;
                    else if( wl == 5 ? !strnicmp( ws, "table", 5 ) : 0 )
                        procf = 5;
                    else if( wl == 4 ? !strnicmp( ws, "drop", 4 ) : 0 )
                        dropf = -1;
                }
                if(( ! wordf )||( spacef ))  {
                    wordf = -1;
                    ws = si;
                    wl = 1;
                }
                else  {
                    ++wl;
                }
                spacef = 0;
            }
            
            else if( *si == ' ' )   {
                spacef = -1;
                lnc2 = lnc;
            }
            else if(( *si == '-' )&&( *( si + 1 )== '-' ))  {
                break;
            }
            else if(( *si == '/' )&&( *( si + 1 )== '*' ))  {
                char *p;

                if(( p = strstr( si, "*/" ))== NULL )  {
                    fc = -1;
                    break;
                }
                else  {
                    si = p + 2;
                    continue;
                }
            }
            else  {
                procf = 0;
                dropf = 0;
                spacef = 0;
                wordf = 0;
            }
            ++si;
        }
        ln = ln->next;
        //lnc2 = lnc;
        lnc += 1;
        spacef = -1;
    }
    if( fupd )
        TreeItemDelMarked( (struct _TREEITEM *)ed->hTreeItem );
}




tree_mssql( struct editrec *ed, int fupd )  {
    struct linerec *ln;
    int lnc, lnc2;
    int wl;
    char *si, *ws;
    int wordf, spacef, procf, dropf;  
    int pc;
    int fc = 0;
    String strName;
    HTREEITEM (*TreeItemFunc)( const char * lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine );

    if( fupd )
        TreeItemFunc = TreeItemUpd;
    else
        TreeItemFunc = TreeItemIns;

    if( fupd )
        TreeItemMark2Del( (struct _TREEITEM *)ed->hTreeItem );

    
    ln = ed->firstline;
    lnc = lnc2 = 1;

    pc = 0;
    spacef = 0;
    wordf = 0;
    procf = 0;
    dropf = 0;

    while( ln != NULL )  {

        si = ln->strz;
        while( *si )  {

            if( fc )  {
                char *p;

                if(( p = strstr( si, "*/" ))== NULL )  {
                    break;
                }
                else  {
                    si = p + 2;
                    fc = 0;
                    continue;
                }
            }

            if(( spacef )||( *si == '(' ))  {
                if( wordf && procf )  {
                    if( ! dropf )  {
                        strName.val( ws, wl );
                        TreeItemFunc( strName, 3, 15 + procf, (struct _TREEITEM *)ed->hTreeItem, CE_TVI_FUNC, ed, ed->assoc, lnc2 );
                    }
                    procf = 0;
                    dropf = 0;
                }
            }

            if( wordf ? isalnumu( *si ) : isalphau( *si ))  {
                if(( wordf )&&( spacef ))  {
                    lnc2 = lnc;
                    if( wl == 9 ? !strnicmp( ws, "procedure", 9 ) : 0 )
                        procf = 1;
                    else if( wl == 4 ? !strnicmp( ws, "proc", 4 ) : 0 )
                        procf = 1;
                    else if( wl == 7 ? !strnicmp( ws, "trigger", 7 ) : 0 )
                        procf = 2;
                    else if( wl == 4 ? !strnicmp( ws, "view", 4 ) : 0 )
                        procf = 3;
                    else if( wl == 5 ? !strnicmp( ws, "table", 5 ) : 0 )
                        procf = 4;
                    else if( wl == 4 ? !strnicmp( ws, "drop", 4 ) : 0 )
                        dropf = -1;
                }
                if(( ! wordf )||( spacef ))  {
                    wordf = -1;
                    ws = si;
                    wl = 1;
                }
                else  {
                    ++wl;
                }
                spacef = 0;
            }
            
            else if( *si == ' ' )   {
                spacef = -1;
                lnc2 = lnc;
            }
            else if( *si == '[' )   {

            }
            else if( *si == ']' )   {
                if( *(si + 1 )== '.' )  {
                    spacef = 0;
                    wordf = 0;
                    ++si;
                }
                else
                    spacef = -1;

            }
            else if(( *si == '-' )&&( *( si + 1 )== '-' ))  {
                break;
            }
            else if(( *si == '/' )&&( *( si + 1 )== '*' ))  {
                char *p;

                if(( p = strstr( si, "*/" ))== NULL )  {
                    fc = -1;
                    break;
                }
                else  {
                    si = p + 2;
                    continue;
                }
            }
            else  {
                procf = 0;
                dropf = 0;
                spacef = 0;
                wordf = 0;
            }
            /*if( *si == '-' )  {
                if( *( si + 1 )== '-' )  {
                    break;
                }
            }
            else if( *si == '/' )  {
                if( *( si + 1 )== '*' )  {
                    char *p;

                    if(( p = strstr( si, "* /" ))== NULL )  {
                        fc = -1;
                        break;
                    }
                    else  {
                        si = p + 2;
                        continue;
                    }
                }
            }*/
            ++si;
        }
        ln = ln->next;
        //lnc2 = lnc;
        lnc += 1;
        spacef = -1;
    }
    if( fupd )
        TreeItemDelMarked( (struct _TREEITEM *)ed->hTreeItem );
}



tree_jvs( struct editrec *ed, int fupd )  {
    struct linerec *ln;
    int lnc, lnc2;
    int wl;
    char *si, *ws;
    int wordf, spacef, procf, dropf;  
    int pc;
    int fc = 0;
    String strName;
    HTREEITEM (*TreeItemFunc)( const char * lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine );

    if( fupd )
        TreeItemFunc = TreeItemUpd;
    else
        TreeItemFunc = TreeItemIns;

    if( fupd )
        TreeItemMark2Del( (struct _TREEITEM *)ed->hTreeItem );

    
    ln = ed->firstline;
    lnc = lnc2 = 1;

    pc = 0;
    spacef = 0;
    wordf = 0;
    procf = 0;

    while( ln != NULL )  {

        si = ln->strz;
        while( *si )  {

            if( fc )  {
                char *p;

                if(( p = strstr( si, "*/" ))== NULL )  {
                    break;
                }
                else  {
                    si = p + 2;
                    fc = 0;
                    continue;
                }
            }

            if(( spacef )||( *si == '(' ))  {
                if( wordf && procf )  {
                    strName.val( ws, wl );
                    TreeItemFunc( strName, 3, 7, (struct _TREEITEM *)ed->hTreeItem, CE_TVI_FUNC, ed, ed->assoc, lnc2 );
                    procf = 0;
                }
            }
            if( wordf ? isalnumu( *si ) : isalphau( *si ))  {
                if(( wordf )&&( spacef ))  {
                    lnc2 = lnc;
                    if( wl == 8 ? !strncmp( ws, "function", 8 ) : 0 )
                        procf = 1;
                }
                if(( ! wordf )||( spacef ))  {
                    wordf = -1;
                    ws = si;
                    wl = 1;
                }
                else  {
                    ++wl;
                }
                spacef = 0;
            }
            
            else if( *si == ' ' )   {
                spacef = -1;
                lnc2 = lnc;
            }
            else  {
                procf = 0;
                dropf = 0;
                spacef = 0;
                wordf = 0;
            }
            if( *si == '/' )  {
                if( *( si + 1 )== '/' )  {
                    break;
                }
            }
            else if( *si == '/' )  {
                if( *( si + 1 )== '*' )  {
                    char *p;

                    if(( p = strstr( si, "*/" ))== NULL )  {
                        fc = -1;
                        break;
                    }
                    else  {
                        si = p + 2;
                        continue;
                    }
                }
            }
            ++si;
        }
        ln = ln->next;
        lnc += 1;
        spacef = -1;
    }
    if( fupd )
        TreeItemDelMarked( (struct _TREEITEM *)ed->hTreeItem );
}




tree_vbs( struct editrec *ed, int fupd )  {
    struct linerec *ln;
    int lnc, lnc2;
    int wl;
    char *si, *ws;
    int wordf, spacef, procf, endf;  
    int pc;
    String strName;
    HTREEITEM (*TreeItemFunc)( const char * lpszItem, int iLevel, int nImage, HTREEITEM htiParent, int iType, struct editrec *ed, int iAssoc, int iLine );

    if( fupd )
        TreeItemFunc = TreeItemUpd;
    else
        TreeItemFunc = TreeItemIns;

    if( fupd )
        TreeItemMark2Del( (struct _TREEITEM *)ed->hTreeItem );

    
    ln = ed->firstline;
    lnc = lnc2 = 1;

    pc = 0;
    spacef = 0;
    wordf = 0;
    procf = 0;
    endf = 0;

    while( ln != NULL )  {

        si = ln->strz;
        while( *si )  {
            if(( spacef )||( *si == '(' ))  {
                if( wordf && procf )  {
                    strName.val( ws, wl );
                    TreeItemFunc( strName, 3, 3, (struct _TREEITEM *)ed->hTreeItem, CE_TVI_FUNC, ed, ed->assoc, lnc2 );
                    procf = 0;
                }
            }
            if( wordf ? isalnumu( *si ) : isalphau( *si ))  {
                if(( wordf )&&( spacef ))  {
                    if( ! endf )  {
                        lnc2 = lnc;
                        if( wl == 8 ? !strnicmp( ws, "function", 8 ) : 0 )
                            procf = 1;
                        if( wl == 3 ? !strnicmp( ws, "sub", 3 ) : 0 )
                            procf = 1;
                    }
                    endf = 0;
                    if( wl == 3 ? !strnicmp( ws, "end", 3 ) : 0 )
                        endf = 1;
                    else if( wl == 4 ? !strnicmp( ws, "exit", 4 ) : 0 )
                        endf = 1;

                }
                if(( ! wordf )||( spacef ))  {
                    wordf = -1;
                    ws = si;
                    wl = 1;
                }
                else  {
                    ++wl;
                }
                spacef = 0;
            }
            
            else if( *si == ' ' )   {
                spacef = -1;
                lnc2 = lnc;
            }
            else  {
                procf = 0;
                endf = 0;
                spacef = 0;
                wordf = 0;
            }
            if( *si == '\'' )  {
                break;
            }
            ++si;
        }
        ln = ln->next;
        lnc += 1;
        spacef = -1;
    }
    if( fupd )
        TreeItemDelMarked( (struct _TREEITEM *)ed->hTreeItem );
}





tree_file( struct editrec *ed, int fupd )  {

    if( ed->hTreeItem == NULL )  return 0;

    switch( ed->assoc )  {

        case CE_ASC_C:
            tree_c( ed, fupd );
            break;
            
        case CE_ASC_JAVA:
            tree_java( ed, fupd );
            break;
            
        case CE_ASC_ORASQL:
            tree_orasql( ed, fupd );
            break;

        case CE_ASC_MSSQL:
            tree_mssql( ed, fupd );
            break;

        case CE_ASC_JVS:
            tree_jvs( ed, fupd );
            break;

        case CE_ASC_VBS:
            tree_vbs( ed, fupd );
            break;

    }
}






#ifdef __cplusplus 
} 
#endif  

