#ifndef FIELD_MENU_H
#define FIELD_MENU_H

#include "common.h"

#include "vu.h"
#include "gph.h"
#include "party.h"         /* PtyManaEgg */

struct PtyItem;
struct PtySkillDef;

/*
 * FIELD menus: fldMenuWork (screen state and sprite buffers), the 18 menu
 * windows, the shared cursor, and itemWork (item/menu lists built by the
 * Item* module for every menu screen).
 */

/* ---------------------------------------------------------------- sprites */

/* One textured quad of a menu window or the top menu (TxtGlyph/Menu* draw):
 * a gph.h batch quad ("flag" = type in 0x0061FC28). */
typedef GphBatchQuad FldMenuQuad;

/* ---------------------------------------------------------------- fldMenuWork */

typedef struct FldMenuAnim {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u16 timer;
    /* 0x04 */ f32 t;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ FVECTOR cur;
    /* 0x20 */ FVECTOR start;
    /* 0x30 */ FVECTOR end;
} FldMenuAnim; /* size 0x40 */

typedef struct FldMenuWork {
    /* 0x0000 */ u8 screen;        /* 0xFF closed */
    /* 0x0001 */ u8 directOpen;    /* opened straight into options/load */
    /* 0x0002 */ u8 openDelay;
    /* 0x0003 */ u8 openType;      /* 2 save, 3 load, 4 card browse, 5 Skillman, 6-9 shop, ... */
    /* 0x0004 */ u8 state;
    /* 0x0005 */ u8 topCursor;
    /* 0x0006 */ u8 unk6;
    /* 0x0007 */ u8 pad7;
    /* 0x0008 */ u16 unk8;
    /* 0x000A */ u8 guideMode;
    /* 0x000B */ u8 unkB;
    /* 0x000C */ u8 unkC;
    /* 0x000D */ u8 unkD;
    /* 0x000E */ u8 padE[0xA];
    /* 0x0018 */ s32 unk18;
    /* 0x001C */ s32 unk1C;
    /* 0x0020 */ FldMenuAnim anims[6];      /* top-menu command icons */
    /* 0x01A0 */ FldMenuQuad topQuads[32];
    /* 0x09A0 */ FldMenuQuad topQuads2[4];
    /* 0x0AA0 */ FldMenuQuad topQuads3[128];
    /* 0x2AA0 */ FldMenuQuad sprBufs[13][48]; /* FldMenuGetSprBuf(n), 0xC00 each */
    /* 0xC6A0 */ u8 padC6A0[0x10];
} FldMenuWork; /* size 0xC6B0 (FldMenuWorkInit) */

/* ---------------------------------------------------------------- windows */

typedef struct FldMenuWin {
    /* 0x00000 */ u8 active;
    /* 0x00001 */ u8 state;
    /* 0x00002 */ u8 timer;
    /* 0x00003 */ u8 openType;    /* 9: close */
    /* 0x00004 */ f32 alpha;
    /* 0x00008 */ s32 styleFlags;  /* 1, 2 (FldMenuWinSetFlags/GetFlags) */
    /* 0x0000C */ u32 glyphCount;  /* s32 in 0x00620FC0 */
    /* 0x00010 */ u8 pad10[3];
    /* 0x00013 */ u8 txtWinId;     /* text window texture (TxtWinTextureBind) */
    /* 0x00014 */ s16 portraitCode; /* ChrPortraitSetByCode */
    /* 0x00016 */ u16 unk16;       /* lhu in 0x00621E48; s16 in 0x00621AD8 */
    /* 0x00018 */ s16 x;           /* u16 in 0x00621E48 */
    /* 0x0001A */ s16 y;
    /* 0x0001C */ s16 w;
    /* 0x0001E */ s16 h;
    /* 0x00020 */ u16 w2;
    /* 0x00022 */ u16 h2;          /* s16 in 0x00621E48 */
    /* 0x00024 */ u16 unk24;
    /* 0x00026 */ u8 pad26[2];
    /* 0x00028 */ s32 unk28;       /* 623ED8 tests it with flags as one u64 */
    /* 0x0002C */ s32 flags;
    /* 0x00030 */ u16 selected;    /* s16 stores in 622C18 */
    /* 0x00032 */ u8 pad32[2];
    /* 0x00034 */ f32 unk34;
    /* 0x00038 */ f32 unk38;
    /* 0x0003C */ f32 unk3C;
    /* 0x00040 */ u8 arrowL;       /* scroll arrows (FldMenuWinSetScrollArrows 620DA0, drawn by FldMenuWinDraw 622660) */
    /* 0x00041 */ u8 arrowR;
    /* 0x00042 */ u8 pad42[2];
    /* 0x00044 */ s32 unk44;
    /* 0x00048 */ u8 col[4][3];
    /* 0x00054 */ u8 pad54[0xC];
    /* 0x00060 */ FVECTOR openStart; /* open/close slide: from openStart to openEnd */
    /* 0x00070 */ FVECTOR pos;
    /* 0x00080 */ FVECTOR openEnd;
    /* 0x00090 */ u8 sprPages[0x7800];
    /* 0x07890 */ FldMenuQuad quads[883]; /* sub-buffers: background at 0, body at 32,
                                           * portrait at 80, glyphs at 82 */
    /* 0x15550 */ u8 pad15550[4];
    /* 0x15554 */ s32 layerKind[16];
    /* 0x15594 */ f32 layerVal[16];
    /* 0x155D4 */ s32 layerTex[16]; /* GsTexPacket of the layer (s32 as 623ED8 passes it; 622660 casts to void *) */
    /* 0x15614 */ FldMenuQuad *layerQuad[16];
    /* 0x15654 */ u8 pad15654[0xC];
} FldMenuWin; /* size 0x15660 */

typedef struct FldMenuCursor {
    /* 0x00 */ f32 colorKeys[6][4]; /* Bezier keys [0]/[2] and [1]/[3]; [4], [5] the two current colours */
    /* 0x60 */ f32 t;              /* s32 in 0x00621680 (zeroing) */
    /* 0x64 */ f32 fromX;
    /* 0x68 */ f32 fromY;
    /* 0x6C */ f32 x;              /* FldMenuCursorGetPos */
    /* 0x70 */ f32 y;
    /* 0x74 */ u8 pad74[4];
    /* 0x78 */ u8 shown;
    /* 0x79 */ u8 lrLock;          /* L/R arrows lock */
    /* 0x7A */ u8 pad7A[6];
} FldMenuCursor; /* size 0x80 */

/* ---------------------------------------------------------------- itemWork */

/*
 * Skill book list record (ItemBookRecFill, 0x005F0F70), written into
 * itemWork->books / books3 (declared there as the generic ItemRec, whose
 * +0x6..+0xC fields don't fit this record; the readers need ItemRec's
 * types, so the fillers' callers cast). 0x1C bytes.
 */
typedef struct ItemBookRec {
    /* 0x00 */ s16 itemId;
    /* 0x02 */ u8 icon;
    /* 0x03 */ s8 owner;           /* character index, -1 none */
    /* 0x04 */ s8 type;            /* PtySkillBookType.type */
    /* 0x05 */ s8 slotCount;       /* PtySkillBookType.slotCount */
    /* 0x06 */ s8 skillOwners[3];  /* owner per skill slot, -1 empty */
    /* 0x09 */ s8 skillIcons[3];   /* PtySkillDef.icon of the slotted skills */
    /* 0x0C */ u8 useClass;        /* ItemGetUseClass(2, id) */
    /* 0x0D */ u8 padD;
    /* 0x0E */ s16 index;          /* ptyPartyData->books[] index */
    /* 0x10 */ s32 name;
    /* 0x14 */ struct PtyItem *item;
    /* 0x18 */ s32 value;          /* PtySkillBookType.value */
} ItemBookRec; /* size 0x1C */

/*
 * Skill list record (ItemSkillRecFill, 0x005F13D0), written into
 * itemWork->books2 / books4 (declared there as ItemRec). 0x1C bytes.
 */
typedef struct ItemSkillRec {
    /* 0x00 */ s16 skillId;
    /* 0x02 */ s8 icon;
    /* 0x03 */ s8 owner;           /* character index, -1 unequipped */
    /* 0x04 */ s8 kind;            /* PtySkillDef.unk0 */
    /* 0x05 */ s8 level;
    /* 0x06 */ s8 maxLevel;
    /* 0x07 */ u8 pad7[3];
    /* 0x0A */ s16 index;          /* ptyPartyData->skills[] index */
    /* 0x0C */ s16 exp;
    /* 0x0E */ s16 expNext;
    /* 0x10 */ s32 name;
    /* 0x14 */ struct PtySkillDef *def;
    /* 0x18 */ s32 amount;         /* effect amount at this level */
} ItemSkillRec; /* size 0x1C */

/*
 * Mana egg list record (ItemEggRecFill, 0x005F4160), written into
 * itemWork->eggs and itemWork->magic (ItemMagicRec: 63AB88 reads its icon
 * as s8). 0x18 bytes.
 */
typedef struct ItemEggListRec {
    /* 0x00 */ s16 itemId;
    /* 0x02 */ u8 icon;
    /* 0x03 */ u8 unk3;            /* cleared */
    /* 0x04 */ s8 owner;           /* character index, -1 none */
    /* 0x05 */ s8 useClass;        /* ItemGetUseClass(1, id) */
    /* 0x06 */ s16 index;          /* ptyPartyData->eggs[] index */
    /* 0x08 */ s16 maxMp;
    /* 0x0A */ s16 mp;
    /* 0x0C */ s16 rarity;         /* PtyManaEgg.unk1A */
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ s32 name;
    /* 0x14 */ struct PtyItem *item;
} ItemEggListRec; /* size 0x18 */

/*
 * Mana egg record with its spells (ItemEggSpellRecFill, 0x005F4440), 0x98
 * bytes: itemWork->eggSpells[24] (ItemEggSpellListBuild) and the fusion
 * preview at itemWork +0x6E3C (ItemEggFusePreview). Only +0 is known from C.
 */
typedef struct ItemEggSpellRec {
    /* 0x00 */ s16 itemId;
    /* 0x02 */ u8 pad2[0x96];
} ItemEggSpellRec; /* size 0x98 */


typedef struct ItemEquipSlot {
    /* 0x0 */ s16 id;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ s8 unk3;
    /* 0x4 */ s32 name;
    /* 0x8 */ s32 desc;
} ItemEquipSlot; /* size 0xC */

typedef struct ItemPageSkill {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ s8 valid;
    /* 0x02 */ s16 icon;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ u8 level;
    /* 0x09 */ u8 max;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ s32 name;
    /* 0x14 */ s32 help;           /* a skill pointer in 0x005EBBE8 */
    /* 0x18 */ u8 pad18[4];
} ItemPageSkill; /* size 0x1C */

typedef struct ItemPageMove {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ s8 valid;
    /* 0x02 */ s16 icon;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s8 unk6;            /* stored by ItemCharMovesBuild (5EBE30) */
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 level;
    /* 0x09 */ u8 max;
    /* 0x0A */ u8 slot[4][2];
    /* 0x12 */ u8 pad12[2];
    /* 0x14 */ s32 name;
    /* 0x18 */ s32 help;
    /* 0x1C */ u8 pad1C[4];
} ItemPageMove; /* size 0x20 */

/* Per party member page (ItemPartyStatusBuild). */
typedef struct ItemCharPage {
    /* 0x000 */ s8 chr;            /* character id, 0 = empty slot */
    /* 0x001 */ u8 pad1;
    /* 0x002 */ s16 level;
    /* 0x004 */ u8 tactics[2];
    /* 0x006 */ s8 statusIcons[2]; /* sign-extended in 0x00626310 */
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 hp;
    /* 0x014 */ s32 hpMax;
    /* 0x018 */ u16 mp;
    /* 0x01A */ u16 mpMax;
    /* 0x01C */ s16 sp;
    /* 0x01E */ s16 spMax;
    /* 0x020 */ s16 stats[6];
    /* 0x02C */ s16 stats2[6];
    /* 0x038 */ s16 unk38[6];
    /* 0x044 */ s16 skillCount;
    /* 0x046 */ s16 unk46;
    /* 0x048 */ ItemEquipSlot equip[6];
    /* 0x090 */ ItemPageSkill skills[5];
    /* 0x11C */ ItemPageMove moves[25];
} ItemCharPage; /* size 0x43C */

typedef struct ItemBagRec {
    /* 0x00 */ u16 id;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ s8 count;
    /* 0x04 */ s8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ s8 unk6;
    /* 0x07 */ s8 unk7;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ u8 pad9;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ void *unk10;
    /* 0x14 */ s32 unk14;
} ItemBagRec; /* size 0x18 */

/* 0x1C-byte list record used by the book/egg/shop lists. */
typedef struct ItemRec {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s8 icon;
    /* 0x03 */ s8 unk3;
    /* 0x04 */ s8 type;
    /* 0x05 */ s8 n;
    /* 0x06 */ s8 max;
    /* 0x07 */ s8 owner;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ u8 pad9;
    /* 0x0A */ u16 id;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 name;
    /* 0x14 */ s32 help;           /* a PtySkillDef * (0x005F5548 stores it as one; as a pointer field the readers in 627320/63AB88 are 8-16 words off) */
    /* 0x18 */ s32 price;
} ItemRec; /* size 0x1C */

typedef struct ItemValuable {
    /* 0x0 */ s16 id;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ s8 unk3;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ void *def;
} ItemValuable; /* size 0xC */

typedef struct ItemRow {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 count;
    /* 0x02 */ u8 a[8];
    /* 0x0A */ u8 b[8];
} ItemRow; /* size 0x12 */

/* Shop list record; ItemShopListBuild (5F2E38) fills the start of each one as
 * an ItemBagRec. */
typedef struct ItemShopRec {
    /* 0x00 */ s16 id;
    /* 0x02 */ s8 icon;
    /* 0x03 */ s8 count;
    /* 0x04 */ u8 pad4[2];
    /* 0x06 */ s8 type;
    /* 0x07 */ u8 pad7;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ s8 unk9;
    /* 0x0A */ u16 unkA;
    /* 0x0C */ s32 name;
    /* 0x10 */ s32 help;
    /* 0x14 */ s32 price;
    /* 0x18 */ u8 mask;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ s16 stats[8];
} ItemShopRec; /* size 0x2C */

/* Equipment list entry (ItemEquipListBuild 5EB4F8, the equip screen 62DE20). */
typedef struct ItemListEntry {
    /* 0x00 */ s16 id;
    /* 0x02 */ s16 icon;
    /* 0x04 */ s32 name;
    /* 0x08 */ union {
                   struct PtyItem *item;
               } def;              /* a union: 5EB4F8's stores need its alias set */
    /* 0x0C */ s16 stats[6];       /* unit params after the change */
} ItemListEntry; /* size 0x18 */

typedef struct ItemMagicRec {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s8 icon;
    /* 0x03 */ u8 pad3[2];
    /* 0x05 */ s8 unk5;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 n;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ s32 name;
    /* 0x14 */ s32 help;
} ItemMagicRec; /* size 0x18 */

typedef struct ItemSaleRec {
    /* 0x00 */ u8 pad0[3];
    /* 0x03 */ s8 icon;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ u32 price;
    /* 0x0C */ s32 name;
    /* 0x10 */ s32 help;
    /* 0x14 */ s16 id;
    /* 0x16 */ s8 icon2;
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s32 desc;
    /* 0x1C */ s32 msg;
} ItemSaleRec; /* size 0x20 */

typedef struct ItemResist {
    /* 0x0 */ s8 id;               /* u8 in 0x00631230 */
    /* 0x1 */ s8 type;
    /* 0x2 */ s16 base;            /* u16 in 0x00631230 */
    /* 0x4 */ s16 val;
    /* 0x6 */ u8 pad6[2];
    /* 0x8 */ f32 pct;
} ItemResist; /* size 0xC */

typedef struct ItemStat {
    /* 0x0 */ s8 id;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ f32 val;
} ItemStat; /* size 0x8 */

typedef struct ItemGauge {
    /* 0x0 */ s8 idx;
    /* 0x1 */ s8 kind;
    /* 0x2 */ s16 lo;
    /* 0x4 */ s16 hi;
    /* 0x6 */ u8 pad6[2];
    /* 0x8 */ f32 val;
} ItemGauge; /* size 0xC */

typedef struct ItemShopSlot {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s8 unk2;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x10];
} ItemShopSlot; /* size 0x18 */

typedef struct ItemShopChr {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ s8 icon;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ u8 pad3;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u8 padE[6];
    /* 0x14 */ s32 name;
    /* 0x18 */ s32 help;
    /* 0x1C */ ItemShopSlot slots[6];
} ItemShopChr; /* size 0xAC */

typedef struct ItemJuncSub1 {
    /* 0x0 */ s8 unk0;
    /* 0x1 */ s8 unk1;             /* icon */
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s8 unk4;             /* target */
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 pad6[2];
    /* 0x8 */ s32 unk8;
} ItemJuncSub1; /* size 0xC */

typedef struct ItemJuncSub2 {
    /* 0x0 */ u8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
} ItemJuncSub2; /* size 0xC */

typedef struct ItemJuncEntry {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 unk1;            /* s2[] count */
    /* 0x02 */ s8 unk2;            /* s1[] count */
    /* 0x03 */ s8 unk3;            /* icon */
    /* 0x04 */ u16 unk4;           /* u16: ItemJuncListBuild's ++ is an lhu */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ void *item;
    /* 0x10 */ ItemJuncSub1 s1[6];
    /* 0x58 */ ItemJuncSub2 s2[6];
} ItemJuncEntry; /* size 0xA0 */

typedef struct ItemWork {
    /* 0x0000 */ ItemCharPage chars[4];
    /* 0x10F0 */ s32 gold;              /* u32 in 0x00639108/0x0063AB88 */
    /* 0x10F4 */ u16 menuType;          /* ItemSetMenuType */
    /* 0x10F6 */ u16 counts[6];         /* per list category (FldMenuItemGetCount); s16 in some views */
    /* 0x1102 */ u8 pad1102[2];
    /* 0x1104 */ u16 saleCount;
    /* 0x1106 */ u8 pad1106[8];
    /* 0x110E */ u16 unk110E;
    /* 0x1110 */ u8 slotIcons[8][6]; /* per character, egg/book slot item icons (N = none) */
    /* 0x1140 */ ItemBagRec bag[30];
    /* 0x1410 */ ItemEggListRec eggs[24];
    /* 0x1650 */ ItemRec books[24];  /* ItemBookRec records, read through ItemRec (63AB88's
                                    * 34 reads need ItemRec's types); the fillers cast */
    /* 0x18F0 */ ItemRec books2[24]; /* ItemSkillRec records, the same way */
    /* 0x1B90 */ u8 pad1B90[0x850];
    /* 0x23E0 */ ItemValuable valuables[24]; /* ItemValuableListBuild */
    /* 0x2500 */ ItemRow rows[5];
    /* 0x255A */ u8 pad255A[2];
    /* 0x255C */ ItemShopRec shop[100];
    /* 0x368C */ u8 pad368C[0x1130];
    /* 0x47BC */ ItemListEntry list[30];
    /* 0x4A8C */ ItemEggSpellRec eggSpells[24];
    /* 0x58CC */ ItemSaleRec sale[30];
    /* 0x5C8C */ ItemMagicRec magic[24];
    /* 0x5ECC */ ItemRec books3[24]; /* ItemBookRec records (see books) */
    /* 0x616C */ ItemRec books4[24]; /* ItemSkillRec records (see books2) */
    /* 0x640C */ u8 pad640C[0x854];
    /* 0x6C60 */ s8 curChr;
    /* 0x6C61 */ s8 curSlot;
    /* 0x6C62 */ s8 curCount;
    /* 0x6C63 */ s8 unk6C63;
    /* 0x6C64 */ s8 cur;
    /* 0x6C65 */ s8 unk6C65;
    /* 0x6C66 */ u8 pad6C66[2];
    /* 0x6C68 */ s32 unk6C68;
    /* 0x6C6C */ s16 unk6C6C;
    /* 0x6C6E */ u8 pad6C6E[2];
    /* 0x6C70 */ ItemResist resists[9];
    /* 0x6CDC */ ItemStat statList[10];
    /* 0x6D2C */ u8 pad6D2C[0x10];
    /* 0x6D3C */ u16 unk6D3C[6];
    /* 0x6D48 */ u16 unk6D48[6];
    /* 0x6D54 */ ItemGauge gauges[9];
    /* 0x6DC0 */ ItemStat gauges2[9];
    /* 0x6E08 */ u8 pad6E08[0x18];
    /* 0x6E20 */ union {
        PtyManaEgg eggRecs[2];          /* fusion inputs (0x005F3618, ItemEggFuseCommit) */
        struct {
            u8 pad0[0x20];
            s8 rows;                    /* 0x00642210 (junction screen) */
            u8 pad21[0x19];
            s8 tbl[1][20];
        } junc;
    } u6E20;
    /* 0x6E6E */ u8 pad6E6E[0x5E];
    /* 0x6ECC */ u8 unk6ECC;
    /* 0x6ECD */ u8 pad6ECD[3];
    /* 0x6ED0 */ s32 unk6ED0;
    /* 0x6ED4 */ s16 unk6ED4;
    /* 0x6ED6 */ s16 books3Count;
    /* 0x6ED8 */ s16 books4Count;
    /* 0x6EDA */ s16 bookSlotType;   /* 5F57B0 */
    /* 0x6EDC */ s16 unk6EDC;
    /* 0x6EDE */ s16 unk6EDE;
    /* 0x6EE0 */ s16 unk6EE0;
    /* 0x6EE2 */ s16 unk6EE2;
    /* 0x6EE4 */ ItemShopChr shopChrA[6];
    /* 0x72EC */ ItemShopChr shopChrB[6];
    /* 0x76F4 */ s16 unk76F4;
    /* 0x76F6 */ u8 pad76F6[2];
    /* 0x76F8 */ union {
        ItemRec recs[24];               /* 0x005F5548, 0x0063AB88 */
        struct {
            u8 pad0[0x1A4];
            u8 links[9];                /* 0x005F4778, 0x00637C80 (junction recipes) */
            u8 grid[6][7];              /* flat [col * 7 + row] in the readers */
            u8 grid2[6][7];
            u8 pad1[1];
            u16 unk78FA;
            ItemJuncEntry ents[43];
        } junc;
    } u76F8;
    /* 0x93DC */ u8 pad93DC[2];
    /* 0x93DE */ u16 order[2048];       /* sort order of the list being built */
    /* 0xA3DE */ u8 padA3DE[2];
    /* 0xA3E0 */ u64 keys[2048];        /* sort keys */
    /* 0xE3E0 */ u8 shopNew[0x800];  /* per item id (5F2E38) */
} ItemWork; /* size 0xEBE0 (ItemWorkInit) */

/* ---------------------------------------------------------------- globals */

extern FldMenuWork *fldMenuWork;
extern FldMenuWin *fldMenuWins;       /* 18 windows */
extern FldMenuCursor *fldMenuCursor;
extern ItemWork *itemWork;

#endif /* FIELD_MENU_H */
