/* BATTLE .rodata 0x006E3E60-0x006E3E80, the end of the debug character
 * pages' file (after DbgBtlParTestPage's .rodata) and of BATTLE's
 * .rodata: the strings of DbgBtlIpStopPage (6DEA98.c). Data-only C (plan
 * step 6b, docs/shifting.md).
 */
/* rodata: 0x006E3E60-0x006E3E80 */

/* DbgBtlIpStopPage */
const char dbgBtlIpStopTitleFmt[] = "%sIP STOP%s";
const char dbgBtlIpStopLineFmt[] = "%.2d: %d";
