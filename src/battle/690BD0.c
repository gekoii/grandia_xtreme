/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "party.h"

#define CEIL(x) ((x) == (s32)(x) ? (x) : ((x) > 0 ? (s32)(x) + 1 : ~(s32)(-x)))

extern u8 btlCmdListStrNoEquip[];
extern u8 btlCmdListStrNoMove[];


void BtlCmdListBuild(void) {
    BtlCmdList *p;
    s32 i;

    p = &btlCmdList;
    {
        BtlCmdListCell *c = &p->cells[0];
        for (i = 36; i > 0; i--) {
            c->flags = 0;
            c->unk3 = 0;
            c->name = NULL;
            c->value = -1;
            c++;
        }
    }
    for (i = 5; i >= 0; i--) {
        p->rowState[i] = -1;
        p->rowIcon[i] = 0;
    }
    if (p->mode == 1) {
        BtlCmdListCell *c;
        s16 order[30];
        s32 a;
        s32 j;
        s32 k;
        s32 v;
        s8 n;
        s32 y;

        p->rowIcon[0] = 0x23;
        p->rowState[0] = 1;
        c = &p->cells[0];
        for (j = 0; j < 6; j++) {
            s32 id = ptyCharData[p->unit->params->id - 0x1001].equip[j];
            c->id = j;
            c->flags = 1;
            if (id > 0) {
                c->name = (u8 *)ptyItemTable[id].name;
                c->icon = ptyItemTable[id].icon;
                if (ptyItemTable[id].use == 0) {
                    c->flags = 3;
                }
            } else {
                c->name = btlCmdListStrNoEquip;
                c->icon = -1;
                c->flags = 3;
            }
            c++;
        }
        for (a = 0; a < 30; a++) {
            order[a] = -1;
        }
        for (a = 0; a < 30; a++) {
            if (ptyPartyData->bag[a].count > 0) {
                v = ptyItemSortKeys[ptyPartyData->bag[a].id];
                for (y = 0; y < 30; y++) {
                    if (order[y] < 0) {
                        order[y] = a;
                        break;
                    }
                    if (v < ptyItemSortKeys[ptyPartyData->bag[order[y]].id]) {
                        for (k = 29; k >= y && order[k] < 0; k--) {
                        }
                        for (; k >= y; k--) {
                            order[k + 1] = order[k];
                        }
                        order[y] = a;
                        break;
                    }
                }
            }
        }
        c = &p->cells[6];
        for (a = 0; a < 30; a++) {
            PtyItemSlot *it = &ptyPartyData->bag[order[a]];
            s32 cnt = it->count;
            PtyItem *d;
            s32 m;
            for (m = 0; m < 4; m++) {
                if (btlCmdListReservedItem[m] == order[a]) {
                    cnt--;
                }
            }
            if (cnt > 0) {
                d = &ptyItemTable[it->id];
                c->flags = 1;
                c->id = order[a];
                c->name = (u8 *)d->name;
                c->icon = d->icon;
                if (d->equip != NULL) {
                    if (d->type == 5) {
                        if (!((ptyItemTable[it->id].equip->charMask >> (p->unit->params->id - 0x1001)) & 1)) {
                            c->flags = 3;
                        }
                    } else if (d->type == 7 || d->type == 10) {
                        if (!(p->unit->params->bitCsf & 0x1000000)) {
                            if (!((ptyItemTable[it->id].equip->charMask >> (p->unit->params->id - 0x1001)) & 1)) {
                                c->flags = 3;
                            }
                        }
                    } else {
                        c->flags = 3;
                    }
                } else if (d->use == 0 || d->type == 4) {
                    c->flags = 3;
                }
                c->value = cnt;
                c++;
            }
        }
        p->count = 1;
        for (n = 1; n < 6 && p->cells[n * 6].flags != 0; n++) {
            p->rowState[n] = 0;
            p->rowIcon[n] = 0x40;
            p->count++;
        }
    } else if (p->mode == 2) {
        BtlCmdListCell *c;
        PtyManaEgg *g;
        PtyMove *t;
        s32 b;
        s32 j;

        p->count = ptyCharDefTable[p->unit->params->id - 0x1001].eggSlots;
        for (b = 0; b < p->count; b++) {
            p->rowState[b] = ptyCharData[p->unit->params->id - 0x1001].slots[b];
            if ((u8)p->rowState[b] >= 0x18) {
                p->rowState[b] = -1;
                p->rowIcon[b] = 0x4E;
            } else {
                g = &ptyPartyData->eggs[p->rowState[b]];
                p->rowIcon[b] = ptyItemTable[g->itemId].icon;
                c = &p->cells[b * 6];
                for (j = 0; j < 6 && g->spells[j] > 0; j++) {
                    c->flags = 1;
                    c->id = g->spells[j];
                    t = (PtyMove *)(c->id * 32 + (u32)ptyMoveTable);
                    c->name = (u8 *)t->name;
                    c->icon = t->icon;
                    c->unk3 = g->spellUpgrades[j];
                    c->value = BtlUnitManaEggSpellCost(p->unit, p->rowState[b], j);
                    if (g->mp < c->value) {
                        c->flags |= 2;
                    }
                    c++;
                }
            }
        }
        if (p->unit->params->multiMagic >= 2) {
            for (b = 0; b < p->count; b++) {
                if (p->rowState[b] >= 0) {
                    s32 k;
                    BtlCmdListCell *c2;
                    c2 = &p->cells[b * 6];
                    for (k = 5; k >= 0; k--) {
                        if (c2->flags != 0 && !(c2->flags & 2) && BtlCmdListSpellEggCount(c2->id) >= 2) {
                            c2->flags |= 4;
                        }
                        c2++;
                    }
                }
            }
        }
    } else if (p->mode == 3) {
        BtlCmdListCell *c;
        PtyMoveLevel *e;
        PtyMove *t;
        s32 j;
        s8 n;
        s32 f;

        c = &p->cells[0];
        e = &ptyPartyData->moveLevels[(p->unit->params->id - 0x1001) * 5];
        p->rowState[0] = 0;
        p->rowIcon[0] = p->unit->params->id + 0x14;
        for (j = 0; j < 5; j++) {
            c->id = j;
            c->flags = 1;
            if ((c->level = e->level) != 0) {
                PtyMove *t1 = &ptyMoveTable[e->moveId];
                c->name = (u8 *)t1->name;
                c->icon = t1->icon;
                c->value = CEIL(t1->levels[c->level - 1].cost * p->unit->params->spcSpRate);
                if (p->unit->params->sp < c->value) {
                    c->flags |= 2;
                }
                if (e->level >= e->maxLevel) {
                    c->flags |= 8;
                }
            } else {
                c->name = btlCmdListStrNoMove;
                c->icon = -1;
                c->flags = 3;
            }
            c++;
            e++;
        }
        c = &p->cells[6];
        e = &ptyPartyData->moveLevels[40];
        for (j = 0; j < 50; j++) {
            if (e->level != 0) {
                t = &ptyMoveTable[e->moveId];
                c->level = e->level;
                c->value = CEIL(t->levels[c->level - 1].cost * p->unit->params->spcSpRate);
                f = BtlCmdListComboCheck(t, c->value, p->unit->def->id - 0x1000);
                c->flags = f;
                if ((u8)f) {
                    if (e->level >= e->maxLevel) {
                        c->flags = f | 8;
                    }
                    c->id = j + (0x1009 - p->unit->params->id) * 5;
                    c->name = (u8 *)t->name;
                    c->icon = t->icon;
                    c++;
                }
            }
            e++;
        }
        p->count = 1;
        for (n = 1; n < 6 && p->cells[n * 6].flags != 0; n++) {
            p->rowState[n] = 1;
            p->rowIcon[n] = 0x4F;
            p->count++;
        }
    }
}
