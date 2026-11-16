/* main executable .rodata 0x004A2398-0x004A2430: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A2394.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A2398-0x004A2430 */

/* Effector page (DbgPtclEffectorPage 0040B018). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclEffectorTitleFmt[] = "Effector [%d]"; /* 004A2398 */
const char dbgPtclEffIdxFmt[] = "IDX %6.1d"; /* 004A23A8 */
const char dbgPtclEffScaleFmt[] = "SCL %7.2f"; /* 004A23B8 */
const char dbgPtclEffModeFmt[] = "MODE (%s)"; /* 004A23C8 */
const char dbgPtclEffVectorStr[] = "VCT"; /* 004A23D8 */
const char dbgPtclEffPosStr[] = "POS"; /* 004A23E0 */
const char dbgPtclEffOfsFmt[] = "OFS (%s)"; /* 004A23E8 */
const char dbgPtclVal72fFmt[] = " %7.2f"; /* 004A23F8 */
const char dbgPtclEffCycleFmt[] = "CYCLE %4.1d"; /* 004A2400 */
const char dbgPtclEffSysFmt[] = "SYS (%s)"; /* 004A2410 */
const char dbgPtclLocalStr[] = "LOCAL"; /* 004A2420 */
const char dbgPtclWorldStr[] = "WORLD"; /* 004A2428 */
