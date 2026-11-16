/* FIELD .rodata 0x00675278-0x006752A0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C5278.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00675278-0x006752A0 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef signed char s8;

/* FldMenuItemUpdate (0x00628EC0) copies both tables to its stack. */
/* Title x offset per item category tab (lb). */
const s8 fldMenuItemTabTitleX[6] = { 0x1E, 0x00, 0x14, 0x14, 0x14, 0x0A }; /* 00675278 */
/* Icon index (into menuItemIcons) per item category tab. */
const u16 fldMenuItemTabIcons[6] = { 0x40, 0x45, 0x41, 0x4D, 0x00, 0x06 }; /* 00675280 */
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
