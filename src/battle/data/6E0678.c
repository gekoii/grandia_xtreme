/* BATTLE: the start of the .rodata of the file with the level-up caption
 * code (BtlCaptionLevelUpStamp 697060.c, the notice suffixes in 6975D0.c):
 * font-coded caption suffixes and the stamp sprite. Data-only C (plan step
 * 6b, docs/shifting.md). Don't include the project headers here: declare
 * what the data refers to locally.
 */
/* rodata: 0x006E0678-0x006E06E0 */

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
const u32 D_006E0678[2] = { 0, 0 };

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* Font-coded captions (each character a byte, then 0xF9) */
const char btlStrWasLearned[] = "\237\371v\371\200\371\222\371\237\371k\371\204\371\200\371\221\371\215\371\204\371\203\371@\371"; /* " Was Learned!" */
const char btlStrHasBroken[] = "\237\371g\371\200\371\222\371\237\371a\371\221\371\216\371\212\371\204\371\215\371@\371"; /* " Has Broken!" */

/* The stamp dropped on the level-up caption (BtlCaptionLevelUpStamp) */
const SprRect btlCaptionStampSprite = { 84, 27, 100, 83, 185, 111 };
