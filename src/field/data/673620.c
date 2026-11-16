/* FIELD .rodata 0x00673620-0x00673630: data-only C (plan step 6b, docs/shifting.md).
 * The "save" that starts an original file's .rodata (the jump table at
 * 0x00673630 that follows is still asm, with its function). Split from the
 * planned 673618.c at the save.
 * Check with tools/check_data.py. Do not include the project headers here.
 */
/* rodata: 0x00673620-0x00673630 */

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
