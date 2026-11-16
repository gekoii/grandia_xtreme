/* main executable .rodata 0x004A15B0-0x004A15B8: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A14B4.rodata.s (split at the second "save"); check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A15B0-0x004A15B8 */

typedef unsigned char u8;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata
                                    * (16-aligned: zeros before it); 4019F8's .rodata follows */
