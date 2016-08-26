/* 
 * ===  FUNCTION  ============================================================
 *
 *         Name:  beta_code
 *  Description:  Converts Beta Code characters read from input_buffer
 *                to utf8 bytes and sends them to the output
 *                changes: pos, text[], margin[], text_pos, margin_pos, 
 *                         
 * ===========================================================================
 */
#include "tlg.h"
#include "beta_code.h"
/*------------------------ Function prototypes ----------------------*/
int which_sigma(int);
int getnum();
int get_accents();
void output_utf(int);
void handle_escape_codes( unsigned char, int);
/* ------------------------------------------------------------------ */
char *vowels = "AEHIOUWR";                         /* Rho is a vowel! */
char *hypo_vowels ="AHWUI";   /* vowels that take ypogegrammeni or dialytika */
char *escape_codes = "$&%\"@#^[]<>{}";
char *before_accents = ")(/\\=";
char char_tmp;
/* ------------------------------------------------------------------ */
int beta_code()
{
  int  outchar, sigma_no;
  unsigned char betachar;
  int return_code = 0;
  int processing = 1;
  int no_of_accents = 0;
  int esc_num = 0;
  text_pos = margin_pos = 0;
  /* start Tex bold if continued from previous line */
  if((document == PDF) && (bold_flag)) utf_string(BOLD);  
  while (processing)
  {
    if (pos <= BLOCKSIZE) 
    {
      betachar = input_buffer[pos++];
      if ((betachar > 0x7F)) /* if id data - restore pointer and return*/
      {
        --pos;
        processing = 0;
      }
      else                                      /* beta code */
      {
        outchar = 0;                            /* clear in case is retained */
        if (strchr(escape_codes, betachar))     /* escape codes */
        {
          handle_escape_codes( betachar, getnum() );
        }
        else                                      /* not escape codes */
        {
          /*------------------------------------------------------------
           *  Greek Capitals
           *------------------------------------------------------------*/
          if (betastate == GREEK && betachar == '*')  
          {
             /* -------------------------------------------
             * Ypogegrammeni or dialytika come after 
             * the capital letter
             * If any found, swap its position with the letter
             * so that all accents are  before the letter so 
             * that get_accents() can find them.
             * -------------------------------------------*/
            for(i=0; i <= 2; i++) 
            {
                /* ignore any accents after * and before letter */
              if(strchr(before_accents, input_buffer[pos + i])) { continue;}
              /* Look for A,H,W,I,U folowed by | or + */
              else if(
                        strchr(hypo_vowels, input_buffer[pos + i]) 
                                          &&
                        (          /* if next char  is | or +  */
                            (input_buffer[pos + i + 1] == 0x7c) 
                                          ||
                            (input_buffer[pos + i + 1] == 0x2b)
                        )
                      )
                      {                  /* swap if found */
                        char_tmp = input_buffer[pos + i + 1];
                        input_buffer[pos + i + 1] = input_buffer[pos + i];
                        input_buffer[pos + i] = char_tmp;
                        break;           /* stop searching */
                      } 
                    else {break;} /* not relevant, stop searching */
            }/* end for */
            no_of_accents = get_accents();  /* For caps accents come before the letter */
            betachar = input_buffer[pos++];
            /* Don't waste time unless it is a vowel and has accents */
            if ( strchr(vowels, betachar) && no_of_accents ) 
            {
              switch (betachar) 
              {
                case 'A': outchar = Alpha[accents]; break;
                case 'E': outchar = Epsilon[accents]; break;
                case 'H': outchar = Eta[accents]; break;
                case 'I': outchar = Iota[accents]; break;
                case 'O': outchar = Omicron[accents]; break;
                case 'U': outchar = Ypsilon[accents]; break;
                case 'W': outchar = Omega[accents]; break;
                case 'R': outchar = Rho[accents]; break;
                default: break;
              } /* end switch */
            } /* not accented capital vowel */
            else 
            {
              if (isalpha(betachar))            /* Other Greek capital */
              {
                outchar = greek[betachar];
              } 
              else                              /* '*' not followed by letter*/
              {
                if (betachar == '#')  /* Capital special symbols. eg capital koppa */
                {
                  esc_num = getnum();           /* The number that follows # */
                  switch (esc_num)              /* only four posibilities */
                  {
                    case 1: outchar = KOPA;    break;
                    case 2: outchar = STIGMA;  break;
                    case 3: outchar = ARKOPA;  break;
                    case 5: outchar = SAMPI;   break;
                    default:                   break;
                  }
                }
                else
                {
                  /* any other symbol with a '*' in front of it */
                  outchar = greek[betachar - 0x20]; /* what could they be? */
                }
              } /* end else not a letter */
            } /* end else not accented cap */
          }
          else  /* not a  capital */           
          /*------------------------------------------------------------
           *  i.e. Greek Lower Case or Latin
           *------------------------------------------------------------*/
          {
            if ( betastate == GREEK && isalpha( betachar)) 
            {
              if (betachar == 'S') /* sort out the sigmas */
              {
                sigma_no = getnum();
                switch (sigma_no)
                {
                  case 0:   outchar = which_sigma(pos); break;
                  case 1:   outchar = SIGMEDIAL; break;
                  case 2:   outchar = SIGFINAL; break;
                  /* S3 (lunate sigma) is not used.
                   * EDIT: It is used in Galenus 0057_91
                   * handle it as an ordinary sigma
                   * and let which_sigma() decide if it meant 
                   * to be medial of final
                   */          
                  case 3:   outchar = which_sigma(pos); break;
                  default:  outchar = SIGMEDIAL;
                }
              }
              else                              /* other small greek */
              {
                /* accents follow letter. 
                 * Do not waste time unless 
                 * it is a vowel with accents */
                if ( strchr(vowels, betachar) && get_accents() ) 
                {
                  switch (betachar) 
                  {
                    case 'A': outchar = alpha[accents]; break;
                    case 'E': outchar = epsilon[accents]; break;
                    case 'H': outchar = eta[accents]; break;
                    case 'I': outchar = iota[accents]; break;
                    case 'O': outchar = omicron[accents]; break;
                    case 'U': outchar = ypsilon[accents]; break;
                    case 'W': outchar = omega[accents]; break;
                    case 'R': outchar = rho[accents]; break;
                    default: break;
                  } /* end switch */
                } else if (isalpha(betachar)) /* not an accented character */
                {
                  outchar = greek[betachar - 0x20];
                }
              } /* end else other lc greek */
            } /* end if isalpha GREEK */
            else if ( (betastate == LATIN) && isalpha(betachar) && get_accents()) 
              /*  not a greek letter */
            {
              /*----------------------------------------------------
              *   Latin Only
              *----------------------------------------------------*/
                outchar = betachar; 
                /* There are some accented latin 
                * characters. Use combining accents for them.
                * Emmit character and combining accent directly
                * and then sent a 0x0 to the programme to prevent 
                * double characters.
                * */
                output_utf(outchar);
                switch (accents)
                {
                  case 0x03:  output_utf(UMLAUT); break;
                  case 0x04:  output_utf(GRAVE);  break;
                  case 0x08:  output_utf(ACUTE);  break;
                  case 0x0c:  output_utf(CARET);  break;
                  default:    break;            
                }
                outchar = 0;   /* Already sent, so discart character */
            } /* end if accented LATIN  */
            else        /* all other chars: space, coma etc */
            {
                switch (betachar)
                {
                  case ':':  outchar = 0x0387;break;      /* (or 0x00b7) ano teleia */
                  case '_':  outchar = 0x2014;break;      /* em-dash */
                  case '-':  outchar = '-';   break;      /* (or 0x2010)hyphen */

                  /* This apostrofos is recomended by Beta Code manual
                   * but is incopadible with vim keymap, spell checkers
                   * and kadmos 
                   * case '\'': outchar = 0x2019;break;      
                   * Better use coronis = 0x1fbd
                  */
                  case '\'': outchar = 0x1fbd;break;      /* apostrofos */
               // case '?':  outchar = 0x0323;break;      /* combined underdot. a mess!*/
                  case '?':  break;                       /* Discard underdots */
                  case '`':  break;                       /* discard separators */
                  default :  outchar = betachar; break;
                }
            } /* end all others */
          }  /* end if lower case and latin*/
        } /* end else not escapes */
        if ((outchar != 0) && (outchar != '`'))  /* discard 0x0's and '`' */
        {
         output_utf(outchar);
        }
      }  /* end else process beta codes */
    }   /* end if pos <=  */
    else
    {
      processing = 0;
    }
  }  /* end while */
  /* Terminate unfinished LaTex bold sequence. 
   * The next line will begin with \textbf{  */
  if((document == PDF) && (bold_flag)) output_utf('}');  
  return return_code;
}
