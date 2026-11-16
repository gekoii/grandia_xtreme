/* BATTLE .data, second part (from the start of an original file, the IP
 * gauge module): the IP gauge debug values and sprites, the party panel
 * frame, the spark burst keys and colours, camera quake and stage debug
 * flags, the HUD's HP bar and number fields, and the debug menu's selected
 * numbers. Data-only C (plan step 6b, docs/shifting.md). Don't include the
 * project headers here: declare what the data refers to locally.
 */
/* data: 0x006DF268-0x006DF500 */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

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

/* Scale keys of PtclBtlSprDrawKeyed */
typedef struct {
    f32 scale[4];
    s8 life; /* key 0 time */
    s8 unk11;
    s8 t1;   /* key 1, 2 times */
    s8 t2;
} PtclSparkKeys;

/* main */
extern const SprRect ipIconPartyRects[];   /* party member faces */
extern const SprRect menuHelpVsTypeIcons[];
/* .rodata (6DF500.c) and other files */
extern const SprDigitFont btlHudDigitFont;
extern const SprDigitFont btlHudHitsDigitFont;
extern const SprDigitFont btlHudGaugeDigitFont;

/* IP gauge debug page: position, scale and angle of the dial */
s16 ipDialX = 0;
s16 ipDialY = 0;
f32 ipDialScaleX = 1.0f;
f32 ipDialScaleY = 1.0f;
s32 ipDialAng = 0;

/* The dial, the needle and the party icon ring frames; field: D_006DF284 [1] */
SprRect ipDialRects[4] = {
    { 202, 166, 1, 1, 0xCC, 0xA8 },
    { 14, 14, 0xE1, 0x2D, 0xF0, 0x3C },
    { 42, 16, 0xD5, 1, 0x100, 0x12 },
    { 42, 16, 0xD5, 0x97, 0x100, 0xA8 },
};

/* Pointers under the icons on the dial; field: ipEnemyPointerRect [1] */
SprRect ipPartyPointerRect[2] = {
    { 14, 14, 0xD1, 0x79, 0xE0, 0x88 },
    { 14, 14, 0xE1, 0x79, 0xF0, 0x88 },
};

/* Animated ring around the enemy icons */
SprRect ipEnemyRingRects[4] = {
    { 32, 32, 1, 0xB7, 0x22, 0xD8 },
    { 32, 32, 0x23, 0xB7, 0x44, 0xD8 },
    { 32, 32, 0x45, 0xB7, 0x66, 0xD8 },
    { 32, 32, 0x67, 0xB7, 0x88, 0xD8 },
};

/* Icon and ring; the rects are patched per icon */
SprRectListEntry ipPartyIconSprList[3] = {
    { ipIconPartyRects, -12, -12 },
    { ipEnemyRingRects, -16, -16 },
    { 0 },
};

SprRectListEntry ipEnemyIconSprList[3] = {
    { menuHelpVsTypeIcons, -12, -12 },
    { ipEnemyRingRects, -16, -16 },
    { 0 },
};

/* Party panel frame (texture 0xED), then the 52x52 portrait per character
 * 1-8. The code indexes the portraits by character id (0x1001..) through
 * ipPanelCharRects (0x006D3320), a base biased by -0x1001 elements. */
SprRect ipPanelFrameRect[9] = {
    { 148, 56, 1, 0xA9, 0x96, 0xE2 },
    { 52, 52, 1, 1, 0x36, 0x36 },
    { 52, 52, 0x37, 1, 0x6C, 0x36 },
    { 52, 52, 0x6D, 1, 0xA2, 0x36 },
    { 52, 52, 0xA3, 1, 0xD8, 0x36 },
    { 52, 52, 1, 0x37, 0x36, 0x6C },
    { 52, 52, 0x37, 0x37, 0x6C, 0x6C },
    { 52, 52, 0x6D, 0x37, 0xA2, 0x6C },
    { 52, 52, 0xA3, 0x37, 0xD8, 0x6C },
};

MenuNumField ipPanelHpNumFmt = { &btlHudDigitFont, 0, 1.0f, 1.0f, 144, 34, 5 };

/* Spark bursts (burst types 13-24): scale keys and colour keys (RGBA) */
PtclSparkKeys ptclSparkKeys = { { 0.0f, 4.0f, 2.0f, 0.0f }, 40, 20, 10, 20 };
PtclSparkKeys ptclSparkGlowKeys = { { 0.0f, 3.0f, 2.0f, 0.0f }, 30, 10, 10, 15 };

u8 ptclSparkColBlue[4][4] = {
    { 0x00, 0x40, 0x80, 0x80 }, { 0x40, 0x80, 0xFF, 0x80 }, { 0x00, 0x46, 0x64, 0x80 }, { 0x12, 0x00, 0x40, 0x80 },
};
u8 ptclSparkColRainbow[4][4] = {
    { 0x80, 0x00, 0x00, 0x80 }, { 0x00, 0x80, 0x00, 0x80 }, { 0x00, 0x00, 0x80, 0x80 }, { 0x80, 0x00, 0x00, 0x80 },
};
u8 ptclSparkColGreen[4][4] = {
    { 0x00, 0x80, 0x40, 0x80 }, { 0x80, 0xFF, 0x50, 0x80 }, { 0x00, 0x80, 0x28, 0x80 }, { 0x00, 0x40, 0x20, 0x80 },
};
u8 ptclSparkGlowColBlue[4][4] = {
    { 0x3C, 0x5A, 0x80, 0x80 }, { 0x80, 0xFF, 0xFF, 0x80 }, { 0x3C, 0x5A, 0x80, 0x80 }, { 0x12, 0x0B, 0x5A, 0x80 },
};
u8 ptclSparkGlowColRainbow[4][4] = {
    { 0x80, 0x50, 0x50, 0x80 }, { 0x50, 0x80, 0x50, 0x80 }, { 0x50, 0x50, 0x80, 0x80 }, { 0x80, 0x50, 0x50, 0x80 },
};
u8 ptclSparkGlowColGreen[4][4] = {
    { 0x3C, 0x80, 0x3C, 0x80 }, { 0xA0, 0xFF, 0x80, 0x80 }, { 0x3C, 0x80, 0x3C, 0x80 }, { 0x00, 0x5A, 0x14, 0x80 },
};

/* Camera quake: frames between shake updates ("INT"), countdown */
s8 btlCamQuakeInterval = 0;
s8 btlCamQuakeWait = 0;

/* LIGHT page: selected light */
s32 dbgBtlEnvLightNo = 0;

/* FIELD page: "CLIP" (hide stage parts that block the camera), "CHK_DISP" */
s8 envStageClipOn = 1;
s8 envStageZoneDisp = 0;

/* HUD: HP bar frame, lost part, remaining part (BtlHudHpBarDraw patches
 * the widths and u) */
SprRect btlHudHpBarRects[3] = {
    { 84, 24, 0, 0xB8, 0x55, 0xD1 },
    { 0, 24, 0, 0xD0, 0, 0xE9 },
    { 0, 24, 0, 0xE8, 0x51, 0x101 },
};

MenuNumField btlHudPopupDamageNumFmt = { &btlHudDigitFont, 0, 0.0f, 0.0f, 0, -12, 8 };
MenuNumField btlHudPopupHitsNumFmt = { &btlHudHitsDigitFont, 0, 0.0f, 0.0f, 0, -24, 8 };
MenuNumField btlHudGaugeHitsNumFmt = { &btlHudHitsDigitFont, 0, 0.5f, 0.5f, -5, 0, 8 };
MenuNumField btlHudGaugeBonusNumFmt = { &btlHudGaugeDigitFont, 0, 0.0f, 1.0f, 0, 0, 8 };
MenuNumField btlHudDamageNumFmt = { &btlHudDigitFont, 0, 0.5f, 0.5f, -5, 0, 8 };

/* Debug menu: WORK EGG / WORK SKILL selection, the unit shown by CHAR
 * WORK, CHAR PARAM and PAR TEST, the PC SAVE party member */
s32 dbgBtlEggNo = 0;
s32 dbgBtlSkillNo = 0;
s32 dbgBtlCharNo = 0;
s32 dbgBtlPcNo = 0;
