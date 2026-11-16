/* Port-only C for ItemInvCompareListBuild (0x005EC0C8), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "party.h"
#include "field/menu.h"

/*
 * Lists the bag (key items left out) into itemWork->shop[] and, for each
 * piece of equipment, compares every character's atk / def / mov with the
 * item equipped against the current values: stats[c] = 1 better, 0 same,
 * -1 worse. The characters' equipment is put back afterwards.
 */
void ItemInvCompareListBuild(void) {
    s16 atk[8];
    s16 def[8];
    s16 mov[8];
    s16 equip[8][6];
    s32 i;
    s32 j;
    s32 c;
    s32 n;
    u16 ord;
    s16 id;
    s8 count;
    s16 r;
    ItemShopRec *rec;
    PtyEquipData *eq;

    for (i = 0; i < 8; i++) {
        PtyUnitInitFromChar(&ptyCharCalcUnits[i], i + 0x1001);
        PtyUnitCalcParams(&ptyCharCalcUnits[i], NULL, NULL);
        atk[i] = ptyCharCalcUnits[i].params[0];
        def[i] = ptyCharCalcUnits[i].params[1];
        mov[i] = ptyCharCalcUnits[i].params[5];
        for (j = 0; j < 6; j++) {
            equip[i][j] = ptyCharData[i].equip[j];
        }
    }
    ItemSortInventory();
    n = 0;
    /* Carried over between items: the type-10 case stores at stats[c]
       with c left from the last compare loop (0 before any, else 8, the
       record's padding at +0x2A) */
    c = 0;
    for (i = 0; i < 30; i++) {
        ord = itemWork->order[i];
        id = ptyPartyData->bag[ord].id;
        if (id == 0) {
            continue;
        }
        count = ptyPartyData->bag[ord].count;
        if (count == 0) {
            continue;
        }
        if (ptyItemTable[id].type == PTY_ITEM_KEY) {
            continue;
        }
        r = ItemGetListCategory(0, id);
        rec = &itemWork->shop[n];
        rec->count = count;
        rec->mask = 0;
        rec->unk8 = r;
        ItemInfoFill((ItemBagRec *)rec, (u16)id, (s16)ord);
        eq = ptyItemTable[id].equip;
        if (eq == NULL) {
            /* not counted: the next item overwrites this record */
            continue;
        }
        rec->mask = (u8)eq->charMask;
        for (j = 0; j < 8; j++) {
            if (!((eq->charMask >> j) & 1)) {
                continue;
            }
            switch ((s8)(ptyItemTable[id].type - 5)) {
            case 0:
                ptyCharData[j].equip[0] = id;
                break;
            case 1:
                ptyCharData[j].equip[1] = id;
                break;
            case 2:
                ptyCharData[j].equip[2] = id;
                break;
            case 3:
                ptyCharData[j].equip[3] = id;
                break;
            case 4:
                ptyCharData[j].equip[4] = id;
                break;
            case 5:
                ptyCharData[j].equip[5] = id;
                break;
            }
            PtyUnitCalcParams(&ptyCharCalcUnits[j], NULL, NULL);
            switch ((s8)(ptyItemTable[id].type - 5)) {
            case 0:
                for (c = 0; c < 8; c++) {
                    if (atk[c] < ptyCharCalcUnits[c].params[0]) {
                        r = 1;
                    } else {
                        r = (atk[c] == ptyCharCalcUnits[c].params[0]) ? 0 : -1;
                    }
                    rec->stats[c] = r;
                }
                break;
            case 1:
            case 2:
            case 3:
                for (c = 0; c < 8; c++) {
                    if (def[c] < ptyCharCalcUnits[c].params[1]) {
                        r = 1;
                    } else {
                        r = (def[c] == ptyCharCalcUnits[c].params[1]) ? 0 : -1;
                    }
                    rec->stats[c] = r;
                }
                break;
            case 4:
                for (c = 0; c < 8; c++) {
                    if (mov[c] < ptyCharCalcUnits[c].params[5]) {
                        r = 1;
                    } else {
                        r = (mov[c] == ptyCharCalcUnits[c].params[5]) ? 0 : -1;
                    }
                    rec->stats[c] = r;
                }
                break;
            case 5:
                /* c is 0 or 8 here; 8 is the s16 at +0x2A after stats[] */
                *(s16 *)((u8 *)rec + 0x1A + c * 2) = 0;
                break;
            }
        }
        n++;
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 6; j++) {
            ptyCharData[i].equip[j] = equip[i][j];
        }
    }
    itemWork->unk110E = n;
}
