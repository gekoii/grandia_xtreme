/* BATTLE .rodata: the start of a file's .rodata (its "save" string), the
 * unit info window's frame colours, sprites and action strings. Data-only C
 * (plan step 6b, docs/shifting.md). The text is in the game's font code:
 * each character is a byte (ASCII + 0x1F) followed by 0xF9; the comments
 * give it as text. Don't include the project headers here.
 */
/* rodata: 0x006E0440-0x006E05F0 */

typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;

/* Texture rectangle of a sprite (SprDrawRect): size, then the UV corners */
typedef struct {
    s16 w, h;
    u16 u0, v0;
    u16 u1, v1;
} SprRect;

/* Window background colours (SprCol words): c0 for corners 0 and 2, c1 for
 * corners 1 and 3 */
typedef struct {
    u32 c0, c1;
} ColPair;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* Unit info window background per character (id - 0x1001) */
const ColPair btlUnitInfoCharColors[8] = {
    { 0x2040804C, 0x0000704C },
    { 0x0000004C, 0x7000004C },
    { 0x7030584C, 0x6000004C },
    { 0x2080604C, 0x0050004C },
    { 0x3020804C, 0x2000604C },
    { 0x7060104C, 0x3050004C },
    { 0x7040004C, 0x6000004C },
    { 0x5000704C, 0x0000504C },
};

/* Unit info window sprites. Fields: D_006E04F4 [9], D_006E0500 [10],
 * D_006E0518 [12], D_006E0524 [13], D_006E0530 [14], D_006E053C [15],
 * D_006E0554 [17], D_006E0560 [18] */
const SprRect btlUnitInfoSprites[20] = {
    { 87, 12, 0xA4, 0xF3, 0xFC, 0x100 },
    { 40, 12, 0xA4, 0xE5, 0xCD, 0xF2 },
    { 43, 12, 0xCE, 0xE5, 0xFA, 0xF2 },
    { 10, 10, 0xCD, 0x4F, 0xD8, 0x5A },
    { 127, 10, 0xD8, 0x4F, 0xEA, 0x5A },
    { 2, 10, 0xEA, 0x4F, 0xED, 0x5A },
    { 16, 4, 0xDE, 0x49, 0xEF, 0x4E },
    { 17, 18, 0xEE, 0x49, 0x100, 0x5C },
    { 59, 15, 1, 0xE4, 0x3D, 0xF4 },
    { 58, 15, 0x3E, 0xE4, 0x79, 0xF4 },
    { 38, 13, 0x7A, 0xE6, 0xA1, 0xF4 },
    { 17, 13, 0xED, 0x5D, 0xFF, 0x6B },
    { 46, 10, 0x19, 0xF5, 0x48, 0x100 },
    { 65, 10, 0x49, 0xF5, 0x8B, 0x100 },
    { 20, 20, 0xB7, 0x9D, 0xD0, 0xB6 },
    { 18, 18, 0xD7, 0x69, 0xF0, 0x82 },
    { 54, 18, 0x89, 0x83, 0xD2, 0x9C },
    { 30, 24, 0x2B, 0xBD, 0x54, 0xDE },
    { 76, 128, 0, 0, 0x4D, 0x81 },
    { 52, 128, 0x4C, 0, 0x81, 0x81 },
};

/* The action line */
const char btlStrWaiting[] = "v\371\200\371\210\371\223\371\210\371\215\371\206\371"; /* "Waiting" */
const char btlStrHiddenAction[] = "^\371^\371^\371^\371^\371^\371^\371^\371"; /* "????????" (an enemy's hidden action) */
const char btlStrFallen[] = "e\371\200\371\213\371\213\371\204\371\215\371"; /* "Fallen" */
const char btlStrCommand[] = "b\371\216\371\214\371\214\371\200\371\215\371\203\371Y\371"; /* "Command:" */
const char btlStrDefense[] = "c\371\204\371\205\371\204\371\215\371\222\371\204\371"; /* "Defense" */
static const char strDashes[] = "L\371L\371L\371L\371L\371L\371L\371L\371"; /* "--------", unused */
