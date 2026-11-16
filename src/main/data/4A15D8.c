/* main executable .rodata 0x004A15D8-0x004A1680: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A15D8.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A15D8-0x004A1680 */

/* Particle debug menu (DbgPtclPrintTrack4/3 00401F90/00402010, DbgPtclMenu 004026C8). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclLabelFmt[] = "%s\n"; /* 004A15D8 */
const char dbgPtclTrack4Fmt[] = "%s %3.1d (%d) %3.1d (%d) %3.1d (%d) %3.1d\n"; /* 004A15E0 */
const char dbgPtclTrack3Fmt[] = "%s %3.1d %3.1d %3.1d\n"; /* 004A1610 */
const char dbgPtclStarterStr[] = ">STARTER"; /* 004A1628 */
const char dbgPtclControlerStr[] = ">CONTROLER"; /* 004A1638 */
const char dbgPtclEffectorStr[] = ">EFFECTOR"; /* 004A1648 */
const char dbgPtclGeneratorStr[] = ">GENERATOR"; /* 004A1658 */
const char dbgPtclSaveFileStr[] = "SAVE FILE"; /* 004A1668 */
