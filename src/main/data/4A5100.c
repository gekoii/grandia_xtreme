/* main executable .rodata 0x004A5100-0x004A5420: strings of
 * the debug menu's program call and object attribute pages.
 * Data-only C (plan step 6b, docs/shifting.md); the pages that print them
 * are still separate files (named below). The strings are named globals
 * because the code files refer to them by name. The gaps around this range
 * are those files' own .rodata (jump tables, constants).
 */
/* rodata: 0x004A5100-0x004A5420 */

/* DbgProgramCallPage (4291D8.c) */
const char dbgProgStrProgramCallMode[] = "PROGRAM CALL MODE";
const char dbgProgStrProgram[] = "PROGRAM %d %s";
const char dbgProgStrProgramNotEntry[] = "PROGRAM %d NOT ENTRY";

/* DbgObjAtrMenu (4293A0.c) */
const char dbgAtrMenuStrObjectAtrMenuPage3[] = "OBJECT ATR MENU page %s %d/3%s ";

/* DbgObjAtrPage (429490.c) */
const char dbgObjAtrStrObjectNo[] = "OBJECT NO    %d";
const char dbgObjAtrStrAttribCtrlMode[] = "ATTRIB %x CTRL MODE %x";

/* DbgObjAtrPage (429490.c), DbgPartsAtrPage (42A320.c) */
const char dbgAtrStrBillboard[] = "BILLBOARD    %d";
const char dbgAtrStrAmbientOnlyOn[] = "AMBIENT ONLY ON";
const char dbgAtrStrAmbientOnlyOff[] = "AMBIENT ONLY OFF";
const char dbgAtrStrIgnoreLightOn[] = "IGNORE LIGHT ON";
const char dbgAtrStrIgnoreLightOff[] = "IGNORE LIGHT OFF";
const char dbgAtrStrUvScroolOn[] = "UV SCROOL    ON";
const char dbgAtrStrUvScroolOff[] = "UV SCROOL    OFF";
const char dbgAtrStrTextureOn[] = "TEXTURE      ON";
const char dbgAtrStrTextureOff[] = "TEXTURE      OFF";
const char dbgAtrStrUClampOn[] = "U CLAMP      ON";
const char dbgAtrStrUClampOff[] = "U CLAMP      OFF";
const char dbgAtrStrVClampOn[] = "V CLAMP      ON";
const char dbgAtrStrVClampOff[] = "V CLAMP      OFF";
const char dbgAtrStrAlphaTestOn[] = "ALPHA TEST   ON";
const char dbgAtrStrAlphaTestOff[] = "ALPHA TEST   OFF";
const char dbgAtrStrAlphaMode[] = "ALPHA MODE   %d";
const char dbgAtrStrZTestOn[] = "Z TEST       ON";
const char dbgAtrStrZTestOff[] = "Z TEST       OFF";
const char dbgAtrStrZTestMode[] = "Z TEST MODE %d";
const char dbgAtrStrAlphaBlend[] = "ALPHA BLEND %d";

/* DbgObjAtrPage (429490.c) */
const char dbgObjAtrStrFogOn[] = "FOG          ON ";

/* DbgObjAtrPage (429490.c), DbgPartsAtrPage (42A320.c) */
const char dbgAtrStrFogOff[] = "FOG          OFF";
const char dbgAtrStrAlphaFix[] = "ALPHA FIX   %d";
const char dbgAtrStrUScrStep[] = "U SCR STEP  %d";
const char dbgAtrStrVScrStep[] = "V SCR STEP  %d";
const char dbgAtrStrSurfaceR[] = "SURFACE   R %.3f";
const char dbgAtrStrSurfaceG[] = "SURFACE   G %.3f";
const char dbgAtrStrSurfaceB[] = "SURFACE   B %.3f";
const char dbgAtrStrSurfaceA[] = "SURFACE   A %.3f";
const char dbgAtrStrPartsPrio[] = "PARTS PRIO  %d";
const char dbgAtrStrEnvmapOn[] = "ENVMAP      ON";
const char dbgAtrStrEnvmapOff[] = "ENVMAP      OFF";
const char dbgAtrStrEnvmapA[] = "ENVMAP A    %d";
const char dbgAtrStrDispOff[] = "DISP OFF";
const char dbgAtrStrDispOn[] = "DISP ON";
