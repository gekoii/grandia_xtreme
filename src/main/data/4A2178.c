/* main executable .rodata 0x004A2178-0x004A2190: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A2174.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A2178-0x004A2190 */

/* Live generator page (DbgPtclGenPosPage 00407A10). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclTexIdFmt[] = "TEX_ID %d"; /* 004A2178 */
