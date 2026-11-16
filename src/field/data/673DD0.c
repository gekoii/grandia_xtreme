/* FIELD .rodata 0x00673DD0-0x00673DD8: data-only C (plan step 6b, docs/shifting.md).
 * The "save" that starts an original file's .rodata. Split from the
 * planned 673DC8.c at the save.
 * Check with tools/check_data.py. Do not include the project headers here.
 */
/* rodata: 0x00673DD0-0x00673DD8 */

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
