/* BATTLE .rodata 0x006E07F8-0x006E0AB0: the .rodata of four original files,
 * each starting with its "save": the demo-menu captions (BtlMenuInit
 * 697958.c), two files with nothing else, the debug battle config keywords
 * (DbgBtlCfgParse 69BCC0.c), and the command menu names and icons
 * (BtlCmdMenuInit 69C8D8.c, 69D6C8.c). Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here: declare what
 * the data refers to locally.
 */
/* rodata: 0x006E07F8-0x006E0AB0 */

typedef short s16;
typedef unsigned short u16;

/* Texture rectangle of a sprite (include/sys.h SprRect): size, then the UV
 * corners. */
typedef struct SprRect {
    s16 w;
    s16 h;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
} SprRect;

static const char save[] = "save"; /* a header's unused static: the first thing in each file's .rodata */
/* Demo-menu captions for the system font (sysFontImage2): letters are
 * ASCII - 7, spaces stay */
const char btlMenuStrPressStart[] = "IK>LL LM:KM ;NMMHG"; /* "PRESS START BUTTON" */
const char btlMenuStrDemonstration[] = "=>FHGLMK:MBHG"; /* "DEMONSTRATION" */
const char btlMenuStrCredit[] = "<K>=BM,L- 0"; /* "CREDIT..." */
/* A 128x128 sprite rect nothing refers to */
static const SprRect btlMenuUnusedRect = { 128, 128, 0, 0, 129, 129 };
static const char save_006E0848[] = "save"; /* the next file's .rodata starts here */
static const char save_006E0850[] = "save"; /* the next file's .rodata starts here */
static const char save_006E0858[] = "save"; /* the next file's .rodata starts here */
/* Debug battle config keywords and formats (DbgBtlCfgParse) */
const char dbgBtlCfgStrEnd[] = "END";
const char dbgBtlCfgStrDmgRnd[] = "DMG_RND";
const char dbgBtlCfgFmtInt[] = "%i";
const char dbgBtlCfgStrNoDeath[] = "NO_DEATH";
const char dbgBtlCfgStrNoMpsp[] = "NO_MPSP";
const char dbgBtlCfgStrDbgPcai[] = "DBG_PCAI";
const char dbgBtlCfgStrMgcGbx[] = "MGC_GBX";
const char dbgBtlCfgStrAi[] = "AI";
const char dbgBtlCfgStrAiMgcNo[] = "AI_MGC_NO";
const char dbgBtlCfgStrAiMgcCmb[] = "AI_MGC_CMB";
const char dbgBtlCfgStrPcai1[] = "PCAI1";
const char dbgBtlCfgStrPcai2[] = "PCAI2";
const char dbgBtlCfgStrMcAi[] = "MC_AI";
const char dbgBtlCfgStrMcAtk[] = "MC_ATK";
const char dbgBtlCfgStrStop[] = "STOP";
const char dbgBtlCfgStrSpP[] = "SP_P";
const char dbgBtlCfgFmtInt6[] = "%i %i %i %i %i %i";
const char dbgBtlCfgStrSpC2[] = "SP_C2";
const char dbgBtlCfgFmtInt8[] = "%i %i %i %i %i %i %i %i";
const char dbgBtlCfgStrSpC3[] = "SP_C3";
const char dbgBtlCfgFmtInt7[] = "%i %i %i %i %i %i %i";
const char dbgBtlCfgStrSkl[] = "SKL";
static const char save_006E0960[] = "save"; /* the next file's .rodata starts here */
/* Command menu names, font-coded (each character a byte, then 0xF9) */
const char btlCmdNames[8][27] = {
    "b\371\216\371\214\371\201\371\216\371",
    "c\371\204\371\205\371\204\371\215\371\222\371\204\371N\371d\371\225\371\200\371\203\371\204\371",
    "d\371\222\371\202\371\200\371\217\371\204\371",
    "`\371h\371",
    "h\371\223\371\204\371\214\371\222\371",
    "b\371\221\371\210\371\223\371\210\371\202\371\200\371\213\371",
    "l\371\200\371\206\371\210\371\202\371",
    "l\371\216\371\225\371\204\371\222\371",
}; /* "Combo", "Defense/Evade", "Escape", "AI", "Items", "Critical", "Magic", "Moves" */
const char btlCmdStrDefense[] = "c\371\204\371\205\371\204\371\215\371\222\371\204\371"; /* "Defense" (the label above the unit when move point 0 is selected) */
/* Command icons, by command (BtlCmdMenuDraw 69D6C8.c) */
const SprRect btlCmdIconSprites[8] = {
    { 32, 32, 1, 1, 34, 34 },
    { 32, 32, 35, 35, 68, 68 }, /* D_006E0A5C */
    { 32, 32, 103, 35, 136, 68 },
    { 32, 32, 69, 35, 102, 68 },
    { 32, 32, 69, 1, 102, 34 },
    { 32, 32, 35, 1, 68, 34 },
    { 32, 32, 1, 35, 34, 68 },
    { 32, 32, 103, 1, 136, 34 },
}; /* fields: D_006E0A5C [1] */
