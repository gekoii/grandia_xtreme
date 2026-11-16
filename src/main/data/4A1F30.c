/* main executable .rodata 0x004A1F30-0x004A1F90: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1F30.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1F30-0x004A1F90 */

/* Generator colour page (DbgPtclCtrlGenColorPage 004065E0). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclStRFmt[] = "ST R %4.1d"; /* 004A1F30 */
const char dbgPtclColGFmt[] = "G %4.1d"; /* 004A1F40 */
const char dbgPtclColBFmt[] = "B %4.1d"; /* 004A1F48 */
const char dbgPtclColAFmt[] = "A %4.1d"; /* 004A1F50 */
const char dbgPtclC1RFmt[] = "C1 R %4.1d"; /* 004A1F58 */
const char dbgPtclC2RFmt[] = "C2 R %4.1d"; /* 004A1F68 */
const char dbgPtclEdRFmt[] = "ED R %4.1d"; /* 004A1F78 */
