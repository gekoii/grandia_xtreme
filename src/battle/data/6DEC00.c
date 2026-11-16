/* BATTLE .data, first part: the debug menu's trace/hit overrides, the
 * results screen layout (sprite lists and number fields), the hit effect
 * colours, the tactics window and auto camera panel, the command list
 * panels and number fields, and the text objects of the unit info window,
 * the caption, the learned/broken notice and the demo screen.
 * Data-only C (plan step 6b, docs/shifting.md). The rest of BATTLE's .data
 * (the IP gauge module onwards) is 6DF268.c: it starts a new original file
 * whose .data is 8-aligned, which leaves 4 bytes after btlCmdMenuActorMarker.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x006DEC00-0x006DF268 */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef float f32;
typedef f32 FVECTOR[4] __attribute__((aligned(16)));

/* Texture rectangle of a sprite (SprDrawRect): size, then the UV corners */
typedef struct {
    s16 w, h;
    u16 u0, v0;
    u16 u1, v1;
} SprRect;

/* SprDrawRectList entry: a rect at an offset; a NULL rect ends the list */
typedef struct {
    const SprRect *rect;
    s16 x, y;
} SprRectListEntry;

typedef struct SprDigitFont SprDigitFont;

/* Number field (MenuNumberLayout, MenuNumberDraw) */
typedef struct {
    const SprDigitFont *font;
    SprRectListEntry *sprites; /* output list, set at draw time */
    f32 sx, sy;
    s16 x, y;
    s8 digits;
} MenuNumField;

/* Text object (TxtObjSetString, MenuTextDraw); the first three members
 * and the last three are filled in at run time */
typedef struct {
    void *glyphs;
    SprRectListEntry *sprites;
    SprRect *rects;
    f32 alignX, alignY;
    s16 x, y;
    f32 scale;
    u8 style;   /* TXT_STYLE_* */
    s8 spacing; /* extra pixels per glyph */
    s16 width;
    s16 glyphFirst, glyphLast;
} TxtObj;

/* .rodata (6DF500.c, 6DFD18.c, 6E0320.c) */
extern const SprRect btlResultPanelSprites[];      /* results panel and bar sprites */
extern const SprDigitFont btlResultDigitFont;   /* results digit font */
extern const SprRect btlCmdListPanelRects[];      /* command list panel sprites */
extern const SprDigitFont btlHudDigitFont;   /* HUD digit font */
/* main */
extern const SprDigitFont menuDigitFont;
/* .bss */
extern TxtObj menuTacticsAiText;        /* "AI " */

/* BATTLE debug menu: -1 = off, else forces the trace colour / hit effect attr */
s8 dbgBtlTraceAttr = -1;
s8 dbgBtlHitAttr = -1;

/* Results screen */
SprRectListEntry btlResultTopBarRects[3] = {
    { &btlResultPanelSprites[5], 22, 54 },  /* D_006DFE9C */
    { &btlResultPanelSprites[6], 216, 46 }, /* D_006DFEA8 */
    { 0 },
};

SprRectListEntry btlResultGoldBarRects[3] = {
    { &btlResultPanelSprites[3], 15, 238 }, /* D_006DFE84 */
    { &btlResultPanelSprites[4], 15, 250 }, /* D_006DFE90 */
    { 0 },
};

SprRectListEntry btlResultMemberPanelRects[4] = {
    { &btlResultPanelSprites[0], 8, 86 },
    { &btlResultPanelSprites[1], 65, 86 },  /* D_006DFE6C */
    { &btlResultPanelSprites[2], 262, 86 }, /* D_006DFE78 */
    { 0 },
};

MenuNumField btlResultGoldNumFmt = { &btlResultDigitFont, 0, 1.0f, 1.0f, 0, 0, 8 };
MenuNumField btlResultGoldTotalNumFmt = { &btlResultDigitFont, 0, 1.0f, 1.0f, 0, 0, 8 };
MenuNumField btlResultExpNumFmt = { &btlResultDigitFont, 0, 1.0f, 1.0f, 230, 36, 8 };
MenuNumField btlResultNextExpNumFmt = { &btlResultDigitFont, 0, 1.0f, 1.0f, 230, 62, 8 };
MenuNumField btlResultLevelNumFmt = { &menuDigitFont, 0, 1.0f, 1.0f, 110, 92, 3 };
MenuNumField btlResultElementExpNumFmt = { &btlResultDigitFont, 0, 0.5f, 0.5f, 0, 0, 4 };

/* Hit effect: colour scale per hit attr (r, g, b, a) of the effect model's parts */
FVECTOR fxHitColors[9] = {
    { 128.0f, 128.0f, 128.0f, 128.0f },
    { 133.0f, 70.0f, 69.0f, 128.0f },
    { 92.0f, 109.0f, 128.0f, 128.0f },
    { 10.0f, 88.0f, 138.0f, 128.0f },
    { 110.0f, 89.0f, 110.0f, 128.0f },
    { 138.0f, 106.0f, 67.0f, 128.0f },
    { 69.0f, 58.0f, 138.0f, 128.0f },
    { 71.0f, 88.0f, 128.0f, 128.0f },
    { 94.0f, 138.0f, 124.0f, 128.0f },
};

/* Point light colour (r, g, b) per hit attr (FxHitSlotUpdate), edited by
 * DbgBtlTraceLightPage */
FVECTOR fxHitLightColors[10] = {
    { 256.0f, 256.0f, 256.0f },
    { 266.0f, 70.0f, 69.0f },
    { 92.0f, 218.0f, 256.0f },
    { 10.0f, 88.0f, 276.0f },
    { 220.0f, 89.0f, 220.0f },
    { 276.0f, 212.0f, 67.0f },
    { 69.0f, 58.0f, 276.0f },
    { 71.0f, 88.0f, 256.0f },
    { 94.0f, 276.0f, 124.0f },
    { 0.0f, 192.0f, 256.0f },
};

/* "No. %d" on the TRACE light / colour pages */
u8 dbgBtlTraceLightNo = 0;
u8 dbgBtlTraceColorNo = 0;

/* Tactics window */
TxtObj menuTacticsOkText = { 0, 0, 0, 0.5f, 0.0f, 0, 0, 1.0f, 2, 2 };
TxtObj menuTacticsAiLabelText = { 0, 0, 0, 0.5f, 1.0f, 0, 0, 1.0f, 0, 2 };

/* Text list drawn as the "AI A" / "AI B" banner: [2] (the set letter) is
 * set before each draw (MenuTacticsDraw) */
TxtObj *menuTacticsAiLabel[4] = { &menuTacticsAiLabelText, &menuTacticsAiText, 0, 0 };

/* Auto camera panel sprites; fields: D_006DEE94 [1], D_006DEEA0 [2],
 * D_006DEEAC [3], D_006DEEB8 [4], D_006DEEC4 [5], D_006DEED0 [6],
 * D_006DEEDC [7], D_006DEEE8 [8] */
SprRect menuAutoCamRects[9] = {
    { 128, 32, 1, 0xDF, 0x82, 0x100 },
    { 8, 24, 0x83, 0xE7, 0x8C, 0x100 },
    { 115, 8, 0x8B, 0xF7, 0xFF, 0x100 },
    { 70, 8, 0x96, 0xED, 0xDD, 0xF6 },
    { 35, 24, 0xDC, 0xDD, 0x100, 0xF6 },
    /* cursor */
    { 5, 31, 0xC9, 0xCD, 0xCD, 0xEB },
    { 6, 31, 0xD0, 0xCD, 0xD5, 0xEB },
    { 40, 10, 0xD8, 0xCA, 0xFF, 0xD3 },
    { 40, 6, 0xD8, 0xD6, 0xFF, 0xDB },
};

SprRectListEntry menuAutoCamPanelRects[6] = {
    { &menuAutoCamRects[0], 0, 0 },
    { &menuAutoCamRects[1], -8, 16 },
    { &menuAutoCamRects[2], 0, 32 },
    { &menuAutoCamRects[3], 115, 32 },
    { &menuAutoCamRects[4], 185, 16 },
    { 0 },
};

SprRectListEntry menuAutoCamCursorRects[5] = {
    { &menuAutoCamRects[5], -5, 1 },
    { &menuAutoCamRects[6], 32, 1 },
    { &menuAutoCamRects[7], -5, -9 },
    { &menuAutoCamRects[8], -5, 32 },
    { 0 },
};

/* Command list. The equipment slot names are one string pool (6E0198.c):
 * "Weapon" "Body" "Arm" "Head" "Leg" "Accs" */
extern const char btlCmdListEquipSlotNameText[];
const char *btlCmdListEquipSlotNames[6] = {
    btlCmdListEquipSlotNameText, btlCmdListEquipSlotNameText + 0xD, btlCmdListEquipSlotNameText + 0x16, btlCmdListEquipSlotNameText + 0x1D,
    btlCmdListEquipSlotNameText + 0x26, btlCmdListEquipSlotNameText + 0x2D,
};

MenuNumField btlCmdListNumCount = { &menuDigitFont, 0, 1.0f, 1.0f, 0, 0, 3 };
MenuNumField btlCmdListNumCur = { &menuDigitFont, 0, 1.0f, 0.0f, 0, 0, 3 };
MenuNumField btlCmdListNumMax = { &menuDigitFont, 0, 0.0f, 0.0f, 12, 0, 3 };

SprRectListEntry btlCmdListLeftSprites[8] = {
    { &btlCmdListPanelRects[0], 0, 0 },
    { &btlCmdListPanelRects[1], 8, 0 },
    { &btlCmdListPanelRects[2], 89, 0 },
    { &btlCmdListPanelRects[3], 113, 0 },
    { &btlCmdListPanelRects[4], 169, 0 },
    { &btlCmdListPanelRects[5], 185, 0 },
    { &btlCmdListPanelRects[6], 393, 0 },
    { 0 },
};

SprRectListEntry btlCmdListRightSprites[6] = {
    { &btlCmdListPanelRects[7], 0, 0 },
    { &btlCmdListPanelRects[8], 8, 0 },
    { &btlCmdListPanelRects[9], 58, 0 },
    { &btlCmdListPanelRects[10], 65, 0 },
    { &btlCmdListPanelRects[11], 161, 0 },
    { 0 },
};

SprRectListEntry btlCmdListTitleSprites[8] = {
    { &btlCmdListPanelRects[12], 0, 0 },
    { &btlCmdListPanelRects[13], 8, 0 },
    { &btlCmdListPanelRects[14], 42, 0 },
    { &btlCmdListPanelRects[15], 73, 0 },
    { &btlCmdListPanelRects[16], 312, 0 },
    { &btlCmdListPanelRects[17], 320, 0 },
    { &btlCmdListPanelRects[18], 354, 0 },
    { 0 },
};

/* Unit info window */
MenuNumField btlUnitInfoNumHp = { &menuDigitFont, 0, 1.0f, 0.0f, 120, 0, 6 };
MenuNumField btlUnitInfoNumHpMax = { &menuDigitFont, 0, 0.0f, 0.0f, 136, 0, 6 };
MenuNumField btlUnitInfoNumSp = { &menuDigitFont, 0, 1.0f, 0.0f, 120, 20, 3 };
MenuNumField btlUnitInfoNumSpMax = { &menuDigitFont, 0, 0.0f, 0.0f, 136, 20, 3 };
MenuNumField btlUnitInfoNumGauge[2] = {
    { &menuDigitFont, 0, 1.0f, 1.0f, 0, 0, 3 },
    { &menuDigitFont, 0, 1.0f, 1.0f, 0, 0, 3 },
};
MenuNumField btlUnitInfoNumLevel = { &menuDigitFont, 0, 0.0f, 1.0f, 24, 0, 3 };

TxtObj btlUnitInfoNameText = { 0, 0, 0, 0.0f, 0.0f, 0, 0, 1.0f, 0x21, 2 };
TxtObj btlUnitInfoActionText = { 0, 0, 0, 0.0f, 0.0f, 4, 100, 0.8333333f, 1, 2 };

/* Caption window; field: btlCaptionTextStyle .spacing */
TxtObj btlCaptionText = { 0, 0, 0, 0.5f, 0.5f, 3, 0, 1.0f, 1, 2 };

/* " Was Learned!" / " Has Broken!" notice; field: btlNoticeNameTextStyle .spacing */
TxtObj btlNoticeNameText = { 0, 0, 0, 1.0f, 1.0f, 0, 0, 0.8333333f, 0x22, 2 };
TxtObj btlNoticeSuffixText = { 0, 0, 0, 1.0f, 1.0f, 0, 0, 0.75f, 0x22, 2 };

/* Demo screen: "Press Start", "Demonstration", the credit line */
TxtObj btlMenuDemoPressStartText = { 0, 0, 0, 0.5f, 0.0f, 0, 0, 0.8333333f, 0x32, 10 };
TxtObj btlMenuDemoText = { 0, 0, 0, 0.5f, 0.0f, 0, 0, 0.8333333f, 0x32, 10 };
TxtObj btlMenuDemoCreditText = { 0, 0, 0, 1.0f, 0.0f, 0, 0, 0.6666667f, 2, 2 };

/* Command menu: hue per command for the pulsing name colour; field:
 * D_006DF24A [1] */
u16 btlCmdMenuHues[8] = {
    0x400, 0x8400, 0xC000, 0x1400, 0x6000, 0x2800, 0xD800, 0xA000,
};

/* Blinks over the acting member's status panel */
SprRect btlCmdMenuActorMarker = { 42, 16, 0xD5, 1, 0x100, 0x12 };
