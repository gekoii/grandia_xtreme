/* main executable .rodata 0x004A27A8-0x004A27D0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A27A8.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A27A8-0x004A27D0 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_004A27B0[] = "save"; /* the next file's .rodata starts here (four files with no other .rodata) */
static const char save_004A27B8[] = "save"; /* the next file's .rodata starts here */
static const char save_004A27C0[] = "save"; /* the next file's .rodata starts here */
const f32 ptyNegZeroF = -0.0f; /* 004A27C8; PtyUnitCalcParams (00412910), flag 0x4000 units */
