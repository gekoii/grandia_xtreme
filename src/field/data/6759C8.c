/* FIELD .rodata: the "save" strings of 31 files with no constants of their
 * own (between 6588F8.c's and 65C690.c's .rodata). Data-only C (plan step
 * 6b, docs/shifting.md). The 4 bytes before 0x006759C8 stay asm (after
 * 6588F8.c's constants).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* rodata: 0x006759C8-0x00675AD0 */

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_006759D0[] = "save"; /* the next file's .rodata starts here */
static const char save_006759D8[] = "save"; /* the next file's .rodata starts here */
static const char save_006759E0[] = "save"; /* the next file's .rodata starts here */
static const char save_006759E8[] = "save"; /* the next file's .rodata starts here */
static const char save_006759F0[] = "save"; /* the next file's .rodata starts here */
static const char save_006759F8[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A00[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A08[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A10[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A18[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A20[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A28[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A30[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A38[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A40[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A48[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A50[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A58[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A60[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A68[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A70[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A78[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A80[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A88[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A90[] = "save"; /* the next file's .rodata starts here */
static const char save_00675A98[] = "save"; /* the next file's .rodata starts here */
static const char save_00675AA0[] = "save"; /* the next file's .rodata starts here */
static const char save_00675AA8[] = "save"; /* the next file's .rodata starts here */
static const char save_00675AB0[0x10] = "save"; /* the next file's .rodata starts here */ /* padded: what follows is 16-aligned */
static const char save_00675AC0[] = "save"; /* the next file's .rodata starts here */
