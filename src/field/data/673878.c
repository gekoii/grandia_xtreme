/* FIELD .rodata 0x00673878-0x00673880: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C3874.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00673878-0x00673880 */

typedef unsigned char u8;

/* Entry counts of the 5 flag groups of ItemFlagGroupListBuild (5EB040),
 * which copies the whole struct (src/field/5EB040.c: Counts). */
typedef struct {
    u8 n[5];
} Counts;

const Counts itemFlagGroupSizes = {{3, 4, 5, 6, 8}}; /* 00673878 */
