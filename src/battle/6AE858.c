/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/unit.h"
#include "battle/camera.h"
#include "party.h"

void BtlCamCalcUnitsCentre(f32 *ox, f32 *oz, s32 mode) {
    f32 w, sx, sz;
    f32 f, x, z;
    f32 g, h, k;
    BtlUnit *c;
    BtlUnit *t;
    s32 i;

    w = 0.0f;
    c = btlUnits;
    sz = 0.0f;
    sx = 0.0f;
    if (mode == 0) {
        for (i = 0; i < 16; i++, c++) {
            if (btlCamBossUnit != NULL && i >= 4 && btlCamBossUnit != c) {
                continue;
            }
            if (c->state >= 0x40 && c->body < 0) {
                w += 1.0f;
                sx += c->pos[0];
                sz += c->pos[2];
            }
        }
    } else {
        for (i = 0; i < 16; i++, c++) {
            if (btlCamBossUnit != NULL && i >= 4 && btlCamBossUnit != c) {
                continue;
            }
            if (c->state < 0x40) {
                continue;
            }
            if (c->body >= 0) {
                continue;
            }
            f = c->ipPos;
            if (f > 1.0f) {
                f = 1.0f;
            }
            if (c->params->status & 0x20E0) {
                x = c->pos[0];
                z = c->pos[2];
                w += 1.0f;
                sx += x;
                sz += z;
            } else if (c->ipMode == 2) {
                if (f == 0.0f) {
                    x = c->pos[0];
                    z = c->pos[2];
                    w += 96.0f;
                    sx += x * 96.0f;
                    sz += z * 96.0f;
                    t = &btlUnits[c->target];
                    if (t->state > 0) {
                        w += 144.0f;
                        sx += t->pos[0] * 144.0f;
                        sz += t->pos[2] * 144.0f;
                    }
                } else {
                    x = c->pos[0];
                    z = c->pos[2];
                    g = (1.0f - f) * 8.0f + 16.0f;
                    sx += x * g;
                    sz += z * g;
                    w += g;
                }
            } else {
                x = c->pos[0];
                z = c->pos[2];
                h = (1.0f - f) * 4.0f + 1.0f;
                sx += x * h;
                sz += z * h;
                w += h;
            }
            if (i < 4 || btlCamBossUnit == c) {
                k = 2.0f - btlPartyAliveCount * 0.25f;
                k += k;
                sx += x * k;
                sz += z * k;
                w += k;
            }
        }
    }
    w = 1.0f / w;
    *ox = sx * w;
    *oz = sz * w;
}
