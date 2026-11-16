/* FIELD .rodata 0x006753E8-0x00675430: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C53E8.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x006753E8-0x00675430 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Top/bottom colour of bar colours 0x14-0x1B (FldMenuBarBuild 0x00634408):
 * 0xRRGGBBxx, the low byte (0x4C) unused. */
typedef struct Grad {
    u32 top;
    u32 bottom;
} Grad;

const Grad fldMenuBarGradients[8] = {
    { 0x2040804C, 0x0000704C }, /* 0x14 */
    { 0x0000004C, 0x7000004C }, /* 0x15 */
    { 0x7030584C, 0x6000004C }, /* 0x16 */
    { 0x2080604C, 0x0050004C }, /* 0x17 */
    { 0x3020804C, 0x2000604C }, /* 0x18 */
    { 0x7060104C, 0x3050004C }, /* 0x19 */
    { 0x7040004C, 0x6000004C }, /* 0x1A */
    { 0x5000704C, 0x0000504C }, /* 0x1B */
}; /* 006753E8 */
