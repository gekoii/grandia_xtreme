/* main executable .rodata 0x004A2498-0x004A24B0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A2498.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A2498-0x004A24B0 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Unreferenced zeros after 40C410's jump table; most likely padding before
 * the next file's 16-aligned .rodata (its "save", then 40C870's table). */
const u32 D_004A2498[] = {
    0x0, 0x0,
}; /* 004A2498 */
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
