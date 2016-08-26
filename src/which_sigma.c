/* 
 * ===  FUNCTION  ==============================================================
 *         Name:  which_sigma
 *  Description:  Determines the appropriate sigma (medial or final)
 * =============================================================================
 */

#include "tlg.h"

#define SIGMEDIAL 0x3c3
#define SIGFINAL 0x3c2
char *punctuation_codes = " .,:;_\"%{}$&";	/* used by which_sigma */
int which_sigma(int nextptr)
{
	int scanning;
	int nextcode;
	/* If the next character is a hyphen, it is a medial sigma
	 * Otherwise, a few characters are examined in the input buffer:
	 * if an alphabetic character is found before we hit a space, or
	 * other punctuation character, it is a medial sigma
	 * otherwise it is a final sigma (there is one exception in 4085 - POS(.))
	 */
	if (input_buffer[pos] == '-')
		return(SIGMEDIAL);
	else {
		scanning = 10;
		while(scanning) {
			nextcode = input_buffer[nextptr++];
			if (isalpha(nextcode))
				return(SIGMEDIAL);
			if ((nextcode > 0x7f) || (nextcode < 0x20))
				return(SIGFINAL);
			if (strchr(punctuation_codes, nextcode))
				return(SIGFINAL);
			scanning--;
		}
		return(SIGMEDIAL);
	}
}

 
