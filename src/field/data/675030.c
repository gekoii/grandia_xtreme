/* FIELD .rodata 0x00675030-0x00675050: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C5030.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00675030-0x00675050 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Outline colour (R, G, B, A) by text style; FldMenuWinTextAdd (0x00621400)
 * copies the table to its stack (3 x ldl/ldr). */
const u8 fldMenuTextOutlineColors[6][4] = {
    { 0x00, 0x00, 0x00, 0x00 },
    { 0x20, 0x40, 0x20, 0x00 },
    { 0x20, 0x30, 0x50, 0x00 },
    { 0x30, 0x20, 0x00, 0x00 },
    { 0x40, 0x40, 0x20, 0x00 },
    { 0x40, 0x40, 0x40, 0x00 },
}; /* 00675030 */
/* Default text colour (R, G, B, A), copied with lwl/lwr by the same function. */
const u8 fldMenuTextDefaultColor[4] = { 0x80, 0x80, 0x80, 0x00 }; /* 00675048 */
