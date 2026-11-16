/* BATTLE .rodata: the start of a file's .rodata (its "save" string); its
 * function's jump table follows (asm, jtbl_006DFFF0), then its strings
 * (6E0018.c). Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x006DFFE0-0x006DFFF0 */

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
