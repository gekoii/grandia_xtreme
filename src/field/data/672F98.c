/* FIELD .rodata 0x00672F98-0x006733D0: data-only C (plan step 6b, docs/shifting.md).
 * Drafted by tools/gen_data_c.py from asm/field/data/field/0C2F94.rodata.s; check with
 * tools/check_data.py. Don't include the project headers here: declare what
 * the data refers to locally (extern u8 x[] is fine for an address).
 */
/* rodata: 0x00672F98-0x006733D0 */

/* DbgFldCamScriptMenu (5DBF58) */
const char dbgCamScrStrTitleRun[] = "CAMERA SCRIPT [ RUN ] page %s %d/4%s"; /* 00672F98 */
const char dbgCamScrStrTitleStop[] = "CAMERA SCRIPT [ STOP ] page %s %d/4%s"; /* 00672FC0 */
/* DbgFldPerfPage (5DC0D8): ">PERFORMANCE" */
const char dbgFldPerfStrSync[] = "SYNC %.3f"; /* 00672FE8 */
const char dbgFldPerfStrOcc[] = "     OCC      %.3f"; /* 00672FF8 */
const char dbgFldPerfStrParticle[] = "     PARTICLE %.3f"; /* 00673010 */
const char dbgFldPerfStrParty[] = "     PARTY    %.3f"; /* 00673028 */
const char dbgFldPerfStrGround[] = "     GROUND   %.3f"; /* 00673040 */
const char dbgFldPerfStrTenant[] = "     TENANT   %.3f"; /* 00673058 */
const char dbgFldPerfStrScenario[] = "     SCENARIO %.3f"; /* 00673070 */
const char dbgFldPerfStrHta[] = "     HTA      %.3f"; /* 00673088 */
const char dbgFldPerfStrWindow[] = "     WINDOW   %.3f"; /* 006730A0 */
const char dbgFldPerfStrSort[] = "SORT %.3f"; /* 006730B8 */
const char dbgFldPerfStrSky[] = "     SKY      %.3f"; /* 006730C8 */
const char dbgFldPerfStrShadow[] = "     SHADOW   %.3f"; /* 006730E0 */
/* DbgFldSetFilePage (5DC3D8): MONSTER INFO, set file fields */
const char dbgSetFileStrHeader[] = "------SETFILE--------------------------"; /* 006730F8 */
const char dbgSetFileStrNo[] = "no        %d"; /* 00673120 */
const char dbgSetFileStrGoldNum[] = "gold_num  %d"; /* 00673130 */
const char dbgSetFileStrGoldMax[] = "gold max  %d"; /* 00673140 */
const char dbgSetFileStrGoldMin[] = "gold min  %d"; /* 00673150 */
const char dbgSetFileStrTrapDmg3[] = "trap_dmg3 %d"; /* 00673160 */
const char dbgSetFileStrTrapDmg2[] = "trap_dmg2 %d"; /* 00673170 */
const char dbgSetFileStrTrapDmg1[] = "trap_dmg1 %d"; /* 00673180 */
const char dbgSetFileStrBtlFld[] = "btl_fld   %d"; /* 00673190 */
const char dbgSetFileStrItNum1[] = "it_num1   %d"; /* 006731A0 */
const char dbgSetFileStrItRank1[] = "it_rank1  %d"; /* 006731B0 */
const char dbgSetFileStrItNum2[] = "it_num2   %d"; /* 006731C0 */
const char dbgSetFileStrItRank2[] = "it_rank2  %d"; /* 006731D0 */
const char dbgSetFileStrMcNum[] = "mc_num    %d"; /* 006731E0 */
const char dbgSetFileStrMapNo[] = "map no    %04x"; /* 006731F0 */
const char dbgSetFileStrMonsterHeader[] = "-----MONSTER INF------------------------"; /* 00673200 */
const char dbgSetFileStrMcNo[] = "mc no      %d/%d"; /* 00673230 */
const char dbgSetFileStrMcId[] = "   id     $%03X"; /* 00673248 */
const char dbgSetFileStrLvMax[] = "   lv max  %d"; /* 00673258 */
const char dbgSetFileStrLvMin[] = "   lv min  %d"; /* 00673268 */
const char dbgSetFileStrRate[] = "rate    %d"; /* 00673278 */
const char dbgSetFileStrNum[] = "num     %d"; /* 00673288 */
const char dbgSetFileStrLeader[] = "leader  %d"; /* 00673298 */
const char dbgSetFileStrCmm[] = "cmm     %d"; /* 006732A8 */
/* DbgFldMonsterTablePage (5DC710) */
const char dbgMonTblStrTitle[] = "MONSTER TABLE INF"; /* 006732B8 */
const char dbgMonTblStrTableNo[] = "TABLE NO    %d / %d"; /* 006732D0 */
/* DbgFldMonsterInfoMenu (5DC910) */
const char dbgMonInfoStrTitle[] = "PARAM MENU  Page : %d/%d"; /* 006732E8 */
/* DbgFldMenu (5DCA70): the field debug root menu entries */
const char dbgFldMenuStrMonsterInfo[] = ">MONSTER INFO"; /* 00673308 */
const char dbgFldMenuStrScenario[] = ">SCENARIO"; /* 00673318 */
const char dbgFldMenuStrCameraScript[] = ">CAMERA SCRIPT"; /* 00673328 */
const char dbgFldMenuStrParam[] = ">PARAM"; /* 00673338 */
const char dbgFldMenuStrParty[] = ">PARTY"; /* 00673340 */
const char dbgFldMenuStrTenant[] = ">TENANT"; /* 00673348 */
const char dbgFldMenuStrMapJump[] = ">MAP JUMP"; /* 00673350 */
const char dbgFldMenuStrScnrFlag[] = ">SCNR FLAG"; /* 00673360 */
const char dbgFldMenuStrOcc[] = ">OCC & OCCS"; /* 00673370 */
const char dbgFldMenuStrCharaMove[] = ">CHARA MOVE"; /* 00673380 */
const char dbgFldMenuStrLine[] = ">LINE"; /* 00673390 */
const char dbgFldMenuStrCollision[] = ">COLLISION"; /* 00673398 */
const char dbgFldMenuStrSystem[] = ">SYSTEM"; /* 006733A8 */
const char dbgFldMenuStrModel[] = ">MODEL"; /* 006733B0 */
const char dbgFldMenuStrPerformance[] = ">PERFORMANCE"; /* 006733B8 */
