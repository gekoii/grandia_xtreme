/* FIELD .rodata 0x006714D0-0x006714E8: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C14D0.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x006714D0-0x006714E8 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_006714D8[] = "save"; /* the next file's .rodata starts here */
static const char save_006714E0[] = "save"; /* the next file's .rodata starts here */
