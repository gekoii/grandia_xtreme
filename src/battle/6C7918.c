/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n33.h"
#include "party.h"

s32 BtlCircleSegIntersect(f32 *, f32 *, f32 *);

s32 BtlPathAvoidHeading(BtlUnit *u, f32 *dst, s32 flags, f32 r) {
    BtlPathCtx c;
    f32 seg[4];
    f32 out[4];
    f32 cir[4];
    BtlPathCtx *cp;
    BtlUnit *t;
    BtlUnit *t1;
    u8 *p;
    u8 *p1;
    u8 *pp;
    u8 *q;
    u8 *sv;
    f32 x0, z0;
    f32 ex, ez;
    s32 i;
    s32 best;
    s32 ret;
    s32 a, b, d1, d2;
    f32 rr;
    f32 dx, dz;
    f32 fx, fz, gx, gz;
    f32 bestd;
    f32 d;

    cp = &c;
    best = 16;
    c.ang = ((s32 (*)(f32, f32))BtlAtan2Angle)(dst[0] - u->pos[0], dst[2] - u->pos[2]) & 0xFFFF; /* s32 return: keeps the andi (the u16 return drops it) */
    rr = r * r;
    x0 = dst[0];
    ex = x0 - u->pos[0];
    z0 = dst[2];
    ez = z0 - u->pos[2];
    if (ex * ex + ez * ez <= rr) {
        return c.ang | 0x40000;
    }
    c.x = u->pos[0];
    c.z = u->pos[2];
    c.hi = 0x10000;
    c.r = u->params->colR;
    c.lo = 0;
    t1 = btlUnits;
    p1 = c.flags;
    for (i = 15; i >= 0; i--) {
        if (t1->state >= 4 && t1->alpha > 0.0f && !(t1->params->bitCsf & 0x200)) {
            *p1 = (flags >> 1) & 1;
        } else {
            *p1 = 0;
        }
        p1++;
        t1++;
    }
    cp->flags[u->id] = 0;
    bestd = 99998008.0f;
    seg[0] = cp->x;
    seg[1] = cp->z;
    seg[2] = x0;
    seg[3] = z0;
    t = btlUnits;
    i = 0;
    p = cp->flags;
    for (; i < 16; i++, p++, t++) {
        if (*p == 0) {
            *p = 0;
            continue;
        }
        if ((t->pos[0] - cp->x) * (t->pos[0] - cp->x) + (t->pos[2] - cp->z) * (t->pos[2] - cp->z) <= (t->params->colR + cp->r) * (t->params->colR + cp->r)) {
            *p = 2;
        }
        cir[0] = t->pos[0];
        cir[1] = t->pos[2];
        cir[2] = t->params->colR + cp->r;
        ret = BtlCircleSegIntersect(out, cir, seg);
        if (ret == 1) {
            *p = 0;
        } else if (ret > 0) {
            if (ret >= 4) {
                continue;
            }
            dx = out[2] - dst[0];
            dz = out[3] - dst[2];
            if (!(dx * dx + dz * dz <= rr)) {
                fx = out[2] - cp->x;
                fz = out[3] - cp->z;
                d = fx * fx + fz * fz;
                if (d < bestd) {
                    bestd = d;
                    best = i;
                }
            } else {
                *p = 0;
            }
        }
    }
    if (best >= 16) {
        return cp->ang;
    }
    sv = cp->save;
    pp = cp->flags;
    q = sv;
    for (i = 15; i >= 0; i--) {
        *q++ = *pp++;
    }
    BtlPathBlockedArcExtend(cp, best, 0);
    pp = sv;
    q = cp->flags;
    for (i = 15; i >= 0; i--) {
        *q++ = *pp++;
    }
    BtlPathBlockedArcExtend(cp, best, 1);
    if (cp->lo > 0x8000) {
        if (cp->hi <= 0x7FFF) {
            return cp->ang + 0x10000;
        }
        return (cp->ang + cp->hi - 0x1000) & 0xFFFF;
    } else if (cp->hi <= 0x7FFF) {
        return (cp->ang + cp->lo + 0x1000) & 0xFFFF;
    } else {
        if (u->pos[0] * u->pos[0] + u->pos[2] * u->pos[2] + u->params->colR > 980100.0f) {
            if (cp->lo < 0x10000 - cp->hi) {
                return (cp->ang + cp->lo + 0x1000) & 0xFFFF;
            } else {
                return (cp->ang + cp->hi - 0x1000) & 0xFFFF;
            }
        } else {
            a = cp->ang + cp->lo;
            d1 = (a - u->rot[1]) & 0xFFFF;
            if (d1 > 0x8000) {
                d1 = 0x10000 - d1;
            }
            b = cp->ang + cp->hi;
            d2 = (b - u->rot[1]) & 0xFFFF;
            if (d2 > 0x8000) {
                d2 = 0x10000 - d2;
            }
            if (d1 < d2) {
                return (a + 0x1000) & 0xFFFF;
            }
            return (b - 0x1000) & 0xFFFF;
        }
    }

}
