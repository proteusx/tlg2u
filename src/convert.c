/* ------------------------------------------------------------------ 
 *   Main text conversion function.              
 *   Needs the txt_file to be open and 
 *   the file handle to be valid
 * ------------------------------------------------------------------ */
#include "convert.h"

void output_utf(int);
void convert()
{
  int book_tmp;                                 /* current book number */
  int more_blocks;                              /* keep processing if !=0 */
  int in_count;                                 /* Bytes read from file */
  unsigned char byte;                           /* processed data byte */
  int id_err, beta_err;                         /* error returns */
  id_err = beta_err = 0;
  margin_flag = 0;                   /* start sending text to main 
                                      * 0= text goes to main line,
                                      * 1 text goes to margin 
                                      * FIXME What if marginalia 
                                      * spans more than one line?  */

  /* Seek the first byte of the starting block of the .txt input file  */         
  int seek_err = lseek(txt_handle, start_block * 0x2000, SEEK_SET);
  if (seek_err < 0 ) error_return("Seek error. ");
  more_blocks = 1;
  while (more_blocks)                           /* for each block */
  {
    /* ---------------------- Read one block of data -------------------- */
    in_count = read(txt_handle, input_buffer, sizeof(input_buffer) );
    if (in_count < 0) error_return("TXT Read file error ");
    if (in_count == 0) more_blocks =0;
    /* ------------------------------------------------------------------ */
    pos = 0;
    while ( pos < in_count )                            /* process buffer */
    {
      byte = input_buffer[pos++];
      if (byte > 0 && byte >= 0x80)                       /* ID codes */
      {
        --pos;
        id_err = id_code();
        if (id_err < 0) more_blocks = 0;                  /* EOF */
      }
      else if (byte > 0 && byte < 0x80)                   /* Beta Text */
      {
        /* old code: sscanf(acitation[1], "%d", &book_tmp); */
        /* Retrieve current work id (level b). Converted to int in id_code.c */
        book_tmp = icitation[1];
        /* book == 0 means all books  */
        if ((book == 0) || ( book_tmp == book) ) 
        { 
          margin_utf_count = text_utf_count = 0;
          --pos;
          beta_err = beta_code();              /* FIXME error return???  */
          if (pass) 
          {
            print_line();                      /* print line if second pass or pdf */
          }
          if (document == CANON) lines_count++;
        }
        else if (book_tmp > book) more_blocks = 0; /* no more blocks or book not found */
      }  /* end beta text line */
      betastate = default_betastate; 
    }  /* end pos < count. finished with buffer for this block */
  } /* end while blocks */
} /* end function convert */
