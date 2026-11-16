/* FIELD .rodata 0x00674D08-0x00674E10: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C4D04.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00674D08-0x00674E10 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char fldSndDbgTimeFmt[] = "%d(%d) "; /* 00674D08 */
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
const char fldPointLightDbgNoFmt[] = "NO        %d / %d"; /* 00674D18 */
const char fldPointLightDbgDispFmt[] = "Disp      %d"; /* 00674D30 */
const char fldPointLightDbgIdFmt[] = "ID        %d"; /* 00674D40 */
const char fldPointLightDbgRangeFmt[] = "Range     %.3f"; /* 00674D50 */
const char fldPointLightDbgColorRFmt[] = "Color R   %.3f"; /* 00674D60 */
const char fldPointLightDbgColorGFmt[] = "      G   %.3f"; /* 00674D70 */
const char fldPointLightDbgColorBFmt[] = "      B   %.3f"; /* 00674D80 */
const char fldPointLightDbgPosXFmt[] = "Pos   X   %.3f"; /* 00674D90 */
const char fldPointLightDbgPosYFmt[] = "      Y   %.3f"; /* 00674DA0 */
const char fldPointLightDbgPosZFmt[] = "      Z   %.3f"; /* 00674DB0 */
static const char save_00674DC0[] = "save"; /* the next file's .rodata starts here */
const char fldMemErrAllocFmt[] = "FIELD ERR MEMORY ALLOC ERR size %x\n"; /* 00674DC8 */
const char fldMemErrTopFmt[] = "now top %x end addres %x\n"; /* 00674DF0 */
