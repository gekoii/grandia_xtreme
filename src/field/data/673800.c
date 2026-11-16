/* FIELD .rodata 0x00673800-0x00673850: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C3800.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00673800-0x00673850 */

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_00673808[] = "save"; /* the next file's .rodata starts here */
static const char save_00673810[] = "save"; /* the next file's .rodata starts here */
static const char save_00673818[] = "save"; /* the next file's .rodata starts here */
static const char save_00673820[] = "save"; /* the next file's .rodata starts here */
static const char save_00673828[] = "save"; /* the next file's .rodata starts here */
static const char save_00673830[0x10] = "save"; /* the next file's .rodata starts here */ /* padded: what follows is 16-aligned */
static const char save_00673840[] = "save"; /* the next file's .rodata starts here */
