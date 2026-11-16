/* FIELD .rodata: the "save" strings that start the .rodata of two files
 * with no constants of their own (before 656C08.c's). Data-only C (plan
 * step 6b, docs/shifting.md).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* rodata: 0x00675940-0x00675950 */

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_00675948[] = "save"; /* the next file's .rodata starts here */
