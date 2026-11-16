/* BATTLE .rodata: the strings of the debug menu's TRACE page, after the
 * jump table at 0x006DFFF0 (asm). Data-only C (plan step 6b,
 * docs/shifting.md).
 */
/* rodata: 0x006E0018-0x006E0080 */

const char dbgBtlTraceTitleFmt[] = "%sTRACE%s";
const char dbgBtlTraceTitleColorStr[] = "\\c1 ";
const char dbgBtlTraceColor0Str[] = "\\c0 ";
const char dbgBtlTraceNoFmt[] = "No. %d";
const char dbgBtlTraceStartRFmt[] = "ST R %.3d";
const char dbgBtlTraceGFmt[] = "G %.3d";
const char dbgBtlTraceBFmt[] = "B %.3d";
const char dbgBtlTraceAFmt[] = "A %.3d";
const char dbgBtlTraceEndRFmt[] = "ED R %.3d";
