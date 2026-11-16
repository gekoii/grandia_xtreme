/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"

#include "gph.h"
#include "battle/n30.h"
#include "party.h"

void BtlChrPartDirToWorld(f32 *, BtlUnit *, GphPart *);

void FxWeaponAuraUpdate(BtlUnit *u)
{
    FVECTOR pos;
    FVECTOR pos2;
    FxWeaponAura *p = &fxWeaponAura[u->id];
    BtlUnit *src;
    GphPart *node;
    FxWeaponAura *q; /* p shifted by unkC sprites: &p->sprites[unkC] schedules differently */
    f32 t;
    f32 x, y, z;

    if (u->id < 4) {
        if (p->unk14 <= 0.0f || (BtlChrFindPart(u->model, p->unk8)->flags & 8)) {
            if (u->params->id == 0x1007) {
                if (!(BtlChrFindPart(u->model, 0x302)->flags & 8)) {
                    p->unk14 = 0.5f;
                    p->unkA = 0x302;
                    p->unk8 = 0x302;
                } else {
                    p->unk14 = 0.0f;
                }
            } else if (!(BtlChrFindPart(u->model, 0x366)->flags & 8)) {
                p->unk14 = 0.5f;
                p->unkA = 0x366;
                p->unk8 = 0x366;
            } else if ((u->params->id == 0x1005 || u->params->id == 0x1006)
                       && !(BtlChrFindPart(u->model, 0x302)->flags & 8)) {
                p->unk14 = 0.5f;
                p->unkA = 0x302;
                p->unk8 = 0x302;
            } else {
                p->unk14 = 0.0f;
            }
        }
    }
    if (p->kind > 0 && u->state > 0) {
        if (u->state >= 0x40 && p->stop == 0
            && (p->unk6 == 0 || (u->state >= 0x80 && (btlActingUnit == 0xFF || (u->flags & 8))))) {
            if (u->body >= 0) {
                src = &btlUnits[u->body];
            } else {
                src = u;
            }
            p->unk10 += p->unk14;
            while (p->unk10 >= 1.0f) {
                if (p->pause == 1) {
                    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(pos) : "memory");
                    if (p->unk8 < 0) {
                        if (src->model == NULL || src->model->partCount < 2) {
                            node = NULL;
                        } else {
                            do {
                                node = &src->model->parts[(s32)((f32)(rand() >> 15) * (1.0f / 65536.0f) * 0.999f
                                                               * (f32)(src->model->partCount - 1) + 1.0f)];
                            } while (node->drawFlags & 1);
                        }
                        BtlChrPartToWorld(pos, src, node, 1);
                    } else if (p->unk8 == p->unkA) {
                        f32 t = (f32)(rand() >> 15) * (1.0f / 65536.0f);
                        pos[0] = p->seg0[0] + p->seg1[0] * t;
                        pos[1] = p->seg0[1] + p->seg1[1] * t;
                        pos[2] = p->seg0[2] + p->seg1[2] * t;
                        node = BtlChrFindPart(src->model, p->unk8);
                        BtlChrPartToWorld(pos, src, node, 1);
                    } else {
                        pos[0] = p->seg0[0];
                        pos[1] = p->seg0[1];
                        pos[2] = p->seg0[2];
                        node = BtlChrFindPart(src->model, p->unk8);
                        BtlChrPartToWorld(pos, src, node, 1);
                        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(pos2) : "memory");
                        pos2[0] = p->seg1[0];
                        pos2[1] = p->seg1[1];
                        pos2[2] = p->seg1[2];
                        BtlChrPartToWorld(pos2, src, BtlChrFindPart(src->model, p->unkA), 1);
                        t = (f32)(rand() >> 15) * (1.0f / 65536.0f);
                        pos[0] = (pos[0] - pos2[0]) * t + pos2[0];
                        pos[1] = (pos[1] - pos2[1]) * t + pos2[1];
                        pos[2] = (pos[2] - pos2[2]) * t + pos2[2];
                    }
                    PtclBtlSprEmit(&p->sprites[p->unkC], pos, p->kind, 1, p->unk3C);
                    if (p->dirMode == 1) {
                        pos[0] = p->dir[0];
                        pos[1] = p->dir[1];
                        pos[2] = p->dir[2];
                        BtlChrPartDirToWorld(pos, src, node);
                        q = (FxWeaponAura *)((u8 *)p + (p->unkC << 6));
                        x = pos[0];
                        y = pos[1];
                        z = pos[2];
                        q->sprites[0].vel[0] += x;
                        q->sprites[0].vel[1] += y;
                        q->sprites[0].vel[2] += z;
                    } else if (p->dirMode == 2) {
                        q = (FxWeaponAura *)((u8 *)p + (p->unkC << 6));
                        q->sprites[0].vel[0] += p->dir[0];
                        q->sprites[0].vel[1] += p->dir[1];
                        q->sprites[0].vel[2] += p->dir[2];
                    }
                    if (++p->unkC >= 24) {
                        p->unkC = 0;
                    }
                    if (p->unk6 != 0) {
                        if (--p->timer == 0) {
                            p->pause = 0;
                            p->timer = p->interval;
                        }
                    }
                } else if (p->pause == 0) {
                    if (--p->timer == 0) {
                        p->pause = 1;
                        p->timer = p->unk6;
                    }
                }
                p->unk10 -= 1.0f;
            }
        }
        if (PtclBtlSprUpdate(p->sprites, 0x18, p->kind) <= 0 && p->stop != 0) {
            p->kind = 0;
        }
    }
}
