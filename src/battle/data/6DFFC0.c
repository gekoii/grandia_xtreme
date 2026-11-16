/* BATTLE .rodata: a file's .rodata (its "save" string and the dust
 * particle's sprite rect). Data-only C (plan step 6b, docs/shifting.md).
 * The next original file's .rodata (6DFFE0.c) is 16-aligned, which leaves
 * 12 zero bytes after the rect. Don't include the project headers here.
 */
/* rodata: 0x006DFFC0-0x006DFFE0 */

typedef signed short s16;
typedef unsigned short u16;

/* Texture rectangle of a sprite (SprDrawRect): size, then the UV corners */
typedef struct {
    s16 w, h;
    u16 u0, v0;
    u16 u1, v1;
} SprRect;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* Dust particle (texture 0x33) */
const SprRect ptclBtlDustSprite = { 2, 2, 0, 0, 0x21, 0x21 };
