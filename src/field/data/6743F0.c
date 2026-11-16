/* FIELD .rodata 0x006743F0-0x006743F8: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C43F0.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x006743F0-0x006743F8 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Map type -> template tier (AmapTemplateListsBuild 0x00604170 copies it to
 * its stack with ldl/ldr and indexes it with lbu). */
const u8 amapTypeTierTable[8] = { 0, 1, 1, 2, 3, 4, 5, 6 }; /* 006743F0 */
