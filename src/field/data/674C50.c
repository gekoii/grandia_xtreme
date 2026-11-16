/* FIELD .rodata 0x00674C50-0x00674C60: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C4C50.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00674C50-0x00674C60 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Rotation in degrees by direction 0-3 (0x006132D0). */
const f32 amapRotDegTable[4] = {
    0.0f, 90.0f, 180.0f, 270.0f,
}; /* 00674C50 */
