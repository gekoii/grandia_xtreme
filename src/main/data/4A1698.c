/* main executable .rodata 0x004A1698-0x004A17D0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1694.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1698-0x004A17D0 */

/* Particle starter page (DbgPtclStarterPage 004028D8). */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclStarterPageTitle[] = "Starter %s page %d/%d%s"; /* 004A1698 */
const char dbgPtclTitleColorStr[] = "\\c5 "; /* 004A16B0 */
const char dbgPtclColor0Str[] = "\\c0 "; /* 004A16B8 */
const char dbgPtclStartMaskFmt[] = "START MASK %d"; /* 004A16C0 */
const char dbgOfsPosYFmt[] = "OFS PY %7.1f"; /* 004A16D0 */
const char dbgPtclColor4Str[] = "\\c4 "; /* 004A16E0 */
const char dbgPtclCountFmt[] = "COUNT %d"; /* 004A16E8 */
const char dbgPtclCtrlFmt[] = "CTRL  %d"; /* 004A16F8 */
const char dbgPtclIdFmt[] = "ID    %d"; /* 004A1708 */
const char dbgPosXFmt[] = "POS X %7.1f"; /* 004A1718 */
const char dbgPosYFmt[] = "Y %7.1f"; /* 004A1728 */
const char dbgPosZFmt[] = "Z %7.1f"; /* 004A1730 */
const char dbgRotXFmt[] = "ROT X 0x%.5X(%5.1f)"; /* 004A1738 */
const char dbgRotYFmt[] = "Y 0x%.5X(%5.1f)"; /* 004A1750 */
const char dbgRotZFmt[] = "Z 0x%.5X(%5.1f)"; /* 004A1760 */
const char dbgPtclWorkFmt[] = "WORK %6d(%6d)"; /* 004A1770 */
const char dbgPtclFileFmt[] = "FILE %6d"; /* 004A1780 */
const char dbgPtclSortFmt[] = "SORT %6d"; /* 004A1790 */
const char dbgPtclMgrFmt[] = "MGR  %6d"; /* 004A17A0 */
const char dbgPtclInstFmt[] = "INST %6d"; /* 004A17B0 */
const char dbgPtclAllFmt[] = "ALL  %6d/524288"; /* 004A17C0 */
