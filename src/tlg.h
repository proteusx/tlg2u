#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "macros.h"

/*------ code defines ------- */
#define VERSION "1.0.0"
#define BLOCKSIZE 0x2000                    /* 8k block size, very efficient */
#define LINESIZE 0x400                      /* 512 bytes */
#define MARGINSIZE 0x100                    /* 256 bytes */
#define GREEK 1
#define LATIN 2
#define CANON 0
#define UTF   1
#define PDF 2
#define YES 1
#define NO 0
#define ON 1
#define OFF 0
/* TeX strings used for pdf output */
/* #define SYMBOL_FONT_OPEN "{\\symbol "*/
#define BOLD "\\textbf{"
#define GREEK_FONT "\\greek{}"
#define LATIN_FONT "\\latin{}"
#define SMALL_SCRIPT "{\\scriptsize %s}"    /* For pinting in margins */

/* ------- default parameters ------- */
#define CORPUS "tlg"
#define TXT_DIR  "./"                           /* default texts directory */
#define AUTHOR_CODE "0000"                      /* default author code */
#define BOOK 1

/* ----------------------- Global Variables-------------------------- */
long int lines_count;
int txt_handle;                              /* text file handle */
int start_block;                             /* First block to read from */ 

char txt_file[256], idt_file[256];           /* filename strings */
char text_dir[253];                          /* the tlg files directory */
char author_code[5];                         /* author code number string */
char corpus[5];                              /* tlg, tll, etc */
int book;                                    /* required book number */
int document;                                /* utf or pdf */
int marginalia;                              /* emit marginalia */
int citations;                               /* emit citation numbers */
int lines;                                   /* Line numbers in margin */
int paragraphs;                              /* Paras in left margin */
char par_tmp[100];                           /* Temp variable */
unsigned char input_buffer[BLOCKSIZE];       /* 8k input data buffer*/
int pos;                                     /* input buffer possition */
int pass;
/*-------------- beta code processing variables ----------------------*/
int default_betastate, betastate;            /* default GREEK */
int accents;                            /* encoded accents */
int text_pos, margin_pos;               /* line & margin byte positions */
/* utf are multibyte so these indicate the character postion in buffer */
int margin_utf_count, text_utf_count; 
/* max utf characters, calculated after first pass */
int max_margin, max_text;          
int margin_flag;                   /* used with test markups.
                                    * indicate if characters go 
                                    * to main text or margin
                                    * see escape_codes.h */
int bold_flag;                     /* bold sequence flag, checks for
                                    * multiline bold text sequences   */
unsigned char text[LINESIZE];               /* main text */
unsigned char margin[MARGINSIZE];           /* margin text */

/*-------------- id code   processing variables ----------------------*/
/* First index = id level */
int  icitation[52];                  /* integer part of citation */
char acitation[52][32];              /* Ascii part (text) of citation */
/* ------------------------------------------------------------------ */
int i, j, k;                              /* sandries variables */
char error_msg[253];                /* Error return message text */

/*------------------------ Function prototypes ----------------------*/
int find_block (char *, int);
void convert();

 
