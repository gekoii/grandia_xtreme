/* main executable .rodata 0x004A1FE8-0x004A2100: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1FE4.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1FE8-0x004A2100 */

/* Generator rotation page (DbgPtclCtrlGenRotPage 00406E00). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclRx0Fmt[] = "RX 0 0x%.5x(%5.1f)"; /* 004A1FE8 */
const char dbgPtclAngleValFmt[] = "0x%.5x(%5.1f)"; /* 004A2000 */
const char dbgPtclRy0Fmt[] = "RY 0 0x%.5x(%5.1f)"; /* 004A2010 */
const char dbgPtclRz0Fmt[] = "RZ 0 0x%.5x(%5.1f)"; /* 004A2028 */
const char dbgPtclRdxMdFmt[] = "RDX_MD %3.1d"; /* 004A2040 */
const char dbgPtclRdxStartFmt[] = "RDX S 0x%.5x(%5.1f)"; /* 004A2050 */
const char dbgPtclRdxEndFmt[] = "RDX E 0x%.5x(%5.1f)"; /* 004A2068 */
const char dbgPtclRdyMdFmt[] = "RDY_MD %3.1d"; /* 004A2080 */
const char dbgPtclRdyStartFmt[] = "RDY S 0x%.5x(%5.1f)"; /* 004A2090 */
const char dbgPtclRdyEndFmt[] = "RDY E 0x%.5x(%5.1f)"; /* 004A20A8 */
const char dbgPtclRdzMdFmt[] = "RDZ_MD %3.1d"; /* 004A20C0 */
const char dbgPtclRdzStartFmt[] = "RDZ S 0x%.5x(%5.1f)"; /* 004A20D0 */
const char dbgPtclRdzEndFmt[] = "RDZ E 0x%.5x(%5.1f)"; /* 004A20E8 */
