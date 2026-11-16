/* BATTLE .rodata: the "save" strings of six original files (five of them
 * have nothing else in .rodata), the special move intro's caption and
 * voices, and the results screen's sprites and digit font. Data-only C
 * (plan step 6b, docs/shifting.md). The caption is in the game's font code:
 * each character is a byte (ASCII + 0x1F) followed by 0xF9.
 * Don't include the project headers here.
 */
/* rodata: 0x006DFD18-0x006DFF78 */

typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef float f32;

/* Texture rectangle of a sprite (SprDrawRect): size, then the UV corners */
typedef struct {
    s16 w, h;
    u16 u0, v0;
    u16 u1, v1;
} SprRect;

/* Digit font of a number field (MenuNumberLayout): glyph rects (0-9, then
 * extra marks) and the advance per glyph */
typedef struct {
    const SprRect *rects;
    s8 w[12];
} SprDigitFont;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_006DFD20[] = "save"; /* the next file's .rodata starts here */

/* Special move intro (BtlSpecialIntroUpdate) */
const char btlRageCaption[] = "q\371\200\371\206\371\204\371@\371"; /* "Rage!" */
/* SE played when a member's special move intro starts, by character */
const u16 btlCharIntroVoiceSe[8] = {
    0xFB4, 0xFC9, 0xFE3, 0xFFB, 0x1014, 0x102D, 0x1046, 0x1060,
};

static const char save_006DFD48[] = "save"; /* the next file's .rodata starts here */
static const char save_006DFD50[] = "save"; /* the next file's .rodata starts here */
static const char save_006DFD58[] = "save"; /* the next file's .rodata starts here */

/* Results screen: the title and header states, gold/EXP labels,
 * separators, and from [10] the bonus marks by tier. Fields: D_006DFD6C [1],
 * D_006DFD84 [3], D_006DFDC0 [8], D_006DFDCC [9] */
const SprRect btlResultSprites[15] = {
    { 174, 46, 1, 1, 0xB0, 0x30 },
    { 126, 34, 1, 0x31, 0x80, 0x54 },
    { 150, 34, 1, 0x55, 0x98, 0x78 },
    { 78, 22, 1, 0x79, 0x50, 0x90 },
    { 78, 22, 1, 0x91, 0x50, 0xA8 },
    { 90, 26, 1, 0xA9, 0x5C, 0xC4 },
    { 118, 26, 1, 0xC5, 0x78, 0xE0 },
    { 22, 22, 0x51, 0x79, 0x68, 0x90 },
    { 46, 22, 0x51, 0x91, 0x80, 0xA8 },
    { 94, 34, 0xA1, 0xA1, 0x100, 0xC4 },
    { 70, 30, 0xB9, 0x41, 0x100, 0x60 },
    { 70, 30, 0xB9, 0x61, 0x100, 0x80 },
    { 70, 30, 0xB9, 0x81, 0x100, 0xA0 },
    { 70, 30, 0xB9, 1, 0x100, 0x20 },
    { 70, 30, 0xB9, 0x21, 0x100, 0x40 },
};

/* Bonus multiplier by tier */
const f32 btlResultBonusRates[5] = { 0.1f, 0.2f, 0.3f, 0.5f, 1.0f };

/* Bonus banner by kind (texture 0x98) */
const SprRect btlResultBannerSprites[4] = {
    { 254, 62, 1, 1, 0x100, 0x40 },
    { 254, 62, 1, 0x41, 0x100, 0x80 },
    { 254, 62, 1, 0x81, 0x100, 0xC0 },
    { 254, 62, 1, 0xC1, 0x100, 0x100 },
};

/* Member panel parts [0-2], gold bars [3-4], top bar [5-6] (the .data
 * sprite lists btlResult*Rects) and the header [7] (texture 0x99).
 * Fields: D_006DFE6C [1], D_006DFE78 [2], D_006DFE84 [3], D_006DFE90 [4],
 * D_006DFE9C [5], D_006DFEA8 [6], btlResultHeaderSprite [7] */
const SprRect btlResultPanelSprites[8] = {
    { 57, 16, 1, 1, 0x3B, 0x12 },
    { 197, 8, 0x3A, 1, 0xFF, 0xA },
    { 106, 8, 0x96, 0xB, 0x100, 0x14 },
    { 114, 12, 1, 0x13, 0x74, 0x20 },
    { 182, 31, 1, 0x1F, 0xB8, 0x3F },
    { 194, 68, 1, 0x40, 0xC4, 0x85 },
    { 41, 76, 0xC3, 0x38, 0xED, 0x85 },
    { 144, 120, 1, 0x87, 0x92, 0x100 },
};

/* Member portrait (texture = character id - 0x67) */
const SprRect btlResultPortraitSprite = { 94, 94, 1, 1, 0x60, 0x60 };

/* Glyphs of the results digit font btlResultDigitFont: 0-9, then two marks */
const SprRect btlResultDigitRects[12] = {
    { 16, 20, 1, 0xEB, 0x12, 0x100 },
    { 16, 20, 0x13, 0xEB, 0x24, 0x100 },
    { 16, 20, 0x25, 0xEB, 0x36, 0x100 },
    { 16, 20, 0x37, 0xEB, 0x48, 0x100 },
    { 16, 20, 0x49, 0xEB, 0x5A, 0x100 },
    { 16, 20, 0x5B, 0xEB, 0x6C, 0x100 },
    { 16, 20, 0x6D, 0xEB, 0x7E, 0x100 },
    { 16, 20, 0x7F, 0xEB, 0x90, 0x100 },
    { 16, 20, 0x91, 0xEB, 0xA2, 0x100 },
    { 16, 20, 0xA3, 0xEB, 0xB4, 0x100 },
    { 16, 20, 0xA3, 0xEB, 0xB4, 0x100 },
    { 16, 20, 1, 0xEB, 0x12, 0x100 },
};

/* Results digit font (gold, EXP) */
const SprDigitFont btlResultDigitFont = {
    btlResultDigitRects, { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
};

static const char save_006DFF70[] = "save"; /* the next file's .rodata starts here */
