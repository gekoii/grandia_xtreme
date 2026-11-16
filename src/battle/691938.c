#include "common.h"
#include "protos/battle.h"
#include "pad.h"
#include "battle/n29.h"
#include "party.h"


s32 BtlCmdListInput(void) {
    BtlCmdList *p = &btlCmdList;
    Pad *pad = padPort0;
    s8 row = p->row;
    BtlUnit *chr = p->unit;
    s16 pos = row * 6 + p->col;
    BtlCmdListCell *c;
    BtlCmdListCellAt *v;
    s8 k;
    s8 j;
    s8 m;
    s32 id;

    if (chr == NULL) {
        return -1;
    }
    if ((p->state & 0xF0) != 0x10) {
        return 0;
    }
    if (pad->trigger & 0xC) {
        if (pad->trigger & 4) {
            do {
                if (--p->row < 0) {
                    p->row = p->count - 1;
                }
            } while (p->rowState[p->row] < 0);
            p->unk1C = -1;
            MenuLrArrowsFlash(1);
        } else {
            do {
                if (++p->row >= p->count) {
                    p->row = 0;
                }
            } while (p->rowState[p->row] < 0);
            p->unk1C = 1;
            MenuLrArrowsFlash(2);
        }
        if (p->alive < 0) {
            j = p->row * 6 + p->col;
            while (p->cells[j].flags == 0) {
                if (--j < 0) {
                    j = 35;
                }
            }
            p->row = j / 6;
            p->col = j % 6;
        } else {
            p->col = 0;
            id = p->unit->skillId;
            k = p->row * 6;
            v = BTL_CMD_LIST_CELL_AT(p, k);
            while (v->cell.id != id) {
                v = BTL_CMD_LIST_CELL_AT(v, 1);
                k++;
            }
            p->col = k % 6;
        }
        if (p->row != row) {
            SndSePlay3D(0x7D5, 0, 0, NULL);
            p->state = 0x20;
            MenuCursorMoveTo(btlCmdListX + p->col * -8.0f - 16.0f, btlCmdListY + p->col * 28.0f - 12.0f);
            if (p->mode == 3) {
                BtlCmdListComboPartnersShow();
            }
        }
    } else {
        if (pad->repeat & 0x5000) {
            SndSePlay3D(0x7D1, 0, 0, NULL);
            if (p->alive < 0) {
                if (pad->repeat & 0x1000) {
                    if (--p->col < 0) {
                        m = p->row * 6 + 5;
                        while (p->cells[m].flags == 0) {
                            m--;
                        }
                        p->col = m % 6;
                    }
                } else {
                    if (++p->col >= 6 || p->cells[p->row * 6 + p->col].flags == 0) {
                        p->col = 0;
                    }
                }
                if (p->mode == 3) {
                    BtlCmdListComboPartnersShow();
                }
            }
            MenuCursorMoveTo(btlCmdListX + p->col * -8.0f - 16.0f, btlCmdListY + p->col * 28.0f - 12.0f);
        }
        if (pad->trigger & 0x60) {
            if (pad->trigger & 0x40) {
                SndSePlay3D(0x7D3, 0, 0, NULL);
                p->state = 0x40;
                MenuHelpClose();
                if (p->mode == 3) {
                    IpComboPreviewSet(NULL);
                }
                return -1;
            }
            if (p->cells[p->row * 6 + p->col].flags == 0 || (p->cells[p->row * 6 + p->col].flags & 2)) {
                SndSePlay3D(0x7D4, 0, 0, NULL);
            } else {
                SndSePlay3D(0x7D2, 0, 0, NULL);
                p->state = 0x30;
                MenuHelpClose();
                if (p->mode == 1) {
                    if (p->rowState[p->row] == p->mode) {
                        btlCmdListReservedItem[chr->id] = -1;
                        chr->skillId = ptyCharData[chr->params->id - 0x1001].equip[p->cells[p->row * 6 + p->col].id];
                        return 1;
                    } else {
                        s8 *s;
                        btlCmdListReservedItem[chr->id] = p->cells[p->row * 6 + p->col].id;
                        s = &btlCmdListReservedItem[chr->id];
                        chr->skillId = ptyPartyData->bag[*s].id;
                        if (ptyItemTable[chr->skillId].equip != NULL) {
                            chr->skillId = *s;
                            *s = -1;
                            return 2;
                        }
                        return 1;
                    }
                } else if (p->mode == 2) {
                    chr->skillId = p->cells[p->row * 6 + p->col].id;
                    chr->eggNo[chr->combo - 1] = p->rowState[p->row];
                    chr->eggMagic[chr->combo - 1] = p->col;
                    return 3;
                } else if (p->mode == 3) {
                    c = &p->cells[p->row * 6 + p->col];
                    chr->skillId = ptyPartyData->moveLevels[(chr->params->id - 0x1001) * 5 + c->id].moveId;
                    chr->skillLevel = p->cells[p->row * 6 + p->col].level;
                    IpComboPreviewSet(NULL);
                    return 4;
                }
            }
        } else if ((pad->trigger & 0x2000) && (p->cells[p->row * 6 + p->col].flags & 4)) {
            SndSePlay3D(0x7D2, 0, 0, NULL);
            chr->skillId = p->cells[p->row * 6 + p->col].id;
            chr->eggNo[chr->combo - 1] = p->rowState[p->row];
            chr->eggMagic[chr->combo - 1] = p->col;
            if (chr->combo < chr->params->multiMagic && p->alive != 0) {
                BtlCmdListMultiCastNext();
                chr->combo++;
                p->state = 0x20;
                MenuCursorMoveTo(btlCmdListX + p->col * -8.0f - 16.0f, btlCmdListY + p->col * 28.0f - 12.0f);
            } else {
                p->state = 0x30;
                MenuHelpClose();
                return 3;
            }
        }
    }
    if (pad->trigger & 0x10) {
        p->helpSide = p->helpSide != 1;
        BtlCmdListHelpShow();
        SndSePlay3D(0x7D5, 0, 0, NULL);
        return 0;
    }
    if (pos != p->row * 6 + p->col) {
        BtlCmdListHelpShow();
    }
    return 0;
}
