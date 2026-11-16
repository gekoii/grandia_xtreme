/* FIELD .rodata 0x00671990-0x00672000: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C1990.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00671990-0x00672000 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Format strings of the field debug menus: the scenario menu
 * (DbgFldScenarioMenu, src/field/5D75E0.c), the character pages
 * (DbgChrFacePage 5D76E8, DbgChrGroupPage 5D8330, DbgChrMovePage 5D84E0 and
 * the pages between them) and the tenant menu title (DbgFldTenantMenu,
 * 5D8970). Typos ("SCNRARIO", "FACECIAL", "serch", "TALL") are the game's. */

const char dbgScnMenuStrTitle[] = "SCNRARIO MENU page %s %d/2%s NOW SCNR NO %04x"; /* 00671990 */
const char dbgChrStrFacialId[] = "FACECIAL ID        %d"; /* 006719C0 */
const char dbgChrStrFaceMode[] = "         MODE      %d"; /* 006719D8 */
const char dbgChrStrFaceCounter1[] = "         counter1  %d"; /* 006719F0 */
const char dbgChrStrFaceCounter2[] = "         counter2  %d"; /* 00671A08 */
const char dbgChrStrFaceCounter3[] = "         counter3  %d"; /* 00671A20 */
const char dbgChrStrFaceOrgId[] = "         org_id    %d"; /* 00671A38 */
const char dbgChrStrFaceSubId0[] = "         sub_id[0] %d"; /* 00671A50 */
const char dbgChrStrFaceSubId1[] = "         sub_id[1] %d"; /* 00671A68 */
const char dbgChrStrFaceSubId2[] = "         sub_id[2] %d"; /* 00671A80 */
const char dbgChrStrFaceSubId3[] = "         sub_id[3] %d"; /* 00671A98 */
const char dbgChrStrFaceSubId4[] = "         sub_id[4] %d"; /* 00671AB0 */
const char dbgChrStrFaceSubId5[] = "         sub_id[5] %d"; /* 00671AC8 */
const char dbgChrStrFaceSubId6[] = "         sub_id[6] %d"; /* 00671AE0 */
const char dbgChrStrNeckX[] = "Neck  X %.3f"; /* 00671AF8 */
const char dbgChrStrNeckY[] = "      Y %.3f"; /* 00671B08 */
const char dbgChrStrStatus[] = "STATUS   %d"; /* 00671B18 */
const char dbgChrStrColSt[] = "COL_ST   %d"; /* 00671B28 */
const char dbgChrStrAttr[] = "ATTR     %d"; /* 00671B38 */
const char dbgChrStrAttr2[] = "ATTR2    %d"; /* 00671B48 */
const char dbgChrStrScnrId[] = "SCNR ID  $%x"; /* 00671B58 */
const char dbgChrStrCrSpeed[] = "CR SPEED %.3f"; /* 00671B68 */
const char dbgChrStrR[] = "R        %.3f(%.3f)"; /* 00671B78 */
const char dbgChrStrSerch[] = "serch    %.3f"; /* 00671B90 */
const char dbgChrStrDirect[] = "direct   %x"; /* 00671BA0 */
const char dbgChrStrTarget[] = "target   %x"; /* 00671BB0 */
const char dbgChrStrJump[] = "jump     %x"; /* 00671BC0 */
const char dbgChrStrObj[] = "obj       %x"; /* 00671BD0 */
const char dbgChrStrPosX[] = "pos    X %.3f"; /* 00671BE0 */
const char dbgChrStrTargetX[] = "target X %.3f"; /* 00671BF0 */
const char dbgChrStrAiNo[] = "AI NO    %d"; /* 00671C00 */
const char dbgChrStrAiFrame[] = "AI FRAME %d"; /* 00671C10 */
const char dbgChrStrPartsNum[] = "parts num %d"; /* 00671C20 */
const char dbgChrStrMotionId[] = "MOTION ID    %d"; /* 00671C30 */
const char dbgChrStrMotionSpeed[] = "MOTION Speed %.3f"; /* 00671C40 */
const char dbgChrStrBorderMax[] = "BORDER MAX   %d"; /* 00671C58 */
const char dbgChrStrEventAdrs[] = "EVENT ADRS   %x"; /* 00671C68 */
const char dbgChrStrPoint0[] = "POINT   0    %x"; /* 00671C78 */
const char dbgChrStrPoint1[] = "POINT   1    %x"; /* 00671C88 */
const char dbgChrStrPoint2[] = "POINT   2    %x"; /* 00671C98 */
const char dbgChrStrPoint3[] = "POINT   3    %x"; /* 00671CA8 */
const char dbgChrStrPoint4[] = "POINT   4    %x"; /* 00671CB8 */
const char dbgChrStrPoint5[] = "POINT   5    %x"; /* 00671CC8 */
const char dbgChrStrPoint6[] = "POINT   6    %x"; /* 00671CD8 */
const char dbgChrStrPoint7[] = "POINT   7    %x"; /* 00671CE8 */
const char dbgChrStrFlagId[] = "FLAG ID      %d"; /* 00671CF8 */
const char dbgChrStrTypeMonster[] = "CHAR TYPE        MONSTER"; /* 00671D08 */
const char dbgChrStrTypeTenant[] = "CHAR TYPE        TENANT"; /* 00671D28 */
const char dbgChrStrSit[] = "SIT"; /* 00671D40 */
const char dbgChrStrWake[] = "WAKE"; /* 00671D48 */
const char dbgChrStrDisp[] = "DISP             %s"; /* 00671D50 */
const char dbgChrStrExist[] = "EXIST            %s"; /* 00671D68 */
const char dbgChrStrCollision[] = "COLLISION        %s"; /* 00671D80 */
const char dbgChrStrGravity[] = "GRAVITY          %s"; /* 00671D98 */
const char dbgChrStrShadow[] = "SHADOW           %s"; /* 00671DB0 */
const char dbgChrStrMoveEnable[] = "MOVE ENABLE      %s"; /* 00671DC8 */
const char dbgChrStrMotionOverwrite[] = "MOTION OVERWRITE %s"; /* 00671DE0 */
const char dbgChrStrAutoDirect[] = "AUTO DIRECT      %s"; /* 00671DF8 */
const char dbgChrStrTalk[] = "TALK             %s"; /* 00671E10 */
const char dbgChrStrTalkAutoDirect[] = "TALL AUTO DIRECT %s"; /* 00671E28 */
const char dbgChrStrTalkJumpMove[] = "TALK JUMP MOVE   %s"; /* 00671E40 */
const char dbgChrStrGroundCollision[] = "GROUND COLLISION %s"; /* 00671E58 */
const char dbgChrStrLineCollision[] = "LINE   COLLISION %s"; /* 00671E70 */
const char dbgChrStrAiExec[] = "AI EXEC          %s"; /* 00671E88 */
const char dbgChrStrSetFileFlag[] = "SETFILE FLAG     %d"; /* 00671EA0 */
const char dbgChrStrGroupMax[] = "GROUP MAX   %d"; /* 00671EB8 */
const char dbgChrStrGroupPoint[] = "POINT       %d"; /* 00671EC8 */
const char dbgChrStrGroupInfHeader[] = "-GROUP INF -----------------total--"; /* 00671ED8 */
const char dbgChrStrGroupNo[] = " NO    %4d %4d %4d %4d"; /* 00671F00 */
const char dbgChrStrGroupMaxRow[] = " MAX   %4d %4d %4d %4d  = %d"; /* 00671F18 */
const char dbgChrStrGroupLevel[] = " LEVEL %4d %4d %4d %4d  = %d"; /* 00671F38 */
const char dbgChrStrGlobalNo[] = "GLOBAL NO   %d"; /* 00671F58 */
const char dbgChrStrSeInterval[] = "SE INTERVAL %d"; /* 00671F68 */
const char dbgChrStrSeRandom[] = "   RANDOM   %d"; /* 00671F78 */
const char dbgChrStrSeNormal[] = "   NORMAL   %d"; /* 00671F88 */
const char dbgChrStrSeAttack[] = "   ATTACK   %d"; /* 00671F98 */
const char dbgChrStrMoveDebug[] = "CHARACTER MOVE DEBUG"; /* 00671FA8 */
const char dbgChrStrMoveDirect[] = "DIRECT     %d"; /* 00671FC0 */
const char dbgTenantMenuStrTitle[] = "TENANT (%d / %d) Page %s %d / 6 %s "; /* 00671FD0 */
