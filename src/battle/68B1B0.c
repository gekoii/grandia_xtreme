/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "vu.h"
#include "gph.h"
#include "battle/misc.h"
#include "battle/n30.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

typedef struct {
    s32 x, y, z, w;
} IVec4;


void FxTraceUnitUpdate(FxTraceUnit *o, BtlUnit *u) {
    s32 j;
    s32 m;
    s32 d;
    FxTraceStrip *e;
    FxTraceSpark *f;
    FxTraceStrip *e2;
    FxTraceColor *c;
    s32 r, g;

    if (o->t > 0.0f) {
        o->unk14 -= o->t;
        if (o->unk14 <= 0.0f) {
            o->unk14 = 0.0f;
            FxTraceStopAll(u->id);
            return;
        }
    } else if (btlActingUnit == 0xFF || (u->flags & 8)) {
        o->unk14 += 0.0625f;
        if (o->unk14 > 1.0f) {
            o->unk14 = 1.0f;
        }
    } else {
        o->unk14 -= 0.0625f;
        if (o->unk14 < 0.0f) {
            o->unk14 = 0.0f;
        }
    }

    for (j = 0; j < 6; j++) {
        if (o->strips[j] >= 0) {
            e = &fxTraceStrips[o->strips[j]];
            c = &fxTraceColors[o->kinds[j]];
            d = e->unk14;
            r = c->start.c.r;
            g = c->end.c.r;
            e->c0.c.r = r * u->alpha * o->unk14;
            e->c1.c.r = g * u->alpha * o->unk14;
            if (d > 0) {
                if (o->t <= 0.0f) {
                    e->unk10 += d;
                    if (e->unk10 >= e->count) {
                        e->unk10 = e->count;
                        e->unk14 = 0;
                    }
                }
            } else if (d < 0) {
                e->unk10 += d;
                if (e->unk10 < 2) {
                    FxTraceStop(u->id, j);
                }
            }
        }
    }

    if (o->t > 0.0f) {
        return;
    }
    if (o->motion == NULL) {
        return;
    }
    if (o->height == 1) {
        FxTraceRecordPoints(o, u);
    } else {
        VuVec4 s0;
        IVec4 s1;
        VuVec4 s2;
        f32 f20, f22, f21;
        struct GphMotion *s3;
        s32 i;
        f32 t;
        f32 v;
        f32 lim;
        s32 a;

        copyq(&s0, &u->pos);
        copyq(&s1, &u->rot);
        copyq(&s2, &u->scale);
        i = 1;
        s3 = u->motion;
        f22 = u->unk12C;
        f21 = u->unk128;
        u->unk128 = o->unkC;
        u->motion = o->motion;
        u->unk12C = o->unk10;
        f20 = u->frame;
        for (; i < o->height; i++) {
            t = 1.0f - (f32)i / (f32)o->height;
            u->pos[0] = (o->pos[0] - s0.x) * t + s0.x;
            u->pos[1] = (o->pos[1] - s0.y) * t + s0.y;
            u->pos[2] = (o->pos[2] - s0.z) * t + s0.z;
            a = (o->rot[0] & 0xFFFF) - (s1.x & 0xFFFF);
            if (a > 0x8000) {
                a -= 0x10000;
            } else if (a < -0x8000) {
                a += 0x10000;
            }
            u->rot[0] = (s32)(a * t + s1.x) & 0xFFFF;
            a = (o->rot[1] & 0xFFFF) - (s1.y & 0xFFFF);
            if (a > 0x8000) {
                a -= 0x10000;
            } else if (a < -0x8000) {
                a += 0x10000;
            }
            u->rot[1] = (s32)(a * t + s1.y) & 0xFFFF;
            a = (o->rot[2] & 0xFFFF) - (s1.z & 0xFFFF);
            if (a > 0x8000) {
                a -= 0x10000;
            } else if (a < -0x8000) {
                a += 0x10000;
            }
            u->rot[2] = (s32)(a * t + s1.z) & 0xFFFF;
            u->scale[0] = (o->scale[0] - s2.x) * t + s2.x;
            u->scale[1] = (o->scale[1] - s2.y) * t + s2.y;
            u->scale[2] = (o->scale[2] - s2.z) * t + s2.z;
            if (s3 == o->motion && o->frame <= f20) {
                u->frame = (o->frame - f20) * t + f20;
            } else {
                v = o->frameSpeed * (1.0f - t) + o->frame;
                lim = (f32)u->motion->frameCount - 1.0f;
                u->frame = v;
                if (lim < v) {
                    u->frame = lim;
                }
            }
            BtlChrModelInvalidate(u->id);
            FxTraceRecordPoints(o, u);
        }
        copyq(&u->pos, &s0);
        copyq(&o->pos, &s0);
        copyq(&u->rot, &s1);
        copyq(&o->rot, &s1);
        copyq(&u->scale, &s2);
        copyq(&o->scale, &s2);
        u->frame = f20;
        o->frame = f20;
        u->motion = s3;
        o->motion = s3;
        o->frameSpeed = u->frameSpeed;
        u->unk128 = f21;
        o->unkC = f21;
        u->unk12C = f22;
        o->unk10 = f22;
        BtlChrModelInvalidate(u->id);
        FxTraceRecordPoints(o, u);
    }

    for (m = 0; m < 6; m++) {
        if (o->strips[m] >= 0) {
            e2 = &fxTraceStrips[o->strips[m]];
            if (e2->unk17 >= 0 && (btlActingUnit == 0xFF || (u->flags & 8))) {
                f = &fxTraceSparks[e2->unk17];
                if (f->state == 0) {
                    PtclBtlSprEmit(&f->handle[f->frame], e2->buf[e2->cur].b, f->unk3, 1, o->step);
                    if (++f->frame >= f->count) {
                        f->frame = 0;
                    }
                }
            }
        }
    }
}
