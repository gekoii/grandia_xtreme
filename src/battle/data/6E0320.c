/* BATTLE .rodata: the rest of the command list's .rodata (the file that
 * starts at 6E0198.c): its panel sprites. Data-only C (plan step 6b,
 * docs/shifting.md). The next original file's .rodata (6E0440.c) is
 * 16-aligned, which leaves 12 zero bytes after the last rect.
 * Don't include the project headers here.
 */
/* rodata: 0x006E0320-0x006E0440 */

typedef signed short s16;
typedef unsigned short u16;

/* Texture rectangle of a sprite (SprDrawRect): size, then the UV corners */
typedef struct {
    s16 w, h;
    u16 u0, v0;
    u16 u1, v1;
} SprRect;

/* Panel parts of the .data sprite lists btlCmdListLeftSprites [0-6],
 * btlCmdListRightSprites [7-11] and btlCmdListTitleSprites [12-18].
 * Fields: D_006E032C [1], D_006E0338 [2], ... D_006E03F8 [18] (every 0xC) */
const SprRect btlCmdListPanelRects[19] = {
    { 8, 28, 1, 1, 0xA, 0x1E },
    { 81, 28, 0xE, 1, 0x11, 0x1E },
    { 24, 28, 0x15, 1, 0x2E, 0x1E },
    { 56, 28, 0x32, 1, 0x35, 0x1E },
    { 16, 28, 0x39, 1, 0x4A, 0x1E },
    { 208, 28, 0x4E, 1, 0x51, 0x1E },
    { 17, 28, 0x55, 1, 0x67, 0x1E },
    /* right panel */
    { 8, 20, 1, 0x1F, 0xA, 0x34 },
    { 50, 20, 0xE, 0x1F, 0x11, 0x34 },
    { 7, 20, 0x15, 0x1F, 0x1D, 0x34 },
    { 96, 20, 0x21, 0x1F, 0x24, 0x34 },
    { 17, 20, 0x28, 0x1F, 0x3A, 0x34 },
    /* title panel */
    { 8, 44, 1, 0x35, 0xA, 0x62 },
    { 34, 44, 0xE, 0x35, 0x11, 0x62 },
    { 31, 44, 0x15, 0x35, 0x35, 0x62 },
    { 239, 44, 0x39, 0x35, 0x3C, 0x62 },
    { 8, 44, 0x40, 0x35, 0x49, 0x62 },
    { 34, 44, 0x4D, 0x35, 0x50, 0x62 },
    { 8, 44, 0x54, 0x35, 0x5D, 0x62 },
};

/* Arrow drawn three times (equipment swap) */
const SprRect btlCmdListSwapArrowSprite = { 24, 24, 0xE7, 0x85, 0x100, 0x9E };
/* Label beside the button prompt for multi-castable cells */
const SprRect btlCmdListMultiCastSprite = { 76, 20, 1, 0xA1, 0x4E, 0xB6 };
/* Mark over a combo partner's panel who can't act or lacks SP */
const SprRect btlCmdListComboNgSprite = { 40, 40, 0xD1, 0x6D, 0xEA, 0x86 };
