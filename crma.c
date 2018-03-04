

#include "ced.h"

#include "stdlib.h"
#include "string.h"


extern struct editrec *ed;


/*

//  !!! 
//  Beware, the release CRT function _strnicmp doesn't like non-alpha characters.
//  This may cause the chroma coding not to color properly.
//
//  The following might however also equate non-alpha characters wich are different at the 0x20 bit.
//  

*/

/*int mstrnicmp( const char *s1, const char *s2, int l )  {   // returns 0 if equal else -2
        int n = 0;

        while(( *s1 | 0x20 )==( *s2 | 0x20 ))  {
                if( *s1 == '\0' )  return 0;
                s1++;
                s2++;
                if( ++n == l )  return 0;
                }
        return -1;
        }*/


char* stristr( char *s, char *f )  {
        int l;

        l = strlen( f );
        while( 1 )  {
                while(( *f | 0x20 )!=( *s | 0x20 ))  {  // First find a match on the first character
                        if( *s == '\0' )  
                                return 0;
                        ++s;    
                        }
                if( _strnicmp( f, s, l )== 0 )
                        return s;
                ++s;
                }
        }


static char szscript[] = "<script";
static char szscript_[] = "</script";


char asstype[ CE_CNT_ASSTYPE ][ CE_LEN_ASSTYPE ] = {
                  "None",                               // CE_ASC_NONE
                  "C/C++",                              // CE_ASC_C
                  "Java",                               // CE_ASC_JAVA
                  "Oracle PL/SQL",                      // CE_ASC_ORASQL
                  "SQL Server T-SQL",                   // CE_ASC_MSSQL
                  "Sybase Transact-SQL",                // CE_ASC_SYBSQL
                  "HTML / ASP",                         // CE_ASC_MSASP
                  "XML / XSL ",                         // CE_ASC_XML
                  "Assembler",                          // CE_ASC_ASM
                  "JavaScript",                         // CE_ASC_JVS
                  "VBScript",                           // CE_ASC_VBS
                  "DOS",                                // CE_ASC_DOS
                  "Unix",                               // CE_ASC_UNIX
                  "Windows",                            // CE_ASC_WIN
                  //"Script / Make",
                  //"IDL",
                  //"None (Semicolon Comment)",
                  //"None (Hash Comment)",
                  };




int asscnt = CE_CNT_ASSFNEXT;
struct assrec asslst[ CE_MAX_ASSOCIATIONS ] = {
                "",         CE_ASC_NONE,
                "",         CE_ASC_NONE,
                "c",        CE_ASC_C,
                "h",        CE_ASC_C,
                "cpp",      CE_ASC_C,
                "hpp",      CE_ASC_C,
                "rc",       CE_ASC_C,
                "jav",      CE_ASC_JAVA,
                "java",     CE_ASC_JAVA,
                "sql",      CE_ASC_ORASQL,
                "asp",      CE_ASC_MSASP,
                "asa",      CE_ASC_MSASP,
                "htm",      CE_ASC_MSASP,
                "html",     CE_ASC_MSASP,
                "asm",      CE_ASC_ASM,
                "xml",      CE_ASC_XML,
                "xsl",      CE_ASC_XML,
                "js",       CE_ASC_JVS,
                "vbs",      CE_ASC_VBS,
                "cmd",      CE_ASC_DOS,
                "bat",      CE_ASC_DOS,
                "mak",      CE_ASC_UNIX,
                "ini",      CE_ASC_WIN,
                "reg",      CE_ASC_WIN,
                "inf",      CE_ASC_WIN,
                "def",      CE_ASC_WIN,

                };


int crmacode = -1;
//int crmarcrsvc = 0;
int assocdef = 0;
//int assocsql = 0;




int kwc_html = 84;
struct keywordrec kwl_html[ 84 ] = {
                "!DOCTYPE",      0,
                "A",             0,
                "ADDRESS",       0,
                "APPLET",        0,
                "AREA",          0,
                "B",             0,
                "BASE",          0,
                "BASEFONT",      0,
                "BGSOUND",       0,
                "BIG",           0,
                "BLOCKQUOTE",    0,
                "BODY",          0,
                "BR",            0,
                "CAPTION",       0,
                "CENTER",        0,
                "CITE",          0,
                "CODE",          0,
                "COL",           0,
                "COLGROUP",      0,
                "COMMENT",       0,
                "DD",            0,
                "DFN",           0,
                "DIR",           0,
                "DIV",           0,
                "DL",            0,
                "DT",            0,
                "EM",            0,
                "EMBED",         0,
                "FONT",          0,
                "FORM",          0,
                "FRAME",         0,
                "FRAMESET",      0,
                "H",             0,
                "HEAD",          0,
                "HR",            0,
                "HTML",          0,
                "I",             0,
                "IFRAME",        0,
                "IMG",           0,
                "INPUT",         0,
                "ISINDEX",       0,
                "KBD",           0,
                "LI",            0,
                "LINK",          0,
                "LISTING",       0,
                "MAP",           0,
                "MARQUEE",       0,
                "MENU",          0,
                "META",          0,
                "NOBR",          0,
                "NOFRAMES",      0,
                "OBJECT",        0,
                "OL",            0,
                "OPTION",        0,
                "P",             0,
                "PARAM",         0,
                "PLAINTEXT",     0,
                "PRE",           0,
                "S",             0,
                "SAMP",          0,
                "SCRIPT",        0,
                "SELECT",        0,
                "SMALL",         0,
                "SPAN",          0,
                "STRIKE",        0,
                "STRONG",        0,
                "STYLE",         0,
                "SUB",           0,
                "SUP",           0,
                "TABLE",         0,
                "TBODY",         0,
                "TD",            0,
                "TEXTAREA",      0,
                "TFOOT",         0,
                "TH",            0,
                "THEAD",         0,
                "TITLE",         0,
                "TR",            0,
                "TT",            0,
                "U",             0,
                "UL",            0,
                "VAR",           0,
                "WBR",           0,
                "XMP",           0, };



int kwc_vbs = 126;
struct keywordrec kwl_vbs[ 126 ] = {
       "Abs",                0,
       "And",                0,
       "Array",              0,
       "Asc",                0,
       "Atn",                0,
       "Attribute",          0,
       "Call",               0,
       "Case",               0,
       "CBool",              0,
       "CByte",              0,
       "CCur",               0,
       "CDate",              0,
       "CDbl",               0,
       "Chr",                0,
       "CInt",               0,
       "CLng",               0,
       "Close",              0,
       "Const",              0,
       "Cos",                0,
       "CSng",               0,
       "CStr",               0,
       "Date",               0,
       "DateAdd",            0,
       "DateDiff",           0,
       "DatePart",           0,
       "DateSerial",         0,
       "DateValue",          0,
       "Day",                0,
       "Dim",                0,
       "Do",                 0,
       "Each",               0,
       "Else",               0,
       "ElseIf",             0,
       "Empty",              0,
       "End",                0,
       "Eqv",                0,
       "Erase",              0,
       "Error",              0,
       "Exit",               0,
       "Exp",                0,
       "Explicit",           0,
       "False",              0,
       "Fix",                0,
       "For",                0,
       "FormatCurrency",     0,
       "FormatDateTime",     0,
       "FormatNumber",       0,
       "FormatPercent",      0,
       "Function",           0,
       "GetObject",          0,
       "Hex",                0,
       "Hour",               0,
       "If",                 0,
       "Imp",                0,
       "InStr",              0,
       "InStrRev",           0,
       "Int",                0,
       "Is",                 0,
       "IsArray",            0,
       "IsDate",             0,
       "IsEmpty",            0,
       "IsNull",             0,
       "IsNumeric",          0,
       "IsObject",           0,
       "Join",               0,
       "LBound",             0,
       "LCase",              0,
       "Left",               0,
       "Len",                0,
       "Log",                0,
       "Loop",               0,
       "LTrim",              0,
       "Mid",                0,
       "Minute",             0,
       "Mod",                0,
       "Month",              0,
       "MonthName",          0,
       "Next",               0,
       "Not",                0,
       "Nothing",            0,
       "Now",                0,
       "Null",               0,
       "Oct",                0,
       "On",                 0,
       "Option",             0,
       "Or",                 0,
       "Private",            0,
       "Public",             0,
       "Randomize",          0,
       "ReDim",              0,
       "Rem",                0,
       "Resume",             0,
       "Right",              0,
       "Rnd",                0,
       "Round",              0,
       "RTrim",              0,
       "Second",             0,
       "Select",             0,
       "Set",                0,
       "Sgn",                0,
       "Sin",                0,
       "Space",              0,
       "Split",              0,
       "Sqr",                0,
       "StrComp",            0,
       "String",             0,
       "StrReverse",         0,
       "Sub",                0,
       "Tan",                0,
       "Then",               0,
       "Time",               0,
       "TimeSerial",         0,
       "TimeValue",          0,
       "Trim",               0,
       "True",               0,
       "TypeName",           0,
       "UBound",             0,
       "UCase",              0,
       "VarType",            0,
       "Weekday",            0,
       "WeekdayName",        0,
       "Wend",               0,
       "While",              0,
       "Write",              0,
       "Xor",                0,
       "Year",               0, 
        };



int kwc_c = 60;
struct keywordrec kwl_c[ ] = {
        "#define",   0,
        "#else",     0,
        "#endif",    0,
        "#if",       0,
        "#ifdef",    0,
        "#ifndef",   0,
        "#include",  0,
        "#line",     0,
        "#undef",    0,
        "auto",      0,
        "break",     0,
        "case",      0,
        "catch",     0,
        "char",      0,
        "class",     0,
        "const",     0,
        "continue",  0,
        "default",   0,
        "defined",   0,     // pre-processor directive - can only be after #if or #elif
        "delete",    0,
        "do",        0,
        "double",    0,
        "else",      0,
        "enum",      0,
        "extern",    0,
        "float",     0,
        "for",       0,
        "friend",    0,
        "goto",      0,
        "if",        0,
        "inline",    0,
        "int",       0,
        "long",      0,
        "main",      0,
        "naked",     0,
        "new",       0,
        "operator",  0,
        "private",   0,
        "protected", 0,
        "public",    0,
        "register",  0,
        "return",    0,
        "short",     0,
        "signed",    0,
        "sizeof",    0,
        "static",    0,
        "struct",    0,
        "switch",    0,
        "template",  0,
        "thread",    0,
        "this",      0,
        "throw",     0,
        "try",       0,
        "typedef",   0,
        "union",     0,
        "unsigned",  0,
        "virtual",   0,
        "void",      0,
        "volatile",  0,
        "while",     0 };



int kwc_java = 61;
struct keywordrec kwl_java[ 61 ] = {
        "abstract",     0,
        "boolean",      0,
        "break",        0,
        "byte",         0,
        "byvalue",      0,
        "case",         0,
        "cast",         0,
        "catch",        0,
        "char",         0,
        "class",        0,
        "const",        0,
        "continue",     0,
        "default",      0,
        "do",           0,
        "double",       0,
        "else",         0,
        "extends",      0,
        "false",        0,
        "final",        0,
        "finally",      0,
        "float",        0,
        "for",          0,
        "future",       0,
        "generic",      0,
        "goto",         0,
        "if",           0,
        "implements",   0,
        "import",       0,
        "inner",        0,
        "instanceof",   0,
        "int",          0,
        "interface",    0,
        "length",       0,
        "long",         0,
        "native",       0,
        "new",          0,
        "null",         0,
        "operator",     0,
        "outer",        0,
        "package",      0,
        "private",      0,
        "protected",    0,
        "public",       0,
        "rest",         0,
        "return",       0,
        "short",        0,
        "static",       0,
        "super",        0,
        "switch",       0,
        "synchronized", 0,
        "this",         0,
        "throw",        0,
        "throws",       0,
        "threadsafe",   0,
        "transient",    0,
        "true",         0,
        "try",          0,
        "var",          0,
        "void",         0,
        "volatile",     0,
        "while",        0,  };




int kwc_jvs = 16;
struct keywordrec kwl_jvs[ 16 ] = {
        "break",        0,
        "continue",     0,
        "do",           0,
        "else",         0,
        "false",        0,
        "for",          0,
        "function",     0,
        "if",           0,
        "in",           0,
        "return",       0,
        "switch",       0,
        "this",         0,
        "true",         0,
        "var",          0,
        "while",        0,
        "with",         0,  };



char lfc_orasql[] = "REM";
char ifc_orasql[] = "";
char inc_orasql[] = "_#$";

int kwc_orasql = 619;   //248;
//struct keywordrec kwl_orasql[ 619] = {
char *kws_orasql[ 619 ] = {
       "ABORT",                            
       "ABS",                              
       "ACCEPT",                           
       "ACCESS",                           
       "ACCOUNT",                          
       "ACOS",                             
       "ACTIVATE",                         
       "ADD",                              
       "ADD_MONTHS",                       
       "ADMIN",                            
       "ADVISE",                           
       "AFTER",                            
       "ALL",                              
       "ALL_ROWS",                         
       "ALLOCATE",                         
       "ALTER",                            
       "ANALYZE",                          
       "AND",                              
       "ANY",                              
       "ARCHIVE",                          
       "ARCHIVELOG",                       
       "ARRAY",                            
       "ARRAYLEN",                         
       "AS",                               
       "ASC",                              
       "ASCII",                            
       "ASIN",                             
       "ASSERT",                           
       "ASSIGN",                           
       "AT",                               
       "ATAN",                             
       "ATAN2",                            
       "AUDIT",                            
       "AUTHENTICATED",                    
       "AUTHORIZATION",                    
       "AUTOEXTEND",                       
       "AUTOMATIC",                        
       "AVG",                              
       "BACKUP",                           
       "BASE_TABLE",                       
       "BECOME",                           
       "BEFORE",                           
       "BEGIN",                            
       "BETWEEN",                          
       "BFILE",                            
       "BFILENAME",                        
       "BINARY_INTEGER",                   
       "BITMAP",                           
       "BLOB",
       "BLOCK",                            
       "BODY",                             
       "BOOLEAN",                          
       "BY",                               
       "CACHE",                            
       "CACHE_INSTANCES",                  
       "CANCEL",                           
       "CASCADE",                          
       "CASE",                             
       "CAST",                             
       "CEIL",                             
       "CFILE",                            
       "CHAINED",                          
       "CHANGE",                           
       "CHAR",                             
       "CHAR_BASE",                        
       "CHAR_CS",                          
       "CHARACTER",                        
       "CHARTOROWID",                      
       "CHECK",                            
       "CHECKPOINT",                       
       "CHOOSE",                           
       "CHR",                              
       "CHUNK",                            
       "CLEAR",                            
       "CLOB",                             
       "CLONE",                            
       "CLOSE",                            
       "CLOSE_CACHED_OPEN_CURSORS",        
       "CLUSTER",                          
       "CLUSTERS",                         
       "COALESCE",                         
       "COLAUTH",                          
       "COLUMN",                           
       "COLUMNS",                          
       "COMMENT",                          
       "COMMIT",                           
       "COMMITTED",                        
       "COMPATIBILITY",                    
       "COMPILE",                          
       "COMPLETE",                         
       "COMPOSITE_LIMIT",                  
       "COMPRESS",                         
       "COMPUTE",                          
       "CONCAT",                           
       "CONNECT",                          
       "CONNECT_TIME",                     
       "CONSTANT",                         
       "CONSTRAINT",                       
       "CONSTRAINTS",                      
       "CONTENTS",                         
       "CONTINUE",                         
       "CONTROLFILE",                      
       "CONVERT",                          
       "COS",                              
       "COSH",                             
       "COST",                             
       "COUNT",                            
       "CPU_PER_CALL",                     
       "CPU_PER_SESSION",                  
       "CRASH",
       "CREATE",                           
       "CURRENT",                          
       "CURRENT_SCHEMA",                   
       "CURRENT_USER",                     
       "CURRVAL",                          
       "CURSOR",                           
       "CYCLE",                            
       "DANGLING",                         
       "DATA_BASE",                        
       "DATABASE",                         
       "DATAFILE",                         
       "DATAFILES",                        
       "DATAOBJNO",                        
       "DATE",                             
       "DBA",                              
       "DBHIGH",                           
       "DBLOW",                            
       "DBMAC",                            
       "DEALLOCATE",                       
       "DEBUG",                            
       "DEBUGOFF",                         
       "DEBUGON",                          
       "DEC",                              
       "DECIMAL",                          
       "DECLARE",                          
       "DEFAULT",                          
       "DEFERRABLE",                       
       "DEFERRED",                         
       "DEFINTION",                        
       "DEGREE",                           
       "DELAY",                            
       "DELETE",                           
       "DEREF",                            
       "DESC",                             
       "DIGITS",                           
       "DIRECTORY",                        
       "DISABLE",                          
       "DISCONNECT",                       
       "DISMOUNT",                         
       "DISPOSE",                          
       "DISTINCT",                         
       "DISTRIBUTED",                      
       "DML",                              
       "DO",                               
       "DOUBLE",                           
       "DROP",                             
       "DUMP",                             
       "EACH",                             
       "ELSE",                             
       "ELSIF",                            
       "EMPTY_BLOB",                       
       "EMPTY_CLOB",                       
       "ENABLE",                           
       "END",                              
       "ENFORCE",                          
       "ENTRY",                            
       "ESCAPE",                           
       "ESTIMATE",                         
       "EVENTS",                           
       "EXCEPT",                           
       "EXCEPTION",
       "EXCEPTION_INIT",                   
       "EXCEPTIONS",                       
       "EXCHANGE",                         
       "EXCLUDING",                        
       "EXCLUSIVE",                        
       "EXECUTE",                          
       "EXISTS",                           
       "EXIT",                             
       "EXP",                              
       "EXPIRE",                           
       "EXPLAIN",                          
       "EXTENT",                           
       "EXTENTS",                          
       "EXTERNALLY",                       
       "FAILED_LOGIN_ATTEMPTS",            
       "FALSE",                            
       "FAST",                             
       "FETCH",                            
       "FILE",                             
       "FIRST_ROWS",                       
       "FLAGGER",                          
       "FLOAT",                            
       "FLOB",                             
       "FLOOR",                            
       "FLUSH",                            
       "FOR",                              
       "FORCE",                            
       "FOREIGN",                          
       "FORM",                             
       "FREELIST",                         
       "FREELISTS",                        
       "FROM",                             
       "FULL",                             
       "FUNCTION",                         
       "GENERIC",                          
       "GLOBAL",                           
       "GLOBAL_NAME",                      
       "GLOBALLY",                         
       "GOTO",                             
       "GRANT",                            
       "GREATEST",                         
       "GROUP",                            
       "GROUPS",                           
       "HASH",                             
       "HASHKEYS",                         
       "HAVING",                           
       "HEADER",                           
       "HEAP",                             
       "HEXTORAW",                         
       "IDENTIFIED",                       
       "IDGENERATORS",                     
       "IDLE_TIME",                        
       "IF",                               
       "IMMEDIATE",                        
       "IN",                               
       "INCLUDING",                        
       "INCREMENT",                        
       "IND_PARTITION",                    
       "INDEX",                            
       "INDEXED",                          
       "INDEXES",
       "INDICATOR",                        
       "INITCAP",                          
       "INITIAL",                          
       "INITIALLY",                        
       "INITRANS",                         
       "INSERT",                           
       "INSTANCE",                         
       "INSTANCES",                        
       "INSTEAD",                          
       "INSTR",                            
       "INSTRB",                           
       "INT",                              
       "INTEGER",                          
       "INTERFACE",                        
       "INTERMEDIATE",                     
       "INTERSECT",                        
       "INTO",                             
       "IS",                               
       "ISOLATION",                        
       "ISOLATION_LEVEL",                  
       "KEEP",                             
       "KEY",                              
       "KILL",                             
       "LABEL",                            
       "LAST_DAY",                         
       "LAYER",                            
       "LEAST",                            
       "LENGTH",                           
       "LENGTHB",                          
       "LESS",                             
       "LEVEL",                            
       "LIBRARY",                          
       "LIKE",                             
       "LIMIT",                            
       "LIMITED",                          
       "LINK",                             
       "LIST",                             
       "LN",                               
       "LOB",                              
       "LOCAL",                            
       "LOCK",                             
       "LOCKED",                           
       "LOG",                              
       "LOGFILE",                          
       "LOGGING",                          
       "LOGICAL_READS_PER_CALL",           
       "LOGICAL_READS_PER_SESSION",        
       "LONG",                             
       "LOOP",                             
       "LOWER",                            
       "LPAD",                             
       "LTRIM",                            
       "MAKE_REF",                         
       "MANAGE",                           
       "MASTER",                           
       "MAX",                              
       "MAXARCHLOGS",                      
       "MAXDATAFILES",                     
       "MAXEXTENTS",                       
       "MAXINSTANCES",                     
       "MAXLOGFILES",
       "MAXLOGHISTORY",                    
       "MAXLOGMEMBERS",                    
       "MAXSIZE",                          
       "MAXTRANS",                         
       "MAXVALUE",                         
       "MEMBER",                           
       "MIN",                              
       "MINEXTENTS",                       
       "MINIMUM",                          
       "MINUS",                            
       "MINVALUE",                         
       "MLS_LABEL_FORMAT",                 
       "MLSLABEL",                         
       "MOD",                              
       "MODE",                             
       "MODIFY",                           
       "MONTHS_BETWEEN",                   
       "MOUNT",                            
       "MOVE",                             
       "MTS_DISPATCHERS",                  
       "MULTISET",                         
       "NATIONAL",                         
       "NATURAL",                          
       "NATURALN",                         
       "NCHAR",                            
       "NCHAR_CS",                         
       "NCLOB",                            
       "NEEDED",                           
       "NESTED",                           
       "NETWORK",                          
       "NEW",                              
       "NEW_TIME",                         
       "NEXT",                             
       "NEXT_DAY",                         
       "NEXTVAL",                          
       "NLS_CHARSET_DECL_LEN",             
       "NLS_CHARSET_ID",                   
       "NLS_CHARSET_NAME",                 
       "NLS_INITCAP",                      
       "NLS_LOWER",                        
       "NLS_UPPER",                        
       "NLSSORT",                          
       "NOARCHIVELOG",                     
       "NOAUDIT",                          
       "NOCACHE",                          
       "NOCOMPRESS",                       
       "NOCYCLE",                          
       "NOFORCE",                          
       "NOLOGGING",                        
       "NOMAXVALUE",                       
       "NOMINVALUE",                       
       "NONE",                             
       "NOORDER",                          
       "NOOVERRIDE",                       
       "NOPARALLEL",                       
       "NORESETLOGS",                      
       "NOREVERSE",                        
       "NORMAL",                           
       "NOSORT",                           
       "NOT",                              
       "NOTHING",
       "NOWAIT",                           
       "NULL",                             
       "NUMBER",                           
       "NUMBER_BASE",                      
       "NUMERIC",                          
       "NVARCHAR2",                        
       "NVL",                              
       "OBJECT",                           
       "OBJNO",                            
       "OBJNO_REUSE",                      
       "OF",                               
       "OFF",                              
       "OFFLINE",                          
       "OID",                              
       "OIDINDEX",                         
       "OLD",                              
       "ON",                               
       "ONLINE",                           
       "ONLY",                             
       "OPCODE",                           
       "OPEN",                             
       "OPTIMAL",                          
       "OPTIMIZER_GOAL",                   
       "OPTION",                           
       "OR",                               
       "ORDER",                            
       "ORGANIZATION",                     
       "OSLABEL",                          
       "OTHERS",                           
       "OUT",                              
       "OVERFLOW",                         
       "OWN",                              
       "PACKAGE",                          
       "PARALLEL",                         
       "PARTITION",                        
       "PASSWORD",                         
       "PASSWORD_GRACE_TIME",              
       "PASSWORD_LIFE_TIME",               
       "PASSWORD_LOCK_TIME",               
       "PASSWORD_REUSE_MAX",               
       "PASSWORD_REUSE_TIME",              
       "PASSWORD_VERIFY_FUNCTION",         
       "PCTFREE",                          
       "PCTINCREASE",                      
       "PCTTHRESHOLD",                     
       "PCTUSED",                          
       "PCTVERSION",                       
       "PERCENT",                          
       "PERMANENT",                        
       "PLAN",                             
       "PLS_INTEGER",                      
       "PLSQL_DEBUG",                      
       "POSITIVE",                         
       "POSITIVEN",                        
       "POST_TRANSACTION",                 
       "POWER",                            
       "PRAGMA",                           
       "PRECISION",                        
       "PRESERVE",                         
       "PRIMARY",                          
       "PRIOR",
       "PRIVATE",                          
       "PRIVATE_SGA",                      
       "PRIVILEGE",                        
       "PRIVILEGES",                       
       "PROCEDURE",                        
       "PROFILE",                          
       "PUBLIC",                           
       "PURGE",                            
       "QUEUE",                            
       "QUOTA",                            
       "RAISE",                            
       "RANGE",                            
       "RAW",                              
       "RAWTOHEX",                         
       "RBA",                              
       "READ",                             
       "READUP",                           
       "REAL",                             
       "REBUILD",                          
       "RECORD",                           
       "RECOVER",                          
       "RECOVERABLE",                      
       "RECOVERY",                         
       "REF",                              
       "REFERENCES",                       
       "REFERENCING",                      
       "REFRESH",                          
       "REFTOHEX",                         
       "RELEASE",                          
       "REMR",                             
       "RENAME",                           
       "REPLACE",                          
       "RESET",                            
       "RESETLOGS",                        
       "RESIZE",                           
       "RESOURCE",                         
       "RESTRICTED",                       
       "RETURN",                           
       "RETURNING",                        
       "REUSE",                            
       "REVERSE",                          
       "REVOKE",                           
       "ROLE",                             
       "ROLES",                            
       "ROLLBACK",                         
       "ROUND",                            
       "ROW",                              
       "ROWID",                            
       "ROWIDTOCHAR",                      
       "ROWLABEL",                         
       "ROWNUM",                           
       "ROWS",                             
       "ROWTYPE",                          
       "RPAD",                             
       "RTRIM",                            
       "RULE",                             
       "RUN",                              
       "SAMPLE",                           
       "SAVEPOINT",                        
       "SB4",                              
       "SCAN_INSTANCES",
       "SCHEMA",                           
       "SCN",                              
       "SCOPE",                            
       "SD_ALL",                           
       "SD_INHIBIT",                       
       "SD_SHOW",                          
       "SEG_BLOCK",                        
       "SEG_FILE",                         
       "SEGMENT",                          
       "SELECT",                           
       "SEPERATE",                         
       "SEQUENCE",                         
       "SERIALIZABLE",                     
       "SESSION",                          
       "SESSION_CACHED_CURSORS",           
       "SESSIONS_PER_USER",                
       "SET",                              
       "SHARE",                            
       "SHARED",                           
       "SHARED_POOL",                      
       "SHRINK",                           
       "SIGN",                             
       "SIN",                              
       "SINH",                             
       "SIZE",                             
       "SKIP",                             
       "SKIP_UNUSABLE_INDEXES",            
       "SMALLINT",                         
       "SNAPSHOT",                         
       "SOME",                             
       "SORT",                             
       "SOUNDEX",                          
       "SPACE",                            
       "SPECIFICATION",                    
       "SPLIT",                            
       "SQL",                              
       "SQL_TRACE",                        
       "SQLCODE",                          
       "SQLERRM",                          
       "SQRT",                             
       "STANDBY",                          
       "START",                            
       "STATEMENT",                        
       "STATEMENT_ID",                     
       "STATISTICS",                       
       "STDDEV",                           
       "STOP",                             
       "STORAGE",                          
       "STORE",                            
       "STRUCTURE",                        
       "SUBSTR",                           
       "SUBSTRB",                          
       "SUBTYPE",                          
       "SUCCESSFUL",                       
       "SUM",                              
       "SWITCH",                           
       "SYNONYM",                          
       "SYS_OP_ENFORCE_NOT_NULL$",         
       "SYS_OP_NTCIMG$",                   
       "SYSDATE",                          
       "SYSDBA",
       "SYSOPER",                          
       "SYSTEM",                           
       "TABAUTH",                          
       "TABLE",                            
       "TABLES",                           
       "TABLESPACE",                       
       "TABLESPACE_NO",                    
       "TABNO",                            
       "TAN",                              
       "TANH",                             
       "TASK",                             
       "TEMPORARY",                        
       "TERMINATE",                        
       "THAN",                             
       "THE",                              
       "THEN",                             
       "THREAD",                           
       "TIME",                             
       "TIMESTAMP",                        
       "TO",                               
       "TO_CHAR",                          
       "TO_DATE",                          
       "TO_MULTI_BYTE",                    
       "TO_NUMBER",                        
       "TO_SINGLE_BYTE",                   
       "TOPLEVEL",                         
       "TRACE",                            
       "TRACING",                          
       "TRANSACTION",                      
       "TRANSITIONAL",                     
       "TRANSLATE",                        
       "TRIGGER",                          
       "TRIGGERS",                         
       "TRUE",                             
       "TRUNC",                            
       "TRUNCATE",                         
       "TX",                               
       "TYPE",                             
       "UB2",                              
       "UBA",                              
       "UID",                              
       "UNARCHIVED",                       
       "UNDO",                             
       "UNION",                            
       "UNIQUE",                           
       "UNLIMITED",                        
       "UNLOCK",                           
       "UNRECOVERABLE",                    
       "UNTIL",                            
       "UNUSABLE",                         
       "UNUSED",                           
       "UPDATABLE",                        
       "UPDATE",                           
       "UPPER",                            
       "USAGE",                            
       "USE",                              
       "USER",                             
       "USERENV",                          
       "USING",                            
       "VALIDATE",                         
       "VALIDATION",                       
       "VALUE",                            
       "VALUES",                           
       "VARCHAR",                          
       "VARCHAR2",                         
       "VARIANCE",                         
       "VARYING",                          
       "VIEW",                             
       "VIEWS",                            
       "VSIZE",                            
       "WHEN",                             
       "WHENEVER",                         
       "WHERE",                            
       "WHILE",                            
       "WITH",                             
       "WITHOUT",                          
       "WORK",                             
       "WRITE",                            
       "WRITEDOWN",                        
       "WRITEUP",                          
       "XID",                              
       "XOR",                              
       };


char lfc_mssql[] = "";
char ifc_mssql[] = "@#";
char inc_mssql[] = "_@#$";      // shouldn't have @ but was quick fix to allow @@ keywords

int kwc_mssql = 356;        //200;
//short kwl_mssql[ 356 ];
//struct keywordrec kwl_mssql[ 356 ] = {
char *kws_mssql[ 356 ] = {
        "@@CONNECTIONS",
        "@@CPU_BUSY",
        "@@CURSOR_ROWS",
        "@@DATEFIRST",
        "@@DBTS",
        "@@ERROR",
        "@@FETCH_STATUS",
        "@@IDENTITY",
        "@@IDLE",
        "@@IO_BUSY",
        "@@LANGID",
        "@@LANGUAGE",
        "@@LOCK_TIMEOUT",          
        "@@MAX_CONNECTIONS",       
        "@@MAX_PRECISION",         
        "@@NESTLEVEL",             
        "@@OPTIONS",               
        "@@PACK_RECEIVED",         
        "@@PACK_SENT",             
        "@@PACKET_ERRORS",         
        "@@PROCID",                
        "@@REMSERVER",             
        "@@ROWCOUNT",              
        "@@SERVERNAME",            
        "@@SERVICENAME",           
        "@@SPID",                  
        "@@TEXTSIZE",              
        "@@TIMETICKS",             
        "@@TOTAL_ERRORS",
        "@@TOTAL_READ",            
        "@@TOTAL_WRITE",           
        "@@TRANCOUNT",             
        "@@VERSION",               
        "abs",                     
        "acos",                    
        "ADD",                     
        "ALL",                     
        "ALTER",                   
        "AND",                     
        "ANY",                     
        "app_name",                
        "AS",                      
        "ASC",                     
        "ascii",                   
        "asin",                    
        "atan",                    
        "atn2",                    
        "AUTHORIZATION",           
        "AVG",                     
        "BACKUP",                  
        "BEGIN",                   
        "BETWEEN",
        "binary",                  
        "bit",                     
        "BREAK",                   
        "BROWSE",                  
        "BULK",                    
        "BY",                      
        "CASCADE",                 
        "CASE",                    
        "cast",                    
        "ceiling",                 
        "char",                    
        "character",               
        "charindex",               
        "CHECK",                   
        "CHECKPOINT",              
        "CLOSE",                   
        "CLUSTERED",               
        "COALESCE",                
        "col_length",              
        "col_name",                
        "COLUMN",
        "columnproperty",          
        "COMMIT",                  
        "COMMITTED",               
        "COMPUTE",                 
        "CONFIRM",                 
        "CONSTRAINT",              
        "CONTAINS",                
        "CONTAINSTABLE",           
        "CONTINUE",                
        "CONTROLROW",              
        "CONVERT",                 
        "cos",                     
        "cot",                     
        "COUNT",                   
        "CREATE",                  
        "CROSS",                   
        "CUBE",
        "CURRENT",                 
        "CURRENT_DATE",            
        "CURRENT_TIME",            
        "CURRENT_TIMESTAMP",       
        "CURRENT_USER",            
        "CURSOR",                  
        "cursor_status",           
        "DATABASE",                
        "databaseproperty",        
        "datalength",              
        "dateadd",                 
        "datediff",                
        "datename",                
        "datepart",                
        "datetime",                
        "day",                     
        "db_id",                   
        "db_name",                 
        "DBCC",                    
        "DEALLOCATE",              
        "dec",                     
        "decimal",                 
        "DECLARE",
        "DEFAULT",                 
        "degrees",                 
        "DELETE",                  
        "DENY",                    
        "DESC",                    
        "difference",              
        "DISK",                    
        "DISTINCT",                
        "DISTRIBUTED",             
        "DOUBLE",                  
        "DROP",                    
        "DUMMY",                   
        "DUMP",                    
        "ELSE",                    
        "END",                     
        "ERRLVL",                  
        "ERROREXIT",               
        "ESCAPE",                  
        "EXCEPT",                  
        "EXEC",                    
        "EXECUTE",
        "EXISTS",                  
        "EXIT",                    
        "exp",                     
        "FETCH",                   
        "FILE",                    
        "file_id",                 
        "file_name",               
        "filegroup_id",            
        "filegroup_name",          
        "filegroupproperty",       
        "fileproperty",            
        "FILLFACTOR",              
        "float",                   
        "floor",                   
        "FLOPPY",                  
        "FOR",                     
        "FOREIGN",
        "formatmessage",           
        "FREETEXT",                
        "FREETEXTTABLE",           
        "FROM",                    
        "FULL",                    
        "fulltextcatalogproperty", 
        "fulltextserviceproperty", 
        "getansinull",             
        "getdate",                 
        "GO",                      
        "GOTO",                    
        "GRANT",                   
        "GROUP",                   
        "HAVING",                  
        "HOLDLOCK",                
        "host_id",                 
        "host_name",               
        "ident_incr",              
        "ident_seed",              
        "IDENTITY",                
        "IDENTITY_INSERT",         
        "IDENTITYCOL",             
        "IF",
        "image",                   
        "IN",                      
        "INDEX",                   
        "index_col",               
        "indexproperty",           
        "INNER",                   
        "INSENSITIVE",             
        "INSERT",                  
        "int",                     
        "integer",                 
        "INTERSECT",               
        "INTO",                    
        "IS",                      
        "is_member",               
        "is_srvrolemember",        
        "isdate",                  
        "isnull",                  
        "isnumeric",               
        "ISOLATION",               
        "JOIN",                    
        "KEY",
        "KILL",                    
        "LEFT",                    
        "len",                     
        "LEVEL",                   
        "LIKE",                    
        "LINENO",                  
        "LOAD",                    
        "log",                     
        "log10",                   
        "lower",                   
        "ltrim",                   
        "MAX",                     
        "MIN",                     
        "MIRROREXIT",              
        "money",                   
        "month",                   
        "NATIONAL",
        "nchar",                   
        "newid",                   
        "NOCHECK",                 
        "NONCLUSTERED",            
        "NOT",                     
        "NULL",                    
        "NULLIF",                  
        "numeric",                 
        "object_id",               
        "object_name",             
        "objectproperty",          
        "OF",                      
        "OFF",                     
        "OFFSETS",                 
        "ON",                      
        "ONCE",                    
        "ONLY",                    
        "OPEN",                    
        "OPENDATASOURCE",          
        "OPENQUERY",               
        "OPENROWSET",              
        "OPTION",                  
        "OR",
        "ORDER",                   
        "OUTER",                   
        "OVER",                    
        "parsename",               
        "patindex",                
        "PERCENT",                 
        "PERM",                    
        "PERMANENT",               
        "permissions",             
        "pi",                      
        "PIPE",                    
        "PLAN",                    
        "power",                   
        "PRECISION",               
        "PREPARE",                 
        "PRIMARY",                 
        "PRINT",                   
        "PRIVILEGES",              
        "PROC",                    
        "PROCEDURE",               
        "PROCESSEXIT",
        "PUBLIC",                  
        "quotename",               
        "radians",                 
        "RAISERROR",               
        "rand",                    
        "READ",                    
        "READTEXT",                
        "real",                    
        "RECONFIGURE",             
        "REFERENCES",              
        "REPEATABLE",              
        "replace",                 
        "replicate",               
        "REPLICATION",             
        "RESTORE",                 
        "RESTRICT",                
        "RETURN",
        "reverse",                 
        "REVOKE",                  
        "RIGHT",                   
        "ROLLBACK",                
        "ROLLUP",                  
        "round",                   
        "ROWCOUNT",                
        "ROWGUIDCOL",              
        "rtrim",                   
        "RULE",                    
        "SAVE",                    
        "SCHEMA",                  
        "SCROLL",                  
        "SELECT",                  
        "SERIALIZABLE",            
        "SESSION_USER",            
        "SET",                     
        "SETUSER",                 
        "SHUTDOWN",                
        "sin",                     
        "smalldatetime",           
        "smallint",                
        "smallmoney",
        "SOME",                    
        "soundex",                 
        "space",                   
        "sqrt",                    
        "square",                  
        "STATISTICS",              
        "stats_date",              
        "str",                     
        "stuff",                   
        "substring",               
        "SUM",                     
        "suser_id",                
        "suser_name",              
        "suser_sid",               
        "suser_sname",             
        "SYSTEM_USER",             
        "TABLE",                   
        "tan",                     
        "TAPE",                    
        "TEMP",                    
        "TEMPORARY",
        "text",                    
        "textptr",                 
        "TEXTSIZE",                
        "textvalid",               
        "THEN",                    
        "timestamp",               
        "tinyint",                 
        "TO",                      
        "TOP",                     
        "TRAN",                    
        "TRANSACTION",             
        "TRIGGER",                 
        "TRUNCATE",                
        "TSEQUAL",                 
        "typeproperty",            
        "UNCOMMITTED",             
        "unicode",
        "UNION",                   
        "UNIQUE",                  
        "UPDATE",                  
        "UPDATETEXT",              
        "upper",                   
        "USE",                     
        "USER",                    
        "user_id",                 
        "user_name",               
        "VALUES",                  
        "varbinary",               
        "varchar",                 
        "VARYING",                 
        "VIEW",                    
        "WAITFOR",                 
        "WHEN",                    
        "WHERE",                   
        "WHILE",                   
        "WITH",                    
        "WORK",                    
        "WRITETEXT",               
        "year",                    
        };


char lfc_sybsql[] = "";
char ifc_sybsql[] = "_@#";
char inc_sybsql[] = "_@#$";

int kwc_sybsql = 194;
//struct keywordrec kwl_sybsql[ ] = {
char *kws_sybsql[ 194 ] =  {
        "ADD",                        
        "ALL",                        
        "ALTER",                      
        "AND",                        
        "ANY",                        
        "ARITH_OVERFLOW",             
        "AS",                         
        "ASC",                        
        "AT",                         
        "AUTHORIZATION",              
        "AVG",                        
        "BEGIN",                      
        "BETWEEN",                    
        "bit",                        
        "BREAK",                      
        "BROWSE",                     
        "BULK",                       
        "BY",                         
        "CASCADE",                    
        "char",                       
        "CHAR_CONVERT",               
        "CHECK",                      
        "CHECKPOINT",                 
        "CLOSE",                      
        "CLUSTERED",                  
        "COMMIT",                     
        "COMPUTE",                    
        "CONFIRM",                    
        "CONSTRAINT",                 
        "CONTINUE",                   
        "CONTROLROW",                 
        "CONVERT",                    
        "COUNT",                      
        "CREATE",                     
        "CURRENT",                    
        "CURSOR",                     
        "DATABASE",                   
        "datetime",                   
        "DBCC",                       
        "DEALLOCATE",                 
        "dec",                        
        "decimal",                    
        "DECLARE",                    
        "DEFAULT",                    
        "DELETE",                     
        "DESC",                       
        "DISK",                       
        "DISTINCT",                   
        "DOUBLE",                     
        "DUMMY",                      
        "DUMP",                       
        "ELSE",                       
        "END",                        
        "ENDTRAN",                    
        "ERRLVL",                     
        "ERRORDATA",                  
        "ERROREXIT",                  
        "ESCAPE",                     
        "EXCEPT",                     
        "EXEC",                       
        "EXECUTE",                    
        "EXISTS",                     
        "EXIT",                       
        "FETCH",                      
        "FILLFACTOR",                 
        "float",                      
        "FOR",                        
        "FOREIGN",                    
        "FROM",                       
        "GO",
        "GOTO",                       
        "GRANT",                      
        "GROUP",                      
        "HAVING",                     
        "HOLDLOCK",                   
        "IDENTITY_INSERT",            
        "IF",                         
        "image",                      
        "IN",                         
        "INDEX",                      
        "INSERT",                     
        "int",                        
        "integer",                    
        "INTERSECT",                  
        "INTO",                       
        "IS",                         
        "ISOLATION",                  
        "KEY",                        
        "KILL",                       
        "LEVEL",                      
        "LIKE",                       
        "LINENO",                     
        "LOAD",                       
        "MAX",                        
        "MAX_ROWS_PER_PAGE",          
        "MIN",                        
        "MIRROR",                     
        "MIRROREXIT",                 
        "money",                      
        "NATIONAL",                   
        "nchar",                      
        "NOHOLDLOCK",                 
        "NONCLUSTERED",               
        "NOT",                        
        "NULL",                       
        "numeric",                    
        "NUMERIC_TRANSACTION",        
        "nvarchar",                   
        "OF",                         
        "OFF",                        
        "OFFSETS",                    
        "ON",                         
        "ONCE",                       
        "ONLINE",                     
        "ONLY",                       
        "OPEN",                       
        "OPTION",                     
        "OR",                         
        "ORDER",                      
        "OVER",                       
        "PARTITION",                  
        "PERM",                       
        "PERMANENT",                  
        "PLAN",                       
        "PRECISION",                  
        "PREPARE",                    
        "PRIMARY",                    
        "PRINT",                      
        "PRIVILEGES",                 
        "PROC",                       
        "PROCEDURE",                  
        "PROCESSEXIT",                
        "PUBLIC",                     
        "RAISERROR",                  
        "READ",                       
        "READTEXT",                   
        "real",                       
        "RECONFIGURE",                
        "REFERENCES",                 
        "REPLACE",                    
        "RETURN",                     
        "REVOKE",                     
        "ROLE",                       
        "ROLLBACK",                   
        "ROWCOUNT",                   
        "ROWS",                       
        "RULE",                       
        "SAVE",                       
        "SCHEMA",                     
        "SELECT",                     
        "SET",                        
        "SETUSER",                    
        "SHARED",                     
        "SHUTDOWN",                   
        "smalldatetime",              
        "smallint",                   
        "smallmoney",                 
        "SOME",                       
        "STATISTICS",                 
        "STRIPE",                     
        "SUM",                        
        "SYB_IDENTITY",               
        "SYB_RESTREE",                
        "TABLE",                      
        "TEMP",                       
        "TEMPORARY",                  
        "text",                       
        "TEXTSIZE",                   
        "tinyint",                    
        "TO",                         
        "TRAN",                       
        "TRANSACTION",                
        "TRIGGER",                    
        "TRUNCATE",                   
        "TSEQUAL",                    
        "UNION",                      
        "UNIQUE",                     
        "UNPARTITION",                
        "UPDATE",                     
        "USE",                        
        "USER",                       
        "USER_OPTION",                
        "USING",                      
        "VALUES",                     
        "varbinary",                  
        "varchar",                    
        "VARYING",                    
        "VIEW",                       
        "WAITFOR",                    
        "WHERE",                      
        "WHILE",                      
        "WITH",                       
        "WORK",                       
        "WRITETEXT",                          
        };





crma_ini( )  {
        int n;

        //init_getkeyword_c( &kwc_c, kwl_c );
        IniGetAssosiations( &asscnt, asslst );
        assocdef = asslst[ 0 ].lng;

        for( n = 0; n < kwc_c; ++n )
                kwl_c[ n ].wlen = strlen( kwl_c[ n ].word );
        for( n = 0; n < kwc_java; ++n )
                kwl_java[ n ].wlen = strlen( kwl_java[ n ].word );
        /*for( n = 0; n < kwc_orasql; ++n )
                kwl_orasql[ n ].wlen = strlen( kwl_orasql[ n ].word );*/
        /*for( n = 0; n < kwc_mssql; ++n )
                kwl_mssql[ n ].wlen = strlen( kwl_mssql[ n ].word );*/
        /*for( n = 0; n < kwc_sybsql; ++n )
                kwl_sybsql[ n ].wlen = strlen( kwl_sybsql[ n ].word );*/
        for( n = 0; n < kwc_html; ++n )
                kwl_html[ n ].wlen = strlen( kwl_html[ n ].word );
        for( n = 0; n < kwc_vbs; ++n )
                kwl_vbs[ n ].wlen = strlen( kwl_vbs[ n ].word );
        for( n = 0; n < kwc_jvs; ++n )
                kwl_jvs[ n ].wlen = strlen( kwl_jvs[ n ].word );
        }


int frescrn = -1;

crma_file( struct editrec *ed )  {
        char *s;
        int n;

        if( ! *ed->fname )  {
                ed->assoc = asslst[ 0 ].lng;
                }
        else if(( s = strrchr( ed->fname, '.' )) != NULL )  {
                ++s;
                for( n = 2; n < asscnt; ++n )  {
 
                        if( _stricmp( s, asslst[ n ].fnx ) == 0 )  {    // Check for CE_LEN_FILENEXT
                                ed->assoc = asslst[ n ].lng;
                                break;
                                }
                        }

                if( n == asscnt )
                        ed->assoc = CE_ASC_NONE;
                }
        else    {
                ed->assoc = asslst[ 1 ].lng;
                }
        crma_lines( ed->firstline );
        }



crma_lines( struct linerec *ln )  {

        if( ! crmacode )  return(-1);
 
        while( ln != NULL )  {
                crma_line( ln );
                ln = ln->next;
                }
        }

struct linerec *lnstrd;     // to circumvent recursion of crma_line calls for strd blocks that might cause stack overflow crashes

crma_line( struct linerec *ln )  {

        frescrn = 0;
        if( ! crmacode )  return( 0 );
        while( 1 )  {
                lnstrd = NULL;
                crma_line_( ln );
                if( lnstrd == NULL )  break;
                ln = lnstrd;
                }
        return( frescrn );
        }


crma_line_( struct linerec *ln )  {


        switch( ed->assoc )  {

                case CE_ASC_C:
                        //crmarcrsvc = 0;
                        crma_c( ln, kwl_c, kwc_c );
                        break;
                case CE_ASC_JAVA:
                        //crmarcrsvc = 0;
                        crma_c( ln, kwl_java, kwc_java );
                        break;
                case CE_ASC_ORASQL:
                        crma_sql( ln, kws_orasql, kwc_orasql, lfc_orasql, ifc_orasql, inc_orasql );
                        break;
                case CE_ASC_MSSQL:
                        crma_sql( ln, kws_mssql, kwc_mssql, lfc_mssql, ifc_mssql, inc_mssql );
                        break;
                case CE_ASC_SYBSQL:
                        crma_sql( ln, kws_sybsql, kwc_sybsql, lfc_sybsql, ifc_sybsql, inc_sybsql );
                        break;
                case CE_ASC_MSASP:
                        crma_htm( ln );
                        break;
                case CE_ASC_XML:
                        crma_xml( ln );
                        break;
                case CE_ASC_ASM:
                        crma_asm( ln );
                        break;
                case CE_ASC_JVS:
                        crma_c( ln, kwl_jvs, kwc_jvs );
                        break;
                case CE_ASC_VBS:
                        crma_vb( ln, kwl_vbs, kwc_vbs );
                        break;
                case CE_ASC_DOS:
                        crma_dos( ln );
                        break;
                case CE_ASC_UNIX:
                        crma_unix( ln );
                        break;
                case CE_ASC_WIN:
                        crma_win( ln );
                        break;
                case CE_ASC_NONE:
                        ln->cmnt = 0;
                        ln->atrc = 0;
                        break;
                }
        }



crma_delline( struct linerec *ln )  {

        if( ln->cmnt & 2 )  {
                if( !( ln->cmnt & 1 ))
                        cmnt_unblck( ln );
                }
        if( ln->cmnt & 4 )  {
                if( ln->cmnt & 1 )
                        cmnt_blck( ln );
                }

        if( ln->aspb & 2 )  {
                if( !( ln->aspb & 1 ))
                        asp_unblck( ln );
                }
        if( ln->aspb & 4 )  {
                if( ln->aspb & 1 )
                        asp_blck( ln );
                }

        if( ln->jvsb & 2 )  {
                if( !( ln->jvsb & 1 ))
                        jvs_unblck( ln );
                }
        if( ln->jvsb & 4 )  {
                if( ln->jvsb & 1 )
                        jvs_blck( ln );
                }
 
        if( ln->jvsc & 2 )  {
                if( !( ln->jvsc & 1 ))
                        jvsc_unblck( ln );
                }
        if( ln->jvsc & 4 )  {
                if( ln->jvsc & 1 )
                        jvsc_blck( ln );
                }

        if( ln->vbsb & 2 )  {
                if( !( ln->vbsb & 1 ))
                        vbs_unblck( ln );
                }
        if( ln->vbsb & 4 )  {
                if( ln->vbsb & 1 )
                        vbs_blck( ln );
                }

        if( ln->strb & 2 )  {
                if( !( ln->strb & 1 ))
                        strd_unblck( ln );
                }
        if( ln->strb & 4 )  {
                if( ln->strb & 1 )
                        strd_blck( ln );
                }

        if( ln->htmb & 2 )  {
                if( !( ln->htmb & 1 ))
                        htm_unblck( ln );
                }
        if( ln->htmb & 4 )  {
                if( ln->htmb & 1 )
                        htm_blck( ln );
                }
        }


crma_insline( struct linerec *ln )  {

        if( ln->cmnt & 1 )
                cmnt_blck( ln );
        else
                cmnt_unblck( ln );
        if( ln->aspb & 1 )
                asp_blck( ln );
        else
                asp_unblck( ln );
        if( ln->jvsb & 1 )
                jvs_blck( ln );
        else
                jvs_unblck( ln );
        if( ln->jvsc & 1 )
                jvsc_blck( ln );
        else
                jvsc_unblck( ln );

        if( ln->vbsb & 1 )
                vbs_blck( ln );
        else
                vbs_unblck( ln );

        if( ln->strb & 1 )
                strd_blck( ln );
        else
                strd_unblck( ln );
        if( ln->htmb & 1 )
                htm_blck( ln );
        else
                htm_unblck( ln );
        }



crma_c( struct linerec *ln, struct keywordrec kwl[], int kwc )  {
        int l, i, j, fc;
        char c, *s;
        int atrc;
        struct atrsrec atrs[ CE_MAX_CRMALINE ];

        atrc = 0;
        l = strlen( ln->strz );
        i = 0;

        if( ln->cmnt & 2 )  {
                if( !( s = strstr( ln->strz, "/*" )))  {
                        ln->cmnt &= 5;
                        if( !( ln->cmnt & 1 ))
                                cmnt_unblck( ln );
                        }
                }
        if( ln->cmnt & 4 )  {
                if( !( s = strstr( ln->strz, "*/" )))  {
                        ln->cmnt &= 3;
                        if( ln->cmnt & 1 )
                                cmnt_blck( ln );
                        }
                }
        fc = 0;
        while( i < l )  {

                if( atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if(( ln->strz[ i ] == '*' )&&( ln->strz[ i + 1 ] == '/' ))  {
                        if(( ln->cmnt & 1 )&&( ! fc ))  {
                                atrc = 0;
                                addattr( atrs, &atrc, 0, i + 2, CE_CLR_COMMENT );
                                }
                        else    {
                                addattr( atrs, &atrc, i, 2, CE_CLR_NORMAL );
                                }
                        ln->cmnt |= 4;
                        cmnt_unblck( ln );
                        fc = -1;
                        i += 2;
                        continue;                                                   // 2002/01/11
                        }

                c = ln->strz[ i ];
                if( c == '/' )  {
                        if( ln->strz[ i + 1 ] == '/' )  {
                                if( s = strstr( &ln->strz[ i + 2 ], "*/" ))  {      // 2001/12/13
                                        if( ln->cmnt & 1 )  {                       // 2002/01/11
                                                atrc = 0;
                                                addattr( atrs, &atrc, 0, ( s - ln->strz ) + 2, CE_CLR_COMMENT );
                                                i = ( s - ln->strz ) + 2;
                                                ln->cmnt |= 4;
                                                cmnt_unblck( ln );
                                                fc = -1;
                                                continue;
                                                }
                                        else    {
                                                addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                                                }
                                        }
                                else    {
                                        addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                                        }
 
                                //i += 2;                                           // 2001/12/13  - 2002/01/11
                                /*if(( s = strstr( &ln->strz[ i ], "/*" )) )  {     // 2001/12/13 replaced with below
                                        cmnt_unblck( ln );
                                        }*/
                                if( ln->cmnt & 2 )  {
                                        if( ( s = strstr( &ln->strz[ i + 1 ], "/*" )))  {
                                                ln->cmnt &= 5;
                                                if( !( ln->cmnt & 1 ))
                                                        cmnt_unblck( ln );
                                                }
                                        }
                                break;
                                }
                        else if( ln->strz[ i + 1 ] == '*' )  {
                                ln->cmnt &= 5;
                                if( s = strstr( &ln->strz[ i + ( ln->cmnt & 1 ? 1 : 2 )], "*/" ))  {      // 2002/01/11 ???????????
                                        if( ln->cmnt & 1 )  {
                                                char *s1;

                                                s1 = strstr( ln->strz, "*/" );                            // 2002/01/24
                                                if( s1 < s )  {
                                                        addattr( atrs, &atrc, i, ( s - ln->strz ) + 2 - i, CE_CLR_COMMENT );
                                                        }
                                                else  {
                                                        atrc = 0;
                                                        addattr( atrs, &atrc, 0, ( s - ln->strz ) + 2, CE_CLR_COMMENT );
                                                        }
                                                }
                                        else    {
                                                addattr( atrs, &atrc, i, ( s - ln->strz ) + 2 - i, CE_CLR_COMMENT );
                                                }
                                        i = ( s - ln->strz ) + 2;
                                        ln->cmnt |= 4;
                                        if( ! strstr( &ln->strz[ i ], "/*" ))
                                                cmnt_unblck( ln );
                                        fc = -1;
                                        }
                                else    {
                                        /*if( ln->cmnt & 4 )  {                       // 2002/01/11
                                                if( !(s = strstr( &ln->strz[ i + 2 ], "* /" )))  {
                                                        //ln->cmnt &= 3;
                                                        //cmnt_unblck( ln );
                                                        }
                                                }*/
                                        addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                                        ln->cmnt |= 2;
                                        cmnt_blck( ln );
                                        break;                               
                                        }
                                continue;
                                }
                        }
                if(( isalpha( c ))||( c == '#' ))  {
                        j = i + 1;
                        while( isalnumu( ln->strz[ j ] ))
                                j += 1;

                        if( resword( kwl, kwc, &ln->strz[ i ], j - i ))  {
                                addattr( atrs, &atrc, i, j - i, CE_CLR_KEYWORD );
                                }
                        i = j;
                        continue;
                        }
                if( isdigit( c ))  {
                        j = i + 1;
                        if( c == '0' )  {
                                if(( ln->strz[ j ] | 0x20 )== 'x' )  {
                                        j += 1;
                                        while( ishexdigit( ln->strz[ j ] ))
                                                j += 1;
                                        if(( ishexdigit( ln->strz[ j - 1 ] ))&&( ln->strz[ j ] == 'L' ))
                                                j += 1;
                                        }
                                else    {
                                        while( isoctdigit( ln->strz[ j ] ))
                                                j += 1;
                                        if(( isoctdigit( ln->strz[ j - 1 ] ))&&( ln->strz[ j ] == 'L' ))
                                                j += 1;
                                        else if(  isdigit( ln->strz[ j ] ))  {
                                                i = j + 1;
                                                continue;
                                                }
                                        }
                                }
                        else  {
                                while( isdigit( ln->strz[ j ] ))
                                        j += 1;
                                if(( isdigit( ln->strz[ j - 1 ] ))&&( ln->strz[ j ] == 'L' ))
                                        j += 1;
                                }
                        addattr( atrs, &atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }

                if( c == '"' )  {
                        j = i + 1;
                        while( ln->strz[ j ] != '"' )  {
                                if( ln->strz[ j ] == '\\' )
                                        j += 1;
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                j += 1;
                                }
                        if( ln->strz[ j ] == '"' )
                                j += 1;
                        addattr( atrs, &atrc, i, j - i, CE_CLR_STRING );
                        i = j;
                        continue;
                        }

                if( c == '\'' )  {
                        j = i + 1;
                        while( ln->strz[ j ] != '\'' )  {
                                if( ln->strz[ j ] == '\\' )
                                        j += 1;
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                j += 1;
                                }
                        if( ln->strz[ j ] == '\'' )
                                j += 1;
                        addattr( atrs, &atrc, i, j - i, CE_CLR_STRING );
                        i = j;
                        continue;
                        }
                i += 1;
                }
                
        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }



crma_vb( struct linerec *ln, struct keywordrec kwl[], int kwc )  {
        int l, i, j, fc;
        char c;
        int atrc;
        struct atrsrec atrs[ CE_MAX_CRMALINE ];


        atrc = 0;
        l = strlen( ln->strz );
        i = 0;

        fc = 0;
        while( i < l )  {

                if( atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                c = ln->strz[ i ];
                if( isalpha( c ))  {
                        j = i + 1;
                        while( isalnumu( ln->strz[ j ] ))
                                j += 1;

                        if( reswordi( kwl, kwc, &ln->strz[ i ], j - i ))  {
                                addattr( atrs, &atrc, i, j - i, CE_CLR_KEYWORD );
                                }
                        i = j;
                        continue;
                        }
                if( isdigit( c ))  {
                        j = i + 1;
                        while( isdigit( ln->strz[ j ] ))
                                j += 1;
                        addattr( atrs, &atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }
                if( c == '&' )  {
                        j = i + 1;
                        if(( ln->strz[ j ] | 0x20 )== 'h' )  {          // hex number
                                j += 1;
                                if( ! ishexdigit( ln->strz[ j ] ))  {
                                        i = j;
                                        continue;
                                        }
                                while( ishexdigit( ln->strz[ j ] ))
                                        j += 1;
                                }
                        else if(( ln->strz[ j ] | 0x20 )== 'o' )  {     // octal number 
                                j += 1;
                                if( ! isoctdigit( ln->strz[ j ] ))  {      

                                        i = j;
                                        continue;
                                        }
                                while( isdigit( ln->strz[ j ] ))        // !! - will color non-octal digits as well
                                        j += 1;
                                }

                        else    {
                                if( ! isdigit( ln->strz[ j ] ))  {
                                        i = j;
                                        continue;
                                        }
                                while( isdigit( ln->strz[ j ] ))
                                        j += 1;
            
                                }
                        addattr( atrs, &atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }
 
                if( c == '"' )  {
                        j = i + 1;
                        while( ln->strz[ j ] != '"' )  {
                                if( ln->strz[ j ] == '\\' )
                                        j += 1;
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                j += 1;
                                }
                        if( ln->strz[ j ] == '"' )
                                j += 1;
                        addattr( atrs, &atrc, i, j - i, CE_CLR_STRING );
                        i = j;
                        continue;
                        }

                if( c == '\'' )  {          // vbs comment
                        addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                        i = l;
                        break;
                        }
                i += 1;
                }
                
        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }



crma_sql( struct linerec *ln, char *kws[], int kwc, char lfc[], char ifc[], char inc[] )  {
        int l, i, j, fc;
        char c, *s;
        int atrc;
        struct atrsrec atrs[ CE_MAX_CRMALINE ];

        atrc = 0;
        l = strlen( ln->strz );
        i = 0;

        if( ln->cmnt & 2 )  {
                if( !( s = strstr( ln->strz, "/*" )))  {
                        ln->cmnt &= 5;
                        if( !( ln->cmnt & 1 ))
                                cmnt_unblck( ln );
                        }
                }
        if( ln->cmnt & 4 )  {
                if( !( s = strstr( ln->strz, "*/" )))  {
                        ln->cmnt &= 3;
                        if( ln->cmnt & 1 )
                                cmnt_blck( ln );
                        }
                }
        if( ln->strb & 2 )  {
                if( !( s = strchr( ln->strz, '"' )))  {
                        ln->strb &= 5;
                        if( !( ln->strb & 1 ))
                                strd_unblck( ln );
                        }
                }
        if( ln->strb & 4 )  {
                if( !( s = strchr( ln->strz, '"' )))  {
                        ln->strb &= 3;
                        if( ln->strb & 1 )
                                strd_blck( ln );
                        }
                }
        if( ln->strb & 1 )  {
                i = crma_strd( ln, i, &atrc, atrs, 0 );
                }

        fc = 0;
        if( *lfc )  {
                if( _strnicmp( lfc, ln->strz, strlen( lfc ))== 0 )  {
                        addattr( atrs, &atrc, 0, l, CE_CLR_COMMENT );
                        if( s = strstr( &ln->strz[ i ], "*/" ))  {
                                ln->cmnt |= 4;
                                cmnt_unblck( ln );
                                }
                        if( s = strstr( &ln->strz[ i ], "/*" ))  {
                                cmnt_unblck( ln );
                                }
                        i = l;
                        }
                }

        while( i < l )  {

                if( atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' )
                                i += 1;
                        continue;
                        }

                if(( ln->strz[ i ] == '-' )&&( ln->strz[ i + 1 ] == '-' ))  {
                        if( s = strstr( &ln->strz[ i + 2 ], "*/" ))  {
                                if( ln->cmnt & 1 )  {
                                        atrc = 0;
                                        addattr( atrs, &atrc, 0, ( s - ln->strz ) + 2, CE_CLR_COMMENT );
                                        i = ( s - ln->strz ) + 2;
                                        ln->cmnt |= 4;
                                        cmnt_unblck( ln );
                                        fc = -1;
                                        continue;
                                        }
                                else    {
                                        addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                                        }
                                }
                        else    {
                                addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                                }
                        /*if( s = strstr( &ln->strz[ i ], "/*" ))  {
                                cmnt_unblck( ln );
                                }*/
                        if( ln->cmnt & 2 )  {
                                if( ( s = strstr( &ln->strz[ i + 2 ], "/*" )))  {
                                        ln->cmnt &= 5;
                                        if( !( ln->cmnt & 1 ))
                                                cmnt_unblck( ln );
                                        }
                                }
                        break;
                        }

                if(( ln->strz[ i ] == '*' )&&( ln->strz[ i + 1 ] == '/' ))  {
                        if(( ln->cmnt & 1 )&&( ! fc ))  {
                                atrc = 0;
                                addattr( atrs, &atrc, 0, i + 2, CE_CLR_COMMENT );
                                }
                        else    {
                                addattr( atrs, &atrc, i, 2, CE_CLR_NORMAL );
                                }
                        ln->cmnt |= 4;
                        cmnt_unblck( ln );
                        i += 2;
                        fc = -1;
                        }

                if(( ln->strz[ i ] == '/' )&&( ln->strz[ i + 1 ] == '*' )) {
                        ln->cmnt &= 5;
                        if( s = strstr( &ln->strz[ i + ( ln->cmnt & 1 ? 1 : 2 )], "*/" ))  {      // 2002/01/11 ???????????
                                if( ln->cmnt & 1 )  {
                                        char *s1;

                                        s1 = strstr( ln->strz, "*/" );                            // 2002/01/24
                                        if( s1 < s )  {
                                                addattr( atrs, &atrc, i, ( s - ln->strz ) + 2 - i, CE_CLR_COMMENT );
                                                }
                                        else  {
                                                atrc = 0;
                                                addattr( atrs, &atrc, 0, ( s - ln->strz ) + 2, CE_CLR_COMMENT );
                                                }
                                        }
                                else    {
                                        addattr( atrs, &atrc, i, ( s - ln->strz ) + 2 - i, CE_CLR_COMMENT );
                                        }
                                i = ( s - ln->strz ) + 2;
                                ln->cmnt |= 4;
                                if( ! strstr( &ln->strz[ i ], "/*" ))
                                        cmnt_unblck( ln );
                                fc = -1;
                                }
                        else    {
                                addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                                ln->cmnt |= 2;
                                cmnt_blck( ln );
                                break;
                                }
                        continue;
                        }

                c = ln->strz[ i ];
                if(( isalpha( c ))||( strchr( ifc, c )))  {
                        j = i + 1;
                        while(( isalnum( ln->strz[ j ] ))||( strchr( inc, ln->strz[ j ] )))  {
                                j += 1;
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                }
                        if( reswordib( kws, kwc, &ln->strz[ i ], j - i ))  {
                                addattr( atrs, &atrc, i, j - i, CE_CLR_KEYWORD );
                                }
                        i = j;
                        continue;
                        }
                if( isdigit( c ))  {
                        j = i + 1;
                        while( isdigit( ln->strz[ j ] ))
                                j += 1;
                        addattr( atrs, &atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }

                if( c == '"' )  {
                        i = crma_strd( ln, i, &atrc, atrs, -1 );
                        continue;
                        }

                if( c == '\'' )  {
                        j = i + 1;
                        while( ln->strz[ j ] != '\'' )  {
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                j += 1;
                                }
                        if( ln->strz[ j ] == '\'' )
                                j += 1;
                        addattr( atrs, &atrc, i, j - i, CE_CLR_STRING );
                        i = j;
                        continue;
                        }
                i += 1;
                }
        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }



crma_asm( struct linerec *ln )  {
        int l, i, fc;
        int atrc;
        struct atrsrec atrs[ CE_MAX_CRMALINE ];

        atrc = 0;
        l = strlen( ln->strz );
        i = 0;


        fc = 0;

        while( i < l )  {
                if( atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if(( ln->strz[ i ] == ';' )||( ln->strz[ i ] == '#' ))  {
                        addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                        break;
                        }
                i += 1;
                }
        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }



crma_dos( struct linerec *ln )  {
        int i, l;
        int atrc;
        struct atrsrec atrs[ CE_MAX_CRMALINE ];

        atrc = 0;
        l = strlen( ln->strz );
        i = 0;

        while( ln->strz[ i ] == ' ' ) 
                i += 1;

        if( ln->strz[ i ] != '\0'  )  {
                if( ln->strz[ i ] == ':' )  {
                        if( ln->strz[ i + 1 ] == ':' )
                                addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                        }
                else if( strnicmp( &ln->strz[ i ], "REM", 3 )== 0 )  {
                        addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                        }
                else if( strnicmp( &ln->strz[ i ], "@REM", 4 )== 0 )  {
                        addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                        }
                /*else if( strnicmp( &ln->strz[ i ], "ECHO", 4 )== 0 )  {
                        addattr( atrs, &atrc, i, l - i, CE_CLR_STRING );
                        }
                else if( strnicmp( &ln->strz[ i ], "@ECHO", 5 )== 0 )  {
                        addattr( atrs, &atrc, i, l - i, CE_CLR_STRING );
                        }*/
                }
        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }


crma_unix( struct linerec *ln )  {
        int i, l;
        int atrc;
        struct atrsrec atrs[ CE_MAX_CRMALINE ];

        atrc = 0;
        l = strlen( ln->strz );
        i = 0;

        while( ln->strz[ i ] == ' ' ) 
                i += 1;
       
        if( ln->strz[ i ] != '\0'  )  {
                if( ln->strz[ i ] == '#' )
                        addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                }
        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }



crma_win( struct linerec *ln )  {
        int i, l;
        int atrc;
        struct atrsrec atrs[ CE_MAX_CRMALINE ];

        atrc = 0;
        l = strlen( ln->strz );
        i = 0;

        while( ln->strz[ i ] == ' ' ) 
                i += 1;
       
        if( ln->strz[ i ] != '\0'  )  {
                if( ln->strz[ i ] == ';' )
                        addattr( atrs, &atrc, i, l - i, CE_CLR_COMMENT );
                if( ln->strz[ i ] == '[' )
                        addattr( atrs, &atrc, i, l - i, CE_CLR_KEYWORD );
                }

        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }



crma_htm( struct linerec *ln )  {
        int l, i, fc;
        char *s;
        int atrc;
        struct atrsrec atrs[ CE_MAX_CRMALINE ];

        atrc = 0;
        l = strlen( ln->strz );
        i = 0;

        if( ln->cmnt & 2 )  {
                if( !( s = strstr( ln->strz, "<!--" )))  {
                        ln->cmnt &= 5;
                        if( !( ln->cmnt & 1 ))
                                cmnt_unblck( ln );
                        }
                }
        if( ln->cmnt & 4 )  {
                if( !( s = strstr( ln->strz, "-->" )))  {
                        ln->cmnt &= 3;
                        if( ln->cmnt & 1 )
                                cmnt_blck( ln );
                        }
                }

        if( ln->aspb & 2 )  {
                if( !( s = strstr( ln->strz, "<%" )))  {
                        ln->aspb &= 5;
                        if( !( ln->aspb & 1 ))
                                asp_unblck( ln );
                        }
                }
        if( ln->aspb & 4 )  {
                if( !( s = strstr( ln->strz, "%>" )))  {
                        ln->aspb &= 3;
                        if( ln->aspb & 1 )
                                asp_blck( ln );
                        }
                }


        if( ln->jvsb & 2 )  {
                if( !( s = stristr( ln->strz, szscript )))  {
                        ln->jvsb &= 5;
                        if( !( ln->jvsb & 1 ))
                                jvs_unblck( ln );
                        }
                }
        if( ln->jvsb & 4 )  {
                if( !( s = stristr( ln->strz, szscript_ )))  {
                        ln->jvsb &= 3;
                        if( ln->jvsb & 1 )
                                jvs_blck( ln );
                        }
                }

        if( ln->vbsb & 2 )  {
                if( !( s = stristr( ln->strz, szscript )))  {
                        ln->vbsb &= 5;
                        if( !( ln->vbsb & 1 ))
                                vbs_unblck( ln );
                        }
                }
        if( ln->vbsb & 4 )  {
                if( !( s = stristr( ln->strz, szscript_ )))  {
                        ln->vbsb &= 3;
                        if( ln->vbsb & 1 )
                                vbs_blck( ln );
                        }
                }

        if( ln->strb & 2 )  {
                if( !( s = strchr( ln->strz, '"'  )))  {
                        ln->strb &= 5;
                        if( !( ln->strb & 1 ))
                                strd_unblck( ln );
                        }
                }
        if( ln->strb & 4 )  {
                if( !( s = strchr( ln->strz, '"' )))  {
                        ln->strb &= 3;
                        if( ln->strb & 1 )
                                strd_blck( ln );
                        }
                }

        if( ln->htmb & 2 )  {
                if( !( ishtmbeg( ln->strz )))  {
                        ln->htmb &= 5;
                        if( !( ln->htmb & 1 ))
                                htm_unblck( ln );
                        }
                }
        if( ln->htmb & 4 )  {
                if( !( ishtmend( ln->strz )))  {
                        ln->htmb &= 3;
                        if( ln->htmb & 1 )
                                htm_blck( ln );
                        }
                }

        if( ln->cmnt & 1 )  {
                i = crma_htm_cmnt( ln, i, &atrc, atrs, 0 );
                }
        if( ln->aspb & 1 )  {
                i = crma_asp_vbs( ln, i, &atrc, atrs, 0 );
                }
        if( ln->jvsb & 1 )  {
                i = crma_htm_jvs( ln, i, &atrc, atrs, 0 );
                }
        if( ln->vbsb & 1 )  {
                i = crma_htm_vbs( ln, i, &atrc, atrs, 0 );
                }
        if( ln->strb & 1 )  {
                i = crma_strd( ln, i, &atrc, atrs, 0 );
                }
        if( ln->htmb & 1 )  {
                i = crma_htm_tag( ln, i, &atrc, atrs, 0 );
                }

        fc = 0;
        while( i < l )  {

                if( atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if( ln->strz[ i ] == '<' )  {
                        if( ln->strz[ i + 1 ] == '%' )  {
                                i = crma_asp_vbs( ln, i, &atrc, atrs, -1 );
                                continue;
                                }

                        else if( _strnicmp( &ln->strz[ i ], szscript, 7 )== 0 )  {
                                int fvbs = 0;

                                if( stristr( &ln->strz[ i ], "VBScript" )) 
                                        fvbs = -1;
                                i = crma_htm_tag( ln, i, &atrc, atrs, -1 );
                                if( fvbs )
                                        i = crma_htm_vbs( ln, i, &atrc, atrs, -1 );
                                else
                                        i = crma_htm_jvs( ln, i, &atrc, atrs, -1 );
                                continue;
                                }

                        else if(( isalpha( ln->strz[ i + 1 ] ))||(ln->strz[ i + 1 ] == '/' ))   {
                                i = crma_htm_tag( ln, i, &atrc, atrs, -1 );
                                continue;
                                }
                        else
                                if( ln->strz[ i + 1 ] == '!' )
                                if( ln->strz[ i + 2 ] == '-' )
                                if( ln->strz[ i + 3 ] == '-' )  {
                                        i = crma_htm_cmnt( ln, i, &atrc, atrs, -1 );
                                        continue;
                                        }
                        }
                i += 1;
                }
        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }



crma_xml( struct linerec *ln )  {
        int l, i, j, fc;
        char *s;
        int atrc;
        struct atrsrec atrs[ CE_MAX_CRMALINE ];

        atrc = 0;
        l = strlen( ln->strz );
        i = 0;

        if( ln->cmnt & 2 )  {
                if( !( s = strstr( ln->strz, "<!--" )))  {
                        ln->cmnt &= 5;
                        if( !( ln->cmnt & 1 ))
                                cmnt_unblck( ln );
                        }
                }
        if( ln->cmnt & 4 )  {
                if( !( s = strstr( ln->strz, "-->" )))  {
                        ln->cmnt &= 3;
                        if( ln->cmnt & 1 )
                                cmnt_blck( ln );
                        }
                }

        if( ln->strb & 2 )  {
                if( !( s = strchr( ln->strz, '"'  )))  {
                        ln->strb &= 5;
                        if( !( ln->strb & 1 ))
                                strd_unblck( ln );
                        }
                }
        if( ln->strb & 4 )  {
                if( !( s = strchr( ln->strz, '"' )))  {
                        ln->strb &= 3;
                        if( ln->strb & 1 )
                                strd_blck( ln );
                        }
                }

        if( ln->htmb & 2 )  {
                if( !( ishtmbeg( ln->strz )))  {
                        ln->htmb &= 5;
                        if( !( ln->htmb & 1 ))
                                htm_unblck( ln );
                        }
                }
        if( ln->htmb & 4 )  {
                if( !( ishtmend( ln->strz )))  {
                        ln->htmb &= 3;
                        if( ln->htmb & 1 )
                                htm_blck( ln );
                        }
                }

        if( ln->cmnt & 1 )  {
                i = crma_htm_cmnt( ln, i, &atrc, atrs, 0 );
                }
        if( ln->strb & 1 )  {
                i = crma_strd( ln, i, &atrc, atrs, 0 );
                }
        if( ln->htmb & 1 )  {
                i = crma_xml_tag( ln, i, &atrc, atrs, 0 );
                }

        fc = 0;
        while( i < l )  {

                if( atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if( ln->strz[ i ] == '<' )  {
                        if( ln->strz[ i + 1 ] == '?' )  {
                                j = i;
                                while( ln->strz[ j ] != '>' )  {
                                        if( ln->strz[ j ] == '\0' )
                                                break;
                                        j += 1;
                                        }
                                if( ln->strz[ j ] == '>' )  j += 1;
                                addattr( &atrs, &atrc, i, j - i, CE_CLR_XML_TAGDEL );
                                i = j;
                                }
                        else if(( isalpha( ln->strz[ i + 1 ] ))||(ln->strz[ i + 1 ] == '/' ))   {
                                i = crma_xml_tag( ln, i, &atrc, atrs, -1 );
                                continue;
                                }
                        else
                                if( ln->strz[ i + 1 ] == '!' )
                                if( ln->strz[ i + 2 ] == '-' )
                                if( ln->strz[ i + 3 ] == '-' )  {
                                        i = crma_htm_cmnt( ln, i, &atrc, atrs, -1 );
                                        continue;
                                        }
                        }
                i += 1;
                }
        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }




crma_htm_cmnt( struct linerec *ln, int lc, int *atrc, struct atrsrec atrs[], int fn )  {
        int l, i, j;

        l = strlen( ln->strz );
        i = lc;

        if( fn )  {
                ln->cmnt &= 5;
                addattr( atrs, atrc, i, 4, CE_CLR_HTML_TAGDEL );
                i += 4;
                }
        j = i;
        while( i < l )  {
                if( *atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if( ln->strz[ i ] == '-' )
                if( ln->strz[ i + 1 ] == '-' )
                if( ln->strz[ i + 2 ] == '>' )  {
                        addattr( atrs, atrc, j, i - j, CE_CLR_COMMENT );
                        addattr( atrs, atrc, i, 3, CE_CLR_HTML_TAGDEL );
                        i += 3;
                        ln->cmnt |= 4;
                        if( !( strstr( &ln->strz[ i ], "<!--" )))
                                cmnt_unblck( ln );
                        return(i);
                        }
                i += 1;
                }
        if( fn )  {
                ln->cmnt |= 2;
                addattr( atrs, atrc, j, i - j, CE_CLR_COMMENT );
                cmnt_blck( ln );
                return(i);
               }
        return(lc);
        }


crma_htm_jvsc( struct linerec *ln, int lc, int *atrc, struct atrsrec atrs[], int fn )  {
        int l, i, j;

        l = strlen( ln->strz );
        i = lc;

        j = i;
        while( i < l )  {
                if( *atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if( ln->strz[ i ] == '*' )
                if( ln->strz[ i + 1 ] == '/' )  {
                        i += 2;
                        addattr( atrs, atrc, j, i - j, CE_CLR_COMMENT );
                        ln->jvsc |= 4;
                        if( !( strstr( &ln->strz[ i ], "/*" )))
                                jvsc_unblck( ln );
                        return(i);
                        }
                if( _strnicmp( &ln->strz[ i ], szscript_, 8 )== 0 )  {
                        addattr( atrs, atrc, j, i - j, CE_CLR_COMMENT );
                        ln->jvsc |= 4;
                        return(i);
                        }

                i += 1;
                }
        addattr( atrs, atrc, j, i - j, CE_CLR_COMMENT );
        return(i);
        }



crma_strd( struct linerec *ln, int lc, int *atrc, struct atrsrec atrs[], int fn )  {
        int l, i, j;

        l = strlen( ln->strz );
        j = i = lc;
        if( fn )  {
                ln->strb &= 5;
                i += 1;
                }
        while( i < l )  {
                if( *atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if( ed->assoc == CE_ASC_MSASP )  {
                        if(( ln->strz[ i ] == '<' )&&( ln->strz[ i + 1 ] == '%' ))  {
                                addattr( atrs, atrc, j, i - j, CE_CLR_STRING );
                                i = crma_asp_vbs( ln, i, atrc, atrs, -1 );
                                j = i;
                                continue;
                                }
                        }
                if( ln->strz[ i ] == '"' )  {
                        i += 1;
                        addattr( atrs, atrc, j, i - j, CE_CLR_STRING );
                        ln->strb |= 4;
                        strd_unblck( ln );
                        return(i);
                        }
                i += 1;
                }
        addattr( atrs, atrc, j, i - j, CE_CLR_STRING );
        if( fn )  {
                ln->strb |= 2;
                strd_blck( ln );
               }
        return(i);
        }



crma_htm_tag( struct linerec *ln, int lc, int *atrc, struct atrsrec atrs[], int fn )  {
        int l, i, j, fb = 0;

        l = strlen( ln->strz );
        j = i = lc;

        if( fn )  {
                fb = -1;
 
                j = i + 1;
                if( ln->strz[ j ] == '/' )  {
                        j += 1;
                        }
                addattr( atrs, atrc, i, j - i, CE_CLR_HTML_TAGDEL );

                i = j;
                while( isalpha( ln->strz[ j ] ))
                        j += 1;
 
                if( reswordi( kwl_html, kwc_html, &ln->strz[ i ], j - i ))  {
                        addattr( atrs, atrc, i, j - i, CE_CLR_HTML_TAGNAME );
                        }
                i = j;
                }
        while( i < l )  {

                if( *atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if( ln->strz[ i ] == '>' )  {
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_HTML_TAGTEXT );
                        if(( fb )||( ln->htmb & 1 ))
                                addattr( atrs, atrc, i, 1, CE_CLR_HTML_TAGDEL );
                        else
                                addattr( atrs, atrc, i, 1, CE_CLR_ASP_SCRIPTDEL );
                        i += 1;
 
                        ln->htmb |= 4;
                        if( !( ishtmbeg( &ln->strz[ i ] )))
                                htm_unblck( ln );
                        return(i);
                        }

                if(( ln->strz[ i ] == '<' )&&(ln->strz[ i + 1 ] == '%' ))  {
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_HTML_TAGTEXT );
                        i = j = crma_asp_vbs( ln, i, atrc, atrs, -1 );
                        continue;
                        }

                if(( isdigit( ln->strz[ i ] ))||( ln->strz[ i ] == '#' ))  {
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_HTML_TAGTEXT );
                        j = i + 1;

                        if( ln->strz[ i ] == '#' )
                                while( ishexdigit( ln->strz[ j ] ))
                                        j += 1;
                        else
                                while( isdigit( ln->strz[ j ] ))
                                        j += 1;
                        addattr( atrs, atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }
 
                if( ln->strz[ i ] == '"' )  {
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_HTML_TAGTEXT );
                        i = j = crma_strd( ln, i, atrc, atrs, -1 );
                        continue;
                        }

                if( ln->strz[ i ] == '=' )  { 
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_HTML_TAGTEXT );
                        addattr( atrs, atrc, i, 1, CE_CLR_HTML_TAGOPER );
                        j = ( i += 1 );
                        continue;
                        }

                i += 1;
                }
        if( fn )  {
                ln->htmb |= 2;
                htm_blck( ln );
                }
 
        return(i);
        }



crma_xml_tag( struct linerec *ln, int lc, int *atrc, struct atrsrec atrs[], int fn )  {
        int l, i, j, fb;

        l = strlen( ln->strz );
        j = i = lc;

        if( fn )  {
                fb = -1;
 
                j = i + 1;
                if( ln->strz[ j ] == '/' )  {
                        j += 1;
                        }
                addattr( atrs, atrc, i, j - i, CE_CLR_XML_TAGDEL );

                i = j;
                while( isalpha( ln->strz[ j ] ))
                        j += 1;
 
                addattr( atrs, atrc, i, j - i, CE_CLR_XML_TAGNAME );
                i = j;
                }
        while( i < l )  {

                if( *atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if(( ln->strz[ i ] == '/' )&&( ln->strz[ i + 1 ] == '>' ))  {
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_HTML_TAGTEXT );
                        addattr( atrs, atrc, i, 2, CE_CLR_HTML_TAGDEL );
                        i += 2;
                        ln->htmb |= 4;
                        if( !( ishtmbeg( &ln->strz[ i ] )))
                                htm_unblck( ln );
                        return(i);
                        }

                if( ln->strz[ i ] == '>' )  {
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_XML_TAGTEXT );
                        addattr( atrs, atrc, i, 1, CE_CLR_XML_TAGDEL );
                        i += 1;
                        ln->htmb |= 4;
                        if( !( ishtmbeg( &ln->strz[ i ] )))
                                htm_unblck( ln );
                        return(i);
                        }

                if(( isdigit( ln->strz[ i ] ))||( ln->strz[ i ] == '#' ))  {
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_XML_TAGTEXT );
                        j = i + 1;

                        if( ln->strz[ i ] == '#' )
                                while( ishexdigit( ln->strz[ j ] ))
                                        j += 1;
                        else
                                while( isdigit( ln->strz[ j ] ))
                                        j += 1;
                        addattr( atrs, atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }
 
                if( ln->strz[ i ] == '"' )  {
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_XML_TAGTEXT );
                        i = j = crma_strd( ln, i, atrc, atrs, -1 );
                        continue;
                        }

                if( ln->strz[ i ] == '=' )  { 
                        if( i > j )  addattr( atrs, atrc, j, i - j, CE_CLR_XML_TAGTEXT );
                        addattr( atrs, atrc, i, 1, CE_CLR_XML_TAGOPER );
                        j = ( i += 1 );
                        continue;
                        }

                i += 1;
                }
        if( fn )  {
                ln->htmb |= 2;
                htm_blck( ln );
                }
 
        return(i);
        }



crma_asp_vbs( struct linerec *ln, int lc, int *atrc, struct atrsrec atrs[], int fn )  {
        int l, i, j;
        char c, *s;

        l = strlen( ln->strz );
        i = lc;
 
        if( fn )  {
                ln->aspb &= 5;
                addattr( atrs, atrc, i, 2, CE_CLR_ASP_SCRIPTDEL );
                i += 2;
                }

        while( i < l )  {

                if( *atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if(( ln->strz[ i ] == '%' )&&( ln->strz[ i+1 ] == '>' ))  {
                        addattr( atrs, atrc, i, 2, CE_CLR_ASP_SCRIPTDEL );
                        i += 2;

                        ln->aspb |= 4;
                        if( !( s = strstr( &ln->strz[ i ], "<%" )))
                                asp_unblck( ln );
                        return(i);
                        }

                c = ln->strz[ i ];
                if( isalpha( c ))  {
                        j = i + 1;
                        while( isalnumu( ln->strz[ j ] ))
                                j += 1;

                        if( reswordi( kwl_vbs, kwc_vbs, &ln->strz[ i ], j - i ))  {
                                addattr( atrs, atrc, i, j - i, CE_CLR_KEYWORD );
                                }
                        i = j;
                        continue;
                        }
                if( isdigit( c ))  {
                        j = i + 1;
                        while( isdigit( ln->strz[ j ] ))
                                j += 1;
                        addattr( atrs, atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }
                if( c == '&' )  {
                        j = i + 1;
                        if(( ln->strz[ j ] | 0x20 )== 'h' )  {          // hex number
                                j += 1;
                                if( ! ishexdigit( ln->strz[ j ] ))  {
                                        i = j;
                                        continue;
                                        }
                                while( ishexdigit( ln->strz[ j ] ))
                                        j += 1;
                                }
                        else if(( ln->strz[ j ] | 0x20 )== 'o' )  {     // octal number 
                                j += 1;
                                if( ! isoctdigit( ln->strz[ j ] ))  {      

                                        i = j;
                                        continue;
                                        }
                                while( isdigit( ln->strz[ j ] ))        // !! - will color non-octal digits as well
                                        j += 1;
                                }

                        else    {
                                if( ! isdigit( ln->strz[ j ] ))  {
                                        i = j;
                                        continue;
                                        }
                                while( isdigit( ln->strz[ j ] ))
                                        j += 1;
            
                                }
                        addattr( atrs, atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }

                if( c == '"' )  {
                        j = i + 1;
                        while( ln->strz[ j ] != '"' )  {
                                if( ln->strz[ j ] == '\\' )
                                        j += 1;
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                j += 1;
                                }
                        if( ln->strz[ j ] == '"' )
                                j += 1;
                        addattr( atrs, atrc, i, j - i, CE_CLR_STRING );
                        i = j;
                        continue;
                        }

                if( c == '\'' )  {          // vbs comment
                        if( ( s = strstr( &ln->strz[ i ], "%>" )))  {
                                j = s - &ln->strz[ i ];
                                addattr( atrs, atrc, i, j, CE_CLR_COMMENT );
                                i += j;
                                addattr( atrs, atrc, i, 2, CE_CLR_ASP_SCRIPTDEL );
                                i += 2;
                                ln->aspb |= 4;
                                if( !( s = strstr( &ln->strz[ i ], "<%" )))
                                        asp_unblck( ln );
                                return(i);
                                }
                        else    {
                                addattr( atrs, atrc, i, l - i, CE_CLR_COMMENT );
                                i = l;
                                break;
                                }
                        }
                i += 1;
                }
        if( fn )  {
                ln->aspb |= 2;
                asp_blck( ln );
                }
        return(i);
        }



crma_htm_jvs( struct linerec *ln, int lc, int *atrc, struct atrsrec atrs[], int fn )  {
        int l, i, j, fc;
        char c, *s;

        l = strlen( ln->strz );
        i = lc;
 
        if( fn )  {
                ln->jvsb &= 5;
                }

        if( ln->jvsc & 2 )  {
                if( !( s = strstr( ln->strz, "/*" )))  {
                        ln->jvsc &= 5;
                        if( !( ln->jvsc & 1 ))
                                jvsc_unblck( ln );
                        }
                }
        if( ln->jvsc & 4 )  {
                if(( !( s = strstr( ln->strz, "*/" )))&&( !( s = stristr( ln->strz, szscript_ ))))  {
                        ln->jvsc &= 3;
                        if( ln->jvsc & 1 )
                                jvsc_blck( ln );
                        }
                }
        
        if( ln->jvsc & 1 )  {
                i = crma_htm_jvsc( ln, i, atrc, atrs, 0 );
                }

        fc = 0;
        while( i < l )  {

                if( *atrc == CE_MAX_CRMALINE )
                        break;

                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' )
                                i += 1;
                        continue;
                        }

                if(( ln->strz[ i ] == '*' )&&( ln->strz[ i+1 ] == '/' ))  {
                        if(( ln->jvsc & 1 )&&( ! fc ))  {
                                *atrc = 0;
                                addattr( atrs, atrc, 0, i + 2, CE_CLR_COMMENT );
                                }
                        else    {
                                addattr( atrs, atrc, i, 2, CE_CLR_NORMAL );
                                }
                        ln->jvsc |= 4;
                        jvsc_unblck( ln );
                        i += 2;
                        fc = -1;
                        }

                c = ln->strz[ i ];
                if( c == '/' )  {
                        if( ln->strz[ i + 1 ] == '/' )  {
                                if( s = strstr( &ln->strz[ i + 2 ], "*/" ))  {      // 2001/12/13
                                        if( ln->cmnt & 1 )  {
                                                *atrc = 0;
                                                addattr( atrs, atrc, 0, ( s - ln->strz ) + 2, CE_CLR_COMMENT );
                                                i = ( s - ln->strz ) + 2;
                                                ln->jvsc |= 4;
                                                jvsc_unblck( ln );
                                                fc = -1;
                                                continue;
                                                }
                                        else    {
                                                addattr( atrs, atrc, i, l - i, CE_CLR_COMMENT );
                                                }
                                        }
                                else    {
                                        addattr( atrs, atrc, i, l - i, CE_CLR_COMMENT );
                                        }
 
                                //i += 2;                                             // 2001/12/13
                                /*if( s = strstr( &ln->strz[ i ], "/*" ))  {
                                        jvsc_unblck( ln );
                                        }*/
                                if( ln->jvsc & 2 )  {
                                        if( ( s = strstr( &ln->strz[ i + 1 ], "/*" )))  {
                                                ln->jvsc &= 5;
                                                if( !( ln->jvsc & 1 ))
                                                        jvsc_unblck( ln );
                                                }
                                        }
                                i = l;
                                break;
                                }
                        else if( ln->strz[ i + 1 ] == '*' )  {
                                ln->jvsc &= 5;
                                if( s = strstr( &ln->strz[ i + ( ln->jvsc & 1 ? 1 : 2 )], "*/" ))  {      // 2002/01/11 ???????????
                                        if( ln->jvsc & 1 )  {
                                                char *s1;

                                                s1 = strstr( ln->strz, "*/" );                            // 2002/01/24
                                                if( s1 < s )  {
                                                        addattr( atrs, atrc, i, ( s - ln->strz ) + 2 - i, CE_CLR_COMMENT );
                                                        }
                                                else  {
                                                        atrc = 0;
                                                        addattr( atrs, atrc, 0, ( s - ln->strz ) + 2, CE_CLR_COMMENT );
                                                        }
                                                }
                                        else    {
                                                addattr( atrs, atrc, i, ( s - ln->strz ) + 2 - i, CE_CLR_COMMENT );
                                                }
                                        i = ( s - ln->strz ) + 2;
                                        ln->jvsc |= 4;
                                        if( ! strstr( &ln->strz[ i ], "/*" ))
                                                jvsc_unblck( ln );
                                        fc = -1;
                                        }
                                else    {
                                        addattr( atrs, atrc, i, l - i, CE_CLR_COMMENT );
                                        ln->jvsc |= 2;
                                        jvsc_blck( ln );
                                        break;
                                        }
                                continue;
                                }
                        }

                if( _strnicmp( &ln->strz[ i ], szscript_, 8 )== 0 )  {
                        i = crma_htm_tag( ln, i, atrc, atrs, -1 );
                        ln->jvsb |= 4;
                        if( !( s = stristr( &ln->strz[ i ], szscript )))  {
                                jvs_unblck( ln );
                                }
                        return(i);
                        }

                c = ln->strz[ i ];
                if( isalpha( c ))  {
                        j = i + 1;
                        while( isalnumu( ln->strz[ j ] ))
                                j += 1;

                        if( resword( kwl_jvs, kwc_jvs, &ln->strz[ i ], j - i ))  {
                                addattr( atrs, atrc, i, j - i, CE_CLR_KEYWORD );
                                }
                        i = j;
                        continue;
                        }
                if( isdigit( c ))  {
                        j = i + 1;
                        while( isdigit( ln->strz[ j ] ))
                                j += 1;
                        addattr( atrs, atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }
 
                if( c == '"' )  {
                        j = i + 1;
                        while( ln->strz[ j ] != '"' )  {
                                if( ln->strz[ j ] == '\\' )
                                        j += 1;
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                j += 1;
                                }
                        if( ln->strz[ j ] == '"' )
                                j += 1;
                        addattr( atrs, atrc, i, j - i, CE_CLR_STRING );
                        i = j;
                        continue;
                        }
                
                /*if(( ln->strz[ i ] == '/' )&&( ln->strz[ i + 1 ] == '/' ))  {   
                        addattr( atrs, atrc, i, l - i, CE_CLR_COMMENT );
                        i = l;
                        break;
                        }*/

                if(( ln->strz[ i ] == '<' )&&(ln->strz[ i + 1 ] == '%' ))  {
                        i = crma_asp_vbs( ln, i, atrc, atrs, -1 );
                        continue;
                        }

                i += 1;
                }
        if( fn )  {
                ln->jvsb |= 2;
                jvs_blck( ln );
                }
        return(i);
        }



crma_htm_vbs( struct linerec *ln, int lc, int *atrc, struct atrsrec atrs[], int fn )  {
        int l, i, j;
        char c, *s;

        l = strlen( ln->strz );
        i = lc;
 
        if( fn )  {
                ln->vbsb &= 5;
                }

        while( i < l )  {

                if( *atrc == CE_MAX_CRMALINE )
                        break;
                
                if( ln->strz[ i ] == ' ' )  {
                        while( ln->strz[ i ] == ' ' ) 
                                i += 1;
                        continue;
                        }

                if( _strnicmp( &ln->strz[ i ], szscript_, 8 )== 0 )  {
                        i = crma_htm_tag( ln, i, atrc, atrs, -1 );

                        ln->vbsb |= 4;
                        if( !( s = stristr( &ln->strz[ i ], szscript )))     
                                vbs_unblck( ln );
                        return(i);
                        }

                c = ln->strz[ i ];
                if( isalpha( c ))  {
                        j = i + 1;
                        while( isalnumu( ln->strz[ j ] ))
                                j += 1;

                        if( reswordi( kwl_vbs, kwc_vbs, &ln->strz[ i ], j - i ))  {
                                addattr( atrs, atrc, i, j - i, CE_CLR_KEYWORD );
                                }
                        i = j;
                        continue;
                        }
                if( isdigit( c ))  {
                        j = i + 1;
                        while( isdigit( ln->strz[ j ] ))
                                j += 1;
                        addattr( atrs, atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }
                if( c == '&' )  {
                        j = i + 1;
                        if(( ln->strz[ j ] | 0x20 )== 'h' )  {          // hex number
                                j += 1;
                                if( ! ishexdigit( ln->strz[ j ] ))  {
                                        i = j;
                                        continue;
                                        }
                                while( ishexdigit( ln->strz[ j ] ))
                                        j += 1;
                                }
                        else if(( ln->strz[ j ] | 0x20 )== 'o' )  {     // octal number 
                                j += 1;
                                if( ! isoctdigit( ln->strz[ j ] ))  {      

                                        i = j;
                                        continue;
                                        }
                                while( isdigit( ln->strz[ j ] ))        // !! - will color non-octal digits as well
                                        j += 1;
                                }

                        else    {
                                if( ! isdigit( ln->strz[ j ] ))  {
                                        i = j;
                                        continue;
                                        }
                                while( isdigit( ln->strz[ j ] ))
                                        j += 1;
            
                                }
                        addattr( atrs, atrc, i, j - i, CE_CLR_NUMBER );
                        i = j;
                        continue;
                        }
 
                if( c == '"' )  {
                        j = i + 1;
                        while( ln->strz[ j ] != '"' )  {
                                if( ln->strz[ j ] == '\\' )
                                        j += 1;
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                j += 1;
                                }
                        if( ln->strz[ j ] == '"' )
                                j += 1;
                        addattr( atrs, atrc, i, j - i, CE_CLR_STRING );
                        i = j;
                        continue;
                        }
 
                if( c == '\'' )  {
                        addattr( atrs, atrc, i, l - i, CE_CLR_COMMENT );
                        i = l;
                        break;
                        }
                i += 1;
                }
        if( fn )  {
                ln->vbsb |= 2;
                vbs_blck( ln );
                }
        return(i);
        }





cmnt_blck( struct linerec *ln )  {

        if( ln->next != NULL )  {
                if( !( ln->next->cmnt & 1 ))  {        //  if( !( ln->next->cmnt &= 1 ))  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                ln->cmnt |= 1;
                                if( ln->cmnt & 4 )  {
                                        crma_line_( ln );
                                        break;
                                        }
                                if( strstr( ln->strz,  ( ed->assoc == CE_ASC_MSASP ? "-->" : "*/" )))  {
                                        crma_line_( ln );
                                        ln->cmnt |= 4;
                                        break;
                                        }
                                }
                        frescrn = -1;
                        }
                }
        }



cmnt_unblck( struct linerec *ln )  {
 
        if( ln->next != NULL )  {
                if( ln->next->cmnt & 1 )  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                if( !( ln->cmnt & 1 ))  break;
                                ln->cmnt &= 6;
                                if( ln->cmnt & 2 )  break;
                                if( ln->cmnt & 4 )
                                        crma_line_( ln );
                                }
                        frescrn = -1;
                        }
                }
        }


jvsc_blck( struct linerec *ln )  {

        if( ln->next != NULL )  {
                if( !( ln->next->jvsc & 1 ))  {        //  if( !( ln->next->jvsc &= 1 ))  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                ln->jvsc |= 1;
                                ln->jvsb |= 1;
                                /*if( ln->jvsc & 4 )  {
                                        crma_line_( ln );
                                        break;
                                        }
                                if( strstr( ln->strz,  "* /" ))  { 
                                        crma_htm( ln );
                                        ln->jvsc |= 4;
                                        break;
                                        }
                                if( strstr( ln->strz,  szscript_ ))  { 
                                        crma_htm( ln );
                                        ln->jvsc |= 4;
                                        ln->jvsb |= 4;
                                        break;
                                        }*/

                                if(( strstr( ln->strz,  "*/" ))||( stristr( ln->strz,  szscript_ )))  
                                        ln->jvsc |= 4;
                                if( ln->jvsc & 4 )  {
                                        crma_htm( ln );
                                        break;
                                        }
                                crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }



jvsc_unblck( struct linerec *ln )  {
 
        if( ln->next != NULL )  {
                if( ln->next->jvsc & 1 )  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                //if( !( ln->jvsc & 1 ))  break;
                                //ln->jvsb &= 6;
                                //ln->jvsc &= 6;
                                //if( ln->jvsc & 2 )  break;
                                //if( ln->jvsc & 4 )
                                //        crma_htm( ln );
                                if( ln->jvsc & 1 )  {
                                        ln->jvsc &= 6;
                                        crma_htm( ln );
                                        }
                                if( ln->jvsc & 2 )  break;
                                if( ln->jvsc & 4 )
                                        crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }


strd_blck( struct linerec *ln )  {

        if( ln->next != NULL )  {
                if( !( ln->next->strb & 1 ))  {         //  if( !( ln->next->strb &= 1 ))  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                ln->strb |= 1;
                                if( strchr( ln->strz, '"' ) )
                                        ln->strb |= 4;
                                if( ln->strb & 4 )  {
                                        //crma_line_( ln );
                                        lnstrd = ln;        //  prevent recursion for strd
                                        break;
                                        }
                                crma_line_( ln );
                                }
                        frescrn = -1;
                        }
                }
        }



strd_unblck( struct linerec *ln )  {
 
        if( ln->next != NULL )  {
                if( ln->next->strb & 1 )  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                if( ln->strb & 1 )  {
                                        ln->strb &= 6;
                                        crma_line_( ln );
                                        }
                                if( ln->strb & 2 )  break;
                                if( ln->strb & 4 )
                                        crma_line_( ln );
                                }
                        frescrn = -1;
                        }
                }
        }





asp_blck( struct linerec *ln )  {

        if( ln->next != NULL )  {
                if( !( ln->next->aspb & 1 ))  {          //   if( !( ln->next->aspb &= 1 ))  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                ln->aspb |= 1;
                                if( strstr( ln->strz, "%>" ))
                                        ln->aspb |= 4;
                                if( ln->aspb & 4 )  {
                                        crma_htm( ln );
                                        break;
                                        }
                                crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }



asp_unblck( struct linerec *ln )  {
 
        if( ln->next != NULL )  {
                if( ln->next->aspb & 1 )  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                if( ln->aspb & 1 )  {
                                        ln->aspb &= 6;
                                        crma_htm( ln );
                                        }
                                if( ln->aspb & 2 )  break;
                                if( ln->aspb & 4 )
                                        crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }


jvs_blck( struct linerec *ln )  {

        if( ln->next != NULL )  {
                if( !( ln->next->jvsb & 1 ))  {           //  if( !( ln->next->jvsb &= 1 ))  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                ln->jvsb |= 1;
                                if( stristr( ln->strz, szscript_ ))
                                        ln->jvsb |= 4;
                                if( ln->jvsb & 4 )  {
                                        crma_htm( ln );
                                        break;
                                        }
                                crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }



jvs_unblck( struct linerec *ln )  {
 
        if( ln->next != NULL )  {
                if( ln->next->jvsb & 1 )  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                if( ln->jvsb & 1 )  {
                                        ln->jvsb &= 6;
                                        crma_htm( ln );
                                        }
                                if( ln->jvsb & 2 )  break;
                                if( ln->jvsb & 4 )
                                        crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }


vbs_blck( struct linerec *ln )  {

        if( ln->next != NULL )  {
                if( !( ln->next->vbsb & 1 ))  {        //    if( !( ln->next->vbsb &= 1 ))  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                ln->vbsb |= 1;
                                if( stristr( ln->strz, szscript_ ))
                                        ln->vbsb |= 4;
                                if( ln->vbsb & 4 )  {
                                        crma_htm( ln );
                                        break;
                                        }
                                crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }



vbs_unblck( struct linerec *ln )  {
 
        if( ln->next != NULL )  {
                if( ln->next->vbsb & 1 )  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                if( ln->vbsb & 1 )  {
                                        ln->vbsb &= 6;
                                        crma_htm( ln );
                                        }
                                if( ln->vbsb & 2 )  break;
                                if( ln->vbsb & 4 )
                                        crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }


ishtmbeg( char *s )  {

        while( *s )  {
                if( *s == '<' )  {
                        if(( isalpha( *( s + 1 )))||( *( s + 1 )== '/' ))
                                return(-1);
                        }
                ++s;
                }
        return( 0 );
        }


ishtmend( char *s )  {

        while( *s )  {
                if( *s == '>' )  return(-1);
                if( *s == '%' )  {
                        if( *( s + 1 )== '>' )  ++s;
                        }
                ++s;
                }
        return( 0 );
        }


htm_blck( struct linerec *ln )  {

        if( ln->next != NULL )  {
                if( !( ln->next->htmb & 1 ))  {        // if( !( ln->next->htmb &= 1 ))  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                ln->htmb |= 1;
                                if( ishtmend( ln->strz ))
                                        ln->htmb |= 4;
                                if( ln->htmb & 4 )  {
                                        crma_htm( ln );
                                        break;
                                        }
                                //crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }



htm_unblck( struct linerec *ln )  {
 
        if( ln->next != NULL )  {
                if( ln->next->htmb & 1 )  {
                        while( ln->next != NULL )  {
                                ln = ln->next;
                                if( ln->htmb & 1 )  {
                                        ln->htmb &= 6;
                                        crma_htm( ln );
                                        }
                                if( ln->htmb & 2 )  break;
                                if( ln->htmb & 4 )
                                        crma_htm( ln );
                                }
                        frescrn = -1;
                        }
                }
        }






resword( struct keywordrec kwl[], int kwc, char *s, int wl )  {     // !!!!! fix for binary search
        int wc;
        int r;

        for( wc = 0; wc < kwc; ++wc )  {
                if( wl == kwl[ wc ].wlen )  {
                        if(( r = strncmp( s, kwl[ wc ].word, wl )) == 0 )
                                return(-1);
                        if( r < 0 )  {
                                return(0);
                                }
                        }
                }
        return(0);
        }


reswordi( struct keywordrec kwl[], int kwc, char *s, int wl )  {   // sequential search with length check
        int wc;
        int r;

        for( wc = 0; wc < kwc; ++wc )  {
                if( wl == kwl[ wc ].wlen )  {
                        if(( r = _strnicmp( s, kwl[ wc ].word, wl )) == 0 )
                                return(-1);
                        if( r < 0 )  {
                                return(0);
                                }
                        }
                }
        return(0);
        }



reswordib( char *kws[], int kwc, char *s, int wl )  {     // binary search, more appropriate for SQL
        int kwi;
        int r;
        int d, z = 0;   // havw to initialize z to something ???
        char w[ CE_LEN_KEYWORD + 1 ];

        if( wl > CE_LEN_KEYWORD )  return 0;
        strncpy( w, s, wl );
        w[ wl ] = '\0';
        d = kwc / 2;
        kwi = d;
        while( 1 )  {
                if( d > 1 )  {
                        d /= 2;

                        if(( r = stricmp( w, kws[ kwi ] )) == 0 )
                                return(-1);
                        if( r < 0 )  {
                                z = -1;
                                kwi -= d;
                                }
                        else if( r > 0 )  {
                                z = 1;
                                kwi += d;
                                }
                        }
                else   {
                        if(( r = stricmp( w, kws[ kwi ] )) == 0 )
                                return(-1);
                        if( r < 0 )   {
                                if( z == 1 )  return(0);
                                }
                        else    {
                                if( z == -1 )  return(0);
                                }
                        kwi += z;
                        }
                if( kwi < 0 )  break;
                if( kwi >= kwc )  break;
                }
        return(0);
        }


addattr( struct atrsrec atrs[], int *atrc, int pos, int cnt, int atr )  {

        atrs[ *atrc ].pos = pos;
        atrs[ *atrc ].cnt = cnt;
        atrs[ *atrc ].atr = atr;
        *atrc += 1;
}




        /*lnb = ln;
        while( lnb->prev != NULL )  {
                int l, q, t = 0, tf = 0;

                lnb = lnb->prev;
                if( lnb->cmnt & 1 )  continue;
                l = strlen( lnb->strz );

                q = 0;
                while( q < l )  {
                        if( lnb->strz[ q ] == '<' )  {
                                if( lnb->strz[ q + 1 ] != '%' )  {
                                        t = -1;
                                        tf = 1;
                                        }
                                }
                        if( lnb->strz[ q ] == '>' )  {
                                t = 0;
                                tf = 1;
                                }
                        if( lnb->strz[ q ] == '%' )  {
                                if( lnb->strz[ q + 1 ] == '>' )  ++q;
                                }
                        q += 1;
                        }
                if( tf )  {
                        ft = t;
                        break;
                        }
                }*/



/*
                if( ln->strz[ i ] == '<' )  {
                        ft = -1;
                        j = i + 1;
                        if( ln->strz[ j ] == '/' )  j += 1;
                        k = j;
                        while(( ln->strz[ j ] != ' ' )&&( ln->strz[ j ] != '>' )&&( ln->strz[ j ] != '\0' ))
                                j += 1;

                        if( reswordi( kwl_html, kwc_html, &ln->strz[ k ], j - k ))  {
                                if( ln->strz[ j ] == '>' ) j += 1;
                                atrs[ atrc ].pos = i;
                                atrs[ atrc ].cnt = j - i;
                                atrs[ atrc ].atr = CE_CLR_KEYWORD;
                                atrc += 1;
                                }
                        i = j;
                        continue;
                        }
                if( ln->strz[ i ] == '>' )  {
                        //if( ft )  {
                                atrs[ atrc ].pos = i;
                                atrs[ atrc ].cnt = 1;
                                atrs[ atrc ].atr = CE_CLR_KEYWORD;
                                atrc += 1;
                                //}
                        //ft = 0;
                        }

*/




/*
crma_line_mssql( struct linerec *ln )  {
        int l, i, j, k, fc;
        char c, *s;

        if( ln->cmnt & 2 )  {
                if( !( s = strstr( ln->strz, "/*" )))  {
                        ln->cmnt &= 5;
                        if( !( ln->cmnt & 1 ))
                                cmnt_unblck( ln );
                        }
                }
        if( ln->cmnt & 4 )  {
                if( !( s = strstr( ln->strz, "* /" )))  {
                        ln->cmnt &= 3;
                        if( ln->cmnt & 1 )
                                cmnt_blck( ln );
                        }
                }

        atrc = 0;
        l = strlen( ln->strz );
        fc = 0;
        i = 0;

        while( i < l )  {

                if( atrc == CE_MAX_CRMALINE )
                        break;

                if(( ln->strz[ i ] == '-' )&&( ln->strz[ i + 1 ] == '-' ))  {
                        atrs[ atrc ].pos = i;
                        atrs[ atrc ].cnt = l - i;
                        atrs[ atrc ].atr = CE_CLR_COMMENT;
                        atrc += 1;
                        if( s = strstr( &ln->strz[ i ], "* /" ))  {
                                ln->cmnt |= 4;
                                cmnt_unblck( ln );
                                }
                        if( s = strstr( &ln->strz[ i ], "/*" ))  {
                                //ln->cmnt |= 2;
                                cmnt_unblck( ln );
                                }
                        break;
                        }

                if(( ln->strz[ i ] == '*' )&&( ln->strz[ i + 1 ] == '/' ))  {
                        if(( ln->cmnt & 1 )&&( ! fc ))  {
                                atrc = 0;
                                atrs[ atrc ].pos = 0;
                                atrs[ atrc ].cnt = i + 2;
                                atrs[ atrc ].atr = CE_CLR_COMMENT;
                                }
                        else    {
                                atrs[ atrc ].pos = i;
                                atrs[ atrc ].cnt = 2;
                                atrs[ atrc ].atr = CE_CLR_NORMAL;
                                }
                        atrc += 1;
                        ln->cmnt |= 4;
                        cmnt_unblck( ln );
                        i += 2;
                        fc = -1;
                        }

                if(( ln->strz[ i ] == '/' )&&( ln->strz[ i + 1 ] == '*' )) {
                        k = i + 2;
                        ln->cmnt &= 5;
                        if( s = strstr( &ln->strz[ k ], "* /" ))  {
                                if( ln->cmnt & 1 )  {
                                        atrc = 0;
                                        atrs[ atrc ].pos = 0;
                                        atrs[ atrc ].cnt = ( s - ln->strz ) + 2;
                                        }
                                else    {
                                        atrs[ atrc ].pos = i;
                                        atrs[ atrc ].cnt = ( s - ln->strz ) + 2 - i;
                                        }
                                atrs[ atrc ].atr = CE_CLR_COMMENT;
                                atrc += 1;
                                i = ( s - ln->strz ) + 2;
                                ln->cmnt |= 4;
                                if( ! strstr( &ln->strz[ i ], "/*" ))
                                        cmnt_unblck( ln );
                                fc = -1;
                                }
                        else    {
                                atrs[ atrc ].pos = i;
                                atrs[ atrc ].cnt = l - i;
                                atrs[ atrc ].atr = CE_CLR_COMMENT;
                                atrc += 1;
                                ln->cmnt |= 2;
                                cmnt_blck( ln );
                                break;
                                }
                        continue;
                        }

                c = ln->strz[ i ];
                if(( isalphau( c ))||( c == '@' )||( c == '#' ))  {
                        j = i + 1;
                        while(( isalnumu( ln->strz[ j ] ))||( ln->strz[ j ] == '#' )||( ln->strz[ j ] == '$' ))
                                j += 1;

                        if( reswordi( kwl_mssql, kwc_mssql, &ln->strz[ i ], j - i ))  {
                                atrs[ atrc ].pos = i;
                                atrs[ atrc ].cnt = j - i;
                                atrs[ atrc ].atr = CE_CLR_KEYWORD;
                                atrc += 1;
                                }
                        i = j;
                        continue;
                        }
                else if( isdigit( c ))  {
                        j = i + 1;
                        while( isdigit( ln->strz[ j ] ))
                                j += 1;
                        atrs[ atrc ].pos = i;
                        atrs[ atrc ].cnt = j - i;
                        atrs[ atrc ].atr = CE_CLR_NUMBER;
                        atrc += 1;
                        i = j;
                        continue;
                        }

                if( c == '"' )  {
                        j = i + 1;
                        while( ln->strz[ j ] != '"' )  {
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                j += 1;
                                }
                        if( ln->strz[ j ] == '"' )
                                j += 1;
                        atrs[ atrc ].pos = i;
                        atrs[ atrc ].cnt = j - i;
                        atrs[ atrc ].atr = CE_CLR_STRING;
                        atrc += 1;
                        i = j;
                        continue;
                        }

                if( c == '\'' )  {
                        j = i + 1;
                        while( ln->strz[ j ] != '\'' )  {
                                if( ln->strz[ j ] == '\0' )
                                        break;
                                j += 1;
                                }
                        if( ln->strz[ j ] == '\'' )
                                j += 1;

                        atrs[ atrc ].pos = i;
                        atrs[ atrc ].cnt = j - i;
                        atrs[ atrc ].atr = CE_CLR_STRING;
                        atrc += 1;
                        i = j;
                        continue;
                        }
                i += 1;
                }
        if( ln->atrc != atrc )  {
                ln->atrs = ( struct atrsrec * )realloc( ln->atrs, sizeof( struct atrsrec )* atrc );
                if( atrc && ln->atrs == NULL )  {
                        edit_err(1);
                        return(0);
                        }
                }
        if( atrc )
                memcpy( ln->atrs, atrs, sizeof( struct atrsrec )* atrc );
        ln->atrc = atrc;
        return(-1);
        }
*/
