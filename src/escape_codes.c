/* 
 * ===  FUNCTION  ===========================================================
 *         Name:  handle_escape_codes
 *  Description:  Formatting and characters represented by beta 
 *                escape codes: $&%@#^[]<>{} etc.
 *                Input: escape code, optional number (eg: #123)
 *                Changes: pos, and via output_utf:
 *                         text_pos, margin_pos, text and margin. 
 * ==========================================================================
 */
#include "tlg.h"
#include "escape_codes.h"
void output_utf(int);
void handle_escape_codes(unsigned char beta, int number)
{
  xucode = 0;
	switch (beta)
	{
		case '$':  if ((document == PDF) && (betastate == LATIN))
                  utf_string(GREEK_FONT);
               betastate = GREEK; 
               break;

		case '&':  if ((document == PDF) && (betastate == GREEK))
                  utf_string(LATIN_FONT);
               betastate = LATIN; 
               break;

		case '%':  if (number < MAX_PUNCTUATION)     /* punctuation */
               {
                 xucode = punctuation[number];
                 if (document == PDF) 
                 {
                   int found = 1;
                   switch (number) /* escape special Tex chars */
                   {
                     case 5: utf_string("$|$"); break;
                     case 9:   utf_string("\\&"); break;        
                     case 101: utf_string("\\#"); break; 
                     default:  found = 0;          
                   }
                   if (found) break; /* break out if any of these is found */
                   if (xucode == 0x0025) /* escape '%', it is a comment in TeX */
                   {
                     utf_string("\\%");
                     break;
                   }
                 }
                 output_utf(xucode); 
               }
               break;

		case '#':  if (number < MAX_TEXT_SYMBOLS)    /* text symbols */
               { 
                  xucode = text_symbols[number];
                  if ((xucode == 0x0023) && (document == PDF))  /* escape '#' */
                  {
                    utf_string("\\#");
                    break;
                  }
                  output_utf(xucode); 
                }
                break;

		case '\"': if (number < MAX_QUOTATION)      /* quotations */
               { 
                 /* quotation marks for which 
                  * separate codes for opening 
                  * and closing exist          */
                 if ((number == 1) || (number == 2) || (number == 4) ||(number ==5))
                 {
                   output_utf(quotation_open_symbol[number]);
                 }
                 else
                   /* The rest have opening and closing
                    * on the same escape code. 
                    * Must determine which one is needed 
                    * and count opening and closing */
                 {
                    if((input_buffer[pos] == 0x20) 
                        || 
                        (quotation_open[number] == 1)) 
                    {
                      output_utf(quotation_close_symbol[number]);
                      quotation_open[number] = 0; 
                    } else {
                      output_utf(quotation_open_symbol[number]);
                      quotation_open[number] = 1;
                    }
                  }
                }
                break;

		case '[':   if (number < MAX_BRACKET) 
                {
                  if (document == PDF) 
                  {
                    if (number == 3)  utf_string("\\"); /* escape { special Tex char */
                    output_utf(bracket_open_symbol[number]); 
                  }
                  else
                  {
                      output_utf(bracket_open_symbol[number]); 
                  }
                }
                break;

		case ']':   if (number < MAX_BRACKET) 
                {
                  if (document == PDF) 
                  {
                    if (number == 3)  utf_string("\\"); /* escape } special Tex char */
                    output_utf(bracket_close_symbol[number]); 
                  }
                  else
                  {
                      output_utf(bracket_close_symbol[number]); 
                  }
                }
                break;

    case '<':   /*  For Pdf output only:
                 *   
                 *  <0:  Overlined text but we represent it 
                 *      as bold text in brackets < ... >, 
                 * <9:  Lema:  Represented as bold text     
                 * <20: Expanded text is represented as bold text. 
                 *  All others, or utf output is '<' 
                 *  */
                if (number < MAX_QUASI_BRACKET) 
                {
                  if (document == PDF)
                  {
                    if (number == 0)  
                      /* Overlined -> bold: '<' and text, eg in Ptolemaeus */
                    {
                      if (bold_flag == OFF)      /* if not bold already on */
                      {
                        bold_flag = ON;      /* signal bold open */
                        utf_string(BOLD);  /* start Tex bold */
                      }
                      /* print a '<' whether plain or in symbol font */
                      output_utf(quasi_bracket_open_symbol[number]); 
                    }
                    if ( ((number == 20 )|| (number == 9 ))
                                         && 
                                (bold_flag == OFF) ) /* if not bold already */  
                      /* Expanded text -> bold (with with no '<'), (eg. Suda) */
                    {
                      /* do not print bracket, just turn on the bold */
                      bold_flag = ON;      /* signal bold open */
                      utf_string(BOLD);   /* start Tex bold */
                    }
                  } 
                  else    /* UTF: Just print a '<' in all cases */
                  { 
                    output_utf(quasi_bracket_open_symbol[number]);
                  }
                }
                break;

		case '>':   if (number < MAX_QUASI_BRACKET) 
                {
                  if (document == PDF)
                  {
                    if  (number == 0)   /* bold with '>', eg in Ptolemaeus */
                    {
                      /* special font for '>', if required */
                      output_utf(quasi_bracket_close_symbol[number]); 
                      output_utf('}');     /* end latex bold sequence */
                      bold_flag = OFF;      /* signal bold closed */
                    }
                    if ( ((number == 20 )|| (number == 9 ))
                                         && 
                                   (bold_flag) )  /* bold with with no '>', (eg. Suda) */
                    {
                      /* do not print bracket, just turn off bold */
                      output_utf('}');  /* end latex bold sequence */
                      bold_flag = OFF;      /* signal bold closed */
                    }
                  } 
                  else                        /* UTF: Just print '>' */
                  { 
                    output_utf(quasi_bracket_close_symbol[number]);
                  }
                }
                break;

		case '{':   if (number < MAX_TEXT_MARKUP) 
                /* FIXME Reconsider with Xetex */  
                {
                  if (text_markup[number] == 1 ) /* start marginalia */
                  {
                    margin_flag = 1;
                    margin_pos = 0;
                  }
                  else if (document == PDF) /* Bold fonts */
                  {
                    if (text_markup[number] == 2) /* title */
                    {
                      utf_string(BOLD);  /* start Tex bold */
                      bold_flag = ON;      /* signal bold open */
                    }
                    else if (text_markup[number] == 3) /* speaker */
                    {
                      utf_string(BOLD);  /* start Tex bold */
                      bold_flag = ON;      /* signal bold open */
                      utf_string("\\{");
                    }
                    else
                    {
                      utf_string("\\{");
                    }
                  }
                  else /* if UTF print a '{' */
                  {
                    output_utf('{');
                  }
                }
                break;
                
		case '}':   if (number < MAX_TEXT_MARKUP) 
                {
                  if (text_markup[number] == 1 ) /* stop marginalia */
                  {
                    margin_flag = 0;
                    /* do not reset margin_pos, needed for printing */
                    /* remove trailing space */
                    if (input_buffer[pos++] != ' ') --pos; 
                  }
                  else if (document == PDF) /* Bold fonts */
                  {
                    if (text_markup[number] == 2)  /* title */
                    {
                      if (bold_flag)          /* only if bold is on */
                      {
                        output_utf('}');     /* stop Tex bold */
                        bold_flag = OFF;      /* signal bold closed */
                      }
                    }
                    else if (text_markup[number] == 3) /* speaker */
                    {
                      utf_string("\\}");
                      output_utf('}');   /* stop Tex bold */
                      bold_flag = OFF;      /* signal bold open */
                    }
                    else
                    {
                      utf_string("\\}");
                    }
                  }
                  else
                  {
                    output_utf('}');
                  }
                }
                break;

		case '@':   /* Page formats -- FIXME: incomplete */
                /* FIXME Other options @70 - @74: poetry */         
                /* FIXME     page break , form feed optional
                 *           because it messes up text.
                 *   case 1:                            
                 *           output_utf(0x0c);
                 *           break;
                 */
                switch (number)
                {
               /* case 0:     tab = 2 spaces, disable for now              
                           output_utf(0x20);
                           output_utf(0x20);
                           break;              */
                  case 6:                        /* line feed */
                           output_utf(0x0a);
                           break;
                  case 30:                       /* paragraph = 2 lines */
                           output_utf(0x0a);
                           output_utf(0x0a);
                           break;
                  default: break;         
                }
                /* case '^':   Quarter spaces depricated   */  
		default:    break;
	}
}
 
