/* BATTLE .rodata 0x006E1660-0x006E17D0: the .rodata starts of two original
 * files: one with nothing else, then the debug environment pages' file
 * (DbgEnvLightPage 6B5E58.c here; its other pages' strings continue at
 * 0x006E1860). Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x006E1660-0x006E17D0 */

static const char save[0x10] = "save"; /* a header's unused static; padded: the next file's "save" is 16-aligned */
static const char save_006E1670[] = "save"; /* the next file's .rodata starts here */
/* DbgEnvLightPage (the colour codes are shared with the other pages) */
const char dbgEnvLightTitleFmt[] = "%sLIGHT%s";
const char dbgEnvTitleColorStr[] = "\\c1 ";
const char dbgEnvColor0Str[] = "\\c0 ";
const char dbgEnvLightNoFmt[] = "No.%d (%s)";
const char dbgEnvLightOnStr[] = "ON";
const char dbgEnvLightOffStr[] = "OFF";
const char dbgEnvLightAmbStr[] = "AMB";
const char dbgEnvLightCharStr[] = "CHAR";
const char dbgEnvLightRFmt[] = "R   %7.2f";
const char dbgEnvLightGFmt[] = "G   %7.2f";
const char dbgEnvLightBFmt[] = "B   %7.2f";
const char dbgEnvLightAFmt[] = "A   %7.2f";
const char dbgEnvLightGroundStr[] = "GROUND";
const char dbgEnvLightDirNoFmt[] = "DIR %d";
const char dbgEnvLightIntensityFmt[] = "ITS %7.2f";
const char dbgEnvLightAngleXFmt[] = "AX   0x%.4X";
const char dbgEnvLightAngleYFmt[] = "AY   0x%.4X";
const char dbgEnvLightVecFmt[] = "(%5.2f,%5.2f,%5.2f)";
const char dbgEnvLightModeFmt[] = "MODE (%s)";
const char dbgEnvLightDirStr[] = "DIR";
const char dbgEnvLightPointStr[] = "POINT";
const char dbgEnvLightCharToggleStr[] = "CHAR   ";
const char dbgEnvLightGroundToggleStr[] = "GROUND ";
const char dbgEnvLightPosXFmt[] = "PX  %7.2f";
const char dbgEnvLightPosYFmt[] = "PY  %7.2f";
const char dbgEnvLightPosZFmt[] = "PZ  %7.2f";
const char dbgEnvLightRangeFmt[] = "RNG %7.2f";
