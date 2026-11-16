/* FIELD .rodata 0x00672058-0x00672290: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C2058.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00672058-0x00672290 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

/* Format strings of the map-jump debug page (DbgFldMapJumpPage,
 * src/field/5D8DE0.c) and the main-flag page (DbgFldMainFlagPage, 5D90B8). */

const char dbgMapJumpStrTitle[] = "MAP JUMP MENU ( MAP MAX %s %d/%d %s "; /* 00672058 */
const char dbgMapJumpStrMapNo[] = "MAP NO    : %04X(%04X)"; /* 00672080 */
const char dbgMapJumpStrMainFlag[] = "MAIN FLAG : %05d"; /* 00672098 */
const char dbgMapJumpStrRule[] = "---------------------------------------"; /* 006720B0 */
const char dbgMainFlagStrMapNo[] = "MAP NO          : %04x   RESULT %d %d %d %d"; /* 006720D8 */
const char dbgMainFlagStrOldMapNo[] = "OLD MAP NO      : %04x"; /* 00672108 */
const char dbgMainFlagStrPartyId[] = "PARTY ID        : [%04x] [%04x] [%04x] [%04x]"; /* 00672120 */
const char dbgMainFlagStrRetryCounter[] = "RETRY COUNTER   : %04d"; /* 00672150 */
const char dbgMainFlagStrAreaMapNo[] = "AREAMAP NO      : %02d"; /* 00672168 */
const char dbgMainFlagStrMainFlag[] = "MAIN FLAG       : %04d"; /* 00672180 */
const char dbgMainFlagStrEncount[] = "ENCOUNT         : %s"; /* 00672198 */
const char dbgMainFlagStrTenantMove[] = "TENANT  MOVE    : %s"; /* 006721B0 */
const char dbgMainFlagStrMonsterMove[] = "MONSTER MOVE    : %s"; /* 006721C8 */
const char dbgMainFlagStrTenantDisp[] = "TENANT  DISP    : %s"; /* 006721E0 */
const char dbgMainFlagStrMonsterDisp[] = "MONSTER DISP    : %s"; /* 006721F8 */
const char dbgMainFlagStrCameraPause[] = "CAMERA PAUSE    : %s"; /* 00672210 */
const char dbgMainFlagStrCameraStatus[] = "CAMERA STATUS   : %s"; /* 00672228 */
const char dbgMainFlagStrDemoStatus[] = "DEMO STATUS     : %s"; /* 00672240 */
const char dbgMainFlagStrDemoTempDisp[] = "DEMO TEMP DISP  : %s"; /* 00672258 */
const char dbgMainFlagStrOcc[] = "OCC       : %s"; /* 00672270 */
const char dbgMainFlagStrMealEnd[] = "MEAL END  : %s"; /* 00672280 */
