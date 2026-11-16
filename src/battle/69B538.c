/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"

void VuSinCosAngle(f32 *, f32 *, s32);
#define CALLX(a,b,c2,d2,e2,f) BtlHitQueueAddDamage(a,b,f,c2,d2,e2)

void FxDrainOrbUpdate(FxAtk *e) {
    FxDrainOrb *d = e->work;
    BtlUnit *c;
    FxDrainSpark *sp;
    FxDrainSpark *q;
    VuVec4 v;
    f32 sc[2];
    f32 dx, dy, dz;
    s32 diff;
    s32 x;
    s32 i;
    s8 n;

    d->y += d->vy;
    VuSinCosAngle(&sc[0], &sc[1], d->dir);
    d->x += sc[0] * d->speed;
    d->z += sc[1] * d->speed;
    switch (d->state) {
    case 0:
        d->vy -= 0.5f;
        d->speed -= 0.2f;
        if (d->vy <= 0.0f) {
            d->state++;
        }
        break;
    case 1:
        c = e->unit;
        BtlChrGetPartPos(&v, c, 1);
        dx = d->x - v.x;
        dy = d->y - v.y;
        if (dx * dx + dy * dy + (d->z - v.z) * (d->z - v.z) < 6400.0f) {
            CALLX(c, d->amount, c->rot[1], 0x100, 0, 0.0f);
            d->state++;
            break;
        }
        d->vy += (d->y < v.y) ? 0.5f : -0.5f;
        d->vy = (d->vy < -7.5f) ? -7.5f : ((d->vy > 7.5f) ? 7.5f : d->vy);
        diff = (BtlAtan2Angle(v.x - d->x, v.z - d->z) - d->dir) & 0xFFFF;
        if (diff < 0x800) {
            d->dir = (d->dir + diff) & 0xFFFF;
        } else if (diff > 0x7FFF) {
            d->dir = (d->dir - 0x800) & 0xFFFF;
        } else {
            d->dir = (d->dir + 0x800) & 0xFFFF;
        }
        if (d->speed < 10.0f) {
            d->speed += 0.5f;
        }
        break;
    }
    if (d->state < 2 && (btlTickCount & 1)) {
        sp = &d->sparks[d->next];
        sp->age = 0;
        sp->life = (rand() >> 15) * (1.0f / 65536.0f) * 20.0f + 40.0f;
        sp->size = ((rand() >> 15) * (1.0f / 65536.0f) * 10.0f + 5.0f) * 0.5f;
        BtlVecRandomDirInRange(&v, 0, 0xFFFF, 0, 0xFFFF);
        v.w = (rand() >> 15) * (1.0f / 65536.0f) + 1.0f;
        sp->vel[0] = v.x * v.w;
        sp->vel[1] = v.y * v.w;
        sp->vel[2] = v.z * v.w;
        sp->pos[0] = d->x;
        sp->pos[1] = d->y;
        sp->pos[2] = d->z;
        if (++d->next >= 24) {
            d->next = 0;
        }
    }
    q = d->sparks;
    n = 0;
    for (i = 23; i >= 0; i--, q++) {
        if (q->life > 0) {
            n++;
            q->pos[0] += q->vel[0];
            q->pos[1] += q->vel[1];
            q->pos[2] += q->vel[2];
            q->vel[0] *= 0.98f;
            q->vel[1] *= 0.98f;
            q->vel[2] *= 0.98f;
            if (++q->age >= q->life) {
                q->life = 0;
            }
        }
    }
    if (d->state >= 2 && n == 0) {
        FxAtkFree(e);
    }
}
