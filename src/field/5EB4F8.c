#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/menu.h"

extern u8 itemEquipSlotOrder[];
extern u8 itemEquipSlotType[];

s32 ItemEquipListBuild(u16 cat, s16 chr) {
    s32 type;
    s32 u;
    s16 cur;
    s16 n;
    s32 mask;
    s32 slot;
    s32 i;
    s16 id;
    PtyItem *d;
    s32 unk1;
    s32 unkC;
    PtyUnit *un;
    u16 *pm;
    s32 v64;
    s32 v50;
    s32 v54;
    s32 v58;
    s32 v5C;
    s32 v60;

    if (cat >= 6 || chr >= 8) {
        return -1;
    }
    mask = 1 << chr;
    if (cat != 0 && ItemCharHasEquipAllSkill(chr) != 0) {
        mask = 0xFF;
    }
    slot = itemEquipSlotOrder[cat];
    u = PtyPartyGetSlot(chr);
    itemWork->curChr = chr;
    n = 0;
    itemWork->curSlot = slot;
    cur = ptyCharData[chr].equip[slot];
    unk1 = ptyItemTable[cur].icon;
    unkC = ptyItemTable[cur].name;
    itemWork->cur = cur;
    itemWork->unk6C65 = unk1;
    itemWork->unk6C68 = unkC;
    type = itemEquipSlotType[slot];
    ItemSortInventory();
    for (i = 0; i < 30; i++) {
        id = ptyPartyData->bag[itemWork->order[i]].id;
        if (id != 0) {
            d = (PtyItem *)((id << 5) + (u32)ptyItemTable);
            if (d->type == type) {
                pm = &d->equip->charMask;
                if (mask == 0xFF || (*pm & mask)) {
                    unk1 = d->icon;
                    unkC = d->name;
                    ptyCharData[chr].equip[slot] = id;
                    PtyUnitCalcParams(&ptyPartyUnits[u], 0, 0);
                    un = (PtyUnit *)(u * 0x318 + (u32)ptyPartyUnits);
                    v50 = (s16)un->params[0];
                    v54 = (s16)un->params[1];
                    v58 = (s16)un->params[2];
                    v5C = (s16)un->params[3];
                    v60 = (s16)un->params[4];
                    v64 = (s16)un->params[5];
                    itemWork->list[n].name = unkC;
                    itemWork->list[n].def.item = d;
                    itemWork->list[n].id = id;
                    itemWork->list[n].icon = unk1;
                    itemWork->list[n].stats[0] = v50;
                    itemWork->list[n].stats[1] = v54;
                    itemWork->list[n].stats[2] = v58;
                    itemWork->list[n].stats[3] = v5C;
                    itemWork->list[n].stats[4] = v60;
                    itemWork->list[n].stats[5] = v64;
                    n++;
                }
            }
        }
    }
    ptyCharData[chr].equip[slot] = cur;
    PtyUnitCalcParams(&ptyPartyUnits[u], 0, 0);
    itemWork->curCount = n;
    itemWork->unk6C63 = 0;
    return n;
}
