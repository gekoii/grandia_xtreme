/* BATTLE .rodata 0x006E10F8-0x006E1110: after 6AA078.c's jump tables, the
 * start of the original file of 6AA9D0.c (its "save"). Data-only C (plan
 * step 6b, docs/shifting.md).
 */
/* rodata: 0x006E10F8-0x006E1110 */

typedef unsigned int u32;

/* 8 zero bytes before the file's "save" (16-aligned); nothing refers to
 * them */
const u32 D_006E10F8[2] = { 0, 0 };

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
