/* main executable .rodata 0x004A5528-0x004A55D0: strings of
 * the debug menu's object and info menus.
 * Data-only C (plan step 6b, docs/shifting.md); the pages that print them
 * are still separate files (named below). The strings are named globals
 * because the code files refer to them by name. The gaps around this range
 * are those files' own .rodata (jump tables, constants).
 */
/* rodata: 0x004A5528-0x004A55D0 */

/* DbgObjectMenu (42AFB8.c) */
const char dbgObjMenuStrPartsInfo[] = ">PARTS INFO";
const char dbgObjMenuStrAttrib[] = ">ATTRIB";

/* DbgInfoMenu (42B100.c) */
const char dbgInfoMenuStrInfo[] = ">INFO";
const char dbgInfoMenuStrPad[] = ">PAD";
const char dbgInfoMenuStrFile[] = ">FILE";
const char dbgInfoMenuStrSound[] = ">SOUND";
const char dbgInfoMenuStrPerfermance[] = ">PERFERMANCE";
const char dbgInfoMenuStrProgramCall[] = ">PROGRAM CALL";
const char dbgInfoMenuStrMemoryBlock[] = ">MEMORY BLOCK";
const char dbgInfoMenuStrMemoryDump[] = ">MEMORY DUMP";
const char dbgInfoMenuStrSaveMemoryCard[] = ">SAVE MEMORY CARD";
const char dbgInfoMenuStrLoadMemoryCard[] = ">LOAD MEMORY CARD";
