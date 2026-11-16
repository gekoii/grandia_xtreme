/* main executable .rodata 0x004A1BA8-0x004A1C00: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1BA4.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1BA8-0x004A1C00 */

/* Generator position page (DbgPtclCtrlGenPosPage 00404998). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclEffeAFmt[] = "EFFE A %2.1d"; /* 004A1BA8 */
const char dbgPtclEffeBFmt[] = "B %2.1d"; /* 004A1BB8 */
const char dbgPtclEffeCFmt[] = "C %2.1d"; /* 004A1BC0 */
const char dbgPtclEffeDFmt[] = "D %2.1d"; /* 004A1BC8 */
const char dbgPtclAmbRFmt[] = "AMB R %5.1f"; /* 004A1BD0 */
const char dbgPtclAmbGFmt[] = "G %5.1f"; /* 004A1BE0 */
const char dbgPtclAmbBFmt[] = "B %5.1f"; /* 004A1BE8 */
const char dbgPtclAmbAFmt[] = "A %5.1f"; /* 004A1BF0 */
