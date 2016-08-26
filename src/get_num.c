/*--------------------------------------------------------------------------
 * getnum:
 * Collects (and removes) a non-zero number from the current input_buffer
 * position. 
 * Used to detemine escape beta codes
 * called from beta_code.c whenever an escape symbol is encountered.
 * Returns: an integer or zero if no number found, -1 on end of buffer
 * Changes: pos
 *-------------------------------------------------------------------------*/
#include "tlg.h"
#define MAXNUMBERS 64
 int getnum(void)
{
	char bufferchar;
	char modnumber[MAXNUMBERS];	/* the number string */
	int imodnumber = 0;	/* index to modnumber */
	int convnumber = 0;	/* converted modnumber string */
	int processing = 1;

	modnumber[0] = 0;

	while (processing) {
     /* May need to check for (pos <= BLOCKSIZE) too */
		if ( imodnumber < MAXNUMBERS )
    {
			bufferchar = input_buffer[pos++];
			if (isdigit(bufferchar))                  /* if char is digit */
      {
				modnumber[imodnumber++] = bufferchar;   /* put in string */
			} 
      else 
      {
        /* get rid of separator by not restoring the position pointer */
				if (bufferchar != '`') --pos;   
				modnumber[imodnumber] = 0;       /* zero terminate the string */
				sscanf(modnumber, "%d", &convnumber);   /* convert to integer */
				processing = 0;
			}
		} 
    else 
    {
			convnumber = -1;
			processing = 0;
      
		}
	}
	if (convnumber < 0) error_return("Unknown symbol number.\n");
	return convnumber;
}
 
