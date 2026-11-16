/* BATTLE .rodata 0x006E2818-0x006E2830: after 6D3F60.c's jump tables, the
 * start of the original file of BtlHitResolve (6D5398.c; its "save").
 * Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x006E2818-0x006E2830 */

typedef unsigned int u32;

/* 8 zero bytes before the file's "save" (16-aligned); nothing refers to
 * them */
const u32 D_006E2818[2] = { 0, 0 };

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
