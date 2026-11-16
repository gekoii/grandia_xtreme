/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n33.h"
#include "protos/battle.h"

f32 VuSinAngle(s32);

void BtlHudDamageNumUpdate(void) {
    BtlHudDamageNum *d;
    BtlHudDamageNum *o;
    s32 i;
    s32 j;
    s32 t;
    s32 n;
    f32 f;
    f32 x;
    f32 y;
    f32 r;
    f32 k;
    f32 cnt;
    f32 r2;
    f32 ox;
    f32 oy;
    f32 old;
    f32 dx;
    f32 dy;

    d = btlHudDamageNums;
    for (i = 0; i < 0x20; i++, d++) {
        if (d->active == 0) {
            continue;
        }
        if (d->flags & 0x10) {
            if (d->timer < 0x18 && (d->timer & 1)) {
                if (d->timer & 2) {
                    d->dy += (24.0f - (d->timer & 0xFC)) / 24.0f * 16.0f;
                } else {
                    d->dy -= (24.0f - (d->timer & 0xFC)) / 24.0f * 16.0f;
                }
            }
            if ((t = d->timer) > 0x20) {
                d->scale = (d->timer - 0x20) * 0.015625f + 2.0f;
            }
        } else if (d->flags & 0x20) {
            t = d->timer;
            if (t < 0x20) {
                f = VuSinAngle(t << 9);
                d->dy = -48.0f - f * 48.0f;
                f = 1.0f - (1.0f - f) * (1.0f - f);
                d->scale = VuCosAngle(f * 163840.0f + 32768.0f);
            } else {
                d->scale = 1.0f - ((f32)t - 32.0f) * 0.0078125f;
            }
        } else {
            if (d->timer < 0x1C) {
                d->dy += d->timer * 0.5f + -10.0f;
            }
            d->scale = d->timer * 0.015625f + 1.0f;
        }
        if (++d->timer > 0x40) {
            d->active = 0;
        }
        if (d->scale == 0.0f) {
            continue;
        }
        UtilWorldToScreen(&d->scr, &d->pos);
        d->scr[1] += 64.0f;
        cnt = 0.0f;
        old = d->stackY;
        x = d->scr[0] + d->dx;
        y = d->scr[1] + old + d->dy;
        n = d->value < 0 ? -d->value : d->value;
        if (n >= 1000) {
            r = d->scale * 12.0f * 4.0f;
        } else if (n >= 100) {
            r = d->scale * 12.0f * 3.0f;
        } else if (n >= 10) {
            r = 2.0f * (d->scale * 12.0f);
        } else {
            r = d->scale * 12.0f;
        }
        r2 = r * r;
        k = d->scale * 24.0f / r;
        for (j = 0; j < 0x20; j++) {
            o = &btlHudDamageNums[j];
            if (j != i && o->active != 0 && o->timer >= 0) {
                oy = o->scr[1] + o->stackY + o->dy;
                ox = o->scr[0] + o->dx;
                if (y < oy) {
                    dy = (y - oy) * k;
                    dx = x - ox;
                    if (dx * dx + dy * dy < r2) {
                        cnt += 1.0f;
                    }
                }
            }
        }
        if (cnt > 0.0f) {
            d->stackY = old - (cnt * 0.5f + 1.5f);
        }
    }
}
