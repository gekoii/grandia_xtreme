/* FIELD .rodata 0x00675168-0x00675240: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C5168.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00675168-0x00675240 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef signed char s8;
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

/* Digit font of a number field (include/sys.h SprDigitFont). */
typedef struct SprDigitFont {
    /* 0x0 */ const SprRect *rects;
    /* 0x4 */ s8 w[12]; /* advance per glyph */
} SprDigitFont; /* size 0x10 */

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
/* Glyphs of fldMenuDigitFont2: digits 0-9, 10 (unused?), 11 the percent
 * suffix, 12. */
const SprRect fldMenuDigitFont2Rects[13] = {
    { 10, 16, 0x52, 0x70, 0x5D, 0x81 }, /* 0 */
    { 10, 16, 0x5E, 0x70, 0x69, 0x81 }, /* 1 */
    { 10, 16, 0x6A, 0x70, 0x75, 0x81 }, /* 2 */
    { 10, 16, 0x76, 0x70, 0x81, 0x81 }, /* 3 */
    { 10, 16, 0x52, 0x82, 0x5D, 0x93 }, /* 4 */
    { 10, 16, 0x5E, 0x82, 0x69, 0x93 }, /* 5 */
    { 10, 16, 0x6A, 0x82, 0x75, 0x93 }, /* 6 */
    { 10, 16, 0x76, 0x82, 0x81, 0x93 }, /* 7 */
    { 10, 16, 0x52, 0x94, 0x5D, 0xA5 }, /* 8 */
    { 10, 16, 0x5E, 0x94, 0x69, 0xA5 }, /* 9 */
    { 10, 16, 0x5E, 0x94, 0x69, 0xA5 }, /* 10: same as 9 */
    { 13, 13, 0x82, 0x88, 0x90, 0x96 }, /* 11: percent suffix */
    { 10, 16, 0x6A, 0x94, 0x75, 0xA5 }, /* 12 */
}; /* 00675170 */
/* Second menu digit font (FldMenuPercentBuild 0x00624C98, FldMenuNumber2Build
 * 0x00625730). */
const SprDigitFont fldMenuDigitFont2 = {
    fldMenuDigitFont2Rects,
    { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 13 },
}; /* 00675210 */
static const char save_00675220[] = "save"; /* the next file's .rodata starts here */
/* Party panel size (FldMenuPartyPanelOpen 0x00626090, lhu). */
const u16 fldMenuPartyPanelW = 270; /* 00675226 */
const u16 fldMenuPartyPanelH = 100; /* 00675228 */
static const char save_00675230[] = "save"; /* the next file's .rodata starts here */
