#ifndef BATTLE_N29_H
#define BATTLE_N29_H

#include "common.h"
#include "vu.h"
#include "sys.h" /* SprRect, MenuFrame, TxtGlyphBuf, TxtObj */
#include "battle/unit.h"
#include "battle/misc.h"

/*
 * BATTLE menus and attack effects, 0x00690420-0x006A0430 (n29): command
 * sub-lists, unit info window, captions and notices, the menu module, model
 * afterimages and clones, attack effect objects, BTL_CFG.TXT and the command
 * menu. The command menu and sub-list states themselves are in battle/misc.h.
 */

/* ------------------------------------------------------------------ */
/* Command sub-lists (BtlCmdList*, 0x00690420-0x00694F98)              */
/* ------------------------------------------------------------------ */

/*
 * btlCmdList seen from cell k on: BTL_CMD_LIST_CELL_AT(&btlCmdList, k)->cell
 * is btlCmdList.cells[k]. BtlCmdListInput, BtlCmdListMultiCastNext and
 * BtlCmdListComboPartnersShow only match when they address a cell this way
 * (k * 12 added to the list base, the cells' +0x30 folded into the loads).
 */
typedef struct BtlCmdListCellAt {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ BtlCmdListCell cell;
} BtlCmdListCellAt; /* size 0x3C */

#define BTL_CMD_LIST_CELL_AT(p, k) ((BtlCmdListCellAt *)((k) * 12 + (u8 *)(p)))

extern TxtGlyphBuf btlCmdListGlyphBufs[2];   /* 0x006E9208, one per page side */

/* Cell helpers (0x00694D90-0x00694F98). */
void BtlCmdListCellColorSet(SprColorQuad *quad, s32 hue, s32 col, f32 sat);
void BtlCmdListCellSlideIn(BtlCmdListEntry *e, s8 selected);
void BtlCmdListCellSlideOut(BtlCmdListEntry *e);

/* ------------------------------------------------------------------ */
/* Unit info window (BtlUnitInfo*, 0x00694F98-0x00696C30)              */
/* ------------------------------------------------------------------ */

typedef struct BtlUnitInfo {
    /* 0x00 */ BtlUnit *unit;      /* unit shown, NULL closed; BtlUnitInfoOpen takes it as s32 */
    /* 0x04 */ s32 state;          /* btlUnitInfoState: 0 open, 0x10 slide in, 0x20 close; u32 in 695180 */
    /* 0x08 */ s32 timer;
    /* 0x0C */ s32 time;           /* btlFrameCount stamp (text set up one frame later) */
    /* 0x10 */ f32 slideX;         /* name/icon slide-in offset */
    /* 0x14 */ s8 hidden;          /* show btlStrHiddenAction instead of the action */
    /* 0x15 */ s8 skillCount;      /* enemy skill names shown */
    /* 0x16 */ s8 detail;          /* toggled with the button (SE 0x7D5): skill page */
    /* 0x17 */ u8 pad17;
    /* 0x18 */ MenuFrame frame;     /* btlUnitInfoFrame */
    /* 0x54 */ u8 pad54[4];
} BtlUnitInfo; /* size 0x58 (btlUnitInfoGlyphBuf follows) */

extern BtlUnitInfo btlUnitInfo;            /* 0x006E9240 */
extern s32 btlUnitInfoState;               /* 0x006E9244 = btlUnitInfo.state */
extern MenuFrame btlUnitInfoFrame;          /* 0x006E9258 = btlUnitInfo.frame */
extern TxtGlyphBuf btlUnitInfoGlyphBuf;    /* 0x006E9298 name and action */
extern TxtGlyphBuf btlUnitInfoSkillGlyphBuf; /* 0x006E92B0 enemy skill names */
extern SprColorQuad btlUnitInfoGaugeQuads[4];  /* 0x006E92C8 enemy gauge bars */
extern TxtObj btlUnitInfoSkillTexts[6];  /* 0x006E9328 enemy skill names */
extern TxtObj btlUnitInfoNameText;     /* 0x006DF108 */
extern TxtObj btlUnitInfoActionText;   /* 0x006DF130 */
extern SprRect btlStatLabelSprites[];  /* 0x006DF508 HP, MP, SP labels (also the IP panels, 6A9888) */

/* ------------------------------------------------------------------ */
/* Caption and notice windows (BtlCaption*, BtlNotice*)                */
/* ------------------------------------------------------------------ */

/* Stamp dropped on the caption (BtlCaptionLevelUpStamp). */
typedef struct BtlCaptionStamp {
    /* 0x0 */ s8 mode;             /* 1 requested, 2 shown, 3 falling */
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 t;
    /* 0x4 */ f32 vel;
    /* 0x8 */ f32 y;
} BtlCaptionStamp; /* size 0xC */

typedef struct BtlCaption {
    /* 0x00 */ u8 *text;           /* NULL closed */
    /* 0x04 */ s32 state;          /* btlCaptionState: 0x10 fade in, 0x20 close; u32 in 696E40 */
    /* 0x08 */ s32 time;           /* btlFrameCount stamp */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s32 timer;          /* frames left to show (0 = until closed) */
    /* 0x14 */ BtlUnit *unit;      /* icon drawn left of the caption */
    /* 0x18 */ BtlUnit *partners[3]; /* unit->comboPartners */
    /* 0x24 */ MenuFrame frame;
    /* 0x60 */ BtlCaptionStamp stamp;
    /* 0x6C */ u8 pad6C[4];
} BtlCaption; /* size 0x70 (btlNotice follows) */

extern BtlCaption btlCaption;              /* 0x006E9400 */
extern s32 btlCaptionState;                /* 0x006E9404 = btlCaption.state */
extern TxtGlyphBuf btlCaptionGlyphBuf;     /* 0x006E94C0 */
extern TxtObj btlCaptionText;          /* 0x006DF158 */
extern s8 btlCaptionTextStyle;             /* 0x006DF175 = btlCaptionText.spacing */

/* "... Was Learned!" / "... Has Broken!" notice. */
typedef struct BtlNotice {
    /* 0x00 */ u8 *name;           /* NULL closed; the Show functions take it as s32 */
    /* 0x04 */ s8 kind;            /* 0 learned, 1 broken */
    /* 0x05 */ u8 pad5;
    /* 0x06 */ s16 state;          /* 0x10 slide in, 0x13 slide out, 0x20 close */
    /* 0x08 */ s32 time;           /* btlFrameCount stamp */
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s32 timer;
    /* 0x14 */ MenuFrame frame;
} BtlNotice; /* size 0x50 (btlCaptionGlyphBuf follows) */

extern BtlNotice btlNotice;                /* 0x006E9470 */
extern TxtGlyphBuf btlNoticeGlyphBuf;      /* 0x006E94D8 */
extern TxtObj btlNoticeNameText;       /* 0x006DF180 */
extern s8 btlNoticeNameTextStyle;          /* 0x006DF19D = btlNoticeNameText.spacing */
extern TxtObj btlNoticeSuffixText;     /* 0x006DF1A8 */

/* ------------------------------------------------------------------ */
/* Afterimages (FxAfterimage*, 0x00697A98-0x00698568)                  */
/* ------------------------------------------------------------------ */

/* One recorded pose of the unit's model (FxAfterimageUpdate), 0x2C. */
typedef struct FxAfterimagePose {
    /* 0x00 */ f32 pos[3];         /* GphModel.pos */
    /* 0x0C */ s32 rot[3];         /* GphModel.rot (16-bit angles) */
    /* 0x18 */ f32 scale[3];       /* GphModel.scale */
    /* 0x24 */ f32 frame;          /* GphModel.motion[0].frame */
    /* 0x28 */ struct GphMotion *motion; /* GphModel.motion[0].motion */
} FxAfterimagePose; /* size 0x2C */

/*
 * Afterimage state (0x006E9510): copies of one unit's model replayed from a
 * history of its poses (FxAfterimageStart 0x00697B78).
 */
typedef struct FxAfterimage {
    /* 0x00 */ BtlUnit *unit;      /* 0 = off */
    /* 0x04 */ FxAfterimagePose *poses; /* ring buffer [length] (MemAlloc16A) */
    /* 0x08 */ s32 length;
    /* 0x0C */ s32 cur;            /* newest pose */
    /* 0x10 */ s32 lag;            /* poses between the unit and the last copy; grows to length - 1 */
    /* 0x14 */ s32 full;           /* lag reached length - 1 */
    /* 0x18 */ s32 count;          /* copies drawn (<= 8); FxAfterimageStop sets 0, then -1 until freed */
    /* 0x1C */ f32 spread;         /* sideways offset per copy (FxAfterimageSetSpread); s32 in 697B78 (only cleared) */
    /* 0x20 */ f32 color0[4];      /* RGBA of the first copy (FxAfterimageSetColors) */
    /* 0x30 */ f32 color1[4];      /* RGBA of the last copy; [0]/[1] s32 in 697B78 (only cleared) */
    /* 0x40 */ s32 drawLists[8];   /* one per copy */
    /* 0x60 */ s32 frameStamp;     /* btlFrameCount when count reached 0 */
    /* 0x64 */ u8 pad64[0xC];
} FxAfterimage; /* size 0x70 (fxAfterimageSnap follows) */

/* Single tinted snapshot of a unit (FxAfterimageSnapshot 0x00698448), 0x40. */
typedef struct FxAfterimageSnap {
    /* 0x00 */ f32 color[4];       /* GphShadowSetColor colour; alpha fades 8 per frame */
    /* 0x10 */ BtlUnit *unit;
    /* 0x14 */ struct GphMotion *motion; /* 0 = off */
    /* 0x18 */ f32 frame;
    /* 0x1C */ f32 pos[3];
    /* 0x28 */ s32 rot[3];
    /* 0x34 */ f32 scale[3];
} FxAfterimageSnap; /* size 0x40 */

extern FxAfterimage fxAfterimage;          /* 0x006E9510 */
extern FxAfterimageSnap fxAfterimageSnap;  /* 0x006E9580 */
extern s32 fxAfterimageSnapDrawList;       /* 0x006E95C0, sized for the largest unit model */

/* ------------------------------------------------------------------ */
/* Attack effect objects (FxAtk*, 0x00698FB8-0x0069BCC0)               */
/* ------------------------------------------------------------------ */

struct FxAtk;
typedef void (*FxAtkProc)(struct FxAtk *);

/* FxAtk.flags: the low byte is the kind given to FxAtkAlloc. */
#define FXATK_KIND_WAVE         1
#define FXATK_KIND_DELAYED_CAST 2
#define FXATK_KIND_DRAIN_ORB    3
#define FXATK_KIND_MULTI_HIT    4
#define FXATK_KIND_RING         5
#define FXATK_RUN_ALWAYS        0x100 /* keeps updating (and doesn't fade) while another unit acts */
#define FXATK_SYS_DRAW_LIST     0x200 /* model callback draws into btlSysDrawList, else btlFxDrawList */

/* Attack effect object, 32 x 0x30 (fxAtkPool; FxAtkAlloc / FxAtkFree). */
typedef struct FxAtk {
    /* 0x00 */ s32 flags;          /* 0 free; FXATK_* */
    /* 0x04 */ BtlUnit *unit;      /* owner (the effect owner's partner unit when set); views: s32, Chr * */
    /* 0x08 */ FxAtkProc update;   /* FxAtkUpdate */
    /* 0x0C */ FxAtkProc draw;     /* 2D, FxAtkDraw */
    /* 0x10 */ FxAtkProc build;    /* models, FxAtkBuild */
    /* 0x14 */ f32 alpha;          /* fades out while another unit acts */
    /* 0x18 */ void *work;         /* per-kind state (FxAtkMultiHit, FxAtkWave, ...); freed by FxAtkFree */
    /* 0x1C */ s32 pool;           /* model instance memory (BtlModelPoolAlloc / BtlModelCreateWithPool); freed; views: void * */
    /* 0x20 */ void *buf20;        /* freed by FxAtkFree */
    /* 0x24 */ s32 drawList;       /* shadow draw list (FxDelayedCast); freed; views: void * */
    /* 0x28 */ s8 shadowCb;        /* EnvShadowAddCallback slot, -1 none */
    /* 0x29 */ s8 dead;            /* the unit died: freed one frame later */
    /* 0x2A */ s8 actRequested;    /* BtlSchedFxActRequest granted: cancelled with BtlSchedActCancel(0x10) when the unit dies */
    /* 0x2B */ u8 pad2B;
    /* 0x2C */ s32 frameStamp;     /* btlFrameCount when it was marked for freeing */
} FxAtk; /* size 0x30 */

extern FxAtk fxAtkPool[32];        /* 0x006E9628 */
extern s32 fxAtkMultiHitModel;     /* 0x006E9C2C btlSysModelPack model 5 */
extern s32 fxAtkWaveModel;         /* 0x006E9C30 model 6 */
extern s32 fxAtkRingModel;         /* 0x006E9C34 model 7 */

/* FxAtkMultiHit (kind 4) work, 0x24: up to 3 impacts on the target. */
typedef struct FxAtkMultiHit {
    /* 0x00 */ s8 count;           /* impacts (unit params crtclMulti, at most 3) */
    /* 0x01 */ s8 timer;           /* frames to the next impact */
    /* 0x02 */ s8 next;            /* next impact */
    /* 0x03 */ s8 state;
    /* 0x04 */ BtlUnit *target;    /* views: s32 */
    /* 0x08 */ f32 alpha[3];
    /* 0x14 */ struct GphModel *models[3];
    /* 0x20 */ s32 power;          /* FxAtkHitRegister power */
} FxAtkMultiHit; /* size 0x24 */

/* FxAtkWave (kind 1) work, 0x20: wave sent along the unit's facing. */
typedef struct FxAtkWave {
    /* 0x00 */ s16 state;
    /* 0x02 */ s8 delay;           /* echo delay in frames */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ f32 alpha;
    /* 0x08 */ f32 vx;             /* 40 per frame */
    /* 0x0C */ f32 vz;
    /* 0x10 */ f32 radius;         /* unit scopeRadius */
    /* 0x14 */ s32 hitMask;        /* units still to hit */
    /* 0x18 */ struct GphModel *model;
    /* 0x1C */ s32 power;
} FxAtkWave; /* size 0x20 */

/* FxAtkRing (kind 5) work, 0x1C: ring expanding at the target. */
typedef struct FxAtkRing {
    /* 0x00 */ s16 state;
    /* 0x02 */ s8 delay;           /* echo delay in frames */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ f32 alpha;
    /* 0x08 */ f32 radius;         /* full radius */
    /* 0x0C */ f32 t;              /* growth 0..1 (0.05 per frame) */
    /* 0x10 */ s32 hitMask;        /* units still to hit */
    /* 0x14 */ struct GphModel *model;
    /* 0x18 */ s32 power;
} FxAtkRing; /* size 0x1C */

/* FxDelayedCast (kind 2) work, 0x214 (0x220 allocated): a copy of the unit that casts later. */
typedef struct FxDelayedCast {
    /* 0x000 */ BtlUnit unit;      /* copy of the caster, moved to x, z */
    /* 0x200 */ s32 state;
    /* 0x204 */ s32 timer;         /* countdown (300-900 frames), then step frames */
    /* 0x208 */ s32 pulse;         /* pulse angle */
    /* 0x20C */ void *hitData;     /* MgcGetHitData; +0x18 u8 skill kind 0x33-0x36 */
    /* 0x210 */ struct GphModel *model; /* views: s32 */
} FxDelayedCast; /* size 0x214 */

extern s32 fxDelayedCastHide;      /* 0x006E9C28 */
extern s32 fxDelayedCastStep;      /* 0x006E3EBC */

/* Drain orb sparkle, 0x20. */
typedef struct FxDrainSpark {
    /* 0x00 */ s8 age;
    /* 0x01 */ s8 life;            /* 0 free; one view u8 */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ f32 size;
    /* 0x08 */ f32 pos[3];
    /* 0x14 */ f32 vel[3];
} FxDrainSpark; /* size 0x20 */

/* FxDrainOrb (kind 3) work, 0x320 (BtlAlloc32). */
typedef struct FxDrainOrb {
    /* 0x00 */ s8 state;           /* one view u8 */
    /* 0x01 */ s8 next;            /* next sparkle; one view u8 */
    /* 0x02 */ u8 pad2[2];
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ f32 vy;
    /* 0x14 */ f32 speed;
    /* 0x18 */ s32 dir;            /* 16-bit angle */
    /* 0x1C */ s32 amount;         /* applied to the unit on arrival */
    /* 0x20 */ FxDrainSpark sparks[24];
} FxDrainOrb; /* size 0x320 */

/* ------------------------------------------------------------------ */
/* Command menu texts (BtlCmdMenuInit 0x0069C8D8, BtlCmdMenuDraw)      */
/* ------------------------------------------------------------------ */

extern TxtGlyphBuf btlCmdMenuGlyphBuf;       /* 0x006E9D40 glyph buffer for the command names */
extern TxtObj btlCmdMenuNameTexts[8];   /* 0x006E9D58 command names (btlCmdNames) */
extern TxtObj btlCmdMenuDefenseText;    /* 0x006E9E78 "Defense" over move point 0 */

/*
 * BtlUnit.skillLevel as BtlCmdTryLearnMove (0x006A0040) and
 * BtlCmdTryLearnCombo (0x006A01E8) store it: through a u32 8-bit bitfield,
 * which orders the store differently from a plain s8 store.
 */
typedef struct BtlUnitSkillLevelBits {
    u32 v : 8;
} BtlUnitSkillLevelBits;

#endif /* BATTLE_N29_H */
