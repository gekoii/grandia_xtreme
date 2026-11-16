/* main executable .rodata 0x004A4AE8-0x004A4C50: strings of
 * the debug menu's motion parts, motion menu, screen division and point light pages.
 * Data-only C (plan step 6b, docs/shifting.md); the pages that print them
 * are still separate files (named below). The strings are named globals
 * because the code files refer to them by name. The gaps around this range
 * are those files' own .rodata (jump tables, constants).
 */
/* rodata: 0x004A4AE8-0x004A4C50 */

/* DbgMotionPartsPage (427480.c) */
const char dbgMotPartsStrDispOff[] = "%03d : \\c4 DISP OFF\\c7 ";
const char dbgMotPartsStrDispOn[] = "%03d : DISP ON";
const char dbgMotPartsStrPartsListMax[] = "PARTS List max:%d";
const char dbgMotPartsStrEnd[] = "=END===============";

/* DbgMotionMenu (427718.c) */
const char dbgMotMenuStrMotionMenuFrame[] = "MOTION MENU frame:%.3f / %d";

/* DbgScreenDivPage (427AA0.c) */
const char dbgScrDivStrScreenDiv[] = "SCREEN DIV ";
const char dbgScrDivStrTypeNo[] = "TYPE NO %d";
const char dbgScrDivStrModeNo[] = "MODE NO %d";
const char dbgScrDivStrCouterNo[] = "COUTER NO %d";

/* DbgPointLightPage (427C28.c) */
const char dbgPtLightStrPointLight[] = "POINT LIGHT ";
const char dbgPtLightStrLightNo[] = "LIGHT NO %d";
const char dbgPtLightStrPosX[] = "POS    X %.3f";
const char dbgPtLightStrY[] = "       Y %.3f";
const char dbgPtLightStrZ[] = "       Z %.3f";
const char dbgPtLightStrRange[] = "RANGE  %.3f";
const char dbgPtLightStrColorR[] = "COLOR  R %.3f";
const char dbgPtLightStrG[] = "       G %.3f";
const char dbgPtLightStrB[] = "       B %.3f";
const char dbgPtLightStrA[] = "       A %.3f";
const char dbgPtLightStrPower[] = "POWER  %.3f";
