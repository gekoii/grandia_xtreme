/* FIELD .rodata 0x00674610-0x00674660: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C4610.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00674610-0x00674660 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* AmapDbgItemLogDraw (0x0060F7B0); 0x40: the original has 0x14 zero bytes after the string. */
const char amapItemRollLogFmt[0x40] = "ITEM Rank%3d %2d%%[%2d] Group%3d %3d%%[%2d] item%4d"; /* 00674610 */
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
