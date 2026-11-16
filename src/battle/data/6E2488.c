/* BATTLE: the start of the .rodata of the battle HUD file (n33): the
 * sprites of the damage numbers (BtlHudDamageNumDraw 6D2548.c), the hit
 * counter and gauge (BtlHudGaugeDraw 6D1D78.c), the parameter changes
 * (BtlHudStatChangeDraw 6D2E38.c) and the markers (BtlHudMarker*Draw
 * 6D31B0.c to 6D3A10.c). Data-only C (plan step 6b, docs/shifting.md).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* rodata: 0x006E2488-0x006E2720 */

typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

/* Texture rectangle of a sprite (include/sys.h SprRect): size, then the UV
 * corners. */
typedef struct SprRect {
    s16 w;
    s16 h;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
} SprRect;

/* SprDrawRectList entry (include/sys.h SprRectListEntry): a NULL rect ends
 * the list. */
typedef struct SprRectListEntry {
    const SprRect *rect;
    s16 x;
    s16 y;
} SprRectListEntry;

/* Digit font of a number field (include/sys.h SprDigitFont): rects[0..9]
 * the digits, then extra marks; w the advance per glyph. */
typedef struct SprDigitFont {
    const SprRect *rects;
    s8 w[12];
} SprDigitFont;

/* 8 zero bytes before the file's "save" (16-aligned); nothing refers to
 * them */
const u32 D_006E2488[2] = { 0, 0 };

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* Labels over a damage number and the markers */
const SprRect btlHudMarkerRects[8] = {
    { 125, 25, 1, 56, 127, 82 },    /* critical */
    { 97, 25, 1, 83, 99, 109 },     /* D_006E24A4: cancel */
    { 120, 19, 135, 115, 256, 135 }, /* D_006E24B0: cancel resisted */
    { 79, 24, 1, 110, 81, 135 },    /* D_006E24BC: resist (BtlHudMarkerResistDraw) */
    { 116, 25, 128, 56, 245, 82 },  /* D_006E24C8: level down (BtlHudMarkerLevelDownDraw) */
    { 30, 18, 1, 162, 32, 181 },    /* btlHudMpSuffixRect: "MP" after a damage number (flag 0x40) */
    { 30, 18, 33, 162, 64, 181 },   /* btlHudSpSuffixRect: "SP" (flag 0x80) */
    { 96, 96, 47, 136, 72, 161 },   /* D_006E24EC: fail, drawn four times turning (BtlHudMarkerFailDraw) */
}; /* fields: D_006E24A4 [1], D_006E24B0 [2], D_006E24BC [3], D_006E24C8 [4],
    * btlHudMpSuffixRect [5], btlHudSpSuffixRect [6], D_006E24EC [7] */

/* Sprite lists over a damage number (BtlHudDamageNumDraw) */
const SprRectListEntry btlHudCriticalLabel[2] = { { &btlHudMarkerRects[0], -62, -12 }, { 0 } };    /* flag 2 */
const SprRectListEntry btlHudCancelLabel[2] = { { &btlHudMarkerRects[1], -48, -12 }, { 0 } };      /* flag 1 */
const SprRectListEntry btlHudCancelResistLabel[2] = { { &btlHudMarkerRects[2], -60, -9 }, { 0 } }; /* flag 8, shaking */

/* Hit counter digits: 0-9, the overflow mark, then "HITS" */
const SprRect btlHudHitsDigitRects[12] = {
    { 18, 24, 100, 171, 132, 213 },
    { 13, 24, 133, 171, 156, 213 },
    { 18, 24, 157, 171, 188, 213 },
    { 18, 24, 189, 171, 221, 213 },
    { 19, 24, 222, 171, 256, 213 },
    { 19, 24, 92, 214, 125, 256 },
    { 18, 24, 126, 214, 158, 256 },
    { 18, 24, 159, 214, 190, 256 },
    { 18, 24, 191, 214, 223, 256 },
    { 18, 24, 224, 214, 256, 256 },
    { 18, 24, 224, 214, 256, 256 },
    { 26, 14, 1, 136, 46, 161 }, /* btlHudHitsRect: "HITS" after the hit counts */
}; /* fields: btlHudHitsRect [11] */

/* Their font: btlHudPopupHitsNumFmt and btlHudGaugeHitsNumFmt (.data) */
const SprDigitFont btlHudHitsDigitFont = { btlHudHitsDigitRects, { 15, 9, 15, 15, 17, 15, 15, 13, 15, 15, 15, 0 } };

/* Gauge digits 0-9, the overflow mark, then [11] and [12] frame the extra
 * number after the hit counter (BtlHudGaugeDraw) */
const SprRect btlHudGaugeRects[13] = {
    { 10, 16, 82, 112, 93, 129 },
    { 10, 16, 94, 112, 105, 129 },
    { 10, 16, 106, 112, 117, 129 },
    { 10, 16, 118, 112, 129, 129 },
    { 10, 16, 82, 130, 93, 147 },
    { 10, 16, 94, 130, 105, 147 },
    { 10, 16, 106, 130, 117, 147 },
    { 10, 16, 118, 130, 129, 147 },
    { 10, 16, 82, 148, 93, 165 },
    { 10, 16, 94, 148, 105, 165 },
    { 10, 16, 94, 148, 105, 165 },
    { 13, 13, 130, 136, 144, 150 },
    { 10, 16, 106, 148, 117, 165 }, /* D_006E2658 */
}; /* fields: D_006E2658 [12] */

/* Their font: btlHudGaugeBonusNumFmt (.data) */
const SprDigitFont btlHudGaugeDigitFont = { btlHudGaugeRects, { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 13 } };

/* Parameter change labels [0-4], then the up arrow [5], the down arrow [6]
 * and "normal" [7] (BtlHudStatChangeDraw) */
const SprRect btlHudStatChangeRects[8] = {
    { 62, 30, 1, 1, 64, 32 },
    { 62, 30, 65, 1, 128, 32 },
    { 62, 30, 186, 83, 249, 114 },
    { 62, 30, 129, 1, 192, 32 },
    { 62, 30, 193, 1, 256, 32 },
    { 52, 21, 1, 33, 54, 55 },
    { 102, 21, 55, 33, 158, 55 },
    { 92, 21, 159, 33, 252, 55 },
};

/* Marker type 1, evade (BtlHudMarkerEvadeDraw) */
const SprRect btlHudEvadeRect = { 58, 15, 131, 153, 190, 169 };
const SprRectListEntry btlHudEvadeLabel[2] = { { &btlHudEvadeRect, -29, -8 }, { 0 } };

/* Marker type 2, miss (BtlHudMarkerMissDraw) */
const SprRect btlHudMissRect = { 44, 15, 145, 136, 190, 152 };
const SprRectListEntry btlHudMissLabel[2] = { { &btlHudMissRect, -22, -8 }, { 0 } };
