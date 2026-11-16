/* main executable .rodata 0x004A1B70-0x004A1B90: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1B6C.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1B70-0x004A1B90 */

/* Controller generator page title (DbgPtclCtrlGenPage 004047B8). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclGenTitleFmt[] = "Generator [%d] %s page %d/5%s"; /* 004A1B70 */
