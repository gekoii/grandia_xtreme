/* FIELD .rodata 0x00672018-0x00672040: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C2018.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00672018-0x00672040 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Title of the party debug menu (DbgFldPartyMenu, src/field/5D8BB0.c). */

const char dbgPartyMenuStrTitle[] = "PARTY (%d / %d) Page %s %d / 6 %s "; /* 00672018 */
