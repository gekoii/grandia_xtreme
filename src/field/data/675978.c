/* FIELD .rodata: three files' "save" strings and the flag list's text
 * shadow offsets. Data-only C (plan step 6b, docs/shifting.md); the user is
 * FldFlagListUpdate (asm/field/field/0A7F20.s), which copies the table to
 * the stack (probably a local array's initializer in the original).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* rodata: 0x00675978-0x006759A0 */

typedef signed char s8;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_00675980[] = "save"; /* the next file's .rodata starts here */
/* {dx, dy} of the four copies drawn behind the list text */
const s8 fldFlagListShadowOfs[4][2] = {
    { 2, 2 }, { -2, -2 }, { 2, -2 }, { -2, 2 },
}; /* 00675988 */
static const char save_00675990[] = "save"; /* the next file's .rodata starts here */
