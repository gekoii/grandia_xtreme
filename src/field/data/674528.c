/* FIELD .rodata 0x00674528-0x00674548: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C4528.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00674528-0x00674548 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Rotation in degrees by direction 0-3 (0x0060DE90). */
const f32 amapGimmickRotDegTable[4] = {
    0.0f, 90.0f, 180.0f, 270.0f,
}; /* 00674528 */
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_00674540[] = "save"; /* the next file's .rodata starts here */
