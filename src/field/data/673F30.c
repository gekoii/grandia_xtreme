/* FIELD .rodata 0x00673F30-0x00673F50: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C3F30.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00673F30-0x00673F50 */

static const char save[0x10] = "save"; /* a header's unused static: the first thing in each file's .rodata */ /* padded: what follows is 16-aligned */
static const char save_00673F40[] = "save"; /* the next file's .rodata starts here */
