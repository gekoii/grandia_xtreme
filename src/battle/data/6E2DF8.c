/* BATTLE .rodata 0x006E2DF8-0x006E3170, inside the debug character pages'
 * file (after DbgBtlCharWorkTransformPage's .rodata): the strings of
 * the character work pages (6DC0C8.c, 6DC410.c, 6DC6F0.c) and the title of
 * DbgBtlCharParamPage (6DC928.c). Data-only C (plan step 6b,
 * docs/shifting.md).
 */
/* rodata: 0x006E2DF8-0x006E3170 */

/* DbgBtlCharWorkStatusPage */
const char dbgBtlWorkExeSpFmt[] = "EXE SP %d";
const char dbgBtlWorkGoodFmt[] = "GOOD %d";
const char dbgBtlWorkBadFmt[] = "BAD %d";
const char dbgBtlWorkTranspFmt[] = "TRANSP %d";
const char dbgBtlWorkAntiMFmt[] = "ANTI_M %d";
const char dbgBtlWorkRegeneFmt[] = "REGENE %d";
const char dbgBtlWorkSleepFmt[] = "SLEEP  %d";
const char dbgBtlWorkConfuFmt[] = "CONFU  %d";
const char dbgBtlWorkSealSFmt[] = "SEAL_S %d";
const char dbgBtlWorkSealMFmt[] = "SEAL_M %d";
const char dbgBtlWorkBindFmt[] = "BIND   %d";
const char dbgBtlWorkVanishFmt[] = "VANISH %d";
const char dbgBtlWorkExpFmt[] = "EXP    %d";
const char dbgBtlWorkWallFmt[] = "WALL   %d";
const char dbgBtlWorkPowUpFmt[] = "POWUP %.4X";
const char dbgBtlWorkAtkFmt[] = "ATK   %d";
const char dbgBtlWorkDefFmt[] = "DEF   %d";
const char dbgBtlWorkMgrFmt[] = "MGR   %d";
const char dbgBtlWorkActFmt[] = "ACT   %d";
const char dbgBtlWorkMovFmt[] = "MOV   %d";
const char dbgBtlWorkIpModeFmt[] = "IP MD %d";
const char dbgBtlWorkIpValFmt[] = "VAL %.5f";
const char dbgBtlWorkIpSpdFmt[] = "SPD %.5f";
const char dbgBtlWorkIpDmgFmt[] = "DMG %.5f";
const char dbgBtlWorkIpRateFmt[] = "RATE %.2f";
const char dbgBtlWorkIpStopFmt[] = "STOP %d";
const char dbgBtlWorkIpRetFmt[] = "RET %.5f";
const char dbgBtlWorkMvpFmt[] = "MVP    %d";
/* DbgBtlCharWorkAttackPage (dbgBtlIntFmt is shared with the parameter pages) */
const char dbgBtlWorkComboFmt[] = "COMBO  %d";
const char dbgBtlWorkAtkNoFmt[] = "ATK NO %d";
const char dbgBtlWorkScpTypeFmt[] = "SCP  T 0x%X";
const char dbgBtlWorkScpRFmt[] = "R %.1f";
const char dbgBtlWorkMgcNoFmt[] = "MGC NO %d";
const char dbgBtlWorkMgcLvFmt[] = "LV %d";
const char dbgBtlWorkPartsChrFmt[] = "PARTS CHR %d";
const char dbgBtlWorkRtActFmt[] = "RT ACT %d";
const char dbgBtlWorkRtPFmt[] = "P   %d";
const char dbgBtlWorkBodyFmt[] = "BODY %d";
const char dbgBtlIndexValueFmt[] = "%d %d";
const char dbgBtlWorkItemFmt[] = "ITEM     %d";
const char dbgBtlWorkItemRateFmt[] = "RATE   %.2f";
const char dbgBtlWorkTreasureFmt[] = "TREASURE %d";
const char dbgBtlWorkMvRotVFmt[] = "MV ROTV 0x%.4x";
const char dbgBtlWorkWtRotVFmt[] = "WT ROTV 0x%.4x";
const char dbgBtlWorkRotVMaxFmt[] = "   MAX  0x%.4x";
const char dbgBtlWorkEggNoFmt[] = "EGG NO  %d";
const char dbgBtlWorkEggMgcFmt[] = "MGC %d";
const char dbgBtlIntFmt[] = "%d";
/* DbgBtlCharWorkMotionPage */
const char dbgBtlWorkMotFmt[] = "MOT    %d";
const char dbgBtlWorkAnmFmt[] = "ANM    %7.1f";
const char dbgBtlWorkAnmSpFmt[] = "SP %7.1f";
const char dbgBtlWorkAnmStartFmt[] = "S %7.1f";
const char dbgBtlWorkAnmEndFmt[] = "E %7.1f";
const char dbgBtlWorkAnmLoopFmt[] = "LOOP %d";
const char dbgBtlWorkHeadLookFmt[] = "H LOOK %d";
const char dbgBtlWorkWakeFmt[] = "WAKE   %d";
const char dbgBtlWorkCombiLFmt[] = "COMBI L %d";
const char dbgBtlWorkFaceModeFmt[] = "FACE MD %d";
const char dbgBtlWorkFaceIdFmt[] = "ID %d";
const char dbgBtlWorkFaceC1Fmt[] = "C1 %d";
const char dbgBtlWorkFaceC2Fmt[] = "C2 %d";
const char dbgBtlWorkFaceC3Fmt[] = "C3 %d";
/* DbgBtlCharParamPage */
const char dbgBtlParamTitleFmt[] = "%sCHAR PARAM%s page %d/8";
