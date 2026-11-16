/*
 * Party, characters, items, skills and moves (the pty* globals).
 *
 * Most tables come from SYS/GRXSYS.PAC (pointers set by SysDataInit,
 * 0x0041D1B8, and fixed up by PtySysTablesRelocate, 0x00410538); the party
 * and character records live in the save image (ptyCharData points at
 * ptyCharStatsBuf, ptyPartyData 0x19EC bytes before it); the battle units
 * are allocated by SysDataInit.
 *
 * Every layout here was merged from the per-file views in src/ by byte
 * offset. Where views disagreed on a field's type the comment says so. A
 * field's type changes alias sets and so codegen (docs/cleanup.md): switch
 * files to this header one at a time, with a re-check.
 */
#ifndef PARTY_H
#define PARTY_H

#include "common.h"

#define PTY_CHAR_COUNT 8         /* playable characters, ids 0x1001-0x1008 */
#define PTY_CHAR_ID_BASE 0x1001
#define PTY_PARTY_SIZE 4         /* ptyPartyUnits[0..3] are the party */
#define PTY_UNIT_COUNT 16        /* ptyPartyUnits[4..15] are enemies */
#define PTY_ENEMY_COUNT 12
#define PTY_ITEM_COUNT 0x800
#define PTY_MOVE_COUNT 0x12D     /* skills/magic/specials, combos from PTY_MOVE_COMBO_FIRST */
#define PTY_MOVE_COMBO_FIRST 0xDD
#define PTY_SKILL_COUNT 0x97
#define PTY_BAG_SIZE 30
#define PTY_KEY_ITEM_SIZE 24
#define PTY_BOOK_SIZE 24
#define PTY_SKILL_SLOTS 100
#define PTY_EGG_SIZE 24
#define PTY_CHAR_MOVES 5         /* special moves per character */
#define PTY_MOVE_LEVEL_MOVES 40  /* PTY_CHAR_COUNT * PTY_CHAR_MOVES */
#define PTY_COMBO_COUNT 50       /* combo levels follow at moveLevels[40] */
#define PTY_EGG_TYPE_COUNT 44    /* ptyEggFusionTable is s16[44][44] */

/* Item types (PtyItem.type). */
#define PTY_ITEM_WEAPON 5
#define PTY_ITEM_ARMOR 6         /* 6-10: armor, glove, hat, boots, accessory */
#define PTY_ITEM_MANA_EGG 11
#define PTY_ITEM_SKILL_BOOK 12
#define PTY_ITEM_KEY 14

/* ------------------------------------------------------------------ */
/* Save data: ptyPartyData                                             */
/* ------------------------------------------------------------------ */

/* Bag and key-item slot (+0x22 bag[30], +0x9A keyItems[24]). 4 bytes. */
typedef struct PtyItemSlot {
    /* 0x0 */ s16 id;      /* item id, 0 = empty */
    /* 0x2 */ s8 count;    /* one view: u8 (5F0470-family "cnt") */
    /* 0x3 */ u8 pad3;
} PtyItemSlot;

/* Skill book slot (+0x10FA, 24). 4 bytes. */
typedef struct PtySkillBook {
    /* 0x0 */ s16 itemId;  /* 0 / <= 0 = empty */
    /* 0x2 */ s8 owner;    /* character index 0-7, -1 none */
    /* 0x3 */ s8 unk3;
} PtySkillBook;

/* Learned skill slot (+0x115A, 100). 8 bytes. Debug names (BATTLE "WORK SKILL"
 * page, 0x006D9FE8): skill_no, eqp_pc, lv, exp. */
typedef struct PtySkill {
    /* 0x0 */ u8 id;       /* ptySkillTable index, 0 = empty */
    /* 0x1 */ s8 owner;    /* character index, -1 unequipped */
    /* 0x2 */ s8 level;    /* up to ptySkillTable[id].maxLevel */
    /* 0x3 */ s8 unk3;     /* two views: u8 pad */
    /* 0x4 */ s16 exp;     /* points toward the next level */
    /* 0x6 */ s16 unk6;
} PtySkill;

/* Mana egg slot (+0x147A, 24). 0x1C bytes. Filled by PtyManaEggAdd. Debug names
 * (BATTLE "WORK EGG" page, 0x006D9FE8): item_no, id, eqp_pc, mmp, mp, mgc_no,
 * bit_spc. */
typedef struct PtyManaEgg {
    /* 0x00 */ s16 itemId;          /* <= 0 = empty */
    /* 0x02 */ s8 type;             /* ptyManaEggTable index (= item subtype); s8 needed by PtyManaEggAdd, Bank-family views u8 */
    /* 0x03 */ s8 owner;            /* character index, -1 none */
    /* 0x04 */ s16 maxMp;           /* views: s16 x8, u16 x2 */
    /* 0x06 */ s16 mp;              /* views: s16 x9, u16 x2 */
    /* 0x08 */ s16 spells[6];       /* move ids; two views u16 */
    /* 0x14 */ u8 spellUpgrades[6]; /* upgrade bits per spell (1 << level) */
    /* 0x1A */ s8 unk1A;            /* set from the fusion result (ItemEggFusePreview); one view u8 */
    /* 0x1B */ u8 pad1B;
} PtyManaEgg;

/*
 * Special-move / combo level slot (+0x171A, 90): moveLevels[chr * 5 + i] for
 * the 40 character moves, moveLevels[40 + combo] for the 50 combos. 8 bytes.
 */
typedef struct PtyMoveLevel {
    /* 0x0 */ s16 moveId;  /* one view: u16 */
    /* 0x2 */ u16 exp;
    /* 0x4 */ u8 level;    /* 0 = not learned */
    /* 0x5 */ u8 maxLevel;
    /* 0x6 */ u8 pad6[2];
} PtyMoveLevel;

/* Party-wide save data, 0x19EC bytes (ptyCharData follows it directly). */
typedef struct PtyPartyData {
    /* 0x0000 */ s32 gold;                 /* two views: u32 (PtySetGold caps 99999999) */
    /* 0x0004 */ s32 shopPoints;           /* two views: u32 (cap 999999) */
    /* 0x0008 */ u8 pad8[0x18];
    /* 0x0020 */ u8 autoTactics;           /* battle btlAutoTactics */
    /* 0x0021 */ u8 pad21;
    /* 0x0022 */ PtyItemSlot bag[PTY_BAG_SIZE];
    /* 0x009A */ PtyItemSlot keyItems[PTY_KEY_ITEM_SIZE];
    /* 0x00FA */ s8 storage[PTY_ITEM_COUNT];    /* count per item id; one view u8 */
    /* 0x08FA */ u8 itemFlags[PTY_ITEM_COUNT];  /* per item id: 1 sold, 2 seen */
    /* 0x10FA */ PtySkillBook books[PTY_BOOK_SIZE];
    /* 0x115A */ PtySkill skills[PTY_SKILL_SLOTS];
    /* 0x147A */ PtyManaEgg eggs[PTY_EGG_SIZE];
    /* 0x171A */ PtyMoveLevel moveLevels[PTY_MOVE_LEVEL_MOVES + PTY_COMBO_COUNT];
    /* 0x19EA */ u8 pad19EA[2];
} PtyPartyData;

/* ------------------------------------------------------------------ */
/* Save data: ptyCharData                                              */
/* ------------------------------------------------------------------ */

/*
 * Per-character save record, 8 x 0x84 at ptyCharStatsBuf (save +0x1A18).
 * The "growth" fields hold the rolled level-up gains alone (no bonus):
 * PtyCharLevelUpStats adds each roll to both, PtyCharLevelDownStats takes it
 * from both, PtyCharInit zeroes them.
 */
typedef struct PtyChar {
    /* 0x00 */ s32 exp;            /* one view: u32 */
    /* 0x04 */ s32 maxHp;          /* base max HP (cap 99999) */
    /* 0x08 */ s32 hp;
    /* 0x0C */ s16 maxSp;          /* base max SP (cap 999); one view u16 */
    /* 0x0E */ s16 sp;             /* one view: u16 */
    /* 0x10 */ s16 stats[6];       /* base str, sta, itl, spi, agi, run (cap 9999; unit stats[]); one view u16 */
    /* 0x1C */ s32 status;         /* status bits kept between battles (0x9814 mask); s32: only 5EE550 wants u32 and casts */
    /* 0x20 */ s32 hpGrowth;
    /* 0x24 */ s16 spGrowth;
    /* 0x26 */ s16 statGrowth[6];
    /* 0x32 */ s16 equip[6];       /* item ids: weapon, armor, glove, hat, boots, accessory; one view u16 */
    /* 0x3E */ s16 slots[6];       /* egg/book slot: index into eggs[] (first ptyCharDefTable.eggSlots) or books[]; -1 empty.
                                    * s16 for the -1 sentinel; the u16 readers cast */
    /* 0x4A */ s16 skills[15];     /* skills[] indices, 3 per book slot ((slot - eggSlots) * 3 + j); -1 empty */
    /* 0x68 */ s8 tactics[2];      /* battle tactics per auto set (PtyCharGetTactics); views: s8 x3, u8 x3, u8[2], u8[4] */
    /* 0x6A */ u8 pad6A[2];
    /* Battle record (BATTLE's PC SAVE page 6DE140: "BTL COUNT", "KILL", "KILL MAX",
     * "KILL TOTAL", "DEAD", "MVP"); PtyCharInit clears them. */
    /* 0x6C */ s32 battleCount;
    /* 0x70 */ s32 kills;
    /* 0x74 */ s32 killMax;
    /* 0x78 */ s32 killTotal;
    /* 0x7C */ s32 deaths;
    /* 0x80 */ s32 mvp;
} PtyChar;

/* ------------------------------------------------------------------ */
/* GRXSYS tables                                                       */
/* ------------------------------------------------------------------ */

/* Weapon trace definition (PtyCharDef.traces), 0x10 bytes; enemy data uses the same records. */
typedef struct PtyTraceDef {
    /* 0x0 */ u16 parts[2];        /* model parts the trace runs between */
    /* 0x4 */ s16 offset0[3];      /* offset from parts[0] */
    /* 0xA */ s16 offset1[3];      /* offset from parts[1] */
} PtyTraceDef;

/* Character definition, 8 x 0x7C (GRXSYS +0x90). */
typedef struct PtyCharDef {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ s8 unk1;            /* -> unit guardDec */
    /* 0x02 */ s8 unk2;            /* -> unit avoidWait */
    /* 0x03 */ s8 unk3;            /* -> unit avoidEvade */
    /* 0x04 */ s32 initExp;
    /* 0x08 */ s32 initMaxHp;
    /* 0x0C */ u16 initMaxSp;
    /* 0x0E */ u16 initStats[6];
    /* 0x1A */ s16 colR;           /* -> unit colR */
    /* 0x1C */ s16 colH;           /* -> unit colH */
    /* 0x1E */ u16 unk1E;          /* -> unit runTime */
    /* 0x20 */ s16 unk20;          /* -> unit restartItem (x 0.0001) */
    /* 0x22 */ s16 unk22[3];       /* -> unit restartEquip/Guard/Evade (x 0.0001) */
    /* 0x28 */ u16 unk28;          /* -> unit ipStopDmg */
    /* 0x2A */ u16 unk2A;          /* -> unit ipStopDown */
    /* 0x2C */ s16 unk2C[3];       /* -> unit exeSp/Good/Bad */
    /* 0x32 */ u8 unk32[5];        /* -> unit spSup* */
    /* 0x37 */ u8 pad37;
    /* 0x38 */ s32 unitFlags;      /* -> unit bitCsf (| 0x400) */
    /* 0x3C */ s8 unk3C;           /* -> unit regIp */
    /* 0x3D */ s8 unk3D;           /* -> unit regKb = (100 - x) / 100 */
    /* 0x3E */ s8 unk3E[4];        /* -> unit kregGood..kregOther */
    /* 0x42 */ s8 unk42[5];        /* -> unit kregPowD / kregPowU */
    /* 0x47 */ u8 unk47;           /* -> unit comboMax */
    /* 0x48 */ s16 initEquip[6];   /* views: s16, u16 */
    /* 0x54 */ s16 initSlots[6];   /* item ids of the initial eggs/books; views: s16, u16 */
    /* 0x60 */ s8 eggSlots;        /* how many of PtyChar.slots[] hold mana eggs; one view u8 */
    /* 0x61 */ u8 initSkills[15];  /* one view: u8[6][3] */
    /* 0x70 */ u8 pad70;
    /* 0x71 */ s8 traceCount;      /* weapon trace definitions (FxTraceStart) */
    /* 0x72 */ u8 pad72[2];
    /* 0x74 */ s32 name;           /* text pointer (relocated); views: s32, u32 */
    /* 0x78 */ PtyTraceDef *traces; /* relocated; [traceCount] */
} PtyCharDef;

typedef struct PtyMoveEffect PtyMoveEffect;

/* Per-level data of a move (PtyMove.levels, ptyMoveTable +8), 0x18 bytes. */
typedef struct PtyMoveLevelData {
    /* 0x00 */ s16 mpCost;
    /* 0x02 */ s16 cost;           /* scaled by unit spcSpRate (ItemMoveGetCost) */
    /* 0x04 */ s16 unk4[4];        /* the battle readers take unk4[4] = scope */
    /* 0x0C */ s16 scope;          /* scope radius (BtlUnit scopeRadius) */
    /* 0x0E */ s16 unkE;
    /* 0x10 */ PtyMoveEffect *effects; /* relocated when effectCount > 0; read as (s32) in
                                        * 6D4C60, 6CF0E8, 6CC2A8, 699BE0 (no union needed) */
    /* 0x14 */ s8 effectCount;
    /* 0x15 */ u8 pad15[3];
} PtyMoveLevelData;

/* One effect of a move level or item use, 0x1C bytes. */
struct PtyMoveEffect {
    /* 0x00 */ s8 kind;            /* s8: the AI pickers (6C50C0, 6C5388, 6C56D8, 6C5948, ...) need it; six cast (u8) */
    /* 0x01 */ u8 hitFlags;        /* 2, 4 (6D4DE0) */
    /* 0x02 */ s16 base;
    /* 0x04 */ s16 power;
    /* 0x06 */ u8 pad6[0xA];
    /* 0x10 */ s32 flags;
    /* 0x14 */ s8 chance;          /* status chance (6D5398) */
    /* 0x15 */ u8 pad15;
    /* 0x16 */ s8 element;
    /* 0x17 */ u8 statMask;        /* status mask (6C56D8, 6C5948, 5F6480) */
    /* 0x18 */ u8 stat;            /* 5F6BC8 */
    /* 0x19 */ u8 pad19[3];
};

/* Move (skill/magic/special move/combo) definition, 0x12D x 0x20 (GRXSYS +0x8C). */
typedef struct PtyMove {
    /* 0x00 */ s8 kind;            /* one view: u8 */
    /* 0x01 */ s8 unk1;
    /* 0x02 */ s8 target;          /* target area; one view u8 */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 reach;
    /* 0x06 */ u8 icon;
    /* 0x07 */ s8 levelCount;
    /* 0x08 */ PtyMoveLevelData *levels; /* relocated; views: many pointer types, s32 x4 */
    /* 0x0C */ u8 padC[3];
    /* 0x0F */ s8 unkF;            /* one view: u8 */
    /* 0x10 */ s32 name;           /* text pointer (relocated); views s32 x5, u8 * x2, void * x2.
                                     * s32 needed: BtlUnitActionBegin (0x006CE660) keeps this load
                                     * behind an s32 store only in s32's alias set */
    /* 0x14 */ s32 help;           /* text pointer (relocated): help window name line (41A538) */
    /* 0x18 */ s32 helpDesc;       /* text pointer (relocated): help window description (41A618) */
    /* 0x1C */ s8 members[4];      /* combos: characters taking part (id - 0x1000, 0 none) */
} PtyMove;

/* Equipment data of an item (PtyItem.equip), 0x2C bytes. */
typedef struct PtyEquipData {
    /* 0x00 */ u16 charMask;       /* who can equip it; one view s16 */
    /* 0x02 */ s16 stats[6];       /* added to unit params[6] */
    /* 0x0E */ s8 unkE;            /* weapon: killer -> unit eqpKiller */
    /* 0x0F */ s8 unkF;            /* weapon: element -> unit eqpAttr (MenuHelpSetElementColor(s8)) */
    /* 0x10 */ s32 unk10;          /* weapon: -> unit eqpBitSta; armor: resist bits 0x10 << j */
    /* 0x14 */ s8 unk14;           /* percent */
    /* 0x15 */ u8 effectType;      /* PtyUnitApplyEffect */
    /* 0x16 */ s16 effectValue;
    /* 0x18 */ s16 elements[9];    /* added to unit regMgcAttr */
    /* 0x2A */ u8 pad2A[2];
} PtyEquipData;

/* Use data of an item (PtyItem.use); fields seen up to +0x15, only used through pointers. */
typedef struct PtyItemUse {
    /* 0x00 */ s8 kind;            /* one view: u8 */
    /* 0x01 */ s8 unk1;
    /* 0x02 */ s8 target;          /* one view: u8 */
    /* 0x03 */ u8 pad3;
    /* 0x04 */ u16 moveId;          /* ptyMoveTable entry the item uses (6CE660) */
    /* 0x06 */ s8 rate;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u8 effectCount;     /* effects relocated when non-zero */
    /* 0x0F */ s8 unkF;
    /* 0x10 */ PtyMoveEffect *effects; /* views: Act *, Elem *, Info *, s8 * */
    /* 0x14 */ u8 pad14;
    /* 0x15 */ u8 unk15;
} PtyItemUse;

/* Item definition, 0x800 x 0x20 (GRXSYS +0x84). */
typedef struct PtyItem {
    /* 0x00 */ s8 type;            /* PTY_ITEM_*; views: s8 x22, u8 x12 */
    /* 0x01 */ u8 icon;
    /* 0x02 */ s8 model;           /* pickup model group */
    /* 0x03 */ s8 rank;            /* one view: u8 */
    /* 0x04 */ s32 price;
    /* 0x08 */ u8 subtype;         /* egg type, book type, ... */
    /* 0x09 */ u8 unk9;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ s32 name;           /* text pointer (relocated), kept s32 like the other GRXSYS text
                                     * pointers: views s32 x21, u8 * x4, void * x1, u32 x1 */
    /* 0x10 */ s32 help;           /* text pointer (relocated): help window name line (41A538) */
    /* 0x14 */ s32 helpDesc;       /* text pointer (relocated): help window description (41A618) */
    /* 0x18 */ PtyEquipData *equip; /* views: u16 * mask, Equip *, Item *, void *, s32 */
    /* 0x1C */ PtyItemUse *use;    /* views: many pointer types, s32 x8. BtlUnitActionBegin
                                     * (0x006CE660) needs one load of it as s32 (alias order):
                                     * *(s32 *)&ptyItemTable[i].use */
} PtyItem;

/* Passive skill definition, 0x97 x 0x2C (GRXSYS +0xA4). */
typedef struct PtySkillDef {
    /* 0x00 */ s8 unk0;            /* views: s8, u8 */
    /* 0x01 */ s8 category;        /* 1-4, element exp kind; one view u8 */
    /* 0x02 */ u8 icon;            /* one view: s8 */
    /* 0x03 */ s8 maxLevel;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 effects[3];      /* PtyUnitApplyEffect types */
    /* 0x0B */ u8 padB;
    /* 0x0C */ s16 amount;         /* per level */
    /* 0x0E */ s16 expNeeded[9];   /* two views declare [15] */
    /* 0x20 */ s32 name;           /* text pointer (relocated); views s32 x3, u8 * x3 */
    /* 0x24 */ s32 help;           /* text pointer (relocated): help window name line (41A538) */
    /* 0x28 */ s32 helpDesc;       /* text pointer (relocated): help window description (41A618) */
} PtySkillDef;

/* Level-up growth and exp table, 8 x 0x3F8 (GRXSYS +0x94). */
typedef struct PtyGrowth {
    /* 0x000 */ s8 growth[16];     /* {hi, lo} roll bounds: maxHp, maxSp, stats[6] */
    /* 0x010 */ s32 exp[250];      /* exp to reach each level; one view u32 */
} PtyGrowth;

/* Mana egg type, 0x3C bytes (GRXSYS +0x80). */
typedef struct PtyManaEggType {
    /* 0x00 */ s16 maxMp;          /* upper bound of the rolled MP */
    /* 0x02 */ s16 minMp;
    /* 0x04 */ s16 spells[6];      /* views: s16, u16 */
    /* 0x10 */ s8 upgradeChance[6][7]; /* percent per spell and upgrade bit */
    /* 0x3A */ u8 pad3A[2];
} PtyManaEggType;

/* Skill book type, 4 bytes (GRXSYS +0x6C, indexed by PtyItem.subtype). */
typedef struct PtySkillBookType {
    /* 0x0 */ s8 type;
    /* 0x1 */ s8 slotCount;        /* skills it holds */
    /* 0x2 */ s16 value;           /* one view: two s8 */
} PtySkillBookType;

/*
 * Mana egg fusion results (GRXSYS +0x70): s16 [egg type b][egg type a],
 * kept flat because the code indexes it as result[a + (b - 0x12D) * 44 - 0x12D]
 * (item ids 0x12D+). A plain s16 * changes the address arithmetic's operand
 * order in ItemEggCanFuse (0x005F3588).
 */
typedef struct PtyEggFusionTable {
    /* 0x000 */ s16 result[PTY_EGG_TYPE_COUNT * PTY_EGG_TYPE_COUNT];
} PtyEggFusionTable;

/* Combo definition, 50 x 0x14 (GRXSYS +0x74). */
typedef struct PtyCombo {
    /* 0x00 */ s16 moveId;
    /* 0x02 */ s8 chars[4];        /* character index, -1 none */
    /* 0x06 */ s8 moves[4];        /* special move of each (moveLevels[chr * 5 + move]) */
    /* 0x0A */ s16 ipNeed[4];       /* IP each member needs (/ 10000) */
    /* 0x12 */ u8 pad12[2];
} PtyCombo; /* size 0x14 */

/* Special-move level thresholds, 40 x 0x18 (GRXSYS +0xA8). */
typedef struct PtyMoveLevelThreshold {
    /* 0x00 */ s16 moveId;
    /* 0x02 */ s16 exp[7];         /* exp for each level (read with a variable index) */
    /* 0x10 */ s8 req[2];          /* prerequisites (6A0040) */
    /* 0x12 */ u8 lvl[2];
    /* 0x14 */ u8 pad14[4];
} PtyMoveLevelThreshold; /* size 0x18 */

/* ------------------------------------------------------------------ */
/* Battle units                                                        */
/* ------------------------------------------------------------------ */

/* Per-level growth of an enemy (PtyEnemyRollStats): {max, min} bounds of a
 * random gain rolled once per level above 1. */
typedef struct PtyEnemyGrowth {
    /* 0x00 */ s32 expMax;      /* exp gain, returned and added to the unit's exp */
    /* 0x04 */ s32 expMin;
    /* 0x08 */ s16 hpMax;       /* max HP gain (x3 from level 71, scaled 41-70) */
    /* 0x0A */ s16 hpMin;
    /* 0x0C */ s8 gains[8][2];  /* {max, min}: maxMp, maxSp, stats[6] */
} PtyEnemyGrowth; /* size 0x1C */

/* Enemy definition record (SYS/GBX.PAC), read by PtyUnitInitEnemy,
 * PtyEnemyRollStats and PtyUnitCalcParams. */
typedef struct PtyEnemyData {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u8 icon;         /* -> PtyUnit.icon */
    /* 0x03 */ u8 size;
    /* 0x04 */ u8 race[2];
    /* 0x06 */ u8 type;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s32 exp;         /* base exp, before the level rolls */
    /* 0x0C */ u8 padC[2];
    /* 0x0E */ u16 expSkl[2];
    /* 0x12 */ u8 pad12[0x11];
    /* 0x23 */ s8 avoid;        /* added to avoidWait and avoidEvade */
    /* 0x24 */ s32 params[9];   /* level 1 maxHp, maxMp, maxSp, stats[6] */
    /* 0x48 */ s16 colR;        /* collision radius / height */
    /* 0x4A */ s16 colH;
    /* 0x4C */ u16 runTime;
    /* 0x4E */ u16 ipStopDmg;
    /* 0x50 */ u16 ipStopDown;
    /* 0x52 */ s16 magicRate[9]; /* percent, added to unit magicRate */
    /* 0x64 */ s32 staResist;   /* bit 0x10 << i: immune to status i (unit regSta) */
    /* 0x68 */ s16 regMgcAttr[9]; /* percent, element damage */
    /* 0x7A */ s8 regMgcIpr;    /* percent */
    /* 0x7B */ s8 regIp;
    /* 0x7C */ s32 bitCsf;
    /* 0x80 */ s8 regKb;        /* unit regKb = (100 - x) / 100 */
    /* 0x81 */ s8 antiHalf;
    /* 0x82 */ s16 exeSp;
    /* 0x84 */ s16 exeGood;
    /* 0x86 */ s16 exeBad;
    /* 0x88 */ s8 kregGood;
    /* 0x89 */ s8 kregSpi;
    /* 0x8A */ s8 kregSeal;
    /* 0x8B */ s8 kregOther;
    /* 0x8C */ s8 kregPow[5];
    /* 0x91 */ s8 skills[6];    /* ptySkillTable ids */
    /* 0x97 */ s8 skillRolls[6]; /* chance (tens, percent) and level (units, 0 = random) */
    /* 0x9D */ s8 statRates[6]; /* percent applied to stats[] */
    /* 0xA3 */ u8 padA3[5];
    /* 0xA8 */ s32 name;        /* text pointer */
    /* 0xAC */ u8 padAC[6];
    /* 0xB2 */ u8 effectTypes[2];
    /* 0xB4 */ u16 effectValues[2];
    /* 0xB8 */ PtyEnemyGrowth growth;
} PtyEnemyData; /* size >= 0xD4 */

/* Enemy parameters for PtyUnitInitEnemy/PtyUnitCalcParams, 12 x 0x38. */
typedef struct PtyEnemyParams {
    /* 0x00 */ s32 maxHp;
    /* 0x04 */ s32 maxMp;
    /* 0x08 */ s32 maxSp;
    /* 0x0C */ s32 stats[6];
    /* 0x24 */ u8 effectTypes[2];
    /* 0x26 */ s16 effectValues[2];
    /* 0x2A */ s8 skills[6];       /* ptySkillTable ids (PtyEnemyRollStats) */
    /* 0x30 */ s8 skillLevels[6];
    /* 0x36 */ u8 pad36[2];
} PtyEnemyParams;

/*
 * Battle unit parameters, 0x318 bytes: ptyPartyUnits (16: party 0-3,
 * enemies 4-15) and ptyCharCalcUnits (8, stats of characters outside the
 * party). BATTLE's units point at them (btlUnits +0x1F0). Computed by
 * PtyUnitInitFromChar / PtyUnitInitEnemy and PtyUnitCalcParams.
 * Field names follow the labels of BATTLE's CHAR PARAM debug pages
 * (DbgBtlCharParam*Page, 0x006DCB28-0x006DDDC0: "mhp", "bit_sta",
 * "guard_dec", ...); types follow PtyUnitCalcParams (0x00412910), the only
 * complete view, where the debug pages disagree it is noted.
 */
typedef struct PtyUnit {
    /* 0x000 */ s32 name;          /* text pointer, PtyCharDef.name; views: s32 x2, u8 * x1 */
    /* 0x004 */ s16 id;            /* "id": character id 0x1001+ / enemy id */
    /* 0x006 */ u8 icon;           /* "icon"; set to id - 1; one view s8 */
    /* 0x007 */ s8 size;           /* "size"; one view u8 */
    /* 0x008 */ s8 race[2];        /* "race" (two values); one view u8 */
    /* 0x00A */ s8 type;           /* "type"; one view u8 */
    /* 0x00B */ u8 padB;
    /* 0x00C */ s16 level;         /* "lv"; one view u16 */
    /* 0x00E */ s16 unkE;
    /* 0x010 */ s32 exp;           /* "exp" */
    /* 0x014 */ s32 nextExp;       /* "next_exp" */
    /* 0x018 */ s32 maxHp;         /* "mhp" */
    /* 0x01C */ s32 hp;            /* "hp" */
    /* 0x020 */ s32 maxMp;         /* "mmp" */
    /* 0x024 */ s32 mp;            /* "mp" */
    /* 0x028 */ s32 maxSp;         /* "msp" */
    /* 0x02C */ s32 sp;            /* "sp" */
    /* 0x030 */ s32 stats[6];      /* "_str" "_sta" "_itl" "_spi" "_agi" "_run" */
    /* 0x048 */ s32 status;        /* "bit_sta": status bits (0x8000 dead, ...); one view u32 */
    /* 0x04C */ s32 status2;       /* "bit_sta2" */
    /* 0x050 */ s32 params[6];     /* "atk" "def" "mgp" "mgr" "act" "mov"; 5EB4F8 reads the low halves as s16 */
    /* 0x068 */ s8 comboMax;       /* "combo_max"; 412910 view u8 */
    /* 0x069 */ u8 pad69[3];
    /* 0x06C */ f32 guardDec;      /* "guard_dec" */
    /* 0x070 */ f32 avoidWait;     /* "avoid_wait" */
    /* 0x074 */ f32 avoidEvade;    /* "avoid_evade" */
    /* 0x078 */ s16 ipStopDmg;     /* "ip_stop_dmg"; views u16 x2 */
    /* 0x07A */ s16 ipStopDown;    /* "ip_stop_down"; views u16 x2 */
    /* 0x07C */ f32 killer[13];    /* "killer" */
    /* 0x0B0 */ f32 regRace[13];   /* "reg_race" */
    /* 0x0E4 */ s32 defRace[13];   /* "def_race" */
    /* 0x118 */ f32 magicRate[9];  /* "magic_rate" */
    /* 0x13C */ s32 magicPow[9];   /* "magic_pow"; one battle view declares [22] */
    /* 0x160 */ f32 itemUpBomb;    /* "item_up_bomb" */
    /* 0x164 */ f32 itemUpScr;     /* "item_up_scr" */
    /* 0x168 */ f32 itemUpDrug;    /* "item_up_drug" */
    /* 0x16C */ s8 crtclMulti;     /* "crtcl_multi"; 412910 view u8 */
    /* 0x16D */ s8 eqpAttr;        /* "eqp_attr"; 412910 view u8 */
    /* 0x16E */ s8 eqpKiller;      /* "eqp_killer"; 412910 view u8 */
    /* 0x16F */ u8 cmdDisable;     /* "cmd_disable" */
    /* 0x170 */ s32 eqpBitSta;     /* "eqp_bit_sta" */
    /* 0x174 */ f32 eqpStaProb;    /* "eqp_sta_prob" */
    /* 0x178 */ f32 absorbHp;      /* "absorb_hp" */
    /* 0x17C */ s32 crtclUp;       /* "crtcl_up" */
    /* 0x180 */ s32 atkDown;       /* "atk_down" */
    /* 0x184 */ s32 defDown;       /* "def_down" */
    /* 0x188 */ f32 spcSpdRate;    /* "spc_spd_rate" */
    /* 0x18C */ f32 spcRetRate;    /* "spc_ret_rate" */
    /* 0x190 */ s32 spcPow;        /* "spc_pow" */
    /* 0x194 */ f32 spcSpRate;     /* "spc_sp_rate": special-move SP cost scale */
    /* 0x198 */ f32 mgcSpdRate;    /* "mgc_spd_rate" */
    /* 0x19C */ f32 mgcRetRate;    /* "mgc_ret_rate" */
    /* 0x1A0 */ f32 mgcMpRate;     /* "mgc_mp_rate": spell MP cost scale (PtyManaEggSpellCost) */
    /* 0x1A4 */ s32 mgc1IpCan;     /* "mgc1_ip_can" */
    /* 0x1A8 */ s32 multiMagic;    /* "multi_magic" */
    /* 0x1AC */ f32 itmSpdRate;    /* "itm_spd_rate" */
    /* 0x1B0 */ s32 guardHp;       /* "guard_hp" */
    /* 0x1B4 */ s32 guardSp;       /* "guard_sp" */
    /* 0x1B8 */ f32 escapeUp;      /* "escape_up" */
    /* 0x1BC */ f32 strikeBack;    /* "strike_back" */
    /* 0x1C0 */ s32 noDamage;      /* "no_damage" */
    /* 0x1C4 */ f32 refDamage;     /* "ref_damage" */
    /* 0x1C8 */ f32 takeDmgMp;     /* "take_dmg_mp" */
    /* 0x1CC */ f32 autoGuard;     /* "auto_guard" */
    /* 0x1D0 */ f32 autoYomi;      /* "auto_yomi" */
    /* 0x1D4 */ f32 angryRate;     /* "angry_rate" */
    /* 0x1D8 */ f32 angryProb;     /* "angry_prob" */
    /* 0x1DC */ f32 angry;         /* "angry"; zeroed with sp by PtyClearSp; two views s32 */
    /* 0x1E0 */ f32 sArmorRate;    /* "s_armor_rate" */
    /* 0x1E4 */ f32 lure;          /* "lure" */
    /* 0x1E8 */ f32 antiLvDrain;   /* "anti_lv_drain" */
    /* 0x1EC */ f32 antiKb;        /* "anti_kb" */
    /* 0x1F0 */ f32 antiRetire;    /* "anti_retire" */
    /* 0x1F4 */ f32 antiCounter;   /* "anti_counter" */
    /* 0x1F8 */ f32 antiCancel;    /* "anti_cancel" */
    /* 0x1FC */ f32 antiHalf;      /* "anti_half" */
    /* 0x200 */ f32 getGoldRate;   /* "get_gold_rate" */
    /* 0x204 */ f32 getRateUp;     /* "get_rate_up" */
    /* 0x208 */ f32 getItemUp;     /* "get_item_up" */
    /* 0x20C */ f32 getExpRate;    /* "get_exp_rate" */
    /* 0x210 */ f32 combiHint;     /* "combi_hint" */
    /* 0x214 */ f32 mcActDisp;     /* "mc_act_disp" */
    /* 0x218 */ s32 lvupMhp;       /* "lvup_mhp": extra level-up gain (PtyCharGainExp passes */
    /* 0x21C */ s32 lvupStats[6];  /* "lvup_str" .. "lvup_run"   the unit to PtyCharLevelUpStats) */
    /* 0x234 */ f32 atkRef;        /* "atk_ref" */
    /* 0x238 */ f32 startTransp;   /* "start_transp" */
    /* 0x23C */ f32 startSleep;    /* "start_sleep" */
    /* 0x240 */ f32 startPara;     /* "start_para" */
    /* 0x244 */ f32 startIpOfs;    /* "start_ip_ofs" */
    /* 0x248 */ f32 mcAwayDec;     /* "mc_away_dec" */
    /* 0x24C */ s32 comboUp;       /* "combo_up" */
    /* 0x250 */ f32 regMgcAttr[9]; /* "reg_mgc_attr": element damage rates */
    /* 0x274 */ f32 regMgcIpr;     /* "reg_mgc_ipr" */
    /* 0x278 */ f32 regSta[12];    /* "reg_sta": status resist rates */
    /* 0x2A8 */ f32 regIp;         /* "reg_ip" */
    /* 0x2AC */ f32 regKb;         /* "reg_kb" */
    /* 0x2B0 */ s32 bitCsf;        /* "bit_csf": flags (PtyCharDef.unitFlags | 0x400) */
    /* 0x2B4 */ f32 colR;          /* "col_r": collision radius */
    /* 0x2B8 */ f32 colH;          /* "col_h": collision height */
    /* 0x2BC */ s16 runTime;       /* "run_time"; views u16 x2 */
    /* 0x2BE */ s16 exeSp;         /* "exe_sp"; one view u16 */
    /* 0x2C0 */ s16 exeGood;       /* "exe_good"; one view u16 */
    /* 0x2C2 */ s16 exeBad;        /* "exe_bad"; one view u16 */
    /* 0x2C4 */ f32 kregGood;      /* "kreg_good" */
    /* 0x2C8 */ f32 kregSpi;       /* "kreg_spi" */
    /* 0x2CC */ f32 kregSeal;      /* "kreg_seal" */
    /* 0x2D0 */ f32 kregOther;     /* "kreg_other" */
    /* 0x2D4 */ f32 kregPowD[5];   /* "kreg_pow_d" */
    /* 0x2E8 */ f32 kregPowU[5];   /* "kreg_pow_u" */
    /* 0x2FC */ s8 spSupCmb;       /* "sp_sup_cmb"; 412910 view u8 (also 2FD-300) */
    /* 0x2FD */ s8 spSupCrt;       /* "sp_sup_crt" */
    /* 0x2FE */ s8 spSupDmg;       /* "sp_sup_dmg" */
    /* 0x2FF */ s8 spSupDown;      /* "sp_sup_down" */
    /* 0x300 */ s8 spSupGuard;     /* "sp_sup_guard" */
    /* 0x301 */ u8 pad301[3];
    /* 0x304 */ f32 restartItem;   /* "restart_item" */
    /* 0x308 */ f32 restartEquip;  /* "restart_equip" */
    /* 0x30C */ f32 restartGuard;  /* "restart_guard" */
    /* 0x310 */ f32 restartEvade;  /* "restart_evade" */
    /* 0x314 */ s16 expSkl[2];     /* "exp_skl0" "exp_skl1"; one view u16 */
} PtyUnit;

/* ------------------------------------------------------------------ */
/* Globals                                                             */
/* ------------------------------------------------------------------ */

extern PtyPartyData *ptyPartyData;               /* 0x004A9444 */
extern PtyChar *ptyCharData;                     /* 0x004A9428, [PTY_CHAR_COUNT] */
extern PtyChar ptyCharStatsBuf[PTY_CHAR_COUNT];  /* 0x004EF198, in the save image */
extern PtyItem *ptyItemTable;                    /* 0x004A9440, [PTY_ITEM_COUNT] */
extern s16 *ptyItemSortKeys;                     /* 0x004A942C, [PTY_ITEM_COUNT] */
extern PtyMove *ptyMoveTable;                    /* 0x004A9494, [PTY_MOVE_COUNT] */
extern PtySkillDef *ptySkillTable;               /* 0x004A9480, [PTY_SKILL_COUNT] */
extern PtyCharDef *ptyCharDefTable;              /* 0x004A9450, [PTY_CHAR_COUNT] */
extern PtyGrowth *ptyGrowthTable;                /* 0x004A944C, [PTY_CHAR_COUNT] */
extern PtyManaEggType *ptyManaEggTable;          /* 0x004A941C */
extern PtySkillBookType *ptySkillBookTypeTable;  /* 0x004A947C */
extern PtyEggFusionTable *ptyEggFusionTable;     /* 0x004A9430 */
extern PtyCombo *ptyComboTable;                  /* 0x004A9488, [PTY_COMBO_COUNT] */
extern PtyMoveLevelThreshold *ptyMoveLevelThresholds; /* 0x004A9468, [PTY_MOVE_LEVEL_MOVES] */
extern PtyUnit *ptyPartyUnits;                   /* 0x004A943C, [PTY_UNIT_COUNT] */
extern PtyUnit *ptyCharCalcUnits;                /* 0x004A9484, [PTY_CHAR_COUNT] */
extern PtyEnemyParams *ptyBattleParams;          /* 0x004A9464, [PTY_ENEMY_COUNT] */
extern s32 ptyEnemySkillCount;                   /* 0x004A9474 */
extern s32 ptyApplyingEquip;                     /* 0x004ABCDC */
extern f32 ptyLevelDownExpRate;                  /* 0x0049A51C */
extern f32 ptyNegZeroF;                          /* 0x004A27C8 */

#endif /* PARTY_H */
