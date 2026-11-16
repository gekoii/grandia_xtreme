/* FIELD .rodata: the memory card load screen's strings (the spelling is
 * the original's), then the "save" strings that start the next two files'
 * .rodata. Data-only C (plan step 6b, docs/shifting.md); the users are
 * McLoadFlowUpdate (65F0B8.c) and its neighbours (65F598.c-65FA28).
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* rodata: 0x00675C10-0x00675CB0 */

const char mcLoadBadStateFmt[] = " ERROR : ILEAGAL STATUS  %d "; /* 00675C10 */
const char mcLoadSlotOutStr[] = " Memory Card Slot Out"; /* 00675C30 */
const char mcLoadSlotNumFmt[] = "%02d"; /* 00675C48 */
const char mcLoadFileExt[] = ".dat"; /* 00675C50 */
const char mcLoadNotPs2Str[] = "Not PS2 Memory Card"; /* 00675C58 */
const char mcLoadDirPattern[] = "/BASLUS-20417GrandiaX/*"; /* 00675C70 */
const char mcLoadDirPath[] = "/BASLUS-20417GrandiaX"; /* 00675C88 */
static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
static const char save_00675CA8[] = "save"; /* the next file's .rodata starts here */
