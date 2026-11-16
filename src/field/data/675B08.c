/* FIELD .rodata: the memory card save screen's strings (the spelling is
 * the original's). Data-only C (plan step 6b, docs/shifting.md); the users
 * are McSaveFlowUpdate (65CF90.c) and its neighbours (65D590.c-65DF18).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* rodata: 0x00675B08-0x00675BA0 */

const char mcSaveBadStateFmt[] = " ERROR : Ileagal  Status :%d "; /* 00675B08 */
const char mcSaveSlotNumFmt[] = "%02d"; /* 00675B28 */
const char mcSaveFileExt[] = ".dat"; /* 00675B30 */
const char mcSaveDirPattern[] = "/BASLUS-20417GrandiaX/*"; /* 00675B38 */
const char mcSaveIconSysName[] = "icon.sys"; /* 00675B50 */
const char mcSaveIconFileName[] = "icon0.ico"; /* 00675B60 */
const char mcSaveGameFileName[] = "BASLUS-20417GrandiaX"; /* 00675B70 */
const char mcSaveFlowDirPath[] = "/BASLUS-20417GrandiaX"; /* 00675B88 */
