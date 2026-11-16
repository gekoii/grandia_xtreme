/* FIELD .rodata 0x00674338-0x00674360: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C4338.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00674338-0x00674360 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
/* 8-byte constant FldThreatGaugeDraw (0x006020A8) copies to its stack
 * (ldl/ldr); 0x8-0xF are zeros (an unlabeled constant or padding). */
const u8 fldThreatGaugeRodata[0x10] = { 2, 0, 0, 2, 2, 2, 0, 0 }; /* 00674340 */
static const char save_00674350[] = "save"; /* the next file's .rodata starts here */
