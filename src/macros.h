/*-----------------------------------------------------------------------------
 *  used for all error returns
 *----------------------------------------------------------------------------*/
#define error_return(msg) \
        do { perror(msg); exit(2); } while (0)

 /*--------------------------------------
 * send a string to output buffer
 *-------------------------------------*/
#define utf_string(s) \
  for(j =0 ; j < strlen(s); j++) output_utf(s[j])

/*--------------------------------------
 * used in id-code routine
 *-------------------------------------*/

#define read7bit \
        icitation[id_level] = input_buffer[pos++] & 0x7F

#define read14bit \
        high = (input_buffer[pos++] & 0x7F) << 7 ;\
        low =  input_buffer[pos++] & 0x7F;\
        icitation[id_level] = high | low

#define read_char \
        acitation[id_level][0] = input_buffer[pos++] & 0x7F;\
        acitation[id_level][1] = 0


#define read_string \
        char_pos = id_char = 0;\
        while( char_pos < 30 )\
        {\
          id_char = input_buffer[pos++];\
          if( id_char ==  0xFF) break;\
          acitation[id_level][char_pos++] = id_char & 0x7F;\
        }\
        acitation[id_level][char_pos] = 0

/* Clear ASCII data of level x */
#define no_chars(x) for (j = 0; j < 32; j++)  acitation[x][j]=0;

/*-----------------------------------------------------------------------------
 * store_accents:
 * Stores accent character passed as a parameter to <accents>
 * 0 00 00 --- 0 00 00 no accent
 * |  |  |
 * |  |   ---- 01 psili, 10 dasia, 11 dialytika
 * |   ------- 01 varia, 10 oxia,  11 perispomeni
 * -----------  1 ypogegrammeni
 *
 *----------------------------------------------------------------------------*/

#define store_accents \
	switch (bufferchar) \
	{ \
		case ')':   accents  |=    1; break; \
		case '(':   accents  |=    2; break; \
		case '+':   accents  |=    3; break; \
		case '\\':  accents  |=    4; break; \
		case '/':   accents  |=    8; break; \
		case '=':   accents  |=  0xC; break; \
		case '|':   accents  |= 0x10; break; \
		default: break; \
	} \
	accents &= 0x1f


