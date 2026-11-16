/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"

extern u8 *btlCmdListEquipSlotNames[];
extern u8 btlTargetAreaNames[][13];
extern u8 btlStrMp[];
extern u8 btlStrSp[];
extern u8 btlCmdListItemPageNames[][13];
extern u8 btlCmdListMovePageNames[][13];

f32 VuSinAngle(s32);

void BtlCmdListUpdate(void) {
    BtlCmdList *p = &btlCmdList;
    BtlCmdListEntry *e;
    BtlCmdListCell *c;
    PtyItem *it;
    PtyItemUse *ef;
    PtyMove *rf;
    s8 i;
    s8 k;
    s16 n;
    f32 fa;
    f32 fb;
    f32 f;
    s8 s;
    s8 *f8;
    s8 *g8;

    if (p->unit == NULL) {
        return;
    }
    switch (p->state) {
    case 0:
        p->state = 1;
        p->timer = btlFrameCount;
        break;
    case 1:
        if (p->timer == btlFrameCount) {
            break;
        }
        if (p->unk22 == 0) {
            p->unk22 = 1;
        }
        f8 = p->pageShown;
        f8[p->side] = 1;
        TxtGlyphBufReset(&btlCmdListGlyphBufs[p->side]);
        if (p->mode == 1) {
            TxtObjSetString(&p->titles[p->side], &btlCmdListGlyphBufs[p->side], btlCmdListItemPageNames[p->row]);
        } else if (p->mode == 2) {
            TxtObjSetString(&p->titles[p->side], &btlCmdListGlyphBufs[p->side],
                          ptyItemTable[ptyPartyData->eggs[p->rowState[p->row]].itemId].name);
        }
        if (p->mode == 3) {
            TxtObjSetString(&p->titles[p->side], &btlCmdListGlyphBufs[p->side], btlCmdListMovePageNames[p->row]);
        }
        k = p->row * 6;
        e = p->entries[p->side];
        fa = btlCmdListX;
        fb = btlCmdListY;
        for (i = 0; i < 6; i++) {
            c = &p->cells[i + k];
            e->cell = c;
            e->flags = c->flags;
            e->unk2 = 0x10;
            e->alpha = 0.0f;
            e->type = 1;
            e->unk10[2] = fa;
            e->x = e->unk10[0] = fa + (i * 8.0f + 64.0f) * p->unk1C;
            e->unk10[1] = fb;
            e->y = fb;
            e->unk10[3] = fb;
            e->unk10[4] = 0.0f;
            e->playerPage = 1;
            if (e->flags != 0) {
                TxtObjSetString(&e->texts[0], &btlCmdListGlyphBufs[p->side], c->name);
                if (p->mode == 1) {
                    e->quad.w = 0x170;
                    TxtObjSetString(&e->texts[2], &btlCmdListGlyphBufs[p->side], p->rowState[p->row] ? btlCmdListEquipSlotNames[i] : NULL);
                    if (e->cell->name != NULL) {
                        if (p->rowState[p->row] == 0) {
                            it = &ptyItemTable[ptyPartyData->bag[e->cell->id].id];
                        } else {
                            it = &ptyItemTable[ptyCharData[p->unit->params->id - 0x1001].equip[e->cell->id]];
                        }
                        ef = it->use;
                        TxtObjSetString(&e->texts[1], &btlCmdListGlyphBufs[p->side],
                                      (ef != NULL && ef->target > 0) ? btlTargetAreaNames[ef->target] : NULL);
                        if (ef != NULL && (p->rowState[p->row] != 0 || it->equip == 0)) {
                            if (ef->unk1 == 2 || ef->unk1 == 4) {
                                BtlCmdListCellColorSet(&e->quad, 0xF800, i, 0.4f);
                            } else {
                                BtlCmdListCellColorSet(&e->quad, 0xA000, i, 0.1f);
                            }
                        } else {
                            BtlCmdListCellColorSet(&e->quad, 0x4000, i, 0.1f);
                        }
                    } else {
                        TxtObjSetString(&e->texts[1], &btlCmdListGlyphBufs[p->side], NULL);
                        BtlCmdListCellColorSet(&e->quad, 0x4000, i, 0.1f);
                    }
                } else if ((u8)(p->mode - 2) < 2) {
                    if (p->mode == 2) {
                        rf = &ptyMoveTable[e->cell->id];
                    } else {
                        rf = &ptyMoveTable[ptyPartyData->moveLevels[(p->unit->params->id - 0x1001) * 5 + e->cell->id].moveId];
                    }
                    TxtObjSetString(&e->texts[1], &btlCmdListGlyphBufs[p->side], rf->target > 0 ? btlTargetAreaNames[rf->target] : NULL);
                    if (rf->unk1 == 2 || rf->unk1 == 4) {
                        BtlCmdListCellColorSet(&e->quad, 0xF800, i, 0.4f);
                    } else {
                        BtlCmdListCellColorSet(&e->quad, 0xA000, i, 0.1f);
                    }
                    e->quad.w = 0x146;
                    TxtObjSetString(&e->texts[3], &btlCmdListGlyphBufs[p->side], p->mode != 2 ? btlStrSp : btlStrMp);
                }
            }
            fa += -8.0f;
            fb += 28.0f;
            e++;
        }
        p->state = 0x10;
        break;
    case 0x10:
        break;
    case 0x20:
        p->side ^= 1;
        f8 = p->pageShown;
        f8[p->side] = 0;
        {
            BtlCmdListEntry *q = p->entries[p->side ^ 1];
            s8 j;
            for (j = 0; j < 6; j++) {
                q->type = 2;
                q->unk2 = 8;
                q->unk10[4] = p->unk1C * -12.0f;
                q++;
            }
        }
        p->state = 0;
        break;
    case 0x30:
        {
            BtlCmdListEntry *q = p->entries[p->side];
            s8 j;
            for (j = 0; j < 6; j++) {
                if (j == p->col) {
                    q->type = 1;
                    q->unk2 = 0x10;
                    q->unk10[0] = q->x;
                    q->unk10[1] = q->y;
                    q->unk10[2] = 32.0f;
                    q->unk10[3] = 264.0f;
                    q->quad.w = q->texts[0].width + 28.0f;
                    if (p->unit->combo >= 2) {
                        q->quad.w = q->quad.w + 24.0f;
                    }
                    q->playerPage = 0;
                } else {
                    q->type = 2;
                    q->unk2 = 0x18;
                    q->unk10[4] = 3.0f;
                }
                q++;
            }
        }
        if (p->mode == 1 && p->rowState[p->row] == 0 &&
            ptyItemTable[ptyPartyData->bag[p->entries[p->side][p->col].cell->id].id].equip != 0) {
            f8 = p->pageShown;
            f8[p->side ^ 1] = 0;
            p->timer = btlFrameCount;
            p->state++;
        } else {
            p->state += 3;
        }
        break;
    case 0x31:
        if (p->timer == btlFrameCount) {
            break;
        }
        f8 = p->pageShown;
        f8[p->side ^ 1] = 1;
        TxtGlyphBufReset(&btlCmdListGlyphBufs[p->side ^ 1]);
        TxtObjSetString(&p->titles[p->side ^ 1], &btlCmdListGlyphBufs[p->side ^ 1], NULL);
        {
            BtlCmdListEntry *r;
            BtlCmdListCell *cc;
            u8 fl2;
            r = p->entries[p->side ^ 1];
            n = (s8)ptyItemTable[ptyPartyData->bag[p->cells[p->row * 6 + p->col].id].id].type - 5;
            cc = &p->cells[n];
            r->cell = cc;
            fl2 = cc->flags;
            r->type = 3;
            r->unk2 = 0x10;
            r->flags = fl2 | 2;
            r->playerPage = 0;
            r->unk10[2] = 12.0f;
            r->alpha = 0.0f;
            r->unk10[0] = -52.0f;
            r->unk10[1] = 216.0f;
            r->x = -52.0f;
            r->y = 216.0f;
            r->unk10[3] = 216.0f;
            r->unk10[4] = 0.0f;
            TxtObjSetString(&r->texts[0], &btlCmdListGlyphBufs[p->side ^ 1], cc->name);
            r->quad.w = r->texts[0].width + 28.0f;
            TxtObjSetString(&r->texts[2], &btlCmdListGlyphBufs[p->side ^ 1], btlCmdListEquipSlotNames[n]);
            TxtObjSetString(&r->texts[1], &btlCmdListGlyphBufs[p->side ^ 1], NULL);
            BtlCmdListCellColorSet(&r->quad, 0x4000, 0, 0.1f);
            r++;
            {
                s8 j;
                for (j = 1; j < 6; j++, r++) {
                    r->cell = NULL;
                    r->flags = 0;
                }
            }
        }
        btlCmdListSwapArrowAnim = -0x10;
        p->state++;
        break;
    case 0x32:
        if (++btlCmdListSwapArrowAnim >= 0x30) {
            btlCmdListSwapArrowAnim = 0;
        }
        break;
    case 0x40:
        {
            BtlCmdListEntry *q = p->entries[p->side];
            s8 j;
            for (j = 0; j < 6; j++) {
                q->type = 2;
                q->unk2 = 0x18;
                if (j & 1) {
                    q->unk10[4] = 3.0f;
                } else {
                    q->unk10[4] = -3.0f;
                }
                q++;
            }
        }
        p->state++;
        break;
    case 0x41:
        if (p->entries[p->side][0].alpha <= 0.0f) {
            p->state = 0xF0;
        }
        break;
    case 0xF0:
        p->state = 0xF1;
        p->timer = btlFrameCount;
        break;
    case 0xF1:
        if (p->timer != btlFrameCount) {
            p->unit = NULL;
        }
        break;
    }
    {
        s8 j;
        e = p->entries[0];
        for (j = 0; j < 12; j++) {
            if (e->type != 0) switch (e->type) {
            case 1:
                BtlCmdListCellSlideIn(e, p->col == j % 6);
                break;
            case 2:
                BtlCmdListCellSlideOut(e);
                break;
            case 3:
                BtlCmdListCellSlideIn(e, 0);
                break;
            }
            e++;
        }
    }
    s = p->side ^ 1;
    g8 = p->pageShown;
    if (g8[s] != 0 && p->entries[s][0].alpha <= 0.0f) {
        g8[s] = 0;
    }
    if (p->unk22 > 0) {
    if (p->unk22 < 9) {
        if (p->unk22 == 8) {
            p->titleSlide = p->rightSlide = p->leftSlide = 0.0f;
        } else {
            f = 1.0f - VuSinAngle(p->unk22 << 11);
            p->titleSlide = f * 200.0f;
            p->rightSlide = f * -160.0f;
            p->leftSlide = f * -80.0f;
        }
        p->unk23 = p->unk22 << 4;
        p->unk22++;
    }
    }
}
