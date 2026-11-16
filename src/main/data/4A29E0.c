/* main executable: the start of a file's .rodata - the sprite rectangles of
 * the menu's face-button pad (MenuButtonPadDraw 418610) and sparkle
 * (MenuSparkleDraw 418C98). Data-only C (plan step 6b, docs/shifting.md).
 * The next file's .rodata (4A2A90.c) starts 16-aligned, which recreates the
 * zeros after menuSparkleSprite. Don't include the project headers here:
 * declare what the data refers to locally.
 */
/* rodata: 0x004A29E0-0x004A2A90 */

typedef unsigned short u16;

/* Texture rectangle of a sprite (SprDrawRect): size, then the UV corners
 * (SprRect in include/sys.h) */
typedef struct {
    short w;
    short h;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
} SprRect;

/* A header's unused static string: the first thing in each file's .rodata */
static const char save[] = "save";

/* MenuButtonPadDraw: the four face buttons, laid out as a diamond (top,
 * bottom, left, right = flag bits 0x1000, 0x4000, 0x8000, 0x2000). A button
 * whose bit is set uses row 1 or row 2 instead of row 0 (k = 4 when
 * menuWidgetFrame & 0x10, else 8: it blinks). Labels inside:
 * D_004A2A00 = [2], D_004A2A0C = [3]. */
const SprRect menuButtonPadSprites[3 * 4] = {
    /* normal */
    { 0xE, 0x11, 0xD9, 0x01, 0xE8, 0x13 },  /* top */
    { 0xE, 0x11, 0xE9, 0x01, 0xF8, 0x13 },  /* bottom */
    { 0x11, 0xE, 0xD9, 0x14, 0xEB, 0x23 },  /* left */
    { 0x11, 0xE, 0xEC, 0x14, 0xFE, 0x23 },  /* right */
    /* lit, frame 1 */
    { 0xE, 0x11, 0xD9, 0x24, 0xE8, 0x36 },
    { 0xE, 0x11, 0xE9, 0x24, 0xF8, 0x36 },
    { 0x11, 0xE, 0xD9, 0x37, 0xEB, 0x46 },
    { 0x11, 0xE, 0xEC, 0x37, 0xFE, 0x46 },
    /* lit, frame 2 */
    { 0xE, 0x11, 0xD9, 0x47, 0xE8, 0x59 },
    { 0xE, 0x11, 0xE9, 0x47, 0xF8, 0x59 },
    { 0x11, 0xE, 0xD9, 0x5A, 0xEB, 0x69 },
    { 0x11, 0xE, 0xEC, 0x5A, 0xFE, 0x69 },
};

/* MenuSparkleDraw: drawn centred (-12, -12) */
const SprRect menuSparkleSprite[1] = {
    { 0x18, 0x18, 0x99, 0xBD, 0xB2, 0xD6 },
};
