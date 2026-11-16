/* FIELD .rodata 0x00670CB8-0x00670CD0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C0CB4.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00670CB8-0x00670CD0 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* 8 zero bytes before the next file's "save", unreferenced and without a
 * label of their own (the asm label D_00670CB4 is 4 bytes earlier and
 * stays asm): a static. */
static const u32 zero_00670CB8[2] = { 0, 0 }; /* 00670CB8 */
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
