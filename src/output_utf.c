
/*------------------------------------------------------------------------ 
 * output_utf:
 * Converts a ucs-2 code into a UTF-8 byte sequence in line_buffer
 * Changes: text and margin, text_pos, margin_pos, margin_char_count
 *-----------------------------------------------------------------------*/

#include "output_utf.h"

void output_utf(unsigned long ucode)
{
  int count = 0;                                /* byte counter */
  int utf_bytes[5];                             /* hold the converte utf */
	if ((text_pos + 4) > LINESIZE) {
		error_return("text_pos out of range");
	} else if (ucode == 0){
		/* do nothing */
	} else if (ucode < 0x80) {                           /* 1 byte utf8 */
    utf_bytes[count++] = ucode;
  /* --------------------------------------------------------------------- */  

	} else if (ucode < 0x800) {                          /* 2 byte utf8 */
		utf_bytes[count++] = ((ucode >> 6) & M00011111 ) | M11000000; /* MSbyte */
		utf_bytes[count++] = ( ucode       & M00111111 ) | M10000000;
  /* --------------------------------------------------------------------- */  

	} else if (ucode < 0x10000) {                        /* 3 byte utf8 */
		utf_bytes[count++] = ((ucode >> 12) & M00001111 ) | M11100000;
		utf_bytes[count++] = ((ucode >>  6) & M00111111 ) | M10000000;
		utf_bytes[count++] = ( ucode        & M00111111 ) | M10000000;
  /* --------------------------------------------------------------------- */  

	} else if (ucode < 0x200000){                        /* 4 byte utf8 */
		utf_bytes[count++] = ((ucode >> 18) & M00000111 )  | M11110000;
		utf_bytes[count++] = ((ucode >> 12) & M00111111 )  | M10000000;
		utf_bytes[count++] = ((ucode >>  6) & M00111111 )  | M10000000;
		utf_bytes[count++] = ( ucode        & M00111111 )  | M10000000;
	} else { error_return("Too big UCS. Bugger-Off!");}

  /* --------------------------------------------------------------
   * put the utf bytes in the main line text or margin                 
   * -------------------------------------------------------------- */    
  for(i = 0; i < count; i++)
  {
    if (margin_flag == 0)  text[text_pos++] = utf_bytes[i];
    else   margin[margin_pos++] = utf_bytes[i];
//    printf("%d  %c\n", text_pos, ucode);
  }
  /* utf may be more than one bytes 
   * since this routine is called once for each utf char
   * we count them here 
   * used for display formating
   */
  if (margin_flag == 0) text_utf_count++; 
  if (margin_flag == 1) margin_utf_count++; 
  if (pass == 0) 
  {
    if (text_utf_count > max_text) max_text = text_utf_count;
    if (margin_utf_count > max_margin) max_margin = margin_utf_count;
  }
}

 
