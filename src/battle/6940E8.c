/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "sys.h"
#include "battle/n29.h"

extern u8 btlCmdListNumCount[];
extern SprRect menuNumGlyphTimes;


void BtlCmdListDrawPage(s8 arg0) {
    s32 k1;
    s32 k0;
    s32 j2;
    BtlCmdListEntry *p3;
    BtlCmdListEntry *p5;
    s32 k2;
    BtlCmdListEntry *p1;
    BtlCmdListEntry *p2;
    s32 j;
    s32 t;
    BtlCmdListEntry *p4;
    BtlCmdListCell *u3;
    BtlCmdListCell *u4;
    BtlCmdListCell *u5;
    u8 fl;
    f32 s;
    f32 lim;
    BtlCmdList *b = &btlCmdList;

    SprBegin(0.0f, 0.0f);
    p1 = b->entries[(s8)(arg0 ^ b->side)];
    for (j = 0; j < 6; j++, p1++) {
        if (p1->flags) {
            sprState.ox = p1->x;
            sprState.oy = p1->y;
            sprState.col[0].a = p1->alpha * 128.0f;
            SprDrawColorQuad(&p1->quad, p1->alpha);
        }
    }
    SprFlush();

    GphTexBind(0xEB);
    p2 = b->entries[(s8)(arg0 ^ b->side)];
    for (k1 = 0; k1 < 6; k1++, p2++) {
        if (p2->flags && p2->cell->value >= 0) {
            sprState.oy = p2->y;
            sprState.col[0].a = p2->alpha * 128.0f;
            if (!p2->playerPage) {
                if (b->unit->combo >= 2) {
                    sprState.ox = p2->x + 28.0f + (f32)p2->texts[0].width + 28.0f;
                    SprDrawRect(&menuNumGlyphTimes, -0x1B, -0x16);
                    MenuNumberDraw(btlCmdListNumCount, b->unit->combo);
                }
            } else {
                switch (b->mode) {
                case 1:
                    sprState.ox = p2->x + 308.0f;
                    SprDrawRect(&menuNumGlyphTimes, -0x24, -0x16);
                    break;
                case 2:
                    sprState.ox = p2->x + 248.0f;
                    break;
                case 3:
                    sprState.ox = p2->x + 248.0f;
                    break;
                }
                if ((p2->cell->unk3 & 1) && (btlTickCount & 4)) {
                    sprState.col[0].r = 0x60;
                    sprState.col[0].g = 0x40;
                    MenuNumberDraw(btlCmdListNumCount, p2->cell->value);
                    sprState.col[0].g = 0x80;
                    sprState.col[0].r = 0x80;
                } else {
                    MenuNumberDraw(btlCmdListNumCount, p2->cell->value);
                }
            }
        }
    }
    SprFlush();

    TxtGlyphBufUpload(&btlCmdListGlyphBufs[(s8)(arg0 ^ b->side)]);
    p3 = b->entries[(s8)(arg0 ^ b->side)];
    for (k0 = 0; k0 < 6; k0++, p3++) {
        if (p3->flags) {
            SprState *d = &sprState;
            d->ox = p3->x + 28.0f;
            d->col[0].a = p3->alpha * 128.0f;
            d->oy = p3->y;
            s = 1.0f;
            if (p3->playerPage && b->col == k0 % 6) {
                s = 1.2f;
            }
            lim = 180.0f;
            d->sy = s;
            d->sx = s;
            if (b->mode == 1) {
                lim = 240.0f;
            }
            if (p3->texts[0].width * sprState.sx > lim) {
                sprState.sx = lim / p3->texts[0].width;
            }
            MenuTextSetAddColor(0xC, 0xC, 0x14);
            if (p3->flags & 2) {
                sprState.col[0].b = 0x40;
                sprState.col[0].g = 0x40;
                sprState.col[0].r = 0x40;
                MenuTextDraw(&p3->texts[0]);
                sprState.col[0].b = 0x80;
                sprState.col[0].g = 0x80;
                sprState.col[0].r = 0x80;
            } else {
                if (p3->playerPage && (u3 = p3->cell)->unk3) {
                    if (u3->unk3 & 0x20) {
                        if (btlTickCount & 2) {
                            MenuTextSetAddColor(8, 8, 0x40);
                        } else {
                            MenuTextSetAddColor(0x20, 0x20, 0x40);
                        }
                    }
                    u3 = p3->cell;
                    fl = u3->unk3;
                    if ((fl & 6) && (btlTickCount & 8)) {
                        if ((fl & 0x40) && (btlTickCount & 0x10)) {
                            sprState.col[0].b = 0;
                            sprState.col[0].r = 0x70;
                            sprState.col[0].g = 0x70;
                        } else {
                            sprState.col[0].g = 0x30;
                            sprState.col[0].b = 0x30;
                        }
                    } else if ((u3->unk3 & 0x40) && (btlTickCount & 8)) {
                        sprState.col[0].b = 0;
                        sprState.col[0].r = 0x70;
                        sprState.col[0].g = 0x70;
                    }
                }
                MenuTextDraw(&p3->texts[0]);
                sprState.col[0].b = 0x80;
                sprState.col[0].g = 0x80;
                sprState.col[0].r = 0x80;
                if (p3->playerPage && (p3->cell->unk3 & 8)) {
                    sprState.col[0].r = 0x60;
                    sprState.col[0].g = 0x20;
                    sprState.col[0].b = 0x60;
                    t = btlTickCount;
                    p3->texts[0].style &= ~2;
                    for (j2 = 0; j2 < 2; j2++) {
                        t = (t + 0x20) & 0x3F;
                        sprState.col[0].a = p3->alpha * 128.0f * ((f32)(0x40 - t) * 0.015625f);
                        sprState.ox = p3->x + 28.0f + 4.0f + (f32)t * 0.4f;
                        MenuTextDraw(&p3->texts[0]);
                    }
                    p3->texts[0].style |= 2;
                    sprState.col[0].b = 0x80;
                    sprState.col[0].g = 0x80;
                    sprState.col[0].r = 0x80;
                }
            }
        }
    }

    sprState.sy = 1.0f;
    sprState.sx = 1.0f;
    p4 = b->entries[(s8)(arg0 ^ b->side)];
    for (k2 = 0; k2 < 6; k2++, p4++) {
        if (p4->flags && p4->playerPage) {
            SprState *d = &sprState;
            d->oy = p4->y;
            d->col[0].a = p4->alpha * 128.0f;
            MenuTextSetAddColor(0x10, 0x18, 0x10);
            switch (b->mode) {
            case 1:
                d->ox = p4->x + 328.0f;
                MenuTextDraw(&p4->texts[1]);
                d->ox -= 32.0f;
                if (p4->texts[2].width * sprState.sx > 48.0f) {
                    sprState.sx = 48.0f / p4->texts[2].width;
                }
                MenuTextDraw(&p4->texts[2]);
                sprState.sx = 1.0f;
                break;
            case 2:
            case 3:
                u4 = p4->cell;
                if (u4->value >= 0) {
                    sprState.ox = p4->x + 286.0f;
                    if (u4->unk3 & 0x10) {
                        if (btlTickCount & 2) {
                            MenuTextSetAddColor(0, 0x38, 0);
                        } else {
                            MenuTextSetAddColor(0x10, 0x20, 0x10);
                        }
                    }
                    MenuTextDraw(&p4->texts[1]);
                    sprState.ox -= 24.0f;
                    sprState.oy += 8.0f;
                    if (b->mode == 2) {
                        MenuTextSetAddColor(0x10, 8, 0x30);
                    } else {
                        MenuTextSetAddColor(0x10, 0x28, 8);
                    }
                    MenuTextDraw(&p4->texts[3]);
                }
                break;
            }
        }
    }
    SprFlush();

    if (b->mode == 1) {
        GphTexBind(0xF0);
    } else if (b->mode > 0) {
        if (b->mode < 4) {
            GphTexBind(0xF1);
        }
    }
    p5 = b->entries[(s8)(arg0 ^ b->side)];
    for (k2 = 0; k2 < 6; k2++, p5++) {
        if (!p5->flags) {
            continue;
        }
        u5 = p5->cell;
        if (u5->icon < 0) {
            continue;
        }
        sprState.ox = p5->x;
        sprState.oy = p5->y;
        sprState.col[0].a = p5->alpha * 128.0f;
        switch (b->mode) {
        case 1:
            SprDrawRect((SprRect *)&menuItemIcons[u5->icon], 0, -0x18);
            break;
        case 3:
            SprDrawRect((SprRect *)&menuElementIcons[u5->icon], 0, -0x18);
            if (p5->playerPage && (btlTickCount & 0x40)) {
                sprState.sy = 0.9f;
                sprState.sx = 0.9f;
                sprState.ox = p5->x + 212.0f;
                for (t = 0; t < p5->cell->level; t++) {
                    SprDrawRect(&sprStarIconRects[0], -0x18, -0xE);
                    sprState.ox -= 12.0f;
                }
                if ((p5->cell->flags & 8) && !(btlTickCount & 0x10)) {
                    sprState.ox += (f32)p5->cell->level * 12.0f * 0.5f - sprState.sx * 5.0f;
                    SprDrawRect(&sprStarIconRects[1], -sprStarIconRects[1].w / 2, -0x10);
                }
                sprState.sy = 1.0f;
                sprState.sx = 1.0f;
            }
            break;
        case 2:
            SprDrawRect((SprRect *)&menuElementIcons[u5->icon], 0, -0x18);
            if (p5->playerPage && p5->cell->unk3 && (btlTickCount & 0x40)) {
                sprState.ox = p5->x + 212.0f;
                for (j = 0; j < 7; j++) {
                    if ((p5->cell->unk3 >> j) & 1) {
                        SprDrawRect((SprRect *)&fldMenuJunctionIcons[j], -0x14, -0x10);
                        sprState.ox -= 22.0f;
                    }
                }
            }
            break;
        }
    }
    SprFlush();
}
