/* BATTLE .rodata: the start of a file's .rodata (its "save" string), the
 * tactics window's strings and its button prompt sprite. Data-only C (plan
 * step 6b, docs/shifting.md). The text is in the game's font code: each
 * character is a byte (ASCII + 0x1F, 0x9F a space) followed by 0xF9; the
 * comments give it as text. Don't include the project headers here.
 */
/* rodata: 0x006E00A8-0x006E0198 */

typedef signed short s16;
typedef unsigned short u16;

/* Texture rectangle of a sprite (SprDrawRect): size, then the UV corners */
typedef struct {
    s16 w, h;
    u16 u0, v0;
    u16 u1, v1;
} SprRect;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */

/* Tactic names */
const char menuTacticsNameStrs[8][24] = {
    "l\371\200\371\215\371\224\371\200\371\213\371",
    "o\371\213\371\200\371\230\371\237\371e\371\200\371\210\371\221\371",
    "e\371\216\371\202\371\224\371\222\371",
    "f\371\216\371\237\371v\371\210\371\213\371\203\371",
    "l\371\204\371\200\371\215\371\210\371\204\371",
    "r\371\200\371\205\371\204\371\223\371\230\371\237\371P\371\222\371\223\371",
    "v\371\210\371\223\371\207\371\203\371\221\371\200\371\226\371",
    "f\371\216\371\237\371r\371\214\371\200\371\221\371\223\371",
}; /* "Manual" "Play Fair" "Focus" "Go Wild" "Meanie" "Safety 1st" "Withdraw" "Go Smart" */
const char menuTacticsOkStr[] = "n\371j\371"; /* "OK" */
const char menuTacticsAiStr[] = "`\371h\371\237\371"; /* "AI " */
/* Tactic set letters */
const char menuTacticsSetLetterStrs[2][3] = { "`\371", "a\371" }; /* "A" "B" */

/* Drawn next to the button prompt (texture 0xEC). btlTargetAreaNames
 * (0x006E0193, +0xB) is not a field: it is the target area name table of
 * 6E0198.c (btlTargetAreaNameText) biased by -1 element (13 bytes). */
const SprRect menuTacticsEditSprite = { 78, 20, 0x4F, 0xA1, 0x9E, 0xB6 };
