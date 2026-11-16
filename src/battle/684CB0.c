/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "vu.h"
#include "sys.h"
#include "party.h"
#include "battle/n28.h"

f32 VuSinAngle(s32);

s32 BtlEnemyDrop(BtlUnit *b, s8 flag) {
    PtyUnit *st;
    BtlChrData *pt;
    BtlDropObj *e;
    s32 ret;
    s32 kind;
    s32 found;
    s32 k;
    s32 t;
    s32 *g;
    s32 a;
    s32 a2;
    s32 c;
    f32 r;
    f32 f;
    f32 s;
    VuVec3 v;

    ret = 0;
    kind = 1;
    if (b->id < 4) {
        return 0;
    }
    found = 0;
    r = (rand() >> 15) * (1.0f / 65536.0f);
    pt = b->def->data;
    st = btlUnits[b->attacker].params;
    if (b->item > 0 && btlHandoff.dropCount < 12 && r + st->getRateUp > pt->itemRate * 0.01f) {
        if (0.0f < st->getItemUp) {
            BtlEnemyDropRoll(b, st->getItemUp * 100.0f);
        }
        if (b->item > 0) {
            /* ptyItemTable[b->item] written out: the plain index orders the add differently */
            kind = ((PtyItem *)((u32)ptyItemTable + (b->item << 5)))->model;
            ret = b->item;
            btlHandoff.dropIds[btlHandoff.dropCount] = ret;
            btlHandoff.dropCount++;
            found = 1;
            b->item = 0;
            b->treasure = 0;
        }
    }
    if (!found) {
        if (flag != 0) {
            return 0;
        }
        if (b->item < 0) {
            return 0;
        }
        if (pt->goldLargeRate * 0.01f < r + st->getGoldRate) {
            kind = 2;
        }
        k = kind - 1;
        g = &btlGoldTotal;
        t = *g + pt->gold[k];
        *g = t;
        if (t >= 0) {
            c = (t > 99999999) ? 99999999 : t;
        } else {
            c = 0;
        }
        *g = c;
        if (btlGoldDropCount <= 0x40) {
            btlGoldDrops[btlGoldDropCount] = pt->gold[kind - 1];
            btlGoldDropCount++;
        }
    }
    if (kind <= 0) {
        return ret;
    }
    e = &btlDropObjsByUnit[b->id];
    e->kind = kind;
    e->state = 0;
    e->x = b->pos[0];
    e->y = b->pos[1];
    e->z = b->pos[2];
    if (e->y < 100.0f) {
        e->y = 100.0f;
    }
    if (kind < 3) {
        a = btlGoldDrops[btlGoldDropCount - 1];
        if (a <= 0) {
            BtlDropObjFree(e);
            return ret;
        }
        f = a;
        if (kind == 1) {
            f *= 0.02f;
        } else {
            f *= 0.0066666668f;
        }
        if (f < 1.0f) {
            f = 1.0f;
        } else if (5.0f < f) {
            f = 5.0f;
        }
        if (kind != 1) {
            e->coinCount = f * 7.0f;
        } else {
            e->coinCount = f;
        }
        if (e->coinCount <= 0) {
            e->coinCount = 1;
        }
        e->coins = BtlAlloc32(e->coinCount * 0x1C);
        if (e->coins == NULL) {
            BtlDropObjFree(e);
            return ret;
        }
    } else {
        if (flag) {
            a2 = rand() >> 15;
            s = (rand() >> 15) * (1.0f / 65536.0f);
            s += s;
            s += 2.0f;
            e->vx = VuSinAngle(a2) * s;
            e->vz = VuCosAngle(a2) * s;
        } else {
            e->vx = e->vz = 0.0f;
        }
        v.x = e->x;
        v.y = e->y;
        v.z = e->z;
        SndSePlay3D(0x7D7, 0, 8, &v);
    }
    e->alpha = 0;
    e->model = 0;
    e->frameStamp = btlFrameCount;
    if (e->modelBuf == 0) {
        e->frameStamp = btlFrameCount + 1;
    }
    return ret;
}
