/* main executable .rodata 0x004A1C38-0x004A1D60: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1C38.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1C38-0x004A1D60 */

/* Generator emit page (DbgPtclCtrlGenEmitPage 00405008). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclAxFmt[] = "AX 0x%.5x(%5.1f)"; /* 004A1C38 */
const char dbgPtclAxWidthFmt[] = "W 0x%.5x(%5.1f)"; /* 004A1C50 */
const char dbgPtclAyFmt[] = "AY 0x%.5x(%5.1f)"; /* 004A1C60 */
const char dbgPtclSpdFmt[] = "SPD  %6.2f"; /* 004A1C78 */
const char dbgPtclVal62fFmt[] = "%6.2f"; /* 004A1C88 */
const char dbgPtclAccFmt[] = "ACC  %6.2f"; /* 004A1C90 */
const char dbgPtclLifeFmt[] = "LIFE %4.1d"; /* 004A1CA0 */
const char dbgPtclVal4dFmt[] = "%4.1d"; /* 004A1CB0 */
const char dbgPtclProbFmt[] = "PROB %4.3f"; /* 004A1CB8 */
const char dbgPtclVal43fFmt[] = "%4.3f"; /* 004A1CC8 */
const char dbgPtclRadiusFmt[] = "R  %5.1f"; /* 004A1CD0 */
const char dbgPtclVal51fFmt[] = "%5.1f"; /* 004A1CE0 */
const char dbgPtclAngXFmt[] = "ANG X  0x%.5x(%5.1f)"; /* 004A1CE8 */
const char dbgPtclAngXWidthFmt[] = "XW 0x%.5x(%5.1f)"; /* 004A1D00 */
const char dbgPtclAngYFmt[] = "Y  0x%.5x(%5.1f)"; /* 004A1D18 */
const char dbgPtclAngYWidthFmt[] = "YW 0x%.5x(%5.1f)"; /* 004A1D30 */
const char dbgPtclOnceNumFmt[] = "ONCE NUM %4.1d"; /* 004A1D48 */
