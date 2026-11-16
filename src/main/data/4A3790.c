/* main executable: the debug CAMERA page's texts (DbgCameraPage 420C78;
 * its other constants follow in 420C78.c). Part of the .rodata of the file
 * that 4A36A0.c starts. Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x004A3790-0x004A3970 */

const char dbgCamStrCameraMenu[] = "CAMERA MENU";
const char dbgCamStrMode[] = " mode  : %d,  %s";
const char dbgCamStrPos[] = " Pos   : ( %7.2f, %7.2f, %7.2f)";
const char dbgCamStrTarget[] = " Target: ( %7.2f, %7.2f, %7.2f)";
const char dbgCamStrEyeVec[] = " Eye_Vec: ( %7.2f, %7.2f, %7.2f)";
const char dbgCamStrAngle[] = " angle : ( %7.2f, %7.2f, %7.2f)";
const char dbgCamStrDst[] = " dst   :  %7.2f";
const char dbgCamStrZoom[] = " zoom  :  %7.2f";
const char dbgCamStrQuakeTime[] = " quake :  time %d\n";
const char dbgCamStrNextCycl[] = "     next cycl %d\n";
const char dbgCamStrBase[] = "          base %.3f %.3f %.3f\n";
const char dbgCamStrRndm[] = "          rndm %.3f %.3f %.3f\n";
const char dbgCamStrCycl[] = "          cycl %.3f %.3f %.3f\n";
const char dbgCamStrBaseTrgt[] = "     base trgt %.3f %.3f %.3f\n";
const char dbgCamStrRndmTrgt[] = "     rndm trgt %.3f %.3f %.3f\n";
const char dbgCamStrUperVec[] = " Uper_Vec: ( %7.2f, %7.2f, %7.2f)";
const char dbgCamStrPers[] = " pers  :  %7.2f";
