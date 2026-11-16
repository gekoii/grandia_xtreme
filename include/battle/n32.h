#ifndef BATTLE_N32_H
#define BATTLE_N32_H

#include "common.h"
#include "battle/unit.h"

/*
 * BATTLE units, targeting and AI (0x006B8CB0-0x006C6218): battle file
 * loading, unit setup and scheduling, the data loader, target selection
 * (BtlUnitSelectTarget and its BtlTargetPick* / BtlTargetFilter* steps),
 * enemy AI (AiEnemy*) and party AI (AiParty*, AiTactic*, Ai*).
 * The units themselves are in battle/unit.h, the program state in
 * battle/misc.h.
 */

/* ------------------------------------------------------------------ */
/* Files and model pools (0x006B8CB0-0x006B9050)                       */
/* ------------------------------------------------------------------ */

struct ChrModelPool; /* main/n05.h */

s32 ChrModelPoolInit(struct ChrModelPool *pool, s32 buf, s32 size);
s32 BtlModelPoolAlloc(struct ChrModelPool *pool);

/* Pack file: a count, then {offset, size} per entry, offsets from the start. */
typedef struct BtlPakEntry {
    /* 0x0 */ s32 offset;          /* 0 = no entry */
    /* 0x4 */ s32 size;
} BtlPakEntry; /* size 0x8 */

typedef struct BtlPak {
    /* 0x0 */ s32 count;
    /* 0x4 */ BtlPakEntry e[1];    /* [count] */
} BtlPak;

/* ------------------------------------------------------------------ */
/* Target selection (BtlUnitSelectTarget 0x006BCCD0)                   */
/* ------------------------------------------------------------------ */

/*
 * Candidate set passed through the BtlTargetPick* / BtlTargetFilter*
 * steps: each filter clears flags[] of the units it rejects, each pick
 * stores the chosen unit in idx.
 */
typedef struct BtlTargetSel {
    /* 0x00 */ BtlUnit *self;      /* the unit choosing */
    /* 0x04 */ s32 idx;            /* picked unit */
    /* 0x08 */ f32 prob;           /* chance that the pick replaces the plain target (BtlUnitSelectTarget: 0.6 / 0.4) */
    /* 0x0C */ s8 flags[BTL_UNIT_COUNT]; /* candidate units */
} BtlTargetSel; /* size 0x1C */

s32 BtlTargetFilterByRate(BtlTargetSel *s, s32 start, s32 n);
void BtlTargetPickByDistance(BtlTargetSel *s, s32 start, s32 n, s32 max);
void BtlTargetPickByIp(BtlTargetSel *s, s32 start, s32 n, f32 h);
void BtlTargetPickByCrowding(BtlTargetSel *s, s32 start, s32 n, s32 max);
void BtlTargetPickByStat(BtlTargetSel *s, s32 start, s32 n, void *field, s32 mode);
void BtlTargetPickByRank(BtlTargetSel *s, s32 start, s32 n, s32 max);
void BtlTargetFilterByStatMod(BtlTargetSel *s, s32 start, s32 n, u16 mask);
void BtlTargetFilterByStatus(BtlTargetSel *s, s32 start, s32 n, u16 mask, s8 inv);
void BtlTargetFilterNearGroup(BtlTargetSel *s, s32 start, s32 n, f32 r);
void BtlTargetFilterNotTargeted(BtlTargetSel *s, s32 start, s32 n, s32 mask);

/* ------------------------------------------------------------------ */
/* Enemy actions (BtlChrData.actionSets, AiEnemyChooseCommand 0x006C19E8) */
/* ------------------------------------------------------------------ */

/*
 * BtlChrData.actionSets read as an s32, as AiEnemyChooseCommand and
 * BtlEnemyActionStart do: the load then stays behind their s32 stores to
 * the unit (alias order). The action records are BtlAction (battle/unit.h).
 */
#define BTL_ACTION_SETS(data) ((BtlActionSet *)*(s32 *)&(data)->actionSets)

/*
 * View of BtlActionSet (battle/unit.h) as AiEnemyChooseCommand reads it:
 * actions[] as s32, so the loads stay behind the s32 stores to the unit
 * (alias order).
 */
typedef struct AiActionSet {
    /* 0x00 */ s16 type;           /* condition (AiEnemySelectActionSet) */
    /* 0x02 */ s8 prob[6];
    /* 0x08 */ s32 actions[5];     /* struct BtlAction * */
} AiActionSet; /* size 0x1C */

/* ------------------------------------------------------------------ */
/* AI state                                                            */
/* ------------------------------------------------------------------ */

extern f32 btlAreaCtrX;            /* 0x006E3F18 x of the last action area centre (BtlUnitCollectAreaTargets) */
extern f32 btlAreaCtrZ;            /* 0x006E3E98 z, read by BtlUnitHitAreaTargets */
extern s32 aiComboMask;            /* 0x006F3DE0 unit bits of a combo leader and its partners */
extern u8 aiSpellAllowTable[0x80]; /* 0x006F3DE8 1 for spell ids 1-0x49 (AiSpellAllowInit) */
extern s8 aiEnemyMaxRank;          /* 0x006F3E68 highest rank (PtyUnit.size) among living enemies */
extern s32 aiStatusUsableMask;     /* 0x006F3E6C status bits no living enemy resists */
extern s8 aiStatusOpenerReady;     /* 0x006F3E70 tactic 4 one-shot area status attack available; u8 in AiTacticsInit */
extern s8 aiBuffOpenerReady;       /* 0x006F3E71 tactic 6 one-shot buff available */
extern s8 aiAreaOpenerReady;       /* 0x006F3E72 default tactic one-shot area attack available */
extern f32 aiBossHpRatio;          /* 0x006F3E74 highest HP ratio among living rank >= 5 enemies */

BtlActionSet *AiEnemySelectActionSet(BtlUnit *u);
s32 AiEnemyFindPack(BtlUnit *u);
s32 AiComboFindPartners(BtlUnit *u, BtlAction *act, s8 flag);
BtlUnit *AiComboFindPartner(BtlUnit *self, s16 id, s32 min, s8 flag);
s32 AiComboJoin(BtlUnit *u, BtlAction *act);

#endif /* BATTLE_N32_H */
