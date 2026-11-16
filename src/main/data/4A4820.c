/* main executable .rodata 0x004A4820-0x004A4AB0: strings of
 * the debug menu's sound, performance and motion object pages.
 * Data-only C (plan step 6b, docs/shifting.md); the pages that print them
 * are still separate files (named below). The strings are named globals
 * because the code files refer to them by name. The gaps around this range
 * are those files' own .rodata (jump tables, constants).
 */
/* rodata: 0x004A4820-0x004A4AB0 */

/* DbgSoundEqPage (4260C8.c) */
const char dbgSndEqStrEqualizer[] = "EQUALIZER %d";
const char dbgSndEqStrLevel[] = "LEVEL %d";
const char dbgSndEqStrErr[] = "ERR %d";

/* DbgSoundMenu (426620.c) */
const char dbgSndMenuStrSoundMenuPage4[] = "SOUND MENU page %s %d/4%s ";

/* DbgPerfPage (426748.c) */
const char dbgPerfStrPerformanceMenu[] = "PERFORMANCE MENU ";
const char dbgPerfStrStartFrame[] = "START FRAME         %.3f";
const char dbgPerfStrDrawProgram[] = "DRAW PROGRAM        %.3f";
const char dbgPerfStrFbEffect[] = "FB EFFECT           %.3f";
const char dbgPerfStrSyncProgram[] = "SYNC PROGRAM        %.3f";
const char dbgPerfStrSortProgram[] = "SORT PROGRAM        %.3f";
const char dbgPerfStrGrxEffect[] = "GRX EFFECT          %.3f";
const char dbgPerfStrDebugDisp[] = "DEBUG DISP          %.3f";
const char dbgPerfStrEndFrame[] = "END FRAME           %.3f";
const char dbgPerfStrSetParam[] = "SET PARAM           %.3f";
const char dbgPerfStrFrameSkip[] = "FRAME SKIP          %.3f";

/* DbgMotionObjPage (426940.c) */
const char dbgMotObjStrDataMax[] = "DATA MAX:%d";
const char dbgMotObjStrObj[] = "OBJ:%d";
const char dbgMotObjStrRun[] = "RUN";
const char dbgMotObjStrStop[] = "STOP";
const char dbgMotObjStrSs[] = "SS";
const char dbgMotObjStrMot0Stop[] = "MOT0:STOP / %d";
const char dbgMotObjStrMot0[] = "MOT0:%d";
const char dbgMotObjStrFrm[] = "Frm %.1f";
const char dbgMotObjStrSpd[] = "Spd %.2f";
const char dbgMotObjStrLoopOn[] = "LOOP ON";
const char dbgMotObjStrLoopOff[] = "LOOP OFF";
const char dbgMotObjStrDashes[] = "--------";
const char dbgMotObjStrCrsOn[] = "CRS ON";
const char dbgMotObjStrCrsOff[] = "CRS OFF";
const char dbgMotObjStrSdwOn[] = "SDW ON";
const char dbgMotObjStrSdwOff[] = "SDW OFF";
const char dbgMotObjStrStgOn[] = "STG ON";
const char dbgMotObjStrStgOff[] = "STG OFF";
const char dbgMotObjStrPosX[] = "POS X %d";
const char dbgMotObjStrRotY[] = "ROT Y";
const char dbgMotObjStrRef[] = "REF %d";
const char dbgMotObjStrSmt[] = "SMT %d";
