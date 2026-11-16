/* FIELD .data: the damage-number font, the camera module's work pointers,
 * the map variant table and the map load timer. Data-only C (plan step 6b,
 * docs/shifting.md); the users are FldDmgNumBuildSprites (5C5B00.c), the
 * camera functions (5CA680.c-5CE5D0.c) and the map loader (5D2848.c).
 * The range ends at 0x00666810 although the last object ends at 0x00666808:
 * the next file (666810.c) is 16-aligned, which recreates the zeros.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* data: 0x00666788-0x00666810 */

typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

/* A glyph of a sprite font: texture rectangle */
typedef struct {
    u16 u;
    u16 v;
    u16 uw;
    u16 vh;
} Glyph;

typedef struct {
    Glyph *tbl;  /* glyph per character */
    s8 adv[12];  /* advance width per character */
} Font;

extern Glyph fldDmgDigitGlyphs[];   /* .rodata: the damage digits' glyphs */
extern u8 camFollowWork[];   /* .bss: follow camera work (0x610) */
extern u8 camDebugTriBuf[];  /* .bss: count + up to 128 triangles */
extern u8 camTrailBuf[];     /* .bss: head, tail, 32 points */
extern u8 camScrEndScript[]; /* one-word script 0x1FFFF (end) */

/* FldDmgNumBuildSprites: digits 0-9 and two more characters */
Font fldDmgDigitFont = {
    fldDmgDigitGlyphs,
    { 15, 13, 16, 15, 15, 15, 15, 14, 15, 17, 17, 15 },
}; /* 00666788 */

void *camFollow = camFollowWork;     /* 00666798 */
void *camDebugTris = camDebugTriBuf; /* 0066679C */
void *camTrail = camTrailBuf;        /* 006667A0 */
void *camFollowCur = 0;              /* 006667A4: follow work cached by CamAiFollowUpdate */
f32 camFollowHeightSpeed = 0.0f;     /* 006667A8: vertical follow speed (CamAiFollowUpdate) */
void *camPresetCur = 0;              /* 006667AC: follow work cached by CamAiPresetUpdate */
void *camScrPc = camScrEndScript;    /* 006667B0: camera script program counter */

/* MapHasVariant: the 0x24 maps that have variants */
u16 mapVariantTable[0x24] = {
    0x4030, 0x4040, 0x4050, 0x40A0, 0x4800, 0x4810, 0x4820, 0x48A0,
    0x48F0, 0x5010, 0x5020, 0x5024, 0x5030, 0x50A0, 0x50F0, 0x5800,
    0x5810, 0x5824, 0x5830, 0x58A0, 0x58F0, 0x7000, 0x7010, 0x7020,
    0x70A0, 0x70F0, 0x8000, 0x8010, 0x8020, 0x80A0, 0x80F0, 0x9000,
    0x9010, 0x9014, 0x9020, 0x9040,
}; /* 006667B8 */

/* the map loader: vblank counts (GsGetVblankCount) */
s32 mapLoadStartTime = 0; /* 00666800: when the MDT read finishes */
s32 mapLoadTime = 0;      /* 00666804: updated in load states 1 and 10 */
