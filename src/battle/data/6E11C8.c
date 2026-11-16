/* BATTLE .rodata 0x006E11C8-0x006E1390, inside the original file that
 * starts at 0x006E1190 (after 6AE2F8.c's jump tables): the strings of the
 * debug camera page DbgBtlCameraPage (6B02A8.c). Data-only C (plan step 6b,
 * docs/shifting.md).
 */
/* rodata: 0x006E11C8-0x006E1390 */

/* DbgBtlCameraPage (2 pages: the camera, then its quake settings) */
const char dbgBtlCamTitleFmt[] = "%sCAMERA%s page %d/2";
const char dbgBtlCamTitleColorStr[] = "\\c1 ";
const char dbgBtlCamColor0Str[] = "\\c0 ";
const char dbgBtlCamDistFmt[] = "DST %.1f";
const char dbgBtlCamTargetFmt[] = "TGT %7.1f %7.1f %7.1f";
const char dbgBtlCamPosFmt[] = "CMR %7.1f %7.1f %7.1f";
const char dbgBtlCamRotFmt[] = "ROT 0x%.5X 0x%.5X 0x%.5X";
const char dbgBtlCamFovFmt[] = "ANG %7.1f";
const char dbgBtlCamModeFmt[] = "MODE  %d";
const char dbgBtlCamFlagFmt[] = "FLAG  %d";
const char dbgBtlCamPhaseFmt[] = "PHASE %d";
const char dbgBtlCamCountFmt[] = "COUNT %d";
const char dbgBtlCamQuakeStr[] = "QUAKE";
const char dbgBtlCamQuakeTypeFmt[] = "TYPE   %d";
const char dbgBtlCamQuakeCoordFmt[] = "COORD  %s";
const char dbgBtlCamQuakeViewStr[] = "VIEW";
const char dbgBtlCamQuakeWorldStr[] = "WORLD";
const char dbgBtlCamQuakeChangeStr[] = "CHANGE ";
const char dbgBtlCamQuakeTimeStr[] = "TIME   ";
const char dbgBtlCamQuakeNextFmt[] = "NEXT   %d";
const char dbgBtlCamQuakeTimeFmt[] = "TIME   %d";
const char dbgBtlCamQuakeIntFmt[] = "INT    %d";
const char dbgBtlCamQuakeCycleFmt[] = "CYCLEX %.1f";
const char dbgBtlCamYFmt[] = "Y %.1f";
const char dbgBtlCamZFmt[] = "Z %.1f";
const char dbgBtlCamQuakeBase1Fmt[] = "BASE1X %.1f";
const char dbgBtlCamQuakeRnd1Fmt[] = "RND1 X %.1f";
const char dbgBtlCamQuakeOfsFmt[] = "OFS    %.1f";
const char dbgBtlCamQuakeBase2Fmt[] = "BASE2X %.1f";
const char dbgBtlCamQuakeRnd2Fmt[] = "RND2 X %.1f";
