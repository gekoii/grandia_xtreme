#ifndef BATTLE_MISC_H
#define BATTLE_MISC_H

#include "common.h"
#include "vu.h"
#include "sys.h" /* SprCol, SprColorQuad, TxtObj */
#include "battle/unit.h"

struct Camera;          /* camera.h */
struct GphModel;        /* gph.h */
struct ChrModelData;    /* main character model data */
struct PtyMoveEffect;   /* party.h */
struct PtclPtbHeader;   /* ptcl.h */
struct PtclStarterSlot;

/*
 * BATTLE program state shared by several files: debug settings, phase and
 * frame counters, the command menu and its sub-lists, the magic program
 * manager, weapon traces/trails/clones and the BATTLE particle bursts.
 * (btlHandoff is in sys.h, the units in battle/unit.h.)
 */

/* ------------------------------------------------------------------ */
/* Debug settings (BTL_CFG.TXT, BtlDebugCfgLoad)                       */
/* ------------------------------------------------------------------ */

/* btlDebugCfg.flags */
#define BTL_DBG_DMG_RND    0x1   /* inverted: damage randomness off when set */
#define BTL_DBG_NO_DEATH   0x2
#define BTL_DBG_NO_MPSP    0x4
#define BTL_DBG_DEMO       0x10
#define BTL_DBG_AI         0x20  /* random party AI */
#define BTL_DBG_MGC_GBX    0x40
#define BTL_DBG_NO_FOG_CTR 0x100 /* EnvFogUpdate skips the fog distance */

typedef struct BtlDebugCfg {
    /* 0x00 */ s32 flags;          /* BTL_DBG_*; 11 files declare only this (s32 btlDebugCfg) */
    /* 0x04 */ s32 aiNo;           /* "MC_AI_NO": forced action set */
    /* 0x08 */ s32 atkNo;          /* "MC_ATK_NO": forced command */
    /* 0x0C */ s32 mgcNo;          /* "AI_MGC_NO": forced skill */
    /* 0x10 */ s32 mgcCmb;         /* "AI_MGC_CMB" */
    /* 0x14 */ s32 ipStopMask;     /* "IP STOP" unit bits (btlDbgIpStopMask) */
    /* 0x18 */ s8 dimKind;         /* "dark": forced EnvDimForSkill kind when >= 0 (btlDbgDimKind) */
    /* 0x19 */ s8 se;
    /* 0x1A */ u8 pad1A[2];
} BtlDebugCfg; /* size 0x1C */

extern BtlDebugCfg btlDebugCfg;    /* 0x006F6330 */
extern s32 btlDbgIpStopMask;       /* 0x006F6344 = btlDebugCfg.ipStopMask */
extern s8 btlDbgDimKind;           /* 0x006F6348 = btlDebugCfg.dimKind */
extern s32 btlDbgStage;            /* 0x006FFFFC crash marker */

/* ------------------------------------------------------------------ */
/* Program state                                                       */
/* ------------------------------------------------------------------ */

/* btlPhase */
#define BTL_PHASE_START     0x01
#define BTL_PHASE_RESULTS   0x02
#define BTL_PHASE_FIGHT     0x80
#define BTL_PHASE_VICTORY   0x81
#define BTL_PHASE_ESCAPE    0x82
#define BTL_PHASE_GAME_OVER 0x83

extern s32 btlPhase;               /* 0x006E3ED4 */
extern s32 btlFrameCount;          /* 0x006E3F04 draw frames */
extern s32 btlTickCount;           /* 0x006E3F10 logic ticks; 6A1F90 reads the low byte ((u8) cast) */
extern s32 btlSysSpeed;            /* 0x006E3F00 ticks per frame */
extern s32 btlSlowMotionRate;      /* 0x006E3F24 */
extern s32 btlSlowMotionTimer;     /* 0x006E3EC0 */
extern s32 btlSlowWait;            /* 0x006E3ECC */
extern s32 btlEncounterFlags;      /* 0x006E3EC4 (0x10 boss-type battle) */
extern s32 btlHudHideFlags;        /* 0x006E3ED0 */
extern s32 btlFromField;           /* 0x006E3F30 */
extern s8 btlExitRequest;          /* 0x006E3F2C */
extern s32 btlStartState;          /* 0x006F222C */
extern s32 btlStartTimer;          /* 0x006F2230 */

/*
 * Current BGM handle: stored as a word (SndBgmPlay's result; 6A5B00, 6CB160,
 * 6C98A8 store it as s32) but 9 files read only its low byte (lbu).
 */
typedef union BtlBgmHandle {
    s32 w;
    u8 b;
} BtlBgmHandle;
extern BtlBgmHandle btlBgmHandle;  /* 0x006E3F34 */

/* Tactics */
extern s8 btlAutoTactics;          /* 0x006E3EB8: 0 manual, 1 A, 2 B; u8 in 69BCC0, 680708 */
extern u8 btlTactics[4][2];        /* 0x006E3EE0 per member, sets A/B; s8 in 68E6B0 (matches with an (s8) cast) */
extern s8 btlTacticsLabelsOn;      /* 0x006E3EFC */

/* Results */
extern s32 btlGoldTotal;           /* 0x006E3EEC */
extern s32 btlExpTotal;            /* 0x006E3F28 */
extern s16 btlElementExp[4];       /* 0x006E3E88 */

/* Draw lists, textures and model packs */
extern s32 btlSysDrawList;         /* 0x006E3E9C */
extern s32 btlEnemyDrawList;       /* 0x006E3EA0 */
extern s32 btlFxDrawList;          /* 0x006E3EB0 */
extern s32 btlPartyDrawList;       /* 0x006E3EB4 */
extern s32 btlTexTable;            /* 0x006E3F38 */
extern s32 btlSysModelPack;        /* 0x006E3EF0 */
extern s32 btlCommonModelPack;     /* 0x006E42A0 */
extern s32 btlVoiceTable;          /* 0x006E3F20: voice id pairs, s16 (*)[2] in 6D0908; stored as s32 by BtlDataLoad (cf. envParamTable) */
extern GphScreenFx btlScreenFx;     /* 0x006F6350 */
extern s32 btlChrModelCount;       /* 0x006E3F50 */
extern struct GphModel **btlChrModelListPtr; /* 0x006E3F54; views: void **, s32 * */
extern struct GphModel *btlChrModelList[0x100]; /* 0x006F8F50 */

/* Files and model reservations */
extern s32 btlFileLoadState;       /* 0x006F3C88 */
extern s32 btlFileLoadBuf;         /* 0x006F3C8C */
extern void (*btlFileLoadedCallback)(void); /* 0x006F3C90 */
extern s32 btlModelReserveData[32];  /* 0x006F3C98 */
extern s8 btlModelReserveCount[32];  /* 0x006F3D18; u8 in 6B8E28 */
extern s8 btlModelReserveNum;      /* 0x006F3D38 */
extern s32 btlMotionSeMuteFirst;   /* 0x006F3D4C */
extern s32 btlMotionSeMuteLast;    /* 0x006F3D50 */

/* ------------------------------------------------------------------ */
/* Command menu (BtlCmdMenu*, 0x0069C4C0-0x006A0000)                   */
/* ------------------------------------------------------------------ */

/* Ring camera motion (BtlCmdMenu.ringCam). */
typedef struct BtlCmdRingCam {
    /* 0x00 */ s32 t;
    /* 0x04 */ s32 inputTimer;     /* frames since the last ring input (cleared on input; pulses the ring) */
    /* 0x08 */ f32 angle;
    /* 0x0C */ f32 vel;            /* pending rotation (+-0x2AAA per step, wrapped at 0x10000) */
    /* 0x10 */ f32 tilt;
    /* 0x14 */ s8 dir;
    /* 0x15 */ u8 pad15[3];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
} BtlCmdRingCam; /* size 0x2C */

/* Target area indicator request (BtlCmdMenuAreaShow). */
typedef struct BtlCmdAreaReq {
    /* 0x00 */ s8 active;
    /* 0x01 */ u8 type;            /* area type (circle, line, fan, all) */
    /* 0x02 */ s16 size;           /* views: s16 n1, u16 dur */
    /* 0x04 */ s16 prevSize;       /* views: s16 n2, u16 prev */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ FVECTOR *center;
    /* 0x0C */ f32 scale;          /* s32 in 69FD90 (only cleared there) */
    /* 0x10 */ f32 fade;           /* s32 in 69FD90 (only cleared there) */
} BtlCmdAreaReq; /* size 0x14 */

typedef struct BtlCmdPoint {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 z;
} BtlCmdPoint; /* size 0x8 */

/* Command menu state (0x006E9C40). */
typedef struct BtlCmdMenu {
    /* 0x00 */ s32 state;          /* 0x10 ring, 0x23 tactics, 0x24/26/27 lists, 0x40-0x47 targets, 0 closed */
    /* 0x04 */ s32 timer;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ s8 unk9;
    /* 0x0A */ s8 sel;             /* ring command */
    /* 0x0B */ s8 unkB;
    /* 0x0C */ BtlUnit *unit;      /* acting member */
    /* 0x10 */ u8 flags;
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s8 targetSel;
    /* 0x19 */ s8 targets[16];     /* unit ids, sorted by x (BtlCmdMenuTargetListBuild) */
    /* 0x29 */ s8 targetsUnsorted[16]; /* the same ids in unit order */
    /* 0x39 */ s8 targetCount;
    /* 0x3A */ u8 enabledMask;
    /* 0x3B */ u8 pad3B;
    /* 0x3C */ BtlCmdRingCam ringCam;
    /* 0x68 */ BtlCmdAreaReq area;
    /* 0x7C */ BtlCmdPoint points[6]; /* Defense/Evade move points */
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ s32 unkB0;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ f32 unkB8;
    /* 0xBC */ f32 unkBC;
    /* 0xC0 */ s32 hidden[16];     /* hidden-action rolls per unit */
} BtlCmdMenu; /* size 0x100 */

extern BtlCmdMenu btlCmdMenu;      /* 0x006E9C40 */
extern struct GphModel *btlCmdMenuRingInst;    /* 0x006E9EA0; views: s32, Obj *, Mdl * */
extern s32 btlCmdMenuRingModelBuf;      /* 0x006E9EA4 */
extern s32 btlCmdMenuRingDrawList;      /* 0x006E9EA8 */
extern struct GphModel *btlCmdMenuLineArea;    /* 0x006E9EB0; s32 in 69C4C0 */
extern struct GphModel *btlCmdMenuCircleArea;  /* 0x006E9EB8 */
extern struct GphModel *btlCmdMenuAllArea;     /* 0x006E9EC0 */
extern struct GphModel *btlCmdMenuPoints[6];   /* 0x006E9EC8 */
extern struct GphModel *btlCmdMenuAllyCursor;  /* 0x006E9EE4 */
extern struct GphModel *btlCmdMenuEnemyCursor; /* 0x006E9EEC */
extern s32 btlCmdMenuModelPool;         /* 0x006E9EF0 */
extern s32 btlCmdMenuDrawList;          /* 0x006E9EF4 */
extern struct Camera btlCmdMenuCam;            /* 0x006E9F00 ring camera; u8[] in 69D010, 69D6C8 */

/* ------------------------------------------------------------------ */
/* Command sub-lists: Items / Magic / Moves (BtlCmdList*)              */
/* ------------------------------------------------------------------ */

/* One cell of the 6x6 list. */
typedef struct BtlCmdListCell {
    /* 0x0 */ u8 flags;            /* 4 multi-castable */
    /* 0x1 */ s8 icon;
    /* 0x2 */ u8 level;
    /* 0x3 */ u8 unk3;             /* flag bits (BtlCmdListDraw 6940E8: 1, 8, 0x10, 0x20, 0x40, and per bit;
                                    * spell upgrades in 690BD0) */
    /* 0x4 */ u8 *name;            /* one view void * */
    /* 0x8 */ s16 id;
    /* 0xA */ s16 value;           /* count / cost */
} BtlCmdListCell; /* size 0xC */

/* One drawn cell entry of a page side. */
typedef struct BtlCmdListEntry {
    /* 0x00 */ s8 flags;           /* one view calls it active */
    /* 0x01 */ s8 type;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ s8 playerPage;      /* entry of the player's page side; one view calls it isPlayer */
    /* 0x04 */ BtlCmdListCell *cell; /* one view: Unit * */
    /* 0x08 */ f32 x;
    /* 0x0C */ f32 y;
    /* 0x10 */ f32 unk10[5];       /* slide: start x/y [0..1], target x/y [2..3], offset/step [4] */
    /* 0x24 */ f32 alpha;
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ SprColorQuad quad;    /* colour quad */
    /* 0x44 */ TxtObj texts[4];
} BtlCmdListEntry; /* size 0xD4 */

/* Sub-list state (0x006E85F0). */
typedef struct BtlCmdList {
    /* 0x000 */ BtlUnit *unit;     /* views: Chr *, Owner *, s32 */
    /* 0x004 */ s32 state;         /* btlCmdListState (0xF0 close) */
    /* 0x008 */ s32 timer;
    /* 0x00C */ s8 mode;           /* btlCmdListMode: 1 Items, 2 Magic, 3 Moves */
    /* 0x00D */ s8 row;
    /* 0x00E */ s8 col;
    /* 0x00F */ s8 count;
    /* 0x010 */ s8 rowState[6];    /* views: s8[12], s8[17], s8[32] */
    /* 0x016 */ u8 rowIcon[6];
    /* 0x01C */ s8 unk1C;
    /* 0x01D */ s8 side;
    /* 0x01E */ s8 pageShown[2];   /* btlCmdListPageShown */
    /* 0x020 */ s8 alive;
    /* 0x021 */ s8 helpSide;       /* MenuHelpOpen side (toggled 0/1) */
    /* 0x022 */ s8 unk22;
    /* 0x023 */ u8 unk23;          /* one view s8 */
    /* 0x024 */ f32 titleSlide;    /* btlCmdListTitleSprites offset */
    /* 0x028 */ f32 rightSlide;
    /* 0x02C */ f32 leftSlide;
    /* 0x030 */ BtlCmdListCell cells[36]; /* 6 rows x 6; one view [6][6] */
    /* 0x1E0 */ BtlCmdListEntry entries[2][6]; /* per page side */
    /* 0xBD0 */ TxtObj titles[2];
} BtlCmdList; /* size 0xC18 */

extern BtlCmdList btlCmdList;      /* 0x006E85F0 */
extern s32 btlCmdListState;        /* 0x006E85F4 = btlCmdList.state */
extern s8 btlCmdListMode;          /* 0x006E85FC = btlCmdList.mode */
extern s8 btlCmdListPageShown[2];  /* 0x006E860E = btlCmdList.pageShown */
extern s8 btlCmdListReservedItem[4]; /* 0x006E9230 */
extern f32 btlCmdListX;            /* 0x006E9234 */
extern f32 btlCmdListY;            /* 0x006E9238 */
extern s8 btlCmdListSwapArrowAnim; /* 0x006E923C */
extern u8 btlCmdListComboNg;       /* 0x006E923D */

/* ------------------------------------------------------------------ */
/* Magic / skill effect programs (Mgc*)                                */
/* ------------------------------------------------------------------ */

struct MgcWork;
typedef void (*MgcProc)(struct MgcWork *);

/*
 * Particle group of an effect program (mgcWork.ptclGroups[], added by
 * MgcPtclGroupAdd 0x006B1CF8): the PTB starters it plays and the starter
 * slots they run in. MgcPtclGroupsUpdate (0x006B1770) moves every
 * generator to pos/rot plus the starter's own offset.
 */
typedef struct MgcPtclGroup {
    /* 0x00 */ struct PtclPtbHeader *ptb;  /* starters / controllers (MgcPtclGroupLoad) */
    /* 0x04 */ s32 count;                  /* starters used (<= 16) */
    /* 0x08 */ s32 frame;                  /* starter time; -1 not added, 999999 stopped */
    /* 0x0C */ struct PtclStarterSlot *slots[16]; /* one per starter, NULL until started */
    /* 0x4C */ f32 pos[3];
    /* 0x58 */ s32 rot[3];
} MgcPtclGroup; /* size 0x64 */

typedef struct MgcWork {
    /* 0x000 */ s32 state;
    /* 0x004 */ s32 phase;
    /* 0x008 */ s32 count;
    /* 0x00C */ f32 motionTime;    /* mgcMotionTime; s32 in 6B0F10 */
    /* 0x010 */ s16 id;            /* mgcId */
    /* 0x012 */ s8 part;
    /* 0x013 */ u8 unk13;
    /* 0x014 */ s8 unk14;          /* u8 in 6B0F10 */
    /* 0x015 */ u8 pad15[3];
    /* 0x018 */ BtlUnit *caster;   /* views: Chr *, Chara *, void * */
    /* 0x01C */ struct PtyMoveEffect *hitData; /* mgcHitData: the current hit's effect record in the move / item
                                    * data; MgcSelectHitData (0x006B2818) needs pointer-typed stores and steps it by records */
    /* 0x020 */ MgcProc draw;      /* views: s32, void * */
    /* 0x024 */ MgcProc update;    /* views: s32 x4 */
    /* 0x028 */ MgcProc next;      /* views: s32 */
    /* 0x02C */ MgcProc model;     /* views: s32 */
    /* 0x030 */ MgcProc partStart; /* views: s32 */
    /* 0x034 */ MgcProc nextPart;  /* views: s32 */
    /* 0x038 */ MgcPtclGroup *ptclGroups[256]; /* one view s32[256] (6B0F10) */
    /* 0x438 */ struct ChrModelData *resources[8]; /* particle models (BtlPakGetEntry, MgcTexLoadAll) */
    /* 0x458 */ s32 lightIds[8];   /* mgcLightIds (-1 free) */
    /* 0x478 */ struct ChrModelData *modelData[256];
    /* 0x878 */ struct GphModel **models[256];
    /* 0xC78 */ s32 drawList;
    /* 0xC7C */ s32 textures;
    /* 0xC80 */ s32 ptclSystem;
    /* caster record (MgcSetActorInfo 0x006B2968) */
    /* 0xC84 */ s32 actorId;       /* BtlUnit.id */
    /* 0xC88 */ s32 actorAction;   /* BtlUnit.action */
    /* 0xC8C */ s32 actorSkillId;  /* BtlUnit.skillId */
    /* 0xC90 */ u8 actorBody;      /* BtlUnit.body */
    /* 0xC91 */ u8 padC91[3];
    /* 0xC94 */ s32 actorParams;   /* BtlUnit.params */
    /* 0xC98 */ s32 unkC98;
    /* 0xC9C */ s32 unkC9C;
} MgcWork; /* size 0xCA0 (mgcAbortStep follows) */

extern MgcWork mgcWork;            /* 0x006F2DC8 */
extern f32 mgcMotionTime;          /* 0x006F2DD4 = mgcWork.motionTime */
extern s16 mgcId;                  /* 0x006F2DD8 = mgcWork.id */
extern s32 mgcLightIds[8];         /* 0x006F3220 = mgcWork.lightIds */
extern s32 mgcAbortStep;           /* 0x006F3A68 */
extern s32 mgcPtclModelCount;      /* 0x006F3A6C */
extern s8 mgcSeLoadBusy;           /* 0x006F3A70 */
extern s32 mgcEndFrame;            /* 0x006F3A74 */

/* ------------------------------------------------------------------ */
/* Weapon traces, trails and clones (Fx*, 0x0068AD00-0x00698E10)       */
/* ------------------------------------------------------------------ */

/* Trace colour kind ("ST"/"ED" on the TRACE page). */
typedef struct FxTraceColor {
    /* 0x0 */ SprCol start;       /* views: s32, u8[8] for both */
    /* 0x4 */ SprCol end;
} FxTraceColor; /* size 0x8 */

extern FxTraceColor fxTraceColors[11]; /* 0x006E7C10 */

struct FxTraceOfs;
struct PtclBtlSpr;       /* battle/n30.h */
struct PtclDebris;
struct GphAnimSet;       /* gph.h */

/* Trace strip: point ring buffer (0x20). */
typedef struct FxTraceStrip {
    /* 0x00 */ s32 cur;
    /* 0x04 */ s32 count;
    /* 0x08 */ SprCol c0;         /* u8 in 68B1B0 */
    /* 0x0C */ SprCol c1;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s8 unk14;
    /* 0x15 */ s8 unk15;
    /* 0x16 */ s8 unk16;
    /* 0x17 */ s8 unk17;
    /* 0x18 */ s8 unk18;
    /* 0x19 */ u8 pad19[3];
    /* 0x1C */ struct FxTraceOfs *buf; /* point pairs (count of them); s32 in 3 files */
} FxTraceStrip; /* size 0x20 */

extern FxTraceStrip fxTraceStrips[16]; /* 0x006E6210 */

/* Spark emitter attached to a trace or trail (0x10). */
typedef struct FxTraceSpark {
    /* 0x0 */ s8 count;
    /* 0x1 */ s8 frame;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ s8 state;            /* u8 in 68C2B8, 68D0D8 */
    /* 0x5 */ s8 unk5;
    /* 0x6 */ u8 pad6[2];
    /* 0x8 */ s32 time;
    /* 0xC */ struct PtclBtlSpr *handle; /* sprite buffer (count of them, battle/n30.h) */
} FxTraceSpark; /* size 0x10 */

extern FxTraceSpark fxTraceSparks[16]; /* 0x006E6410 */

typedef struct FxTracePart {
    /* 0x0 */ s32 part0;
    /* 0x4 */ s32 part1;
} FxTracePart; /* size 0x8 */

typedef struct FxTraceOfs {
    /* 0x00 */ FVECTOR a;
    /* 0x10 */ FVECTOR b;
} FxTraceOfs; /* size 0x20 */

/* Per-unit trace state (FxTraceStart 0x0068C320), 16 x 0x170. */
typedef struct FxTraceUnit {
    /* 0x000 */ struct GphMotion *motion; /* unit motion when started (0 = free); views: s32 x4, void * */
    /* 0x004 */ f32 frame;
    /* 0x008 */ f32 frameSpeed;
    /* 0x00C */ f32 unkC;          /* the unit's unk128 / unk12C when the trace started (FxTraceUnitUpdate) */
    /* 0x010 */ f32 unk10;
    /* 0x014 */ f32 unk14;
    /* 0x018 */ f32 t;
    /* 0x01C */ f32 step;
    /* 0x020 */ s16 width;
    /* 0x022 */ s16 height;         /* step count */
    /* 0x024 */ s8 flags[6];       /* u8 in 68C7E8, 68C890, 68AD00 */
    /* 0x02A */ u8 kinds[6];       /* fxTraceColors index */
    /* 0x030 */ s32 strips[6];
    /* 0x048 */ FxTracePart parts[6];
    /* 0x078 */ u8 pad78[8];
    /* 0x080 */ FxTraceOfs ofs[6];
    /* 0x140 */ FVECTOR pos;       /* unit pos/rot/scale copied as quadwords */
    /* 0x150 */ IVECTOR rot;
    /* 0x160 */ FVECTOR scale;
} FxTraceUnit; /* size 0x170 */

extern FxTraceUnit fxTraceUnits[16]; /* 0x006E6510 */

/* Free trail created by effect programs (0x38). */
typedef struct FxTrail {
    /* 0x00 */ FxTraceOfs *handle; /* point ring (BtlAlloc32, unk1A entries) */
    /* 0x04 */ struct BtlUnit *owner; /* trail stays lit while its flags & 8 */
    /* 0x08 */ f32 *unk8;          /* point followed (copied into the ring each frame) */
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 *unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 unk1C;
    /* 0x1E */ u16 unk1E;          /* s16 in 68CF20, 68D3C8 */
    /* 0x20 */ s8 unk20;
    /* 0x21 */ s8 unk21;
    /* 0x22 */ u8 pad22[2];
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;          /* s32 in 68CF20, 68D3C8, 68D580 */
    /* 0x30 */ SprCol c0;         /* s32 in 5 files */
    /* 0x34 */ SprCol c1;
} FxTrail; /* size 0x38 */

extern FxTrail fxTrails[32];       /* 0x006E7C68 */

/* Clone group (FxClone*): afterimage copies of one unit. */
typedef struct FxCloneEnt {
    /* 0x0 */ struct GphModel *model;     /* BtlModelCreate; views: s32, Part *, Mdl * */
    /* 0x4 */ f32 alpha;
    /* 0x8 */ f32 target;
    /* 0xC */ f32 step;
} FxCloneEnt; /* size 0x10 */

typedef struct FxClone {
    /* 0x00 */ BtlUnit *unit;      /* views: s32 x7, Chr * */
    /* 0x04 */ FxCloneEnt clones[3];
    /* 0x34 */ s32 pool;           /* BtlModelPoolAlloc */
    /* 0x38 */ s32 drawList;       /* all clones */
    /* 0x3C */ s32 drawLists[3];   /* per clone */
    /* 0x48 */ s8 shadowCb;        /* shadow callback slot, -1 none */
    /* 0x49 */ s8 count;
    /* 0x4A */ s8 circleMode;      /* 1: clones circle (cx, cz) */
    /* 0x4B */ u8 pad4B;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ u8 pad4E[2];
    /* 0x50 */ s32 frameStamp;     /* btlFrameCount of the last update */
    /* 0x54 */ f32 cx;             /* circle centre */
    /* 0x58 */ f32 cz;
    /* 0x5C */ u8 pad5C[4];
} FxClone; /* size 0x60 (fxAtkPool follows) */

extern FxClone fxClone;            /* 0x006E95C8 */

/* ------------------------------------------------------------------ */
/* BATTLE particles (PtclBurst*, PtclBtlDust*)                         */
/* ------------------------------------------------------------------ */

/* Particle type, 0x50 (also weapon trace sparks). */
typedef struct PtclBurstType {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ u8 tex;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ struct GphAnimSet **animSet; /* one of ptclAnimSetTex38.. (battle/n30.h); views: PSub *, s32 * */
    /* 0x10 */ f32 scale[4];       /* scale keys */
    /* 0x20 */ s16 scaleT0;
    /* 0x22 */ s16 scaleT1;
    /* 0x24 */ f32 scaleRate[3];
    /* 0x30 */ u8 color[4][4];     /* colour keys */
    /* 0x40 */ s16 colorT0;
    /* 0x42 */ s16 colorT1;
    /* 0x44 */ f32 colorRate[3];
} PtclBurstType; /* size 0x50 */

/* const is needed: 0x0068C178 and 0x006ACBA8 only match with a const table (load scheduling). */
extern const PtclBurstType ptclBurstTypeTable[17]; /* 0x006DF5E0 */

typedef void (*PtclBurstCb)(void *);

/* Particle burst slot, 0x50. */
typedef struct PtclBurst {
    /* 0x00 */ s8 type;
    /* 0x01 */ u8 texId;           /* GphTexBind */
    /* 0x02 */ s8 active;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ u16 unk4;           /* s16 in 6AB540 */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s16 count;
    /* 0x0E */ s16 max;
    /* 0x10 */ f32 timer;
    /* 0x14 */ f32 rate;
    /* 0x18 */ void *parts;        /* PtclBtlSpr[max], or PtclDebris[8] for type 10 (battle/n30.h); s32 in 4 files */
    /* 0x1C */ s16 count2;
    /* 0x1E */ s16 max2;
    /* 0x20 */ f32 timer2;
    /* 0x24 */ f32 rate2;
    /* 0x28 */ struct PtclBtlSpr *parts2; /* glow sprites; s32 in 4 files */
    /* 0x2C */ void *unk2C;        /* s32 in 4 files */
    /* 0x30 */ PtclBurstCb cb30;   /* s32 in 6AB458, void * in 6AB540 */
    /* 0x34 */ PtclBurstCb cb34;
    /* 0x38 */ PtclBurstCb cb38;
    /* 0x3C */ void *unk3C;
    /* 0x40 */ f32 ofs[4];
} PtclBurst; /* size 0x50 */

extern PtclBurst ptclBursts[16];   /* 0x006F27D0 */

/* Dust sprite, 0x30. */
typedef struct PtclBtlDust {
    /* 0x00 */ f32 unk0;           /* s32 in 689D00 */
    /* 0x04 */ f32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;          /* velocity (PtclBtlDustSpawn) */
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ u8 pad2C[4];
} PtclBtlDust; /* size 0x30 */

extern PtclBtlDust ptclBtlDustPool[128]; /* 0x006E4990 */
extern void *ptclBtlDustGroups[32];      /* 0x006E6190; views: s32, s32 *, Eff * */

#endif /* BATTLE_MISC_H */
