/* FIELD .rodata 0x00674088-0x006740A8: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C4084.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00674088-0x006740A8 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef signed char s8;

/* Upgrade bit -> upgrade id copied into the egg spell records
 * (ItemEggSpellRecFill 0x005F4440 copies all 7 bytes to its stack, lb). */
const s8 itemEggUpgradeIdTable[7] = { 1, 2, 3, 4, 5, 6, 7 }; /* 00674088 */
static const char save[0x10] = "save"; /* a header's unused static: the first thing in each file's .rodata */ /* padded: what follows is 16-aligned */
static const char save_006740A0[] = "save"; /* the next file's .rodata starts here */
