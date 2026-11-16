#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "party.h"
#include "gph.h"

extern s8 dbgBtlHitAttr;

void VuVecCopy(void *, void *);
void VuSinCosAngle(f32 *, f32 *, s32);
void FxHitSpawn(s32, s32, VuVec4 *, s32);

void BtlHitShowEffect(BtlUnit *u, BtlHit *h, BtlHitResult *info, s32 a3) {
    VuVec4 v;
    f32 s, c;
    s32 k;
    s32 *p;

    k = (dbgBtlHitAttr < 0) ? a3 : dbgBtlHitAttr;
    if ((info->flags & 0x200004) == 0x200000) {
        VuVecCopy(&v, h->srcPos);
    } else {
        VuSinCosAngle(&s, &c, h->angle);
        v.x = u->pos[0] + u->params->colR * 0.5f * s;
        v.y = BtlChrGetCenterY(u);
        v.z = u->pos[2] + u->params->colR * 0.5f * c;
    }
    if (info->flags & 4) {
        FxHitSpawn(0x40, u->id, &v, h->angle);
        if (!(info->flags & 0x2000)) {
            PtclBurstSpawn(0xA, u, &v);
        }
    } else {
        if (!(info->flags & 0x800)) {
            BtlHudGaugeAddHit(info->damage, u, btlHitDamageAccum[u->id] < 1);
            if (info->flags & 0x8000) {
                BtlHudComboPopupAdd(u->id);
            }
        }
        if (!(info->flags & 0x4000)) {
            if (info->reaction == 0x45) {
                FxHitSpawn(0x50, u->id, &v, h->angle);
            } else {
                if (info->flags & 0x80000) {
                    FxHitSpawn(0x30, u->id, &v, h->angle);
                } else {
                    FxHitSpawn((u8)(info->reaction == 0x42 ? k + 0x20 : k + 0x10), u->id, &v, h->angle);
                }
                FxAfterimageSnapshot(u, 255.0f, -255.0f, -255.0f);
            }
        }
        if (!(info->flags & 0x2000)) {
            PtclBurstSpawn(k, u, &v);
            if (info->flags & 2) {
                SndSePlay3D(0x7E5, 0, 8, &u->pos);
                if (!(info->flags & 0x8000)) {
                    GphScreenFadeSetColor(255.0f, 0.0f, 0.0f, 96.0f);
                    GphScreenFadeStart(0, 255.0f, 0.0f, 0.0f, 0.0f, 16.0f);
                }
            }
        }
    }
    if (info->flags & 0x100000) {
        p = &btlHitDamageAccum[u->id];
        if (*p < 0) {
            *p = info->damage;
        } else {
            *p = *p + info->damage;
        }
    } else if (!(info->flags & 0x1000)) {
        if (btlHitDamageAccum[u->id] < 0) {
            BtlHudDamageNumSpawn(info->damage, &v, info->flags);
        } else {
            BtlHudDamageNumSpawn(info->damage + btlHitDamageAccum[u->id], &v, info->flags);
            btlHitDamageAccum[u->id] = -1;
        }
    }
    if (info->damage == 0) {
        if (!(info->flags & 0x1000)) {
            SndSePlay3D(0x7E2, 0, 8, &u->pos);
        }
    } else {
        SndSePlay3D(0x7E3, 0, 8, &u->pos);
    }
    if (k > 0) {
        u->flag |= 0x100 << (k - 1);
    }
}
