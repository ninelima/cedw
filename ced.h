 
#define CE_LEN_LINE             32

#define CE_LEN_FILEBUFR         4096

#define CE_LEN_TAB              8

#ifdef _WIN32
#define CE_LEN_FILENAME         100      //80      // long names max 256 ???
#define CE_LEN_FILENEXT         40      // ??
#define CE_LEN_FILEPATH         1000    //400
#define CE_LEN_MULTIFILE        2000    //800
#else
#define CE_LEN_FILENAME         13      // 8.3 file names
#define CE_LEN_FILENEXT         3
#define CE_LEN_FILEPATH         200
#define CE_LEN_MULTIFILE        400     // Multi select files
#endif

#define CE_LEN_FINDSTR          100
#define CE_LEN_FILLSTR          100


#define CE_MAX_FILES            1000    // * CE_LEN_FILENAME = 100K

#define CE_MAX_FILENHIST        40      // * CE_LEN_FILEPATH = 40K

#define CE_MAX_FINDHIST         40

#define CE_MAX_CLPBORD          10

#define CE_MAX_BOOKMARKS        40

#define CE_MAX_UNDO             200
#define CE_MAX_UNDOLINES        800

#define CE_MAX_CRMALINE         200

//#define CE_MAX_KEYWORD          100
#define CE_LEN_KEYWORD          20

#define CE_MAX_ASSOCIATIONS     200
#define CE_CNT_ASSTYPE          14
#define CE_LEN_ASSTYPE          40
#define CE_CNT_ASSFNEXT         26

#define CE_DEF_SCRNY            25
#define CE_DEF_SCRNX            80


#define CE_CNT_COLORS           15
#define CE_CNT_COLORSET         16

#define CE_CLR_NORMAL           1
#define CE_CLR_SELECT           2
#define CE_CLR_KEYWORD          3
#define CE_CLR_STRING           4
#define CE_CLR_NUMBER           5
#define CE_CLR_COMMENT          6

#define CE_CLR_ASP_SCRIPTDEL    7
#define CE_CLR_HTML_TAGNAME     8
#define CE_CLR_HTML_TAGDEL      9
#define CE_CLR_HTML_TAGTEXT     10
#define CE_CLR_HTML_TAGOPER     11
//#define CE_CLR_HTML_COMMENT     12

#define CE_CLR_XML_TAGNAME      12
#define CE_CLR_XML_TAGDEL       13
#define CE_CLR_XML_TAGTEXT      14
#define CE_CLR_XML_TAGOPER      15 
//#define CE_CLR_XML_COMMENT      16

#define CE_CLR_MATCHBRACKET     16


#define CE_CLR_STATUS           17
#define CE_CLR_STATIN           18

#define CE_CLR_UNCOMMENT        19


#define CE_ASC_NONE             0
#define CE_ASC_C                1
#define CE_ASC_JAVA             2
#define CE_ASC_ORASQL           3
#define CE_ASC_MSSQL            4
#define CE_ASC_SYBSQL           5
#define CE_ASC_MSASP            6
#define CE_ASC_XML              7
#define CE_ASC_ASM              8
#define CE_ASC_JVS              9
#define CE_ASC_VBS              10
#define CE_ASC_DOS              11
#define CE_ASC_UNIX             12
#define CE_ASC_WIN              13

//#define CE_ASC_HTML             

//#define CE_ASC_PASCAL           
//#define CE_ASC_VB               


#define CE_KEY_ESC     0x1B
#define CE_KEY_CR      0x0D
#define CE_KEY_CTRLCR  0x0A
#define CE_KEY_TAB     0x09
#define CE_KEY_BS      0x08
#define CE_KEY_CTRLBS  0x7F
#define CE_KEY_LF      0x0A
#define CE_KEY_SPACE   ' '


#define CE_CLR_C1 1  // normal text            //7            /* White on Black */
#define CE_CLR_C2 2  // selected text          //15           /* Hi White on Black */
#define CE_CLR_C3 3  // status bar text        //112          /* Black on White */
#define CE_CLR_C4 4  // status bar indicators  //127          /* Hi White on White */
#define CE_CLR_C5 5                            //1            /* White Underscore on Black */
#define CE_CLR_C6 6                            //9            /* Hi White Underscore on Black */
#define CE_CLR_C7 7                            //121          /* Hi White Underscore on White */

/*
*
*  Define Utility Keys for IBM PC.
*
*  An effort was made to preserve alphanumeric ALTternates in an ASCII
*  representation with bit 7 set (i.e. 80h + the code).
*
*  Function Keys are defined for: Unshifted (F1..F10), Shifted (SF1..SF10),
*  Ctrl-Shifted (CF1..CF10), and Alt-Shifted (AF1..AF10).
*
*  The remaining keys are self explanatory.
*/

#define CE_KEY_CTRLA  0x01
#define CE_KEY_CTRLB  0x02
#define CE_KEY_CTRLC  0x03
#define CE_KEY_CTRLD  0x04
#define CE_KEY_CTRLE  0x05
#define CE_KEY_CTRLF  0x06
#define CE_KEY_CTRLG  0x07
#define CE_KEY_CTRLH  0x08
#define CE_KEY_CTRLI  0x09
#define CE_KEY_CTRLJ  0x0A
#define CE_KEY_CTRLK  0x0B
#define CE_KEY_CTRLL  0x0C
#define CE_KEY_CTRLM  0x0D
#define CE_KEY_CTRLN  0x0E
#define CE_KEY_CTRLO  0x0F
#define CE_KEY_CTRLP  0x10
#define CE_KEY_CTRLQ  0x11
#define CE_KEY_CTRLR  0x12
#define CE_KEY_CTRLS  0x13
#define CE_KEY_CTRLT  0x14
#define CE_KEY_CTRLU  0x15
#define CE_KEY_CTRLV  0x16
#define CE_KEY_CTRLW  0x17
#define CE_KEY_CTRLX  0x18
#define CE_KEY_CTRLY  0x19
#define CE_KEY_CTRLZ  0x1A


#define CE_KEY_F1   0x80
#define CE_KEY_F2   0x81
#define CE_KEY_F3   0x82
#define CE_KEY_F4   0x83
#define CE_KEY_F5   0x84
#define CE_KEY_F6   0x85
#define CE_KEY_F7   0x86
#define CE_KEY_F8   0x87
#define CE_KEY_F9   0x88
#define CE_KEY_F10  0x89

#define CE_KEY_SHFT1  0x90
#define CE_KEY_SHFT2  0x91
#define CE_KEY_SHFT3  0x92
#define CE_KEY_SHFT4  0x93
#define CE_KEY_SHFT5  0x94
#define CE_KEY_SHFT6  0x95
#define CE_KEY_SHFT7  0x96
#define CE_KEY_SHFT8  0x97
#define CE_KEY_SHFT9  0x98
#define CE_KEY_SHFT10 0x99

#define CE_KEY_CTRLF1  0xA0
#define CE_KEY_CTRLF2  0xA1
#define CE_KEY_CTRLF3  0xA2
#define CE_KEY_CTRLF4  0xA3
#define CE_KEY_CTRLF5  0xA4
#define CE_KEY_CTRLF6  0xA5
#define CE_KEY_CTRLF7  0xA6
#define CE_KEY_CTRLF8  0xA7
#define CE_KEY_CTRLF9  0xA8
#define CE_KEY_CTRLF10 0xA9

#define CE_KEY_ALTF1  0xE0
#define CE_KEY_ALTF2  0xE1
#define CE_KEY_ALTF3  0xE2
#define CE_KEY_ALTF4  0xE3
#define CE_KEY_ALTF5  0xE4
#define CE_KEY_ALTF6  0xE5
#define CE_KEY_ALTF7  0xE6
#define CE_KEY_ALTF8  0xE7
#define CE_KEY_ALTF9  0xE8
#define CE_KEY_ALTF10 0xE9
 
#define CE_KEY_HOME            0x8A    /* HOME key     */
#define CE_KEY_CURLF           0x8B    /* <-           */
#define CE_KEY_END             0x8C    /* END key      */
#define CE_KEY_CURUP           0x8D    /* up arrow     */
#define CE_KEY_CURDN           0x8E    /* down arrow   */
#define CE_KEY_PGUP            0x9A    /* PgUp         */
#define CE_KEY_CURRT           0x9B    /* ->           */
#define CE_KEY_PGDN            0x9C    /* PgDn         */
#define CE_KEY_INSERT          0x9D    /* Ins          */
#define CE_KEY_DEL             0x9E    /* Del          */
#define CE_KEY_CTRLHOME        0xAA    /* Ctrl Home    */
#define CE_KEY_CTRLCURLF       0xAB    /* Ctrl <-      */
#define CE_KEY_CTRLEND         0xAC    /* Ctrl End     */
#define CE_KEY_CTRLINS         0xAD    /* Ctrl Ins     */
#define CE_KEY_CTRLPRTSC       0xAE    /* Ctrl PrtSc   */
#define CE_KEY_CTRLPGUP        0xBA    /* Ctrl PgUp    */
#define CE_KEY_CTRLCURRT       0xBB    /* Ctrl ->      */
#define CE_KEY_CTRLPGDN        0xBC    /* Ctrl PgDn    */

#define CE_KEY_REVTAB          0x8F    /* Shift Tab    */
#define CE_KEY_ALTMINUS        0xBD    /* Alt -        */
#define CE_KEY_ALTEQUAL        0xBE    /* Alt =        */

#define CE_KEY_ALT0 0xB0
#define CE_KEY_ALT1 0xB1
#define CE_KEY_ALT2 0xB2
#define CE_KEY_ALT3 0xB3
#define CE_KEY_ALT4 0xB4
#define CE_KEY_ALT5 0xB5
#define CE_KEY_ALT6 0xB6
#define CE_KEY_ALT7 0xB7
#define CE_KEY_ALT9 0xB8

#define CE_KEY_ALTA 0xC1
#define CE_KEY_ALTB 0xC2
#define CE_KEY_ALTC 0xC3
#define CE_KEY_ALTD 0xC4
#define CE_KEY_ALTE 0xC5
#define CE_KEY_ALTF 0xC6
#define CE_KEY_ALTG 0xC7
#define CE_KEY_ALTH 0xC8
#define CE_KEY_ALTI 0xC9
#define CE_KEY_ALTJ 0xCA
#define CE_KEY_ALTK 0xCB
#define CE_KEY_ALTL 0xCC
#define CE_KEY_ALTM 0xCD
#define CE_KEY_ALTN 0xCE
#define CE_KEY_ALTO 0xCF
#define CE_KEY_ALTP 0xD0
#define CE_KEY_ALTQ 0xD1
#define CE_KEY_ALTR 0xD2
#define CE_KEY_ALTS 0xD3
#define CE_KEY_ALTT 0xD4
#define CE_KEY_ALTU 0xD5
#define CE_KEY_ALTV 0xD6
#define CE_KEY_ALTW 0xD8
#define CE_KEY_ALTX 0xD8
#define CE_KEY_ALTY 0xD9
#define CE_KEY_ALTZ 0xDA



struct coords {
    int x, y;               //  !!! this needs to be long
};


struct menu  {
    char *option;
    int hilite;
};

struct errs {
    int errcde;
    char *errstr;
};


struct atrsrec  {
    int pos;
    int cnt;
    char atr;
};


struct linerec  {
    struct linerec *prev;
    struct linerec *next;
    int llen;
    char *strz;
    int atrc;
    struct atrsrec *atrs;
    short cmnt;
    short aspb;
    short htmb;
    short strb;     // string double quote
    short jvsb;     // client side java script
    short vbsb;     // client side vb script
    short jvsc;     // client side java script comment
};


#ifdef _WIN32
struct ftimerec  {
    unsigned long lo;
    unsigned long hi;
};
#else
struct ftimerec  {
    unsigned date;
    unsigned time;
};
#endif

#ifdef _WIN32
typedef struct _TREEITEM * HTREEITEM;
#endif

struct editrec {
    struct linerec *firstline;
    struct linerec *displline;
    struct linerec *curline;
    struct coords curcrds;
    struct coords displcrds;
    int nolines;
    int changef;
    int reloadf;
    int ostype;
    char fname[CE_LEN_FILEPATH];
    struct editrec *next;
    struct editrec *prev;
    struct ftimerec ftime;
    struct undorec *undo;
    int undocur;
    int undocnt;
    int redocnt;
    int undolns;
    int mru;
    int assoc;
#ifdef _WIN32
    HTREEITEM hTreeItem;
#endif
};


struct bookmrec {
    int linenr;
    int linepos;
    int filenr;
};


struct rgbcolor {
    char color[ 20 ];       // ???? 40
    long rgb;
};


struct undorec {
    int linenr;
    int linepos;
    int filenr;
    int type;
    char chr;
    int cnt;
    int prev;
    struct linerec *line;
};


struct clpbrec {
    struct linerec *line;
    int type;
};


struct keywordrec {
    char word[ CE_LEN_KEYWORD ];
    int wlen;
};


struct assrec {
    char fnx[ CE_LEN_FILENEXT ];
    int lng;
};



blck_cut( int clpbnr );
blck_unmark( );
blck_copy( int clpbnr );
blck_paste( int clpbnr );
blck_del();
blck_all();
blck_pasteblck( int clpbnr );
blck_case( int t );
blck_tab();
blck_revtab();
blck_space();
blck_revspace();
blck_file_home();
blck_file_end();
blck_word_right();
blck_word_left();
blck_page_up();
blck_page_down();
blck_up();
blck_down();
blck_right();
blck_left();
blck_line_home();
blck_line_end();
blck_rightn( int n );
blck_leftn( int n );

bmrk_get( int b, int *linenr, int *filenr );
bmrk_goto( int b );
bmrk_set( int b );
bmrk_clear( int b );
bmrk_clearall();
bmrk_setnext();
bmrk_next();
bmrk_prev();

clpb_free( int clpbnr );
clpb_freeall();

cmnd_file_prev();
cmnd_file_last();
cmnd_file_next();
cmnd_goto_line( int linenr, int linepos );
cmnd_tab();
cmnd_goto_file( int filenr, int linenr, int linepos );
cmnd_del_line();
cmnd_ctrlcr();
cmnd_scroll_up();
cmnd_scroll_down();
cmnd_file_home();
cmnd_file_end();
cmnd_word_right();
cmnd_word_left();
cmnd_char( int ichr );
cmnd_up();
cmnd_down();
cmnd_page_up();
cmnd_page_down();
cmnd_line_home();
cmnd_line_end();
cmnd_right();
cmnd_left();
cmnd_ins();
cmnd_del();
cmnd_bs();
cmnd_cr();
cmnd_scroll_up();
cmnd_scroll_down();
cmnd_scroll_left();
cmnd_scroll_right();
cmnd_page_left();
cmnd_page_right();
cmnd_goto_displ( int linenr );
cmnd_goto_dispx( int x );
cmnd_file_select( struct editrec *edn );

crma_lines( struct linerec *ln );
crma_ini();

dlg_prodinfo();
dlg_pagesetup();
dlg_properties( int *ft );
dlg_goto();
dlg_gotofile();
dlg_bookmarks();
dlg_blck_fill();
dlg_options();

edit_ini();
edit_get_fname( int filenr, char *fname );
edit_beep();
edit_get( struct linerec **line, int *linecnt, unsigned *linepos, int *lcrlf, char *fbufr, unsigned bufrlen );

file_open( char *fn );
file_merge();
file_nxpand( char *fn );
file_close();
file_closeall();
file_save();
file_saveas();
file_saveall();
file_reload();
file_2reload();
file_setcwd( char *dn );
file_getcwd( char *dn );
char* file_getfname( char *fn );

find_dlg();
find_replace_dlg();
find_repeat();
find_prev();
find_bracket();
find_getcurselword( char *word );
find_sel( int w );
find_bracket_c( int *xx, int *yy, struct linerec **lnb  );

int fmru_getfno( int mru );
fmru_goto();
fmru_setcur();

scrn_ini();
scrn_statsl();
scrn_rescrn();
scrn_cursor();
scrn_adjust();
scrn_reline();
scrn_resize( int x, int y );

#ifdef __cplusplus
extern "C"{
#endif
tree_file( struct editrec *ed, int fupd );
tree_insfile( const char *szFileName );
tree_delfile( HTREEITEM hTreeItem );
tree_selfile( HTREEITEM hTreeItem, int fExpand );
tree_dellines( HTREEITEM hTreeItem, int l, int c );
tree_inslines( HTREEITEM hTreeItem, int l, int c );
#ifdef __cplusplus
}
#endif

undo_goto();
undo_undo( );
undo_redo( );

vdeo_chr( int c, int x, int y, int a, int n );


