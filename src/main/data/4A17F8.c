/* main executable .rodata 0x004A17F8-0x004A1930: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A17F4.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A17F8-0x004A1930 */

/* Particle controller pages (DbgPtclCtrlPage 004032B8, DbgPtclCtrlFlagsPage 00403568). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclCtrlTitleFmt[] = "Controler [%d] %s page %d/4%s"; /* 004A17F8 */
const char dbgPtclBlockFmt[] = "BLOCK %d"; /* 004A1818 */
const char dbgPtclUseVcFmt[] = "USE_VC (%s)"; /* 004A1828 */
const char dbgPtclOnStr[] = "ON"; /* 004A1838 */
const char dbgPtclOffStr[] = "OFF"; /* 004A1840 */
const char dbgPtclAlphaFmt[] = "ALPHA %2.1d"; /* 004A1848 */
const char dbgPtclDirGeneFmt[] = "DIR GENE (%s)"; /* 004A1858 */
const char dbgPtclFollowFmt[] = "FOLLOW (%s)"; /* 004A1868 */
const char dbgPtclUseAxFmt[] = "USE AX (%s)"; /* 004A1878 */
const char dbgPtclUseAyFmt[] = "USE AY (%s)"; /* 004A1888 */
const char dbgPtclUseAccFmt[] = "USE ACC (%s)"; /* 004A1898 */
const char dbgPtclAccStopFmt[] = "ACC STOP (%s)"; /* 004A18A8 */
const char dbgPtclAddModeFmt[] = "ADD MODE (%s)"; /* 004A18B8 */
const char dbgPtclSizeLinkFmt[] = "SIZE (X %s Y)"; /* 004A18C8 */
const char dbgPtclEqualStr[] = "="; /* 004A18D8 */
const char dbgPtclNotEqualStr[] = "!="; /* 004A18E0 */
const char dbgPtclRgbChangeFmt[] = "RGB CHANGE (%s)"; /* 004A18E8 */
const char dbgPtclRhFmt[] = "RH (%s)"; /* 004A18F8 */
const char dbgPtclRndStr[] = "RND"; /* 004A1900 */
const char dbgPtclRvFmt[] = "RV (%s)"; /* 004A1908 */
const char dbgPtclRXyzFmt[] = "R XYZ (%s)"; /* 004A1910 */
const char dbgPtclGeneParStr[] = "> GENE PAR"; /* 004A1920 */
