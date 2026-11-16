/* FIELD .rodata 0x00672328-0x00672560: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C2324.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00672328-0x00672560 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Format strings of the flag and counter editors (DbgFldBitCursorDraw
 * src/field/5D9668.c, DbgFldCounterEditPage 5D9D18; "\c<n>" switches the debug
 * font's colour) and the first XVS information page (DbgFldXvsInfoPage,
 * 5D9EE0). */

const char dbgFlagEditStrMarkV[] = "\\c8 V\\c7 "; /* 00672328 */
const char dbgFlagEditStrMarkBar[] = "\\c8 |\\c7 "; /* 00672338 */
const char dbgFlagEditStrStxAdr[] = "STX ADR \\c6 $%3X + %d \\c7 [%d]"; /* 00672348 */
const char dbgFlagEditStrRule[] = "-----+------------------------------------------"; /* 00672368 */
const char dbgFlagEditStrRowNo[] = "\\c5 %4d\\c7 :"; /* 006723A0 */
const char dbgFlagEditStrBit[] = "%d "; /* 006723B0 */
const char dbgFlagEditStrBitSel[] = "\\c6 %d\\c7 "; /* 006723B8 */
const char dbgFlagEditStrRowAdr[] = "\\c5 ( $%04X ) \\c7 "; /* 006723C8 */
const char dbgFlagEditStrRow[] = "%4d : %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ( $%04X )"; /* 006723E0 */
const char dbgCounterEditStrRow[] = "%3x: [%3d] %3x: [%3d] %3x: [%3d] %3x: [%3d]"; /* 00672420 */
const char dbgXvsStrInfo1[] = "XVS INFORMATION 1"; /* 00672450 */
const char dbgXvsStrXvs[] = "XVS              : %s"; /* 00672468 */
const char dbgXvsStrBaffur[] = "XVS BAFFUR       : %s"; /* 00672480 */
const char dbgXvsStrMainCounter[] = "XVS MAIN COUNTER : %d"; /* 00672498 */
const char dbgXvsStrDebug1[] = "XVS DEBUG 1      : %s"; /* 006724B0 */
const char dbgXvsStrDebug2[] = "XVS DEBUG 2      : %s"; /* 006724C8 */
const char dbgXvsStrDebug3[] = "XVS DEBUG 3      : %s"; /* 006724E0 */
const char dbgXvsStrDebug4[] = "XVS DEBUG 4      : %s"; /* 006724F8 */
const char dbgXvsStrErrMsg[] = "XVS ERR MSG      : %s"; /* 00672510 */
const char dbgXvsStrEffect[] = "XVS EFFECT       : %s"; /* 00672528 */
const char dbgXvsStrNonDemo[] = "XVS non DEMO     : %s"; /* 00672540 */
