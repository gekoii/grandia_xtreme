/* FIELD .rodata 0x00671270-0x00671390: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C126C.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00671270-0x00671390 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef short s16;

/* One glyph of a sprite font (the same layout as Glyph in src/field/5C5B00.c):
 * size in pixels and its texture rect (u, v)-(uw, vh). */
typedef struct {
    s16 w;
    s16 h;
    u16 u;
    u16 v;
    u16 uw;
    u16 vh;
} Glyph;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_00671278[] = "save"; /* the next file's .rodata starts here */
static const char save_00671280[] = "save"; /* the next file's .rodata starts here */
static const char save_00671288[] = "save"; /* the next file's .rodata starts here */
/* Glyph rects of the field's damage-number font: the table that
 * fldDmgDigitFont (.data 0x00666788, {tbl, adv[12]}) points at; used by
 * FldDmgNumBuildSprites (src/field/5C5B00.c). [0]-[9] are the digits, [10]
 * repeats [9] and [11] repeats [0], [12] is the wide tag drawn after the
 * number. 13 x 12 bytes; gcc pads to 8 before the next "save". */
const Glyph fldDmgDigitGlyphs[13] = {
    { 0x13, 0x15, 0x01, 0x02, 0x28, 0x2C },
    { 0x12, 0x15, 0x2A, 0x02, 0x4F, 0x2C },
    { 0x14, 0x15, 0x51, 0x02, 0x7A, 0x2C },
    { 0x13, 0x15, 0x7C, 0x02, 0xA3, 0x2C },
    { 0x13, 0x15, 0xA5, 0x02, 0xCB, 0x2C },
    { 0x14, 0x15, 0x01, 0x2E, 0x2A, 0x58 },
    { 0x15, 0x15, 0x2C, 0x2E, 0x57, 0x58 },
    { 0x14, 0x15, 0x59, 0x2E, 0x82, 0x58 },
    { 0x13, 0x15, 0x84, 0x2E, 0xAB, 0x58 },
    { 0x14, 0x15, 0xAD, 0x2E, 0xD5, 0x58 },
    { 0x14, 0x15, 0xAD, 0x2E, 0xD5, 0x58 },
    { 0x13, 0x15, 0x01, 0x02, 0x28, 0x2C },
    { 0x33, 0x10, 0x01, 0x5A, 0x46, 0x71 },
}; /* 00671290 */
static const char save_00671330[] = "save"; /* the next file's .rodata starts here */
static const char save_00671338[] = "save"; /* the next file's .rodata starts here */
static const char save_00671340[] = "save"; /* the next file's .rodata starts here */
static const char save_00671348[] = "save"; /* the next file's .rodata starts here */
static const char save_00671350[] = "save"; /* the next file's .rodata starts here */
static const char save_00671358[] = "save"; /* the next file's .rodata starts here */
static const char save_00671360[] = "save"; /* the next file's .rodata starts here */
static const char save_00671368[] = "save"; /* the next file's .rodata starts here */
static const char save_00671370[0x10] = "save"; /* the next file's .rodata starts here */ /* padded: what follows is 16-aligned */
static const char save_00671380[] = "save"; /* the next file's .rodata starts here */
