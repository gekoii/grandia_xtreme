/* FIELD .rodata 0x00675728-0x00675750: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C5724.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00675728-0x00675750 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef signed short s16;

/* Texture rectangle of a sprite (include/sys.h SprRect). */
typedef struct SprRect {
    /* 0x0 */ s16 w;
    /* 0x2 */ s16 h;
    /* 0x4 */ u16 u0;
    /* 0x6 */ u16 v0;
    /* 0x8 */ u16 u1;
    /* 0xA */ u16 v1;
} SprRect; /* size 0xC */

const u32 D_00675728[] = {
    0x0, 0x0,
}; /* 00675728 */
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
/* Sparkle sprite rect, a 24x24 cell (MiniSparklesDraw 0x0064F130). */
const SprRect miniSparkleRect = { 24, 24, 0x99, 0xBD, 0xB2, 0xD6 }; /* 00675738 */
