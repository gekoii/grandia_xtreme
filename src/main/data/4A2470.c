/* main executable .rodata 0x004A2470-0x004A2480: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A2460.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A2470-0x004A2480 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata;
 * 16-aligned like the code .rodata after it (40C410), so the zeros before it
 * are alignment */
