/* BATTLE: the start of the .rodata of the file with the effect sprites
 * FxCastSparkDraw (6A0D58.c) and FxFailLabelDraw (6A1B40.c). Data-only C
 * (plan step 6b, docs/shifting.md). Don't include the project headers here:
 * declare what the data refers to locally.
 */
/* rodata: 0x006E0DD8-0x006E0E20 */

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

/* 8 zero bytes before the file's "save" (16-aligned); nothing refers to
 * them */
const u32 D_006E0DD8[2] = { 0, 0 };

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* The fail label's 3 animation frames */
const SprRect fxFailLabelRects[3] = {
    { 16, 32, 191, 136, 208, 169 },
    { 24, 32, 209, 136, 234, 169 },
    { 20, 32, 235, 136, 256, 169 },
};

/* The cast spark, drawn as a 3D billboard */
const SprRect fxCastSparkRect = { 10, 10, 0, 0, 33, 33 };
