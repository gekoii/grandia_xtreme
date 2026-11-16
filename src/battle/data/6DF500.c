/* BATTLE .rodata: the start of a file's .rodata (its "save" string), the
 * HP/MP/SP labels, the HUD's digit font, the particle burst types and the
 * debug menu's top page strings. Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here: declare what
 * the data refers to locally.
 */
/* rodata: 0x006DF500-0x006DFCD0 */

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

/* Digit font of a number field (MenuNumberLayout): glyph rects (0-9, then
 * extra marks) and the advance per glyph */
typedef struct {
    const SprRect *rects;
    s8 w[12];
} SprDigitFont;

struct GphAnimSet;

/* Particle burst type (PtclBurst*, also the weapon traces) */
typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    u8 unk3;
    f32 unk4;
    u8 tex;
    u8 pad9[3];
    struct GphAnimSet **animSet;
    f32 scale[4];       /* scale keys */
    s16 scaleT0, scaleT1;
    f32 scaleRate[3];
    u8 color[4][4];     /* colour keys (RGBA) */
    s16 colorT0, colorT1;
    f32 colorRate[3];
} PtclBurstType;

/* .bss: animation sets of the particle textures 0x38-0x3D */
extern struct GphAnimSet *ptclAnimSetTex38;
extern struct GphAnimSet *ptclAnimSetTex39;
extern struct GphAnimSet *ptclAnimSetTex3A;
extern struct GphAnimSet *ptclAnimSetTex3B;
extern struct GphAnimSet *ptclAnimSetTex3C;
extern struct GphAnimSet *ptclAnimSetTex3D;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* 30x18 labels HP, MP, SP (unit info, IP panels); fields: btlStatLabelMp [1],
 * btlStatLabelSp [2] */
const SprRect btlStatLabelSprites[3] = {
    { 30, 18, 0xCD, 0x5B, 0xEC, 0x6E },
    { 30, 18, 0xCD, 0x6F, 0xEC, 0x82 },
    { 30, 18, 0xCD, 0x83, 0xEC, 0x96 },
};

/* Glyphs of the HUD digit font btlHudDigitFont: 0-9, then two marks */
const SprRect btlHudDigitRects[12] = {
    { 19, 21, 1, 2, 0x28, 0x2C },
    { 18, 21, 0x2A, 2, 0x4F, 0x2C },
    { 20, 21, 0x51, 2, 0x7A, 0x2C },
    { 19, 21, 0x7C, 2, 0xA3, 0x2C },
    { 19, 21, 0xA5, 2, 0xCB, 0x2C },
    { 20, 21, 1, 0x2E, 0x2A, 0x58 },
    { 21, 21, 0x2C, 0x2E, 0x57, 0x58 },
    { 20, 21, 0x59, 0x2E, 0x82, 0x58 },
    { 19, 21, 0x84, 0x2E, 0xAB, 0x58 },
    { 20, 21, 0xAD, 0x2E, 0xD5, 0x58 },
    { 20, 21, 0xAD, 0x2E, 0xD5, 0x58 },
    { 19, 21, 1, 2, 0x28, 0x2C },
};

/* Label after the combo popup's damage number */
const SprRect btlHudPopupDamageRect = { 51, 16, 1, 0x5A, 0x46, 0x71 };

/* HUD digit font (panel HP, damage numbers) */
const SprDigitFont btlHudDigitFont = {
    btlHudDigitRects, { 15, 13, 16, 15, 15, 15, 15, 14, 15, 17, 17, 15 },
};

/* field: D_006DF5F0 [0].scale */
const PtclBurstType ptclBurstTypeTable[17] = {
    { 0, 20, 2, 15, 2.5f, 0x36, { 0 }, 0,
      { 0.0f, 2.0f, 1.0f, 0.0f }, 15, 10, { 0.2f, 0.2f, 0.1f },
      { { 0xFF, 0xC8, 0xA0, 0x80 }, { 0xA0, 0x96, 0x6E, 0x80 }, { 0x32, 0x64, 0x6E, 0x80 }, { 0x19, 0x28, 0x37, 0x80 } }, 12, 4, { 0.125f, 0.125f, 0.25f } },
    { 1, 24, 2, 12, 0.6f, 0x3D, { 0 }, &ptclAnimSetTex3D,
      { 0.0f, 2.0f, 1.0f, 0.0f }, 20, 8, { 0.25f, 0.083333336f, 0.125f },
      { { 0x80, 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0x80, 0x80 } }, 0, 0, { 1.0f, 1.0f, 1.0f } },
    { 1, 16, 2, 12, 1.2f, 0x3B, { 0 }, &ptclAnimSetTex3B,
      { 1.0f, 1.2f, 1.5f, 2.0f }, 12, 6, { 0.25f, 0.16666667f, 0.16666667f },
      { { 0xA0, 0x82, 0x50, 0x80 }, { 0x64, 0x5A, 0x50, 0x80 }, { 0x3C, 0x5A, 0x64, 0x80 }, { 0x2D, 0x4B, 0x6E, 0x80 } }, 11, 4, { 0.2f, 0.14285715f, 0.25f } },
    { 1, 16, 2, 12, 1.2f, 0x3A, { 0 }, &ptclAnimSetTex3A,
      { 1.0f, 1.5f, 2.0f, 3.0f }, 12, 6, { 0.25f, 0.16666667f, 0.16666667f },
      { { 0xA0, 0xB4, 0xBE, 0x80 }, { 0x69, 0xA5, 0xB4, 0x80 }, { 0x00, 0x3C, 0x96, 0x80 }, { 0x00, 0x3C, 0x80, 0x80 } }, 12, 8, { 0.25f, 0.25f, 0.125f } },
    { 1, 20, 1, 15, 1.5f, 0x37, { 0 }, 0,
      { 1.0f, 1.0f, 1.0f, 1.0f }, 0, 0, { 1.0f, 1.0f, 1.0f },
      { { 0xC8, 0xB4, 0x55, 0x80 }, { 0xB4, 0x64, 0x55, 0x80 }, { 0x80, 0x64, 0x55, 0x80 }, { 0x80, 0x64, 0x55, 0x00 } }, 12, 8, { 0.125f, 0.25f, 0.125f } },
    { 1, 18, 2, 5, 0.5f, 0x3C, { 0 }, &ptclAnimSetTex3C,
      { 1.0f, 3.0f, 2.0f, 1.0f }, 15, 12, { 0.33333334f, 0.33333334f, 0.083333336f },
      { { 0x80, 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0x80, 0x80 } }, 0, 0, { 1.0f, 1.0f, 1.0f } },
    { 1, 16, 2, 20, 1.0f, 0x38, { 0 }, &ptclAnimSetTex38,
      { 1.0f, 1.0f, 1.0f, 1.0f }, 0, 0, { 1.0f, 1.0f, 1.0f },
      { { 0xB4, 0xB4, 0xB4, 0x80 }, { 0x80, 0x80, 0x80, 0x80 }, { 0x3C, 0x00, 0x64, 0x80 }, { 0x00, 0x00, 0x32, 0x80 } }, 11, 6, { 0.2f, 0.2f, 0.16666667f } },
    { 1, 24, 2, 20, 2.0f, 0x39, { 0 }, &ptclAnimSetTex39,
      { 1.0f, 1.0f, 1.0f, 1.0f }, 0, 0, { 1.0f, 1.0f, 1.0f },
      { { 0x80, 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0x80, 0x80 }, { 0x80, 0x80, 0x80, 0x80 } }, 0, 0, { 1.0f, 1.0f, 1.0f } },
    { 1, 20, 2, 20, 2.0f, 0x36, { 0 }, 0,
      { 0.0f, 2.0f, 1.0f, 0.0f }, 15, 10, { 0.2f, 0.2f, 0.1f },
      { { 0x80, 0x80, 0x50, 0x80 }, { 0x41, 0x80, 0x50, 0x80 }, { 0x32, 0x78, 0x64, 0x80 }, { 0x28, 0x64, 0x8C, 0x80 } }, 12, 4, { 0.125f, 0.125f, 0.25f } },
    { 0, 20, 2, 15, 2.5f, 0x36, { 0 }, 0,
      { 0.0f, 2.0f, 1.0f, 0.0f }, 15, 10, { 0.2f, 0.2f, 0.1f },
      { { 0xFF, 0xC8, 0xA0, 0x80 }, { 0xA0, 0x96, 0x6E, 0x80 }, { 0x32, 0x64, 0x6E, 0x80 }, { 0x19, 0x28, 0x37, 0x80 } }, 12, 4, { 0.125f, 0.125f, 0.25f } },
    { 1, 16, 2, 12, 2.0f, 0x3A, { 0 }, &ptclAnimSetTex3A,
      { 1.5f, 2.0f, 2.5f, 3.0f }, 12, 6, { 0.25f, 0.16666667f, 0.16666667f },
      { { 0x00, 0xFF, 0x00, 0x80 }, { 0x19, 0x80, 0x00, 0x80 }, { 0x3C, 0x64, 0x32, 0x80 }, { 0x80, 0x64, 0x32, 0x40 } }, 12, 8, { 0.25f, 0.25f, 0.125f } },
    { 0, 16, 2, 12, 2.0f, 0x3A, { 0 }, &ptclAnimSetTex3A,
      { 1.0f, 1.5f, 2.0f, 2.5f }, 12, 6, { 0.25f, 0.16666667f, 0.16666667f },
      { { 0x00, 0x80, 0x00, 0x80 }, { 0x28, 0x80, 0x28, 0x80 }, { 0x48, 0x80, 0x18, 0x80 }, { 0x80, 0x44, 0x00, 0x00 } }, 12, 4, { 0.25f, 0.125f, 0.25f } },
    { 0, 16, 2, 12, 2.0f, 0x3A, { 0 }, &ptclAnimSetTex3A,
      { 1.0f, 1.5f, 2.0f, 2.5f }, 12, 6, { 0.25f, 0.16666667f, 0.16666667f },
      { { 0x80, 0x00, 0x80, 0x80 }, { 0x5A, 0x00, 0x80, 0x80 }, { 0x1E, 0x00, 0x80, 0x80 }, { 0x00, 0x40, 0x80, 0x00 } }, 12, 4, { 0.25f, 0.125f, 0.25f } },
    { 0, 16, 3, 12, 2.0f, 0x33, { 0 }, 0,
      { 1.0f, 1.5f, 2.0f, 2.5f }, 12, 6, { 0.25f, 0.16666667f, 0.16666667f },
      { { 0x5A, 0x78, 0x32, 0x80 }, { 0x5A, 0x78, 0x32, 0x80 }, { 0x5A, 0x78, 0x41, 0x40 }, { 0x50, 0x78, 0x5A, 0x00 } }, 12, 4, { 0.25f, 0.125f, 0.25f } },
    { 0, 16, 1, 12, 2.0f, 0x33, { 0 }, 0,
      { 1.0f, 1.5f, 2.0f, 2.5f }, 12, 6, { 0.25f, 0.16666667f, 0.16666667f },
      { { 0x50, 0x46, 0x46, 0x80 }, { 0x50, 0x46, 0x3C, 0x80 }, { 0x32, 0x1E, 0x00, 0x40 }, { 0x0A, 0x0A, 0x00, 0x00 } }, 12, 4, { 0.25f, 0.125f, 0.25f } },
    { 0, 32, 1, 12, 2.0f, 0x33, { 0 }, 0,
      { 1.5f, 1.5f, 2.0f, 2.5f }, 24, 12, { 0.125f, 0.083333336f, 0.083333336f },
      { { 0x64, 0x78, 0x50, 0x30 }, { 0x64, 0x78, 0x32, 0x60 }, { 0x64, 0x50, 0x32, 0x30 }, { 0x64, 0x50, 0x32, 0x00 } }, 24, 8, { 0.125f, 0.0625f, 0.125f } },
    { 1, 32, 3, 12, 1.2f, 0x3B, { 0 }, &ptclAnimSetTex3B,
      { 1.0f, 1.2f, 1.5f, 2.0f }, 24, 12, { 0.125f, 0.083333336f, 0.083333336f },
      { { 0x5A, 0x78, 0x32, 0x80 }, { 0x5A, 0x78, 0x32, 0x80 }, { 0x5A, 0x78, 0x41, 0x40 }, { 0x50, 0x78, 0x5A, 0x00 } }, 22, 8, { 0.1f, 0.071428575f, 0.125f } },
};

/* Round glow used by most battle 3D sprites */
const SprRect ptclGlowRect = { 2, 2, 0, 0, 0x21, 0x21 };

/* Multiplied by a part matrix to get its position */
const FVECTOR btlVecOrigin = { 0.0f, 0.0f, 0.0f, 1.0f };

const char btlCfgFileName[] = "BTL_DATA/BTL_CFG.TXT";

/* Debug menu, top page */
const char dbgBtlTimestopFmt[] = "timestop:%d";
const char dbgBtlTopTitleFmt[] = "\\w30 %sBATTLE%s \\w12";
const char dbgBtlTopTitleColorStr[] = "\\c1 ";
const char dbgBtlTopColor0Str[] = "\\c0 ";
const char dbgBtlCharStr[] = "> CHAR";
const char dbgBtlDataStr[] = "> DATA";
const char dbgBtlEnvStr[] = "> ENV";
const char dbgBtlSysStr[] = "> SYS";
const char dbgBtlSysSpeedFmt[] = "SYS SPEED  x%d";
const char dbgBtlDmgRndStr[] = "DMG_RND  ";
const char dbgBtlNoDeathStr[] = "NO_DEATH ";
const char dbgBtlNoMpSpStr[] = "NO_MPSP  ";
const char dbgBtlDemoModeStr[] = "DEMO MODE";
const char dbgBtlDebugAiStr[] = "DEBUG AI ";
const char dbgBtlMgcGbxStr[] = "MGC GBX  ";
const char dbgBtlNoMotMvStr[] = "NO MOTMV ";
const char dbgBtlNoFogCtrStr[] = "NO FOG_CTR";
const char dbgBtlAiMgcNoFmt[] = "AI_MGC_NO   %d";
const char dbgBtlAiMgcCmbFmt[] = "AI_MGC_CMB  %d";
const char dbgBtlMcAiNoFmt[] = "MC_AI_NO   %d";
const char dbgBtlMcAtkNoFmt[] = "MC_ATK_NO  %d";
const char dbgBtlTraceAttrFmt[] = "trace attr %d";
const char dbgBtlHitAttrFmt[] = "hit attr %d";
const char dbgBtlDarkFmt[] = "dark %d";
const char dbgBtlSePatFmt[] = "se pat %d";
