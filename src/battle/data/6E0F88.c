/* BATTLE: the start of the .rodata of the file with IpGaugeDraw
 * (6A7CF8.c): the combo mark. Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here: declare what
 * the data refers to locally.
 */
/* rodata: 0x006E0F88-0x006E0FB0 */

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
const u32 D_006E0F88[2] = { 0, 0 };

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* The combo mark on the IP gauge */
const SprRect ipComboMarkRect = { 128, 128, 0, 0, 33, 33 };
