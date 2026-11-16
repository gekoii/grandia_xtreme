/* FIELD .rodata 0x00672DC0-0x00672F50: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C2DC0.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00672DC0-0x00672F50 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Format strings of the OCCS page (DbgFldOccsPage, src/field/5DB1D8.c), the
 * OCC&OCS menu title (DbgFldOccMenu, 5DB478) and the camera script list page
 * (DbgFldCamScriptListPage, 5DB568 and the next functions). */

const char dbgOccsStrNumber[] = "OCCS Number: %d / %d"; /* 00672DC0 */
const char dbgOccsStrCat[] = "OCCS cat: %d"; /* 00672DD8 */
const char dbgOccsStrEnabledOn[] = "Enabled    : ON"; /* 00672DE8 */
const char dbgOccsStrEnabledOff[] = "Enabled    : OFF"; /* 00672DF8 */
const char dbgOccsStrDisabledOn[] = "Disabled   : ON"; /* 00672E10 */
const char dbgOccsStrDisabledOff[] = "Disabled   : OFF"; /* 00672E20 */
const char dbgOccMenuStrTitle[] = "OCC&OCS MENU page %s %d/2%s "; /* 00672E38 */
const char dbgCamScrStrListMax[] = "CAMERA SCRIPT LIST MAX %d"; /* 00672E58 */
const char dbgCamScrStrDataNo[] = "DATA NO %03d"; /* 00672E78 */
const char dbgCamScrStrPosX[] = "POS  X:%.3f"; /* 00672E88 */
const char dbgCamScrStrY[] = "     Y:%.3f"; /* 00672E98 */
const char dbgCamScrStrZ[] = "     Z:%.3f"; /* 00672EA8 */
const char dbgCamScrStrMoveX[] = "MOVE X:%.3f"; /* 00672EB8 */
const char dbgCamScrStrTurnX[] = "TURN X:%.3f"; /* 00672EC8 */
const char dbgCamScrStrLength[] = "LENGTH:%.3f"; /* 00672ED8 */
const char dbgCamScrStrPers[] = "PERS  :%.3f"; /* 00672EE8 */
const char dbgCamScrStrZoom[] = "ZOOM  :%.3f"; /* 00672EF8 */
const char dbgCamScrStrClipF[] = "CLIP F:%.3f"; /* 00672F08 */
const char dbgCamScrStrAi[] = "AI    :%s"; /* 00672F18 */
const char dbgCamScrStrTargetPlayer[] = "TARGET: PLAYER"; /* 00672F28 */
const char dbgCamScrStrTargetTenant[] = "TARGET: TENANT %d"; /* 00672F38 */
