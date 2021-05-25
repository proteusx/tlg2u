
/*
 * ===  FUNCTION  ===========================================================
 *         Name:  id_code
 *  Description:  Extract id data from input_buffer and
 *                fill in appropriate entries in citations arrays
 *                affected variables:  icitation[], acitation[], pos
 *                returns -1 EOF, 0 normal return, -2 change book
 *
 * ==========================================================================
 */
#include "tlg.h"
int id_level;	   /* holds translated current id level as an index to ID arrays */
int id_char;	   /* holds the pointer for the ascii part of the ID arrays */
int id_command;	 /* holds the current instruction for ID handling */
int id_process;	 /* if non-zero, command must be processed */

int id_code()
{
	int return_code;
	int level_tmp;
	int processing;
  int high,low, char_pos;                  /* used in 14 bit binary macro */
	unsigned char idchar, next_idchar;

	return_code = 0;
  processing = 1;
	while (processing)
  {
    if (pos <= BLOCKSIZE)
    {
      idchar = input_buffer[pos++];
      if ((idchar < 0x80))                /* if text data - restore pointer and return*/
      {
        --pos;
        processing = 0;
      }
      else                                 /* start by isolating special codes and escapes */
      {
        id_process =0;                     /* no command yet */
        if (idchar >= 0xF0)                /* special codes EOF, EOB, EOASCII, */
        {
          switch (idchar)
          {
            case 0xF0:            /* EOF */
              return_code = -1;   /* return EOF */
              processing = 0;
              break;
            case 0xFE:            /* EOB advance past all NULL bytes 0x00 */
              while (pos <= BLOCKSIZE)  {pos++;}
              break;
            case 0xFF:           /* End of ASCII string -- Test to see if exists at all */
              break;
            case 0xF8:            /* Exception start */
              break;
            case 0xF9:            /* Exception end */
              break;
            default:
              break;
          }
        }
        else if (idchar >= 0xE0)
          /*-------------------------------------
          * 0xE0, 0xE1, 0xE2,..0xE7 = escape codes
          * Right nible = command
          * Affected level = read next byte
          *-------------------------------------*/
        {
          id_command = idchar & 0xF;                 /* Right nibble = command */
          next_idchar = input_buffer[pos++] & 0x7f;  /* id level: read next byte */
          if (next_idchar > 96)                      /* > 96 : descriptor data */
          {
            id_level = next_idchar - 97 + 26;        /* create an index offset */
            if (id_level > 51) {id_level = 51;}      /* default to z */
          }
          else                                 /* author,work,work ab, author ab */
          {
            id_level = next_idchar & 7;             /* must be 0 - 4 */
            if (id_level == 4) {id_level = 3;}      /* adjust d level */
          }
          id_process = 1;                           /* command must be processed */
        }
        else if((idchar >= 80) && (id_process == 0))
        {
          /*--------------------------------------------------
          * The rest are citation data between 0x80 -> 0xDF
          * Right nibble = command. i.e., what are the
          * folowing bytes. e.g. number, string et.c.
          *------------------------------------------------*/

          id_command = idchar & 0xF;
          level_tmp = (idchar >> 4) & 0x7;            /* left nibble = level */
          switch (level_tmp)
          {
            /*------------------------------------------------------
            * offset levels to the proper place in citation array *
            * 0->25, 1->24, 2->23, 3->22, 4->21, 5->13
            *-----------------------------------------------------*/
            case 0: /* z */
            case 1: /* y */
            case 2: /* x */
            case 3: /* w */
            case 4: /* v */
              id_level = 25 - level_tmp;
              id_process = 1;                      /* command must be processed */
              break;
            case 5:
              id_level = 13;  /* n */
              id_process = 1;
              break;
            default: break;
          }
        }  /* end of if ... else if.. on idchar */

        /*----------------------------------------------------------------------
        * now process command and put level data in array
        *-----------------------------------------------------------------------*/
        if (id_process) {
          switch (id_command)
          {
            case 0: if (id_level == 1)
                    {
                      icitation[1]++;      /* Work number increment. Rare case  */
                      break;
                    }
                    /* if has ascii data in acitation[id_level]
                     * increase them e.g. a->b, b->c, etc.
                     * else increase numeric data in icitation[id_level]
                     * Needed for Aristotle's Beker pages  */
                  if (acitation[id_level][0] != 0)
                  {
                    acitation[id_level][0]++;
                    break;
                  }
                  else
                  {
                    icitation[id_level]++;          /* increment ID data */
                    break;
                  }
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
              icitation[id_level] = id_command;      /* literal values 1 - 7 */
              no_chars(id_level);
              break;
            case 8:      read7bit; no_chars(id_level); break;
            case 9:      read7bit; read_char; break;
            case 0xa:    read7bit; read_string; break;
            case 0xb:    read14bit; no_chars(id_level); break;
            case 0xc:    read14bit; read_char; break;
            case 0xd:    read14bit; read_string; break;
            case 0xe:    read_char; break;
            case 0xf:    read_string;
                         /* convert work number string into integer.
                          * Needed for civ E0 81 which increments
                          * work number if this changes in midblock   */
                         if (id_level == 1)
                            sscanf(acitation[1], "%d", &icitation[1]);
                         else
                           icitation[id_level] = 0;
                         break;
            default:
              printf("Unknown id_command: %x,  pos %x\n", id_command, pos);
              break;
          }  /* End switch on id_command */

          /* Adjust lower citation levels */
          switch (id_level)
          {
            case 21:  icitation[22] = 1; no_chars(22);
            case 22:  icitation[23] = 1; no_chars(23);
            case 23:  icitation[24] = 1; no_chars(24);
            case 24:  icitation[25] = 1; no_chars(25);
            default:  break;
          }
        }  /* if id_process */
      } /* else ID data */
    }else{ processing=0; } /* reached end of buffer */
  } /* end while processing */
  return return_code;
}  /* end function */
