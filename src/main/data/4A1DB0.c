/* main executable .rodata 0x004A1DB0-0x004A1EA0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1DAC.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1DB0-0x004A1EA0 */

/* Generator scale page (DbgPtclCtrlGenScalePage 00405900). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclSx0Fmt[] = "SX 0    %5.1f"; /* 004A1DB0 */
const char dbgPtclRateStFmt[] = "RATE ST %5.1f"; /* 004A1DC0 */
const char dbgPtclC1fFmt[] = "C1 %5.1f"; /* 004A1DD0 */
const char dbgPtclC2fFmt[] = "C2 %5.1f"; /* 004A1DE0 */
const char dbgPtclEdfFmt[] = "ED %5.1f"; /* 004A1DF0 */
const char dbgPtclC1dFmt[] = "C1 %4.1d"; /* 004A1E00 */
const char dbgPtclC2dFmt[] = "C2 %4.1d"; /* 004A1E10 */
const char dbgPtclStC1Fmt[] = "ST-C1 %3.1d"; /* 004A1E20 */
const char dbgPtclC1C2Fmt[] = "C1-C2 %3.1d"; /* 004A1E30 */
const char dbgPtclC2EdFmt[] = "C2-ED %3.1d"; /* 004A1E40 */
const char dbgPtclSinXLFmt[] = "SINX L %4.1d"; /* 004A1E50 */
const char dbgPtclStdFmt[] = "ST %4.1d"; /* 004A1E60 */
const char dbgPtclRateFmt[] = "RATE %4.2f"; /* 004A1E70 */
const char dbgPtclSy0Fmt[] = "SY 0    %5.1f"; /* 004A1E80 */
const char dbgPtclSinYLFmt[] = "SINY L %4.1d"; /* 004A1E90 */
