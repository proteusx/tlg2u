/* 
 * ===  FUNCTION  ============================================================
 *         Name:  find_block
 *  Description:  Finds and reads the IDT file corresponding to the 
 *                .TXT file and locates the block of a required 
 *                work number (book)
 *  Input:        Required work number.
 *  Returns:      The starting block number for this work (book).
 *
 *  Needs the xxxx.idt file to exist.
 * ===========================================================================
 */

#include "tlg.h"
#define MAXIDTSIZE 0xF800               /* Largest idt file is tlg2062.idt*/

int find_block ( char *idt_file, int book )
{
  unsigned char idt_buffer[MAXIDTSIZE];
  int idt_pos;
  int i, in_count, block_no, book_tmp, scanning;
  char bk_str[4];
  int idt_handle;
  idt_handle = open(idt_file, O_RDONLY);
  if (idt_handle < 0) 
  {
    printf("%s\n", idt_file);                   /* print filename first */
    error_return("IDT Input file error ");
  }
  in_count = read(idt_handle, idt_buffer, sizeof(idt_buffer));
  if (in_count < 0) error_return(".IDT Read file error ");
  close(idt_handle);
  block_no = -1;                     /* error return if blocks not found */
  idt_pos = 0;
  scanning = 1;
  while (idt_pos < in_count && scanning == 1 )
  { /* look for "new book" code :  02 xx xx xx xx EF 81 */
    if (idt_buffer[idt_pos++] == 0x02 && 
        idt_buffer[idt_pos+4] == 0xef && idt_buffer[idt_pos+5] == 0x81)
    {
      /* bytes 6,7,8 are the digits of the book number (high bit set)  */
      for( i = 6; i < 9; i++) bk_str[i - 6] = idt_buffer[idt_pos + i] & 0x7f; 
      bk_str[3]= 0x0;                           /* termiate string */
      sscanf(bk_str, "%d", &book_tmp);          /* convert to number */
      /* bytes 2,3 are the block number */
      block_no = 256 * idt_buffer[idt_pos + 2] + idt_buffer[idt_pos + 3];
      if (book_tmp == book) scanning = 0;       /* book found stop scanning */
      idt_pos += 9;                             /* advance idt_pos pointer */
    }
  }
  if (idt_pos < in_count) return block_no;
  else return -2;
}
 
