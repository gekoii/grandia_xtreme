#ifndef BATTLE_N28_H
#define BATTLE_N28_H

#include "common.h"
#include "vu.h"
#include "battle/unit.h"
#include "battle/misc.h"

/*
 * BATTLE 0x00680000-0x00690420 (stage B, n28): screen fade, the unused
 * homing pool, the special-move intro, per-unit part display, enemy drops,
 * the results screen, hit effects, dust, the tactics window.
 * Types from other headers are used by their struct tags (GphModel: gph.h).
 */
struct GphModel;
struct GphMotion;

/* ------------------------------------------------------------------ */
/* Screen colour fade (BtlFade*, 0x006818F0-0x00681B28)                */
/* ------------------------------------------------------------------ */

typedef struct BtlFade {
    /* 0x00 */ s8 flags;           /* BtlFadeStart's first argument */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ s32 t;              /* frames run */
    /* 0x08 */ s32 dur;            /* frames of the fade (0 = idle) */
    /* 0x0C */ SprCol from;
    /* 0x10 */ SprCol to;
    /* 0x14 */ SprColorQuad quad;  /* full screen (640x448); col[0] is the current colour */
} BtlFade; /* size 0x2C */

extern BtlFade btlFade;            /* 0x006E3F58 */

/* ------------------------------------------------------------------ */
/* Homing pool (FxHoming*, 0x00681B28-0x00682390; no callers)          */
/* ------------------------------------------------------------------ */

typedef struct FxHoming {
    /* 0x00 */ s32 state;          /* 0 free (FxHomingSetState writes state and unk4) */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ f32 speed;          /* length of the start vector (VuVecNormalizeLen) */
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s8 unk14;
    /* 0x15 */ u8 pad15[3];
    /* 0x18 */ FVECTOR *target;    /* FxHomingUpdate turns toward *target - *pos */
    /* 0x1C */ FVECTOR *pos;
    /* 0x20 */ FVECTOR dir;        /* normalised direction */
} FxHoming; /* size 0x30 */

typedef struct FxHomingPool {
    /* 0x00 */ s32 count;
    /* 0x04 */ FxHoming *entries;  /* count x 0x30, MemAlloc16C */
    /* 0x08 */ f32 speedMax;       /* 70 */
    /* 0x0C */ f32 speedMin;       /* 4 */
    /* 0x10 */ f32 accel;          /* 1 */
    /* 0x14 */ s32 turnMax;        /* 0x3000 */
    /* 0x18 */ f32 unk18;          /* 100 */
    /* 0x1C */ s32 turnMin;        /* 0x100 */
    /* 0x20 */ s32 turnStep;       /* 0xA00 */
} FxHomingPool; /* size 0x24 */

/* ------------------------------------------------------------------ */
/* Special-move intro (BtlSpecialIntro*, 0x00682390-0x00683A20)        */
/* ------------------------------------------------------------------ */

typedef struct BtlSpecialIntro {
    /* 0x00 */ s32 state;
    /* 0x04 */ s32 timer;
    /* 0x08 */ BtlUnit *unit;      /* unit whose intro plays (NULL none) */
    /* 0x0C */ s8 count;           /* members in the combo (1-4); camera spots depend on it */
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ BtlUnit *cur;       /* partner being shown */
} BtlSpecialIntro; /* size 0x14 */

extern BtlSpecialIntro btlSpecialIntro; /* 0x006E3F88; the symbol files also name .unit
                                         * (btlSpecialIntroUnit) and .count (btlSpecialIntroCount) */

/* ------------------------------------------------------------------ */
/* Per-unit part display (ChrPartDisp*, 0x00683A20-0x00683D10)         */
/* ------------------------------------------------------------------ */

typedef struct ChrPartDispSlot {
    /* 0x00 */ s16 ids[16];        /* part ids (<= 0 free) */
    /* 0x20 */ s8 flags[16];       /* hidden flag passed to BtlChrPartsSetHidden */
} ChrPartDispSlot; /* size 0x30 */

extern ChrPartDispSlot chrPartDispTable[BTL_UNIT_COUNT]; /* 0x006E3FA0 */

/* ------------------------------------------------------------------ */
/* Enemy drops (BtlDrop*, 0x00683D10-0x00685210)                       */
/* ------------------------------------------------------------------ */

/* The enemy rewards are BtlChrData +0x0C/+0x18-+0x22 (battle/unit.h). */

/* One coin of a gold drop. */
typedef struct BtlDropCoin {
    /* 0x00 */ f32 vx;
    /* 0x04 */ f32 vy;
    /* 0x08 */ f32 vz;
    /* 0x0C */ s32 rotVelX;        /* 16-bit angles per frame */
    /* 0x10 */ s32 rotVelY;
    /* 0x14 */ s32 bounced;        /* landing SE played */
    /* 0x18 */ struct GphModel *model;
} BtlDropCoin; /* size 0x1C */

/* Drop object (item model or coins). */
typedef struct BtlDropObj {
    /* 0x00 */ s8 kind;            /* 1-2 gold (coins), >= 3 item model group (SysCommonModelGet), 0 free */
    /* 0x01 */ s8 state;
    /* 0x02 */ s16 timer;
    /* 0x04 */ f32 alpha;
    /* 0x08 */ f32 x;
    /* 0x0C */ f32 y;
    /* 0x10 */ f32 z;
    /* 0x14 */ f32 vx;
    /* 0x18 */ f32 vy;
    /* 0x1C */ f32 vz;
    /* 0x20 */ s32 modelBuf;       /* model pool memory (MemFree) */
    /* 0x24 */ s32 frameStamp;     /* btlFrameCount when the buffers may be freed */
    /* 0x28 */ struct GphModel *model; /* item model */
    /* 0x2C */ s32 drawList;
    /* 0x30 */ BtlDropCoin *coins; /* BtlAlloc32(coinCount * 0x1C) */
    /* 0x34 */ s8 coinCount;
    /* 0x35 */ u8 pad35[3];
} BtlDropObj; /* size 0x38 */

extern BtlDropObj btlDropObjsByUnit[4]; /* 0x006E41C8: indexed by unit id (enemies 4-15 = btlDropObjs) */
extern BtlDropObj btlDropObjs[12];      /* 0x006E42A8 */
extern s32 btlGoldDropCount;            /* 0x006E3EF8 */
extern s32 btlGoldDrops[];               /* 0x006F6E40, up to 0x41 entries (BtlEnemyDrop checks <= 0x40) */

/* ------------------------------------------------------------------ */
/* Results screen (BtlResult*, 0x00685210-0x00688E80)                  */
/* ------------------------------------------------------------------ */

/* Skill level-up effect particle. */
typedef struct BtlResultFxPart {
    /* 0x00 */ s16 rot;            /* u16 in 687C80 */
    /* 0x02 */ s16 rotVel;
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 vx;
    /* 0x10 */ f32 vy;
} BtlResultFxPart; /* size 0x14 */

/* Skill level-up effect (two per member). */
typedef struct BtlResultFx {
    /* 0x000 */ s8 state;          /* -1 off */
    /* 0x001 */ u8 id;             /* s8 in 686248 */
    /* 0x002 */ s16 timer;
    /* 0x004 */ s16 x;
    /* 0x006 */ s16 y;
    /* 0x008 */ BtlResultFxPart parts[32];
} BtlResultFx; /* size 0x288 */

/* Skill points gained per skill. */
typedef struct BtlResultGain {
    /* 0x0 */ s16 value;
    /* 0x2 */ u8 id;               /* ptySkillTable id */
    /* 0x3 */ u8 pad3;
} BtlResultGain; /* size 0x4 */

typedef struct BtlResultCast {
    /* 0x0 */ s8 state;            /* -1 off */
    /* 0x1 */ u8 id;
    /* 0x2 */ s16 timer;
    /* 0x4 */ s32 frame;
} BtlResultCast; /* size 0x8 */

/* "LEVEL UP" pop (BtlResultLevelUpPop). */
typedef struct BtlResultPop {
    /* 0x0 */ f32 t;               /* 1.5 at the start, drawn while > 0 */
    /* 0x4 */ f32 x;
    /* 0x8 */ f32 y;
} BtlResultPop; /* size 0xC */

/* One party member panel. */
typedef struct BtlResultMember {
    /* 0x000 */ s8 active;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ f32 x;
    /* 0x008 */ f32 y;
    /* 0x00C */ f32 unkC;          /* s32 in 686248 (cleared) */
    /* 0x010 */ s32 level;          /* PtyUnit.level before the gain */
    /* 0x014 */ s32 expGain;        /* EXP earned (BtlResultStart) */
    /* 0x018 */ s32 expBonus;       /* bonus EXP (btlResultBonusRates) */
    /* 0x01C */ s32 expShown;       /* counts up to expTarget by expStep */
    /* 0x020 */ s32 expTarget;      /* expGain (+ expBonus once the bonus shows) */
    /* 0x024 */ s32 expStep;        /* BtlResultExpStep(expGain) */
    /* 0x028 */ s32 exp;            /* PtyUnit.exp before the gain */
    /* 0x02C */ s32 nextExp;        /* PtyUnit.nextExp before the gain */
    /* 0x030 */ f32 unk30;         /* s32 in 685360 (cleared) */
    /* 0x034 */ s32 elemExp[4];     /* element EXP earned (btlElementExp share) */
    /* 0x044 */ s32 elemBonus[4];   /* bonus element EXP */
    /* 0x054 */ s32 elemTarget[4];
    /* 0x064 */ s32 elemShown[4];   /* counts up to elemTarget by elemStep */
    /* 0x074 */ s8 skillUps[4];     /* skill levels gained per category; u8 in 685360 */
    /* 0x078 */ s8 elemStep;        /* 1, or 3 when an element gain is >= 100 */
    /* 0x079 */ s8 gainIndex;      /* gains[] filled, then the one shown (-1 none) */
    /* 0x07A */ s8 popNext;        /* next pops[] slot */
    /* 0x07B */ u8 pad7B;
    /* 0x07C */ SprColorQuad quad;   /* panel colour quad */
    /* 0x094 */ TxtGlyphBuf glyphBuf; /* for the skill name */
    /* 0x0A8 */ TxtObj nameText;
    /* 0x0CC */ TxtObj skillText;
    /* 0x0F0 */ BtlResultFx fx[2];
    /* 0x600 */ u8 unk600;         /* flags (0x10 ...) */
    /* 0x601 */ u8 pad601;
    /* 0x602 */ s16 unk602;
    /* 0x604 */ s16 unk604;
    /* 0x606 */ BtlResultGain gains[0x6A];
    /* 0x7AE */ u8 pad7AE[2];
    /* 0x7B0 */ BtlResultCast cast;
    /* 0x7B8 */ BtlResultPop pops[8];
} BtlResultMember; /* size 0x818 */

/* Bonus banner (BtlResultBannerUpdate). */
typedef struct BtlResultBanner {
    /* 0x0 */ s8 kind;             /* btlResultBannerSprites index; u8 in 687580 */
    /* 0x1 */ s8 state;            /* 0 off, 1 drop in, 2 hold, 3 leave */
    /* 0x2 */ s16 timer;
    /* 0x4 */ f32 x;              /* slides in from 1004 */
    /* 0x8 */ f32 vel;
} BtlResultBanner; /* size 0xC */

/* Header animation (BtlResultHeaderUpdate). */
typedef struct BtlResultHeader {
    /* 0x00 */ s8 state;
    /* 0x01 */ u8 pad1;
    /* 0x02 */ s16 timer;          /* -2 when done */
    /* 0x04 */ f32 alpha;
    /* 0x08 */ f32 scaleX;
    /* 0x0C */ f32 scaleY;
    /* 0x10 */ f32 alpha2;
    /* 0x14 */ f32 scaleX2;
    /* 0x18 */ f32 scaleY2;
    /* 0x1C */ f32 slideY;
    /* 0x20 */ f32 slideX;
    /* 0x24 */ s16 slideTimer;
    /* 0x26 */ u8 pad26[2];
} BtlResultHeader; /* size 0x28 */

/* Results work, 0x20C8 (BtlResultLoad allocates it). */
typedef struct BtlResult {
    /* 0x0000 */ u8 state;         /* 0x10 slide in, 0x20 EXP, 0x30 bonus, 0x50 element EXP, 0x60 special bonus, 0x70 wait, 0xF0 exit */
    /* 0x0001 */ u8 pad1[3];
    /* 0x0004 */ s32 timer;
    /* 0x0008 */ s32 unk8;         /* BtlResultStart's argument */
    /* 0x000C */ f32 slideX;       /* gold row x (-320 -> 0) */
    /* 0x0010 */ f32 slideX2;      /* party gold row x (-480 -> 0) */
    /* 0x0014 */ s16 slideTimer;
    /* 0x0016 */ u8 pad16[2];
    /* 0x0018 */ s32 goldGain;     /* btlGoldTotal */
    /* 0x001C */ s32 goldShown;    /* counts up to goldGain (BtlResultCountStep) */
    /* 0x0020 */ s32 goldTotal;    /* goldBase + goldShown */
    /* 0x0024 */ s32 goldBase;     /* party gold before the battle */
    /* 0x0028 */ s32 goldStep;
    /* 0x002C */ s8 goldDigitFrames;
    /* 0x002D */ s8 bonusMember;   /* member with the EXP bonus, 4 all, -1 none */
    /* 0x002E */ s8 specialCount;  /* members that used a special move (btlSpecialUsedMask) */
    /* 0x002F */ s8 bonusTier;     /* btlResultBonusRates index for the EXP bonus */
    /* 0x0030 */ s8 specialTier;   /* btlResultBonusRates index for the element bonus */
    /* 0x0031 */ s8 iconTimer;     /* element icon fade-out (-60 .. 16) */
    /* 0x0032 */ u8 pad32[2];
    /* 0x0034 */ BtlResultMember members[4];
    /* 0x2094 */ BtlResultBanner banner;
    /* 0x20A0 */ BtlResultHeader header;
} BtlResult; /* size 0x20C8 */

extern BtlResult *btlResult;       /* 0x006E4548 */
extern TxtGlyphBuf btlResultGlyphBuf; /* 0x006E4550 */
extern s32 btlResultCountSe;       /* 0x006E4564 SE handle (-1 none) */

/* ------------------------------------------------------------------ */
/* Hit effects (FxHit*, 0x00688E80-0x00689D00)                         */
/* ------------------------------------------------------------------ */

struct FxHitSlot;
typedef void (*FxHitProc)(struct FxHitSlot *);

typedef struct FxHitSlot {
    /* 0x00 */ u8 type;            /* 0x10-0x50 effect kind | hit attr (FxHitSpawn) */
    /* 0x01 */ s8 light;           /* EnvLight id, -1 none */
    /* 0x02 */ s16 state;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 frameStamp;     /* btlFrameCount */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s32 angleX;         /* 16-bit angles toward the attacker */
    /* 0x14 */ s32 angleY;
    /* 0x18 */ s32 modelBuf;       /* MemFree */
    /* 0x1C */ FxHitProc update;   /* FxHitSlotUpdate */
    /* 0x20 */ FxHitProc draw;     /* FxHitSlotDraw */
    /* 0x24 */ s32 unk24;
    /* 0x28 */ struct GphModel *model;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ FVECTOR pos;
} FxHitSlot; /* size 0x40 */

/* Point light colour per hit attr. */
typedef struct FxHitColor {
    /* 0x0 */ f32 r;
    /* 0x4 */ f32 g;
    /* 0x8 */ f32 b;
    /* 0xC */ f32 pad;
} FxHitColor; /* size 0x10 */

extern FxHitSlot fxHitSlots[16];             /* 0x006E4570 */
/* Hit effect models (FilePacGetModel) and motions (GRXSYS chunks). The models
 * are void *: 0x00688E80 FxHitLoad reorders its stores with an s32 type. */
extern void *fxHitFlashModel;                /* 0x006E4970 type 0x40 */
extern struct GphMotion *fxHitFlashMotion;   /* 0x006E4974 */
extern void *fxHitModel;                     /* 0x006E4978 types 0x10/0x20 */
extern struct GphMotion *fxHitMotion;        /* 0x006E497C */
extern void *fxHitSkillModel;                /* 0x006E4980 type 0x50 */
extern struct GphMotion *fxHitSkillMotion;   /* 0x006E4984 */
extern void *fxHitCounterModel;              /* 0x006E4988 type 0x30 */
extern struct GphMotion *fxHitCounterMotion; /* 0x006E498C */
extern FxHitColor fxHitLightColors[10]; /* 0x006DED80 (DbgBtlTraceLightPage edits them) */

/* ------------------------------------------------------------------ */
/* Dust groups (ptclBtlDustGroups entries, PtclBtlDustSpawn)           */
/* ------------------------------------------------------------------ */

/* Ring of dust (kinds 0x80-0x82), 0x1A0. */
typedef struct PtclBtlDustRing {
    /* 0x000 */ s32 kind;
    /* 0x004 */ s32 timer;
    /* 0x008 */ f32 spread;        /* 0 / 0.3 / 0.6 by kind, added to the radius factor */
    /* 0x00C */ f32 vx[32];
    /* 0x08C */ f32 vz[32];
    /* 0x10C */ PtclBtlDust *sprites[32];
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ f32 x;             /* centre, copied as a quadword */
    /* 0x194 */ f32 y;
    /* 0x198 */ f32 z;
    /* 0x19C */ f32 unk19C;
} PtclBtlDustRing; /* size 0x1A0 */

/* Spray of dust (kind 0x83), 0x40. */
typedef struct PtclBtlDustSpray {
    /* 0x00 */ s32 kind;
    /* 0x04 */ s32 timer;
    /* 0x08 */ f32 vx[2];
    /* 0x10 */ f32 vz[2];
    /* 0x18 */ PtclBtlDust *sprites[4];
    /* 0x28 */ u8 pad28[8];
    /* 0x30 */ f32 x;              /* copied as a quadword */
    /* 0x34 */ f32 y;
    /* 0x38 */ f32 z;
    /* 0x3C */ f32 unk3C;
} PtclBtlDustSpray; /* size 0x40 */

/* ------------------------------------------------------------------ */
/* Debug                                                               */
/* ------------------------------------------------------------------ */

extern s8 dbgBtlTraceAttr;         /* 0x006DEC00 "trace attr %d" (-1 off) */

/* ------------------------------------------------------------------ */
/* Tactics window (MenuTactics*, 0x0068E238-0x00690420)                */
/* ------------------------------------------------------------------ */

/* Member tag. */
typedef struct MenuTacticsTag {
    /* 0x0 */ s8 slide;            /* 1 up, 2 down, 0 still */
    /* 0x1 */ s8 timer;
    /* 0x2 */ s8 fade;             /* 1 in, -1 out */
    /* 0x3 */ u8 pad3;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 yFrom;
    /* 0xC */ f32 alpha;
} MenuTacticsTag; /* size 0x10 */

/* Member tactic list. */
typedef struct MenuTacticsList {
    /* 0x0 */ s8 state;            /* -1 off */
    /* 0x1 */ s8 sel;              /* selected tactic */
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ f32 alpha;           /* s32 in 68E588 (cleared) */
} MenuTacticsList; /* size 0x8 */

/* Auto camera panel. */
typedef struct MenuAutoCamPanel {
    /* 0x00 */ s8 state;           /* 0 off, 1 open, 2 closing */
    /* 0x01 */ s8 mode;            /* cursor = btlCamParam.subMode */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ f32 alpha;
    /* 0x08 */ f32 slide;
    /* 0x0C */ f32 slideVel;
    /* 0x10 */ f32 cursorX;
    /* 0x14 */ f32 cursorVel;
    /* 0x18 */ SprColorQuad quad;
} MenuAutoCamPanel; /* size 0x30 */

extern MenuTacticsTag menuTacticsTags[4];   /* 0x006E8368 */
extern MenuTacticsList menuTacticsLists[4]; /* 0x006E83A8 */
extern MenuAutoCamPanel menuAutoCamPanel;   /* 0x006E83C8 */
extern s8 menuTacticsState;        /* 0x006E83F8: 0 closed, 1 set selection, 2 member edit */
extern s8 menuTacticsMember;       /* 0x006E83F9; u8 in 68E588 */
extern s8 menuTacticsSet;          /* 0x006E83FA: 1 A, 2 B */
extern s16 menuTacticsOpenTimer;   /* 0x006E83FC */
extern s16 menuTacticsBannerTimer; /* 0x006E83FE */

#endif /* BATTLE_N28_H */
