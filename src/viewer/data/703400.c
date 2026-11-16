/* VIEWER .rodata 0x00703400-0x00703410: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/viewer/data/viewer/0033FC.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00703400-0x00703410 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
