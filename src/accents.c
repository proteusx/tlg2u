#include "tlg.h"
char *greek_accents = ")(+/\\=|";
char *latin_accents = "+/\\=|";
/* get_acents:
 * gets accents in <accents>
 * Returns: number of accents found or zero
 * Changes: accents, pos
 */
int get_accents(void)
{
	unsigned char bufferchar;
	int processing = 1;
	int number_of_accents = 0;

	accents = 0;

	while (processing) 
  {
		if (pos <= BLOCKSIZE) 
    {
			bufferchar = input_buffer[pos++];
      if (((betastate == GREEK) && strchr(greek_accents, bufferchar))
                                   ||  /* FIXME watch this */
          ((betastate == LATIN) && strchr(latin_accents, bufferchar)))
      {
				store_accents;    /* macro */
        number_of_accents++;
      } 
      else
      {
        --pos;
        processing = 0;
      }
		}
    else 
    {
			processing = 0;
		}
	}
	return number_of_accents;
}
 
