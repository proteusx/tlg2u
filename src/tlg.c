#include "tlg.h"
#include "usage_info.h"

int main (int argc, char *argv[])
{
  if (argc < 2)
  {
    usage_info(); 
    return(0);
  }
  setlocale(LC_ALL, "");

  /* --------- defalt values ---------- */
  default_betastate = GREEK;
  book = BOOK;
  strcpy( text_dir,TXT_DIR);
  document = UTF;
  citations = NO;
  lines = NO;
  paragraphs = NO;
  marginalia = NO;
  strcpy(author_code,AUTHOR_CODE);
  strcpy(corpus, CORPUS);                       /* default = tlg */

  /* -----------------------------------
   * intialize other variables
   * ----------------------------------*/
  strcpy(par_tmp, "xxxx");                 /* paragraph temp variable */
  max_text = max_margin = -1;              /* text widths not given */
  start_block = -1;                        /* no default starting block */
  pass = 1;                      /* pass 0 only if specifically asked */
  lines_count = 0;

/* ------------------------------------------------------------------ */
/* process comand line arguments                                      */
/* ------------------------------------------------------------------ */
  --argc;
  ++argv;
  while(argc >0 && argv[0][0] == '-')
  {
    switch (argv[0][1])
    {
      case 'a': strcpy(author_code, argv[1]);  /* must be: nnnn */
                argc--;
                argv++;
                break;
      case 'b': book = atoi(argv[1]);           /* must be: nnn */
                argc--;
                argv++;
                break;
      case 'c': strcpy(corpus, argv[1]);        /* TLG, PH5 or PH7 */
                argc--;
                argv++;
                break;
      case 'd': strcpy(text_dir, argv[1]);
                argc--;
                argv++;
                break;
      case 'B': start_block = atoi(argv[1]);    /* first 8k block of book */
                argc--;
                argv++;
                break;
      case 'T': max_text = atoi(argv[1]);       /* main text width (utf use only) */
                argc--;
                argv++;
                break;
      case 'M': max_text = atoi(argv[1]);       /* margin width (utf only) */
                argc--;
                argv++;
                break;
      case 'l': lines =      YES; break;     /* line numbers in right margin */
      case 'L': default_betastate =  LATIN; break;   /* Text is mainly latin ( TLL or canons ) */
      case 'G': default_betastate =  GREEK; break;   /* Text is mainly Greek (default) */
      case 'n': document =   CANON; break;   /* if program runs to build a canon */
      case 'u': document =   UTF; break;     /* utf output (default) */
      case 'p': document =   PDF; break;     /* pdf output */
      case 'P': paragraphs = YES; break;     /* paragraphs in left margin */
      case 'C': citations =  YES; break;     /* full citations in left margin */
      case 'm': marginalia = YES; break;     /* marginal notes in right margin */
      case 'h': usage_info(); exit(EXIT_SUCCESS);     /* Print usage info and exit. */
      default:  break;
    }
    argc--;
    argv++;
  }
  /* --------------------------------------------
   * if a filename is given n the command line
   * process this file
   * do not block seek
   * do not check book numbers
   * -------------------------------------------*/
  if (argc == 1)         
  {
    strcpy(txt_file, argv[0]);
    book = 0;                        /* book=0: do the whole file */
    start_block = 0;
  }
  /* ------------------------------------------------------------ */
 if (book != 0)  /* if no file name is given in command line */
 {
    /* form filenames for tlg and idt */  
    strcpy(txt_file, text_dir);
    strcat(txt_file, corpus);
    strcat(txt_file, author_code);
    strcpy(idt_file, txt_file);
    strcat(txt_file, ".txt");
    strcat(idt_file, ".idt");
    /* --------------------------------------
     * find the starting block for the book 
     * only if not given in command line
     * -------------------------------------*/
    if (start_block == -1) start_block = find_block(idt_file, book) ; 

    if (start_block < 0)             /* Error return if block not found */
    {
      sprintf(error_msg, "Starting block of book %d not found!", book);
      error_return(error_msg);
    }
 }
  /* ------------------------------------------------------------------ */
  txt_handle = open(txt_file, O_RDONLY);    /* open .txt file */
  if (txt_handle < 0)                       /* error if file not found */
  {
    sprintf(error_msg, "%s: File not found!", txt_file);
    error_return(error_msg);
  }
  /* ----------------------------------------------------------------- 
   *          start processing file convert and print
   * ---------------------------------------------------------------- */
  betastate = default_betastate;
  if ( document  == CANON) /* used only when creating canons. option -n */
  {
    pass = 0;
    convert();
    printf("%d,%d,%ld\n", max_text, max_margin, lines_count); 
  }
  if ( document  == PDF ) /* only second pass and print text */
  {
    pass = 1;
    convert();             /* second pass print */
  }
  if ( document  == UTF ) /* two passes */
  {
    if ((max_text == -1) || (max_margin == -1)) 
      /* -------------------------------
       * first pass only if text widths 
       * are not given in command line 
       * ------------------------------*/
    {
      pass = 0;        /* 1st pass calculate text sizes */
      convert();      
      /* Clear citation registers ready for 2nd pass
       * need to do this in case the whole file
       * is processed from the cmd line*/
      for (i = 0; i < 52; i++) no_chars(i);
      for (i = 0; i < 52; i++) icitation[i] = 0;
    }
    pass = 1;
    convert();         /* second pass print */
  }
  close(txt_handle);
  exit(EXIT_SUCCESS);
}
