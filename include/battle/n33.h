#ifndef BATTLE_N33_H
#define BATTLE_N33_H

#include "common.h"
#include "vu.h"
#include "sys.h" /* SprRect, SprRectListEntry, MenuNumField */
#include "battle/unit.h"

/*
 * BATTLE 0x006C6218-0x006D9FE8: unit movement, the unit state handlers,
 * the battle HUD (damage numbers, per-unit gauges, ranked combo popups,
 * status/stat-change icons, markers), the hit queue and damage resolution,
 * and the victory sequence. The units are in battle/unit.h; this header has
 * the state shared by several of these files.
 */

struct PtyUnit;          /* party.h */
struct PtyMoveEffect;    /* party.h */
struct PtyMoveLevelData; /* party.h */
struct GphModel;         /* gph.h */

/* Segment on the ground plane (BtlSegIntersect 0x006C6218, BtlCircleSegIntersect 0x006C6460). */
typedef struct BtlSeg {
    f32 x0, z0;
    f32 x1, z1;
} BtlSeg; /* size 0x10 */

/* ------------------------------------------------------------------ */
/* Unit movement (0x006C7620-0x006C8720)                               */
/* ------------------------------------------------------------------ */

/*
 * Work of BtlPathAvoidHeading (0x006C7918, a local) and
 * BtlPathBlockedArcExtend (0x006C76F8): the arc of headings blocked by
 * other units around the direction to the destination.
 */
typedef struct BtlPathCtx {
    /* 0x00 */ u8 flags[BTL_UNIT_COUNT];  /* per unit: 1 blocks the way */
    /* 0x10 */ u8 save[BTL_UNIT_COUNT];
    /* 0x20 */ f32 x;              /* mover position */
    /* 0x24 */ f32 z;
    /* 0x28 */ f32 r;              /* mover collision radius */
    /* 0x2C */ s32 ang;            /* heading to the destination */
    /* 0x30 */ s32 lo;             /* blocked arc, relative to ang (16-bit angles) */
    /* 0x34 */ s32 hi;
} BtlPathCtx; /* size 0x38 */

/* ------------------------------------------------------------------ */
/* HUD (BtlHud* 0x006D1518-0x006D44C8, cleared by BtlHudDmgInit)       */
/* ------------------------------------------------------------------ */

/* BtlHudDamageNum.flags */
#define BTL_DMG_CANCEL        0x1   /* btlHudCancelLabel */
#define BTL_DMG_CRITICAL      0x2   /* btlHudCriticalLabel */
#define BTL_DMG_CANCEL_RESIST 0x8   /* btlHudCancelResistLabel */
#define BTL_DMG_MP            0x40  /* "MP" suffix */
#define BTL_DMG_SP            0x80  /* "SP" suffix */

/* Floating damage number (BtlHudDamageNumSpawn 0x006D4100, update 0x006D2140, draw 0x006D2548). */
typedef struct BtlHudDamageNum {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 flags;          /* BTL_DMG_*; 0x10 / 0x20 pick the motion */
    /* 0x08 */ s32 timer;          /* frames shown (off after 0x40) */
    /* 0x0C */ s32 value;
    /* 0x10 */ FVECTOR pos;        /* world position */
    /* 0x20 */ FVECTOR scr;        /* screen position (UtilWorldToScreen) */
    /* 0x30 */ f32 scale;
    /* 0x34 */ f32 dy;
    /* 0x38 */ f32 dx;
    /* 0x3C */ f32 stackY;         /* pushed up while it overlaps older numbers */
} BtlHudDamageNum; /* size 0x40 */

/* Per-unit hit counter / HP gauge (BtlHudGaugeAddHit 0x006D3F60, BtlHudGaugeUpdate, BtlHudGaugeDraw). */
typedef struct BtlHudUnitGauge {
    /* 0x00 */ s32 hits;           /* hit count of the chain; -1 off, 0 HP bar only */
    /* 0x04 */ s32 damage;         /* damage of the chain */
    /* 0x08 */ s32 hpShown;        /* HP before the chain (the bar's lost part) */
    /* 0x0C */ f32 popIn;          /* 0 -> 1 when the counter appears */
    /* 0x10 */ f32 scale;          /* counter size (grows at 5/10/20/30/50 hits) */
    /* 0x14 */ f32 flash;          /* 1 on a hit, fades out */
    /* 0x18 */ f32 barPos;         /* HP bar position, eased toward the HP */
    /* 0x1C */ s32 shake;
    /* 0x20 */ s32 timeout;        /* frames the HP bar stays after the chain */
} BtlHudUnitGauge; /* size 0x24 */

/* Ranked combo popup (BtlHudComboPopupAdd 0x006D3C68, update 0x006D1768, draw 0x006D1920-0x006D1AE8). */
typedef struct BtlHudComboPopup {
    /* 0x00 */ s32 rank;           /* 1 = top row, 0 free */
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 destX;
    /* 0x10 */ f32 destY;
    /* 0x14 */ f32 startX;
    /* 0x18 */ f32 startY;
    /* 0x1C */ s32 timer;          /* removed after 0x80 frames */
    /* 0x20 */ s32 hits;
    /* 0x24 */ s32 damage;
    /* 0x28 */ s32 unit;           /* unit index */
    /* 0x2C */ s32 icon;           /* PtyUnit.icon */
    /* 0x30 */ s16 hitsWidth;      /* MenuNumberWidth of hits / damage */
    /* 0x32 */ s16 damageWidth;
} BtlHudComboPopup; /* size 0x34 */

/* Status icons shown over a unit (BtlHudStatusIconShow 0x006D4228). */
typedef struct BtlHudStatusIcon {
    /* 0x0 */ s32 flags;           /* status bits; icon n shown while bit (0x10 << n) is set */
    /* 0x4 */ s16 index;           /* icon shown (0-11) */
    /* 0x6 */ s16 timer;
} BtlHudStatusIcon; /* size 0x8 */

/* Parameter up/down display (BtlHudStatChangeShow 0x006D41E8). */
typedef struct BtlHudStatChange {
    /* 0x0 */ u16 bits;            /* parameters changed (ATK DEF MGR ACT MOV) */
    /* 0x2 */ u16 powUp;           /* BtlUnit.powUp: up bits, down bits << 8 */
    /* 0x4 */ s16 index;           /* parameter shown (0-4) */
    /* 0x6 */ s16 timer;
} BtlHudStatChange; /* size 0x8 */

/* BtlHudMarker.type */
#define BTL_MARKER_EVADE      1
#define BTL_MARKER_MISS       2
#define BTL_MARKER_CLEAR_MISS 3     /* BtlHudMarkerSpawn: removes the unit's miss marker */
#define BTL_MARKER_RESIST     4
#define BTL_MARKER_LEVEL_DOWN 5
#define BTL_MARKER_FAIL       6

/* Marker over a unit (BtlHudMarkerSpawn 0x006D4260, BtlHudMarkerUpdate 0x006D2FB0), ring of 32. */
typedef struct BtlHudMarker {
    /* 0x00 */ s8 type;            /* BTL_MARKER_*, 0 free */
    /* 0x01 */ s8 state;
    /* 0x02 */ s16 timer;
    /* 0x04 */ BtlUnit *unit;
    /* 0x08 */ f32 x;              /* unit position when spawned */
    /* 0x0C */ f32 z;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
} BtlHudMarker; /* size 0x18 */

#define BTL_HUD_DAMAGE_NUM_COUNT 32
#define BTL_HUD_POPUP_COUNT      12
#define BTL_HUD_MARKER_COUNT     32

extern BtlHudDamageNum btlHudDamageNums[BTL_HUD_DAMAGE_NUM_COUNT];   /* 0x006F3EB0 */
extern BtlHudUnitGauge btlHudUnitGauges[BTL_UNIT_COUNT];             /* 0x006F46B0 */
extern BtlHudComboPopup btlHudEnemyComboPopups[BTL_HUD_POPUP_COUNT]; /* 0x006F48F0 (left side) */
extern BtlHudComboPopup btlHudPartyComboPopups[BTL_HUD_POPUP_COUNT]; /* 0x006F4B60 (right side) */
extern BtlHudStatusIcon btlHudStatusIcons[BTL_UNIT_COUNT];           /* 0x006F4DD0 */
extern BtlHudStatChange btlHudStatChanges[BTL_UNIT_COUNT];           /* 0x006F4E50 */
extern BtlHudMarker btlHudMarkers[BTL_HUD_MARKER_COUNT];             /* 0x006F4ED0 */
extern s8 btlHudMarkerNext;                                          /* 0x006F51D0 next ring slot */

/* HUD sprite data (rodata / data) */
extern SprRect btlHudHpBarRects[];        /* 0x006DF448 frame, lost part, remaining part (patched per draw) */
extern MenuNumField btlHudPopupDamageNumFmt;  /* 0x006DF470 */
extern MenuNumField btlHudPopupHitsNumFmt;    /* 0x006DF488 */
extern MenuNumField btlHudGaugeHitsNumFmt;    /* 0x006DF4A0 */
extern MenuNumField btlHudGaugeBonusNumFmt;   /* 0x006DF4B8 */
extern MenuNumField btlHudDamageNumFmt;       /* 0x006DF4D0 */
extern SprRect btlHudPopupDamageRect;     /* 0x006DF5C0 */
extern const SprRect btlHudMarkerRects[]; /* 0x006E2498 (3 resist, 4 level down, 7 fail) */
extern SprRect btlHudMpSuffixRect;        /* 0x006E24D4 */
extern SprRect btlHudSpSuffixRect;        /* 0x006E24E0 */
extern SprRectListEntry btlHudCriticalLabel[];     /* 0x006E24F8 */
extern SprRectListEntry btlHudCancelLabel[];       /* 0x006E2508 */
extern SprRectListEntry btlHudCancelResistLabel[]; /* 0x006E2518 */
extern SprRect btlHudHitsRect;            /* 0x006E25AC "HITS" */
extern SprRect btlHudGaugeRects[];        /* 0x006E25C8 */
extern SprRect btlHudStatChangeRects[];   /* 0x006E2678 labels [0-4], up [5], down [6], normal [7] */
extern SprRectListEntry btlHudEvadeLabel[];        /* 0x006E26E8 */
extern SprRectListEntry btlHudMissLabel[];         /* 0x006E2708 */

/* ------------------------------------------------------------------ */
/* Hit queue (BtlHitQueueAdd 0x006D4DE0, BtlHitQueueProcess 0x006D4568, */
/* BtlHitResolve 0x006D5398)                                           */
/* ------------------------------------------------------------------ */

/* BtlHit.flags */
#define BTL_HIT_PARTY_ATTACK 0x10000    /* party unit's combo / critical */
#define BTL_HIT_COMBO_FINISH 0x20000
#define BTL_HIT_DATA_FLAG2   0x100000   /* the hit data's flag 2 (PtyMoveEffect +1) */
#define BTL_HIT_SRC_POS      0x200000   /* srcPos is set */

/* Attacker fields of a hit, filled by BtlHitQueueAddAttack (0x006D4C60). */
typedef struct BtlHitSource {
    /* 0x00 */ s32 unit;           /* attacker unit index */
    /* 0x04 */ s32 action;         /* BTL_ACT_* */
    /* 0x08 */ s32 skillId;
    /* 0x0C */ s8 body;            /* BtlUnit.body; u8 in BtlHitQueueAdd* views */
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ struct PtyUnit *params;
    /* 0x14 */ f32 statusProb;     /* PtyUnit.eqpStaProb for plain attacks */
    /* 0x18 */ s32 status;         /* PtyUnit.eqpBitSta for plain attacks */
} BtlHitSource; /* size 0x1C */

/* Queued hit, 32 x 0x70. */
typedef struct BtlHit {
    /* 0x00 */ BtlUnit *target;
    /* 0x04 */ struct PtyMoveEffect *effect; /* hit data (move level effect / MgcGetHitData) */
    /* 0x08 */ s32 angle;          /* direction of the hit (knock-back) */
    /* 0x0C */ s32 flags;          /* BTL_HIT_* | btlHitNextFlags */
    /* 0x10 */ BtlHitSource src;   /* attacker; BtlHitDrainHp takes &hit->src */
    /* 0x2C */ void (*victimHandler)(BtlUnit *); /* btlHitNextVictimHandler */
    /* 0x30 */ void (*callback)(s32, s32, BtlUnit *); /* (arg, BtlHitResolve result, target) */
    /* 0x34 */ s32 callbackArg;
    /* 0x38 */ u8 pad38[8];
    /* 0x40 */ FVECTOR srcPos;     /* btlHitNextSrcPos (BTL_HIT_SRC_POS) */
    /* 0x50 */ s32 damage;         /* BtlHitQueueAddDamage */
    /* 0x54 */ s32 status;
    /* 0x58 */ s8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ f32 unk5C;
    /* 0x60 */ s32 unk60;
    /* 0x64 */ u8 pad64[0xC];
} BtlHit; /* size 0x70 */

/*
 * Outcome of one hit, a local of BtlHitResolve (0x006D5398) filled by
 * BtlHitCalcDamage (0x006D7A40) and passed to BtlHitApplyAilment,
 * BtlHitApplyStatChange and BtlHitShowEffect (0x006D8A68).
 */
typedef struct BtlHitResult {
    /* 0x00 */ s32 damage;
    /* 0x04 */ s32 reaction;       /* state the target is put in (BTL_STATE_HIT_LIGHT/HEAVY/NONE ...) */
    /* 0x08 */ s32 ipDelta;        /* added to BtlUnit.ipPos / 10000 when < 0 */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ f32 knockback;      /* BtlUnitHitReact */
    /* 0x14 */ s32 flags;          /* 4 no damage number, 0x800 no gauge, 0x8000 combo popup ... */
    /* 0x18 */ s32 effects;        /* what happened (0x8000 KO, 6 / 0x70 / 0x700 ... MVP classes) */
    /* 0x1C */ s32 status;         /* ailment bits inflicted */
    /* 0x20 */ f32 power;          /* ailment / stat-change strength */
    /* 0x24 */ s8 unk24;
    /* 0x25 */ s8 hitAttr;         /* hit effect kind (BtlHitShowEffect; dbgBtlHitAttr overrides) */
    /* 0x26 */ u8 pad26[2];
} BtlHitResult; /* size 0x28 */

#define BTL_HIT_QUEUE_COUNT 32

extern BtlHit btlHitQueue[BTL_HIT_QUEUE_COUNT];  /* 0x006F51E0 */
extern s32 btlHitQueueCount;                     /* 0x006F5FE0 */
extern s32 btlHitDamageAccum[BTL_UNIT_COUNT];    /* 0x006F6030 per unit, summed over a multi-hit (-1 none) */

/* Parameters of the next queued hit (BtlHitSet* 0x006D9EC8-0x006D9F60, BtlHitNextClear 0x006D9FC0). */
extern void (*btlHitNextVictimHandler)(BtlUnit *); /* 0x006F602C state handler the hit gives its target
                                                    * (stateHandler, state 0x7F); s32 in the setters */
extern FVECTOR btlHitNextSrcPos;                 /* 0x006F6070; w > 0 when set */
extern s32 btlHitNextSrcPosSet;                  /* 0x006F607C = btlHitNextSrcPos[3] (stored as an int 0) */
extern void (*btlHitNextCallback)(s32, s32, BtlUnit *); /* 0x006F6080; s32 in the setters */
extern s32 btlHitNextCallbackArg;                /* 0x006F6084 */
extern s32 btlHitNextFlags;                      /* 0x006F6088 */

/* ------------------------------------------------------------------ */
/* Victory sequence (BtlUnitStateVictory 0x006C98A8)                   */
/* ------------------------------------------------------------------ */

/* Victory motion pack (BtlVictoryMotionLoad 0x006C9DE0). */
typedef struct BtlVictoryWork {
    /* 0x00 */ s32 motionData;     /* relocated pack entry 0 */
    /* 0x04 */ s32 motionKeys;     /* pack entry 1 */
    /* 0x08 */ s32 motionEnd;      /* btlVictoryMotionEnd: end frame (camera blend length) */
    /* 0x0C */ s32 frame;
    /* 0x10 */ s32 state;          /* load state: 1 loading, 0 done */
    /* 0x14 */ s32 pak;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ struct GphModel *prop; /* btlVictoryPropModel: prop of the pose (character 0x1006) */
} BtlVictoryWork; /* size 0x20 */

extern BtlVictoryWork btlVictoryWork;            /* 0x006F3E88 */
extern s32 btlVictoryLeader;                     /* 0x006F6028 party member who struck the last enemy */

/* ------------------------------------------------------------------ */
/* Actions                                                             */
/* ------------------------------------------------------------------ */

extern s8 btlActItemUsedUp;                      /* 0x006F3EA8 the action's item was used up or broke */
extern u8 btlMgcTagUnknown[];                    /* 0x006E2078 "?": BtlUnit.mgcProg of specials without a program */

/*
 * Codegen notes from switching the files (build/structs/stageB/scratch_n33):
 * - btlHitNextVictimHandler / BtlHit.victimHandler: BtlHitQueueAddDamage
 *   (0x006D5058) and BtlHitQueueAddStatus (0x006D51E0) only match with them
 *   outside int's alias set (their old views used an enum); the real type,
 *   a state handler pointer, does that. The setters take and return s32.
 * - BtlHitQueueAddStatus reads the target's rot[1] through an enum type.
 * - BtlHitSource.body is read signed by BtlHitResolve (0x006D5398).
 * - btlHudMarkerRects must be const (BtlHudMarkerResistDraw 0x006D3570).
 * - PtyMoveEffect +0x1 (hit flags) and +0x14 (s8 chance) are padding in
 *   party.h: 0x006D4DE0 reads pad1, 0x006D5398 reads (s8)pad14[0].
 */

#endif /* BATTLE_N33_H */
