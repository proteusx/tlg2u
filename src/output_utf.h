#include "tlg.h"
/* ------------------------------------------------------ */
#define M00011111  0x1F            /* AND mask for MSByte - 2 byte utf */
#define M11000000  0xC0            /* OR  mask for MSByte - 2 byte utf */ 

#define M00001111  0x0F            /* AND mask 3 byte utf */
#define M11100000  0xE0            /* OR  mask 3 byte utf */

#define M00000111  0x07            /* AND mask 4 byte utf */
#define M11110000  0xF0            /* OR  mask 4 byte utf */

#define M10000000  0x80            /* OR  mask for LSBytes */
#define M00111111  0x3F            /* AND mask for LSBytes */

