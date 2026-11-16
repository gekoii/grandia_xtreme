/* FIELD .rodata 0x00670C60-0x00670C80: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C0C60.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00670C60-0x00670C80 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_00670C68[] = "save"; /* the next file's .rodata starts here */
static const char save_00670C70[] = "save"; /* the next file's .rodata starts here */
