/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x006E1E50-0x006E1E6C */
#include "common.h"
#include "protos/battle.h"
#include "vu.h"
#include "battle/unit.h"
#include "battle/n32.h"
#include "battle/n33.h"

void VuVecNormalize(VuVec3 *, VuVec3 *);

s32 BtlUnitCollectAreaTargets(BtlUnit *arg0, VuVec3 *pos) {
    BtlSeg a;
    VuVec3 v;
    BtlSeg b;
    BtlUnit *u;
    s32 n;
    s32 thr;
    s32 mask;
    s32 flags;
    s32 half;
    s32 base;
    f32 t2;
    f32 t3;
    VuVec3 *c2 = pos;
    BtlUnit *c1 = arg0;

    mask = 0;
    flags = arg0->scopeType;
    thr = (flags & 0x400) ? 4 : 0x40;
    if (flags & 0x100) {
        u = btlUnits;
        n = (flags & 0x200) ? 16 : 4;
    } else {
        n = 12;
        u = btlEnemyUnits;
    }
    switch (flags & 0xFF) {
    case 0:
        break;
    case 1:
        mask = 1 << c1->target;
        break;
    case 2:
    case 6:
        for (; n > 0; n--, u++) {
            if (u->state >= thr) {
                if ((u->pos[0] - pos->x) * (u->pos[0] - pos->x) + (u->pos[2] - pos->z) * (u->pos[2] - pos->z) <= arg0->scopeRadius * arg0->scopeRadius) {
                    mask |= 1 << u->id;
                }
            }
        }
        break;
    case 3:
        a.x0 = arg0->pos[0];
        a.z0 = arg0->pos[2];
        v.x = pos->x - a.x0;
        v.y = 0.0f;
        v.z = pos->z - a.z0;
        VuVecNormalize(&v, &v);
        a.x1 = arg0->pos[0] + v.x * 1100.0f * 3.0f;
        a.z1 = arg0->pos[2] + v.z * 1100.0f * 3.0f;
        for (; n > 0; n--, u++) {
            if (u->state >= thr) {
                t2 = v.z * arg0->scopeRadius;
                t3 = v.x * arg0->scopeRadius;
                b.x0 = u->pos[0] + t2;
                b.x1 = u->pos[0] - t2;
                b.z0 = u->pos[2] - t3;
                b.z1 = u->pos[2] + t3;
                if (BtlSegIntersect(NULL, &a, &b)) {
                    mask |= 1 << u->id;
                }
            }
        }
        break;
    case 4:
        half = arg0->scopeRadius * 182.04445f;
        base = BtlAtan2Angle(c2->x - arg0->pos[0], pos->z - arg0->pos[2]) - half / 2;
        for (; n > 0; n--, u++) {
            if (u->state >= thr) {
                if (((BtlAtan2Angle(u->pos[0] - arg0->pos[0], u->pos[2] - arg0->pos[2]) - base) & 0xFFFF) < half) {
                    mask |= 1 << u->id;
                }
            }
        }
        break;
    case 5:
        for (; n > 0; n--, u++) {
            if (u->state >= thr) {
                mask |= 1 << u->id;
            }
        }
        break;
    }
    btlTargetMask = mask;
    btlAreaCtrX = pos->x;
    btlAreaCtrZ = pos->z;
    return mask;
}
