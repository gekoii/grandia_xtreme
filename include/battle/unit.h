#ifndef BATTLE_UNIT_H
#define BATTLE_UNIT_H

#include "common.h"
#include "vu.h"
#include "chr.h" /* ChrFaceAnim, ChrModelData, ChrPackBinding (pulls in gph.h) */

/*
 * chr.h (and through it gph.h) is included for ChrFaceAnim, ChrModelData and
 * ChrPackBinding. party.h's types are used by their struct tags: PtyUnit,
 * PtyTraceDef and PtyMoveLevelData; include party.h to use the fields.
 */
struct PtyUnit;
struct PtyTraceDef;
struct PtyMoveLevelData;
struct GphMotion;
struct GphModel;

/*
 * BATTLE units: btlUnits (16 x 0x200 at 0x006F6F40; party 0-3, enemies
 * 4-15), the character definitions they point at (btlCharDefs, 8 x 0x8C)
 * and the scheduler masks and counters around them.
 *
 * Field names follow BATTLE's CHAR WORK debug pages where they label a field
 * (DbgBtlCharWorkTransformPage 0x006DBA58 "POS/ANG/SCL/ALPHA/AMB/ACT_ID/...",
 * DbgBtlCharWorkStatusPage 0x006DC0C8, DbgBtlCharWorkAttackPage 0x006DC410,
 * DbgBtlCharWorkMotionPage 0x006DC6F0) and the code otherwise. "views:" notes
 * list the other types the local views of the 203 files declare.
 */

/* ------------------------------------------------------------------ */
/* Character definitions (btlCharDefs)                                 */
/* ------------------------------------------------------------------ */

#define BTL_CHAR_DEF_COUNT 8      /* 4 party members, 4 enemy kinds */
#define BTL_MOTION_COUNT   12

struct BtlUnit;

/*
 * Action record of a character program (BtlActionSet.actions[]): what an
 * enemy unit executes (BTL_UNIT_ACT). Fields from AiEnemyChooseCommand
 * 0x006C19E8, BtlEnemyActionStart 0x006BE540, AiComboFindPartners
 * 0x006C1F20, BtlUnitActionBegin 0x006CE660, BtlUnitApproachTarget
 * 0x006C81F0 and DbgBtlParTestPage 0x006DE570.
 */
typedef struct BtlAction {
    /* 0x00 */ u8 targetCmd;       /* BtlUnitSelectTarget command */
    /* 0x01 */ s8 targetSide;      /* 3 self, 1/5 allies, else enemies (BtlCmdHasTarget kind); 2: BtlUnitCommandValidate */
    /* 0x02 */ s8 scopeMode;       /* 2: scopeType | 0x100, 4: | 0x300, else | 0x200 */
    /* 0x03 */ s8 scopeType;       /* -> BtlUnit.scopeType */
    /* 0x04 */ u8 unk4;
    /* 0x05 */ s8 dimKind;         /* EnvDimForSkill kind when moveId >= 0x52 */
    /* 0x06 */ s16 moveId;         /* -> BtlUnit.skillId */
    /* 0x08 */ s32 name;           /* caption text (BtlCaptionShow); s32 for BtlUnitActionBegin's
                                    * alias order, the text users cast (u8 *) */
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ s16 reach;          /* added to the collision radii (BtlUnitApproachTarget) */
    /* 0x10 */ struct PtyMoveLevelData *level; /* cost, scope radius and hit data (effects), as
                                    * PtyMove.levels[]; FX code reads (s32)level->effects as the power */
    /* 0x14 */ s16 comboIds[4];    /* combo members, PtyUnit.id each ([0] > 0: a combo; AiComboFindPartner finds [1..3]) */
} BtlAction; /* size >= 0x1C */

/*
 * Action set of a character program (BtlChrData.actionSets[]): up to five
 * actions. AiEnemyChooseCommand picks actions[i] with weight prob[i]
 * (>= 100: always). Two files read actions[] as s32 (battle/n32.h
 * AiActionSet, BTL_ACTION_SETS).
 */
typedef struct BtlActionSet {
    /* 0x00 */ s16 type;           /* condition (AiEnemySelectActionSet); views: s16 x2 */
    /* 0x02 */ s8 prob[6];
    /* 0x08 */ BtlAction *actions[5]; /* skills / items / attacks */
} BtlActionSet; /* size 0x1C */

/*
 * Character/enemy program data (BtlCharDef.data, the loaded GBX program).
 * Only partly known; the size is not. +0x0C and +0x18-+0x22 are an enemy's
 * rewards (BtlDefeatAddExp, BtlEnemyDropRoll, BtlEnemyDrop).
 */
typedef struct BtlChrData {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u8 pad2;
    /* 0x03 */ s8 unk3;
    /* 0x04 */ u8 pad4[2];
    /* 0x06 */ s8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ s8 expSklKind[2];   /* element 1-4 of PtyUnit.expSkl[i] (btlElementExp, menuSkillTypeIcons);
                                    * the enemy gauges (params expSkl) drain per frame when set; one view s8[6] */
    /* 0x0E */ s16 gaugeMax[2];
    /* 0x12 */ s16 gaugeMin[2];
    /* 0x16 */ u8 gaugeDrain;      /* read as (s8) */
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s32 gold[2];        /* small / large gold */
    /* 0x20 */ u8 dropRank;        /* sysDropRankTable index */
    /* 0x21 */ s8 goldLargeRate;   /* percent (minus the killer's getGoldRate) */
    /* 0x22 */ s8 itemRate;        /* percent (minus the killer's getRateUp) */
    /* 0x23 */ u8 pad23[0x80];
    /* 0xA3 */ s8 traceCount;      /* weapon traces (FxTraceStart) */
    /* 0xA4 */ struct PtyTraceDef *traces;
    /* 0xA8 */ u8 padA8[4];
    /* 0xAC */ BtlActionSet *actionSets; /* read as s32 in 6C19E8, 6BE540 (BTL_ACTION_SETS) */
    /* 0xB0 */ u8 padB0;
    /* 0xB1 */ u8 actionSetCount;
    /* 0xB2 */ u8 padB2[0x2A];
    /* 0xDC */ s8 unkDC;           /* enemy: init hook condition (BtlUnitsInit) */
    /* 0xDD */ u8 summonCount;     /* extras summoned at the start (BtlUnitsSetup) */
    /* 0xDE */ u16 summonId;
} BtlChrData; /* size >= 0xE0 */

/*
 * Character definition, 8 x 0x8C (btlCharDefs; btlEnemyDefs = entry 4).
 * Set up by BtlUnitIdsSetup (id) and BtlDataLoad / the character programs at
 * 0x708000-0x740000 (hooks, data, motions).
 */
typedef struct BtlCharDef {
    /* 0x00 */ s32 id;             /* character id 0x1001+ / enemy id (from btlHandoff) */
    /* 0x04 */ s8 slot;            /* def index 0-7; one view calls it id */
    /* 0x05 */ s8 type;            /* start kind (BtlStartUpdate: 0-6 from the id); one view u8 */
    /* 0x06 */ s8 unk6;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ void (*setState)(struct BtlUnit *, s32); /* BtlUnitStateSetHandler; 6BA860 stores *(s32 *)&, 6BB648 calls it with one argument */
    /* 0x0C */ void (*init)(struct BtlUnit *);        /* run by BtlUnitsInit */
    /* 0x10 */ s32 (*attack)(struct BtlUnit *);       /* attack override (BtlUnitStateActAttack, Evade); views: s32 */
    /* 0x14 */ void (*validate)();                    /* BtlUnitCommandValidate hook; views: s32 */
    /* 0x18 */ BtlChrData *data;   /* 15 different views (Party, Res, Enemy, Motion, ...) */
    /* 0x1C */ ChrPackBinding *faceData; /* the unit's character pack binding (btlHandoff models); ChrFaceAnimInit's face table */
    /* 0x20 */ s16 dieParts[2];    /* enemy part ids hidden when the part dies (BtlEnemyPartDieModelCb) */
    /* 0x24 */ s16 dieParts2[2];
    /* 0x28 */ ChrModelData *modelPack; /* FilePacGetModel (6BCA58 stores it as s32) */
    /* 0x2C */ struct GphMotion *motions[BTL_MOTION_COUNT];  /* BtlChrSetMotion */
    /* 0x5C */ s16 *motionEvents[BTL_MOTION_COUNT];   /* key-event lists (BtlUnitMotionEvents); s32 in 6AA770 */
} BtlCharDef; /* size 0x8C */

/* The action a unit (an enemy) is executing. */
#define BTL_UNIT_ACT(u) ((u)->def->data->actionSets[(u)->ai].actions[(u)->action])

extern BtlCharDef btlCharDefs[BTL_CHAR_DEF_COUNT]; /* 0x006F63A8 */
extern BtlCharDef btlEnemyDefs[4];                 /* 0x006F65D8 = &btlCharDefs[4] */

/* ------------------------------------------------------------------ */
/* Units                                                               */
/* ------------------------------------------------------------------ */

#define BTL_UNIT_COUNT  16
#define BTL_PARTY_COUNT 4
#define BTL_ENEMY_COUNT 12

/* BtlUnit.state (n33) */
#define BTL_STATE_NONE        0x00
#define BTL_STATE_VANISH      0x01
#define BTL_STATE_GAME_OVER   0x02
#define BTL_STATE_KO          0x03
#define BTL_STATE_KO2         0x04
#define BTL_STATE_RECOVER     0x40
#define BTL_STATE_HIT_LIGHT   0x41
#define BTL_STATE_HIT_HEAVY   0x42
#define BTL_STATE_GUARD       0x43
#define BTL_STATE_EVADE       0x44
#define BTL_STATE_HIT_NONE    0x45
#define BTL_STATE_PART_WAIT   0x7E
#define BTL_STATE_SCRIPTED    0x7F
#define BTL_STATE_WAIT        0x80
#define BTL_STATE_COMMAND     0x81
#define BTL_STATE_VICTORY     0x82
#define BTL_STATE_VICTORY2    0x83
#define BTL_STATE_DEFEND      0x84
#define BTL_STATE_FOLLOW_ESC  0x85
#define BTL_STATE_EQUIP       0x86
#define BTL_STATE_CAST        0xC0
#define BTL_STATE_ACT         0xC1

/* BtlUnit.action ("ATK NO", n29) */
#define BTL_ACT_COMBO    0
#define BTL_ACT_CRITICAL 1
#define BTL_ACT_ITEM     2
#define BTL_ACT_MAGIC    3
#define BTL_ACT_MOVE     4
#define BTL_ACT_ESCAPE   5
#define BTL_ACT_NONE     0x7F

typedef struct BtlUnit {
    /* 0x000 */ s32 id;             /* unit index 0-15; views: u32 x2 */
    /* motion (BtlChrSetMotion 0x006AA770, BtlUnitMotionEvents 0x006BA938) */
    /* 0x004 */ struct GphMotion *motion;  /* views: void *, s32 */
    /* 0x008 */ f32 motionLast;     /* last frame (frameCount - 1) */
    /* 0x00C */ s16 *evtCur;        /* key-event cursor; views: s32 x6 */
    /* 0x010 */ s16 *evtStart;      /* views: s32, void * */
    /* 0x014 */ s16 *evtLoop;       /* cursor restored at the loop; views: s32 x14, void * */
    /* 0x018 */ f32 frame;          /* "ANM" */
    /* 0x01C */ f32 frameSpeed;     /* "ANM SP" */
    /* 0x020 */ f32 loopStart;      /* "ANM S" */
    /* 0x024 */ f32 loopEnd;        /* "ANM E" */
    /* 0x028 */ s16 loopCount;      /* "ANM LOOP" (< 0 forever) */
    /* 0x02A */ s8 motionNo;        /* "MOT": BtlChrSetMotion index; one view u8 */
    /* 0x02B */ u8 headLook;        /* "H LOOK": bit 0 look at the target; views: s8 x2 */
    /* 0x02C */ s8 wake;            /* "WAKE"; views: u8 x2 */
    /* 0x02D */ s8 unk2D;
    /* 0x02E */ u8 pad2E[2];
    /* 0x030 */ s32 evtIds[5];      /* events found this frame, 0-terminated (BtlUnitMotionEvents) */
    /* 0x044 */ s16 *evtArgs[4];    /* their argument lists */
    /* 0x054 */ ChrFaceAnim face;   /* "FACE ID/MD/C1-C3" (passed to ChrFaceAnim*) */
    /* action state (DbgBtlCharWorkTransformPage) */
    /* 0x060 */ s32 state;          /* "ACT_ID": BTL_STATE_* */
    /* 0x064 */ s32 stateCount;     /* "COUNT": frames in the state */
    /* 0x068 */ s32 phase;          /* "PHASE"; one view u32 */
    /* 0x06C */ s32 subPhase;       /* "SUB"; views: u32 x2 */
    /* 0x070 */ s32 target;         /* "TARGET": unit index */
    /* 0x074 */ s32 attacker;       /* "ATKED": last unit that hit this one */
    /* 0x078 */ s32 flags;          /* "STATE": 4 guard, 0x100, 0x200 alternate motions...; views: u32 x4 */
    /* 0x07C */ f32 knockback;      /* "KB V" */
    /* 0x080 */ s32 run;            /* "RUN" */
    /* attack (DbgBtlCharWorkAttackPage) */
    /* 0x084 */ s32 combo;          /* "COMBO" */
    /* 0x088 */ s32 action;         /* "ATK NO": BTL_ACT_*; views: u32 x2 */
    /* 0x08C */ s32 scopeType;      /* "SCP TYPE": target area (1 single .. 7 unique) + flags; read as u8 in 6C0200/6C0288, f32 in 6D0908 */
    /* 0x090 */ f32 scopeRadius;    /* "SCP R" */
    /* 0x094 */ void *mgcProg;      /* magic program passed to MgcStart (btlMgcTagUnknown when unknown) */
    /* 0x098 */ s32 skillId;        /* "MGC NO": move / spell / item id */
    /* 0x09C */ s8 skillLevel;      /* "MGC LV" */
    /* 0x09D */ s8 ai;              /* "AI": tactic / action set in use; views: u8 x3 */
    /* 0x09E */ s8 eggMagic[6];     /* "EGG MGC" */
    /* 0x0A4 */ s16 eggNo[6];       /* "EGG NO"; 3 reads cast (u16) (6CE660 x2, 6CD660); views: u16[8], u16[10] */
    /* 0x0B0 */ s32 turn;           /* "TURN" */
    /* 0x0B4 */ s32 flag;           /* "FLAG" (s32: most users); one view u32 */
    /* 0x0B8 */ s8 comboLeader;     /* "COMBI L": unit index (btlUnitComboLeader); views: u8 x2 */
    /* 0x0B9 */ s8 comboPartners[3]; /* one view u8[4] */
    /* 0x0BC */ u8 padBC[4];
    /* transform (DbgBtlCharWorkTransformPage) */
    /* 0x0C0 */ FVECTOR pos;        /* "POS" (btlUnitPos); most views f32 x, y, z */
    /* 0x0D0 */ FVECTOR unkD0;      /* motion root translation (BtlChrModelUpdate, party KO) */
    /* 0x0E0 */ FVECTOR moveTarget; /* point walked to */
    /* 0x0F0 */ s32 rot[3];         /* "ANG": 16-bit angles (rot[1] facing) */
    /* 0x0FC */ s32 turnLeft;       /* BtlUnitTurnToTarget */
    /* 0x100 */ FVECTOR scale;      /* "SCL" */
    /* 0x110 */ FVECTOR ambient;    /* "AMB" R, G, B */
    /* 0x120 */ f32 alpha;          /* "ALPHA": fade applied to every part (BtlUnitPreDraw); one view s32 */
    /* 0x124 */ f32 alphaStep;      /* "ALPHA D" */
    /* 0x128 */ f32 unk128;         /* cleared by BtlChrSetMotion */
    /* 0x12C */ f32 unk12C;
    /* 0x130 */ f32 dust;           /* "DUST" */
    /* 0x134 */ s32 rotVel;         /* "MV ROT V" */
    /* 0x138 */ s32 rotAccel;       /* "WT ROT V" */
    /* 0x13C */ s32 rotVelMax;      /* "ROT V MAX" */
    /* IP gauge (DbgBtlCharWorkStatusPage) */
    /* 0x140 */ s32 ipMode;         /* "IP MD" */
    /* 0x144 */ f32 ipPos;          /* "IP VAL"; one view s32 */
    /* 0x148 */ f32 ipSpeed;        /* "IP SPD"; views: s32 x4 (6A1C30, 6BF1B8, 6BF670, 6D4A10) */
    /* 0x14C */ f32 ipDamage;       /* "IP DMG"; one view s32 */
    /* 0x150 */ f32 ipRate;         /* "IP RATE" */
    /* 0x154 */ s32 ipStop;         /* "IP STOP" */
    /* 0x158 */ f32 ipRet;          /* "IP RET" */
    /* 0x15C */ s32 retState;       /* "RET ID": interrupted state (BtlUnitStateResume) */
    /* 0x160 */ s32 retPhase;       /* "RET P" */
    /* 0x164 */ void (*stateHandler)(struct BtlUnit *); /* BtlUnitStateSetHandler; one view s32 */
    /* 0x168 */ void (*hookPre)(struct BtlUnit *);      /* BtlUnitUpdate hooks */
    /* 0x16C */ void (*hookPost)(struct BtlUnit *);
    /* 0x170 */ void (*hookDraw)(struct BtlUnit *);
    /* 0x174 */ void (*partHooks[3])(struct BtlUnit *); /* per part (BtlUnitPreDraw); one view s32[3] */
    /* 0x180 */ u8 pad180[8];
    /* 0x188 */ s32 unk188;
    /* 0x18C */ s32 unk18C;
    /* status counters (DbgBtlCharWorkStatusPage; n30) */
    /* 0x190 */ s16 exeSp;          /* "EXE SP" */
    /* 0x192 */ s16 exeGood;        /* "GOOD" */
    /* 0x194 */ s16 exeBad;         /* "BAD" */
    /* 0x196 */ s16 transp;         /* "TRANSP"; 6D5398 views 0x196-0x1B1 as s16[14] */
    /* 0x198 */ s16 antiMagic;      /* "ANTI M" */
    /* 0x19A */ s16 regene;         /* "REGENE" */
    /* 0x19C */ s16 sleep;          /* "SLEEP" (also paralysis) */
    /* 0x19E */ s16 confu;          /* "CONFU" */
    /* 0x1A0 */ s16 sealS;          /* "SEAL S" */
    /* 0x1A2 */ s16 sealM;          /* "SEAL M" */
    /* 0x1A4 */ s16 bind;           /* "BIND" */
    /* 0x1A6 */ s16 vanish;         /* "VANISH" */
    /* 0x1A8 */ s16 statMod[5];     /* "ATK" "DEF" "MGR" "ACT" "MOV" stat up/down timers */
    /* 0x1B2 */ u16 powUp;          /* "POWUP": stat up/down bits; views: s16 x4 */
    /* 0x1B4 */ s32 wall;           /* "WALL" */
    /* 0x1B8 */ s32 mvp;            /* "MVP" points */
    /* 0x1BC */ s16 exp;            /* "EXP"; one view u16 */
    /* 0x1BE */ s16 item;           /* "ITEM": drop */
    /* 0x1C0 */ f32 itemRate;       /* "ITEM RATE" */
    /* 0x1C4 */ s8 treasure;        /* "TREASURE" */
    /* 0x1C5 */ s8 unk1C5;          /* one view u8 */
    /* 0x1C6 */ s8 parts[3];        /* "PARTS": part units of a multi-part enemy (-1 none) */
    /* 0x1C9 */ s8 body;            /* "BODY": body unit of a part (-1 none); one view u8 */
    /* 0x1CA */ u8 pad1CA[2];
    /* 0x1CC */ s32 partsChr;       /* "PARTS CHR": partner / effect owner unit */
    /* 0x1D0 */ s32 rtAct;          /* "RT ACT" */
    /* 0x1D4 */ s32 rtP;            /* "RT P" */
    /* 0x1D8 */ f32 unk1D8;         /* special wait (BtlUnitStateWait); one view s32 */
    /* 0x1DC */ u8 pad1DC[8];
    /* 0x1E4 */ f32 unk1E4;
    /* 0x1E8 */ u8 pad1E8[4];
    /* 0x1EC */ BtlCharDef *def;    /* ~20 different views */
    /* 0x1F0 */ struct PtyUnit *params;    /* ptyPartyUnits[id]; ~20 different views */
    /* 0x1F4 */ struct GphModel *model;    /* views: s32 x4, void *, Obj * */
    /* 0x1F8 */ u8 pad1F8[8];
} BtlUnit; /* size 0x200 */

/*
 * Codegen notes from switching files to BtlUnit (build/structs/scratch_A3):
 * - flags: 4 views are u32; their mask constants need a u suffix
 *   (u->flags &= ~0x80u in 0x006A1C30, ~1u in 0x006D8E28, ~8u in 0x00682390).
 * - id: 0x006AADA0/0x006AAEE0 compare it unsigned ((u32)u->id < 16).
 * - headLook: read signed in 0x006DC6F0 ((s8)u->headLook).
 * - scopeType: 0x006C0200/0x006C0288 read only its low byte ((u8)u->scopeType).
 * - vanish: 0x00680708 (BtlUpdate) stores it through a one-member union
 *   (alias set 0), which keeps the store after the params->status store.
 * - state: 0x006AF700 reads it as volatile.
 */
extern BtlUnit btlUnits[BTL_UNIT_COUNT];        /* 0x006F6F40 */
extern BtlUnit btlEnemyUnits[BTL_ENEMY_COUNT];  /* 0x006F7740 = &btlUnits[4] */
extern s32 btlUnitArrayTail[4];                 /* 0x006F8F40, right after the array */

/* Aliases into btlUnits[0] that a few files declare (indexed with the 0x200 stride). */
extern u8 btlUnitPos[];                         /* 0x006F7000 = btlUnits[0].pos */

/* Unit ids from the hand-off block (BtlUnitIdsSetup). */
typedef struct BtlUnitIds {
    /* 0x00 */ u16 ids[BTL_UNIT_COUNT];
    /* 0x20 */ s8 flags[BTL_UNIT_COUNT];  /* one view u8 */
} BtlUnitIds; /* size 0x30 */

extern BtlUnitIds btlUnitIds;                   /* 0x006F6E10; one view u16[4] */

/* BtlUnitLunge* */
typedef struct BtlUnitLungeState {
    /* 0x0 */ f32 speed;
    /* 0x4 */ f32 decel;
} BtlUnitLungeState; /* size 0x8 */

extern BtlUnitLungeState btlUnitLunge[BTL_UNIT_COUNT]; /* 0x006F3D60 */

/* ------------------------------------------------------------------ */
/* Scheduler (BtlSchedUpdate 0x006BB358) and unit counters             */
/* ------------------------------------------------------------------ */

/* btlActingUnit special values */
#define BTL_ACTOR_NONE   0xFF
#define BTL_ACTOR_KO     0xFE
#define BTL_ACTOR_EFFECT 0xFC

extern s32 btlActingUnit;          /* 0x006E3F40; char * in 680FC8 (debug) */
extern s32 btlActRequestMask;      /* 0x006F3D44 bit per unit waiting to act (0x10000 effect object) */
extern s32 btlActDoneMask;         /* 0x006F3D48 */
extern s32 btlKoRequestMask;       /* 0x006F3D3C */
extern s32 btlKoPendingMask;       /* 0x006F3D40 */
extern s32 btlTargetMask;          /* 0x006E3F4C; u32 in 6C0288, 6BF848 */
extern s32 btlHitUnitMask;         /* 0x006E3E94 */
extern u8 btlSpecialUsedMask;      /* 0x006E3F0C; s8 in 6800E0 */
extern s8 btlPartyAliveCount;      /* 0x006E3F3C; u8 in 6D9950, 6D7A40 */
extern s8 btlEnemyAliveCount;      /* 0x006E3EFD; u8 in 6D46A8, 6BF1B8, 6D9950 */
extern s8 btlPartyStartCount;      /* 0x006E3F2D */
extern s8 btlEnemyStartCount;      /* 0x006E3EC8 */
extern s8 btlPartyHurtLevel;       /* 0x006E3E90: 0 unhurt, 1 damaged, 2 member KO'd */
extern f32 btlInvPartyAvgSpeed;    /* 0x006E3EDC */
extern f32 btlPartyAvgLevel;       /* 0x006E3F1C */
extern s16 btlLevelGap;            /* 0x006E3E80 */
extern s8 btlReviveHpMode[BTL_UNIT_COUNT];   /* 0x006F6090: 0 full, 1 half, 2 one */
extern s32 btlKnockbackAccum[BTL_UNIT_COUNT]; /* 0x006F5FE8 */
extern s8 btlChrModelUpdated[BTL_UNIT_COUNT]; /* 0x006F27B8 */
extern s32 btlCharDefFinishIndex;  /* 0x006F3D54 */
extern s32 btlCharDefFinishBusy;   /* 0x006F3D58 */
extern s32 btlCharDefsPending;     /* 0x006E3F48 */

#endif /* BATTLE_UNIT_H */
