/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"
#include "party.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void FxComboLinkUpdate(void) {
    FxComboLink *d = &fxComboLink;
    VuVec4 a;
    VuVec4 b;
    BtlUnit *u;
    f32 *dist;
    FxComboLinkPart *e;
    FxComboLinkPart *ep;
    s32 i;
    f32 dx, dy, dz, r, t;
    s32 j;

    d->maxDist = 0.0f;
    BtlChrGetPartPos(&a, d->actor, 1);
    u = btlUnits;
    dist = d->dist;
    for (i = 0; i < 4; i++, dist++, u++) {
        if ((d->members >> i) & 1) {
            BtlChrGetPartPos(&b, u, 1);
            dx = b.x - a.x;
            dy = b.y - a.y;
            dz = b.z - a.z;
            r = fsqrt(dx * dx + dy * dy + dz * dz);
            *dist = r;
            t = (d->actor->params->colR + u->params->colR) * 0.5f;
            r -= t * 0.5f;
            *dist = r;
            if (d->maxDist < r) {
                d->maxDist = r;
            }
        } else {
            *dist = 0.0f;
        }
    }
    if (d->maxDist < 480.0f) {
        d->maxDist = 480.0f;
    }
    for (j = 0; j < 4; j++) {
        if (d->radius <= d->dist[j]) {
            ep = &d->parts[d->next];
            ep->life = 30;
            ep->member = j;
            ep->radius = d->radius;
            d->next++;
            if (d->next >= 90) {
                d->next = 0;
            }
        }
    }
    d->radius += 15.0f;
    if (d->ending == 0) {
        if (d->maxDist < d->radius) {
            d->radius = d->actor->params->colR * 0.5f;
        }
    }
    e = d->parts;
    j = 90;
    do {
        if (e->life > 0) {
            e->life--;
        }
        j--;
        e++;
    } while (j > 0);
    if (d->ending != 0) {
        MemFree(d->parts);
        d->parts = NULL;
    }
}
