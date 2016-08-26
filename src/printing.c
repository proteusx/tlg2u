/*------------- print_line ------------------------------- 
 * Prints one line of text to std_out.                      
 * Prepares the output buffers text[], margin[]
 * cit_buff[], line_buff[] and para[]
 * and prints them to stdout as required by the 
 * command line parameters.
 *------------------------------------------------------- */
#include "tlg.h"
void print_line()
{
  /* ---------------- prepare the buffers ------------------------ */

  char cit_buff[100] = "\0";  /* used for citations sprintf - Initialize -  */
  char para[100] = "\0";      /* initialize paragraphs buffer */
  char lines_buff[32] = "\0"; /* initialize lines buffer */
  text[text_pos] = '\0';                            /* terminate string */
  if (marginalia == YES) margin[margin_pos] = '\0'; /* terminate string */

  /* -------------------- paragraphs in left margin ------------------- */
  int cits[6] = {13, 21, 22, 23, 24, 25};
  char par_buff[100] = "\0";
  if ((paragraphs == YES) || (citations == YES))  /* If paragraphs in left margins are required */
  {
    strcpy(para,"\0");
    for (i= 0; i< 5; i++)
    {
      if (icitation[cits[i]]) 
        sprintf(par_buff + strlen(par_buff), "%d", icitation[cits[i]]);
      if (acitation[cits[i]][0]) 
        sprintf(par_buff + strlen(par_buff), "%s", acitation[cits[i]]);
      if (strlen(acitation[cits[i]])  || icitation[cits[i]]) 
        sprintf(par_buff+strlen(par_buff), ".");
    }
    if ( strcmp(par_buff, par_tmp) != 0 )
    {
      para[0] = 0xc2; para[1] = 0xa7; para[2] = 0;  /* prepend § */
      sprintf(para + strlen(para), "%s", par_buff);
      para[strlen(para) -1 ] = 0;                   /* remove last comma */
      strcpy(par_tmp, par_buff);
    }
  }
  /* -------------------- citations numbers ----------------- */
  if (citations == YES)       /* If citation info is required append line nos */
  {
    strcpy(cit_buff, par_buff);
    if (icitation[25]) 
      sprintf(cit_buff + strlen(cit_buff), "%d", icitation[25]);
    if (acitation[25][0]) 
      sprintf(cit_buff + strlen(cit_buff), "%s", acitation[25]);
  }

  /* -------------------- Line numbers every 5 ------------------------ */
  if (lines == YES)                 /* If lines in margins are required */
  {
    if ( icitation[25]  && (!(icitation[25] % 5)) )
      sprintf(lines_buff, "%d", icitation[25]);
  }

  /* --------------------------------------------------------------- 
   *                      print the buffers                   
   * --------------------------------------------------------------- */

  /* ------------------------- 
   * UTF Document 
   * ------------------------- */
   if (document == UTF)
   {
     /* --------- print citations or paragraphs to left margin -------- */
     if (citations == YES) 
     {
       int len = strlen(cit_buff);
       printf("%s ", cit_buff);
       for (j=0; j < 10 - len; j++) printf("%c", ' ');
     }
     else if (paragraphs == YES)  /* citations and paragraphs: exclusive */
     {
       int len = strlen(para);
       printf("%s", para);
       for (j=0; j < 10 - len; j++) printf("%c", ' ');
     }

     /* --------------- print main text ----------------------------- */
     /* FIXME print more lines if overlong  */
     /* In case of freak lines chop  80 chars  */
     if(max_text > 80) max_text = 80;           
     printf("%s", text);
     for(i=0; i< max_text - text_utf_count; i++) printf("%c", ' ');

     /* --------------- print line numbers on the right ------------- */
     if (lines == YES)
     {
       int len = strlen(lines_buff);
       printf("%s", lines_buff);
       for(j=0; j < 4 - len; j++) printf("%c", ' ');
     }

     /* ------------ print marginalia ------------------------------- */
     if (marginalia == YES)
     {
       if (margin_utf_count > 0)
       {
        printf("{ ");
        printf("%s", margin);
        for (j=0; j < max_margin - margin_utf_count; j++) printf("%c", ' ');
        printf(" } ");
       }
       else
       {
        for (j=0; j <= max_margin + 5; j++) printf("%c", ' ');
       }
     }
     printf("\n");                              /* final line feed */
   }   /* end if UTF */

  /* ------------------------- 
   *   PDF Document 
   * ------------------------- 
   * Each line has the form:
   * (cit or para) & text & lines & marginalia\\ 
   * Margin notes are printed in smaller size.
   * ---------------------------------------------*/
   if (document == PDF) 
   {
      if (citations)
      {
        if(strlen(cit_buff)) printf(SMALL_SCRIPT, cit_buff);
      }
      else 
        if (paragraphs && strlen(para)) printf(SMALL_SCRIPT, para);

     printf("&%s", text);
     printf("&");
     if ( lines && strlen(lines_buff))     printf(SMALL_SCRIPT, lines_buff);
     printf("&");
     if ( marginalia && margin_utf_count)  printf(SMALL_SCRIPT, margin);
     printf("\\\\\n");
   }
} /* end function print_line */
