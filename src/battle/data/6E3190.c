/* BATTLE .rodata 0x006E3190-0x006E3E40, inside the debug character pages'
 * file (after DbgBtlCharParamPage's .rodata): the strings of the
 * character parameter pages (6DCB28.c to 6DDDC0.c), the PC save pages
 * (6DDFB0.c, 6DE140.c, 6DE420.c) and DbgBtlParTestPage (6DE570.c).
 * Data-only C (plan step 6b, docs/shifting.md).
 */
/* rodata: 0x006E3190-0x006E3E40 */

/* DbgBtlCharParamBasePage */
const char dbgBtlParamIdFmt[] = "id       0x%.4X";
const char dbgBtlParamIconFmt[] = "icon     %d";
const char dbgBtlParamSizeFmt[] = "size     %d";
const char dbgBtlParamRaceFmt[] = "race     %d";
const char dbgBtlParamTypeFmt[] = "type     %d";
const char dbgBtlParamBitStaFmt[] = "bit_sta  %.8X";
const char dbgBtlParamBitSta2Fmt[] = "bit_sta2 %.8X";
const char dbgBtlParamAtkFmt[] = "atk      %d";
const char dbgBtlParamDefFmt[] = "def      %d";
const char dbgBtlParamMgpFmt[] = "mgp      %d";
const char dbgBtlParamMgrFmt[] = "mgr      %d";
const char dbgBtlParamActFmt[] = "act      %d";
const char dbgBtlParamMovFmt[] = "mov      %d";
const char dbgBtlParamIpStopDmgFmt[] = "ip_stop_dmg  %d";
const char dbgBtlParamIpStopDownFmt[] = "ip_stop_down %d";
const char dbgBtlParamLvFmt[] = "lv        %d";
const char dbgBtlParamExpFmt[] = "exp       %d";
const char dbgBtlParamNextExpFmt[] = "next_exp  %d";
const char dbgBtlParamMhpFmt[] = "mhp       %d";
const char dbgBtlParamHpFmt[] = "hp        %d";
const char dbgBtlParamMmpFmt[] = "mmp       %d";
const char dbgBtlParamMpFmt[] = "mp        %d";
const char dbgBtlParamMspFmt[] = "msp       %d";
const char dbgBtlParamSpFmt[] = "sp        %d";
const char dbgBtlParamStrFmt[] = "_str      %d";
const char dbgBtlParamStaFmt[] = "_sta      %d";
const char dbgBtlParamItlFmt[] = "_itl      %d";
const char dbgBtlParamSpiFmt[] = "_spi      %d";
const char dbgBtlParamAgiFmt[] = "_agi      %d";
const char dbgBtlParamRunFmt[] = "_run      %d";
const char dbgBtlParamComboMaxFmt[] = "combo_max %d";
/* DbgBtlCharParamEquipPage (dbgBtlFloatFmt is shared with the next pages) */
const char dbgBtlParamGuardDecFmt[] = "guard_dec     %.2f";
const char dbgBtlParamAvoidWaitFmt[] = "avoid_wait   %.2f";
const char dbgBtlParamAvoidEvadeFmt[] = "avoid_evade  %.2f";
const char dbgBtlParamItemUpBombFmt[] = "item_up_bomb %.2f";
const char dbgBtlParamItemUpScrFmt[] = "item_up_scr  %.2f";
const char dbgBtlParamItemUpDrugFmt[] = "item_up_drug %.2f";
const char dbgBtlParamEqpAttrFmt[] = "eqp_attr     %d";
const char dbgBtlParamEqpKillerFmt[] = "eqp_killer   %d";
const char dbgBtlParamEqpBitStaFmt[] = "eqp_bit_sta  %.8X";
const char dbgBtlParamEqpStaProbFmt[] = "eqp_sta_prob %.2f";
const char dbgBtlParamAbsorbHpFmt[] = "absorb_hp    %.2f";
const char dbgBtlParamCrtclUpFmt[] = "crtcl_up     %d";
const char dbgBtlParamComboUpFmt[] = "combo_up     %d";
const char dbgBtlParamCrtclMultiFmt[] = "crtcl_multi  %d";
const char dbgBtlParamAtkDownFmt[] = "atk_down     %d";
const char dbgBtlParamDefDownFmt[] = "def_down     %d";
const char dbgBtlParamKillerFmt[] = "killer %.2f";
const char dbgBtlFloatFmt[] = "%.2f";
/* DbgBtlCharParamRacePage */
const char dbgBtlParamRegRaceFmt[] = "reg_race %.2f";
const char dbgBtlParamDefRaceFmt[] = "def_race %d";
/* DbgBtlCharParamMagicPage */
const char dbgBtlParamMagicRateFmt[] = "magic_rate %.2f";
const char dbgBtlParamMagicPowFmt[] = "magic_pow %d";
/* DbgBtlCharParamResistPage */
const char dbgBtlParamColRFmt[] = "col_r        %.2f";
const char dbgBtlParamColHFmt[] = "col_h        %.2f";
const char dbgBtlParamRunTimeFmt[] = "run_time     %d";
const char dbgBtlParamRegMgcIprFmt[] = "reg_mgc_ipr  %.2f";
const char dbgBtlParamRegMgcAttrFmt[] = "reg_mgc_attr %.2f";
const char dbgBtlParamRegIpFmt[] = "reg_ip  %.2f";
const char dbgBtlParamRegKbFmt[] = "reg_kb  %.2f";
const char dbgBtlParamBitCsfFmt[] = "bit_csf %.8X";
const char dbgBtlParamRegStaFmt[] = "reg_sta %.2f";
/* DbgBtlCharParamAbilityPage */
const char dbgBtlParamSpcSpdRateFmt[] = "spc_spd_rate  %.2f";
const char dbgBtlParamSpcRetRateFmt[] = "spc_ret_rate  %.2f";
const char dbgBtlParamSpcPowFmt[] = "spc_pow       %d";
const char dbgBtlParamSpcSpRateFmt[] = "spc_sp_rate   %.2f";
const char dbgBtlParamMgcSpdRateFmt[] = "mgc_spd_rate  %.2f";
const char dbgBtlParamMgcRetRateFmt[] = "mgc_ret_rate  %.2f";
const char dbgBtlParamMgcMpRateFmt[] = "mgc_mp_rate   %.2f";
const char dbgBtlParamMgc1IpCanFmt[] = "mgc1_ip_can   %d";
const char dbgBtlParamMultiMagicFmt[] = "multi_magic   %d";
const char dbgBtlParamItmSpdRateFmt[] = "itm_spd_rate  %.2f";
const char dbgBtlParamGuardHpFmt[] = "guard_hp       %d";
const char dbgBtlParamGuardSpFmt[] = "guard_sp       %d";
const char dbgBtlParamEscapeUpFmt[] = "escape_up     %.2f";
const char dbgBtlParamStrikeBackFmt[] = "strike_back   %.2f";
const char dbgBtlParamCmdDisableFmt[] = "cmd_disable   0x%x";
const char dbgBtlParamNoDamageFmt[] = "no_damage     %d";
const char dbgBtlParamRefDamageFmt[] = "ref_damage    %.2f";
const char dbgBtlParamTakeDmgMpFmt[] = "take_dmg_mp   %.2f";
const char dbgBtlParamAutoGuardFmt[] = "auto_guard    %.2f";
const char dbgBtlParamAutoYomiFmt[] = "auto_yomi     %.2f";
const char dbgBtlParamAngryRateFmt[] = "angry_rate    %.2f";
const char dbgBtlParamAngryProbFmt[] = "angry_prob    %.2f";
const char dbgBtlParamAngryFmt[] = "angry         %.2f";
const char dbgBtlParamSArmorRateFmt[] = "s_armor_rate  %.2f";
const char dbgBtlParamLureFmt[] = "lure          %.2f";
const char dbgBtlParamAntiLvDrainFmt[] = "anti_lv_drain %.2f";
const char dbgBtlParamAntiKbFmt[] = "anti_kb       %.2f";
const char dbgBtlParamAntiRetireFmt[] = "anti_retire   %.2f";
const char dbgBtlParamAntiCounterFmt[] = "anti_counter  %.2f";
const char dbgBtlParamAntiCancelFmt[] = "anti_cancel   %.2f";
const char dbgBtlParamAntiHalfFmt[] = "anti_half     %.2f";
/* DbgBtlCharParamGrowthPage */
const char dbgBtlParamGetGoldRateFmt[] = "get_gold_rate %.2f";
const char dbgBtlParamGetRateUpFmt[] = "get_rate_up   %.2f";
const char dbgBtlParamGetItemUpFmt[] = "get_item_up   %.2f";
const char dbgBtlParamGetExpRateFmt[] = "get_exp_rate  %.2f";
const char dbgBtlParamCombiHintFmt[] = "combi_hint    %.2f";
const char dbgBtlParamMcActDispFmt[] = "mc_act_disp   %.2f";
const char dbgBtlParamLvupMhpFmt[] = "lvup_mhp      %d";
const char dbgBtlParamLvupStrFmt[] = "lvup_str      %d";
const char dbgBtlParamLvupStaFmt[] = "lvup_sta      %d";
const char dbgBtlParamLvupItlFmt[] = "lvup_itl      %d";
const char dbgBtlParamLvupSpiFmt[] = "lvup_spi      %d";
const char dbgBtlParamLvupAgiFmt[] = "lvup_agi      %d";
const char dbgBtlParamLvupRunFmt[] = "lvup_run      %d";
const char dbgBtlParamStartIpOfsFmt[] = "start_ip_ofs  %.2f";
const char dbgBtlParamMcAwayDecFmt[] = "mc_away_dec   %.2f";
const char dbgBtlParamAtkRefFmt[] = "atk_ref       %.2f";
const char dbgBtlParamStartTranspFmt[] = "start_transp  %.2f";
const char dbgBtlParamStartSleepFmt[] = "start_sleep   %.2f";
const char dbgBtlParamStartParaFmt[] = "start_para    %.2f";
const char dbgBtlParamExeSpFmt[] = "exe_sp        %d";
const char dbgBtlParamExeGoodFmt[] = "exe_good      %d";
const char dbgBtlParamExeBadFmt[] = "exe_bad       %d";
const char dbgBtlParamSpSupCmbFmt[] = "sp_sup_cmb    %d";
const char dbgBtlParamSpSupCrtFmt[] = "sp_sup_crt    %d";
const char dbgBtlParamSpSupDmgFmt[] = "sp_sup_dmg    %d";
const char dbgBtlParamSpSupDownFmt[] = "sp_sup_down   %d";
const char dbgBtlParamSpSupGuardFmt[] = "sp_sup_guard   %d";
const char dbgBtlParamRestartItemFmt[] = "restart_item  %.2f";
const char dbgBtlParamRestartEquipFmt[] = "restart_equip %.2f";
const char dbgBtlParamRestartGuardFmt[] = "restart_guard  %.2f";
const char dbgBtlParamRestartEvadeFmt[] = "restart_evade %.2f";
/* DbgBtlCharParamKregPage */
const char dbgBtlParamKregGoodFmt[] = "kreg_good  %.2f";
const char dbgBtlParamKregSpiFmt[] = "kreg_spi   %.2f";
const char dbgBtlParamKregSealFmt[] = "kreg_seal  %.2f";
const char dbgBtlParamKregOtherFmt[] = "kreg_other %.2f";
const char dbgBtlParamKregPowDFmt[] = "kreg_pow_d %.2f";
const char dbgBtlParamKregPowUFmt[] = "kreg_pow_u %.2f";
const char dbgBtlParamExpSkl0Fmt[] = "exp_skl0   %d";
const char dbgBtlParamExpSkl1Fmt[] = "exp_skl1   %d";
/* DbgBtlPcSavePage */
const char dbgBtlPcSaveTitleFmt[] = "%sPC SAVE%s page %d/2";
const char dbgBtlPcSaveNoFmt[] = "PC (%d)";
/* DbgBtlPcSaveStatsPage */
const char dbgBtlPcSaveExpFmt[] = "exp %d";
const char dbgBtlPcSaveMhpFmt[] = "mhp %d";
const char dbgBtlPcSaveHpFmt[] = "hp  %d";
const char dbgBtlPcSaveMspFmt[] = "msp %d";
const char dbgBtlPcSaveSpFmt[] = "sp  %d";
const char dbgBtlPcSaveStrFmt[] = "str %d";
const char dbgBtlPcSaveStaFmt[] = "sta %d";
const char dbgBtlPcSaveItlFmt[] = "itl %d";
const char dbgBtlPcSaveSpiFmt[] = "spi %d";
const char dbgBtlPcSaveAgiFmt[] = "agi %d";
const char dbgBtlPcSaveRunFmt[] = "run %d";
const char dbgBtlPcSaveBitStaFmt[] = "bit_sta %.8X";
const char dbgBtlPcSaveAiFmt[] = "ai  %d";
const char dbgBtlPcSaveAi2Fmt[] = "    %d";
const char dbgBtlPcSaveMhpUpFmt[] = "mhp_up     %d";
const char dbgBtlPcSaveMspUpFmt[] = "msp_up     %d";
const char dbgBtlPcSaveStrUpFmt[] = "str_up     %d";
const char dbgBtlPcSaveStaUpFmt[] = "sta_up     %d";
const char dbgBtlPcSaveItlUpFmt[] = "itl_up     %d";
const char dbgBtlPcSaveSpiUpFmt[] = "spi_up     %d";
const char dbgBtlPcSaveAgiUpFmt[] = "agi_up     %d";
const char dbgBtlPcSaveRunUpFmt[] = "run_up     %d";
const char dbgBtlPcSaveBtlCountFmt[] = "btl_count  %d";
const char dbgBtlPcSaveKillFmt[] = "kill       %d";
const char dbgBtlPcSaveKillMaxFmt[] = "kill_max   %d";
const char dbgBtlPcSaveKillTotalFmt[] = "kill_total %d";
const char dbgBtlPcSaveDeadFmt[] = "dead       %d";
const char dbgBtlPcSaveMvpFmt[] = "mvp        %d";
/* DbgBtlPcSaveEquipPage */
const char dbgBtlPcSaveEquipFmt[] = "equip %d";
const char dbgBtlPcSaveEqpSlotFmt[] = "eqp_slot %d";
const char dbgBtlPcSaveEqpSkillFmt[] = "eqp_skill %d";
/* DbgBtlParTestPage */
const char dbgBtlParTestTitleFmt[] = "%sPAR TEST%s";
const char dbgBtlParTestMovFmt[] = "MOV %d";
const char dbgBtlParTestColRFmt[] = "COL R %.1f";
const char dbgBtlParTestColHFmt[] = "H %.1f";
const char dbgBtlParTestReach1Fmt[] = "REACH1 %d";
const char dbgBtlParTestScope1Fmt[] = "SCOPE1 %d";
const char dbgBtlParTestReach2Fmt[] = "REACH2 %d";
const char dbgBtlParTestScope2Fmt[] = "SCOPE2 %d";
