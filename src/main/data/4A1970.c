/* main executable .rodata 0x004A1970-0x004A1A20: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/main/data/main/0A1970.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x004A1970-0x004A1A20 */

/* Controller emit track page (DbgPtclCtrlEmitTrackPage 00403CC8): 6-character track labels. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

const char dbgPtclEditBoxFmt[] = "%s%s%s"; /* 004A1970 */
const char dbgPtclColor2Str[] = "\\c2 "; /* 004A1978 */
const char dbgPtclEditBoxStr[] = "[   ]"; /* 004A1980 */
const char dbgPtclEditBoxBlinkStr[] = "<   >"; /* 004A1988 */
const char dbgPtclTrackEffeStr[] = "EFFE  "; /* 004A1990 */
const char dbgPtclTrackPxStr[] = "PX    "; /* 004A1998 */
const char dbgPtclTrackPyStr[] = "PY    "; /* 004A19A0 */
const char dbgPtclTrackPzStr[] = "PZ    "; /* 004A19A8 */
const char dbgPtclTrackRxStr[] = "RX    "; /* 004A19B0 */
const char dbgPtclTrackRyStr[] = "RY    "; /* 004A19B8 */
const char dbgPtclTrackRzStr[] = "RZ    "; /* 004A19C0 */
const char dbgPtclTrackAmbStr[] = "AMB   "; /* 004A19C8 */
const char dbgPtclTrackAxStr[] = "AX    "; /* 004A19D0 */
const char dbgPtclTrackAyStr[] = "AY    "; /* 004A19D8 */
const char dbgPtclTrackSpdStr[] = "SPD   "; /* 004A19E0 */
const char dbgPtclTrackAccStr[] = "ACC   "; /* 004A19E8 */
const char dbgPtclTrackLifeStr[] = "LIFE  "; /* 004A19F0 */
const char dbgPtclTrackProbStr[] = "PROB  "; /* 004A19F8 */
const char dbgPtclTrackBAxStr[] = "B_AX  "; /* 004A1A00 */
const char dbgPtclTrackBAyStr[] = "B_AY  "; /* 004A1A08 */
const char dbgPtclTrackBRStr[] = "B_R   "; /* 004A1A10 */
