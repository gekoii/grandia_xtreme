/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n28.h"
#include "protos/battle.h"

f32 VuSinAngle(s32);

#define TRI(x) ((x) < 0.0f ? (x) + 1.0f : 1.0f - (x))

void BtlResultHeaderUpdate(BtlResult *o) {
    BtlResultHeader *w = &o->header;
    f32 a;
    f32 q;
    f32 t;

    if (w->timer >= 0) {
        if (w->state == 0 && w->timer <= 20) {
            if (w->timer == 20) {
                w->scaleY = 1.0f;
                w->scaleX = 1.0f;
                w->alpha = 1.0f;
            } else {
                w->alpha = w->timer * 0.05f;
                a = TRI(VuCosAngle(w->timer << 10));
                q = a / TRI(VuCosAngle(0x5000));
                w->scaleY = q;
                if (1.0f < q) {
                    w->scaleX = (1.0f - q) + 1.0f;
                } else {
                    w->scaleX = (1.0f - q) * 5.0f + 1.0f;
                }
            }
        }
        if (w->timer <= 30) {
            if (w->timer == 30) {
                w->timer = -2;
                w->scaleY2 = 1.0f;
                w->scaleX2 = 1.0f;
                w->alpha2 = 1.0f;
            } else {
                w->alpha2 = (w->timer - 10) * 0.05f;
                a = TRI(VuCosAngle((w->timer - 10) << 10));
                q = a / TRI(VuCosAngle(0x5000));
                w->scaleY2 = q;
                if (1.0f < q) {
                    w->scaleX2 = (1.0f - q) + 1.0f;
                } else {
                    w->scaleX2 = (1.0f - q) * 5.0f + 1.0f;
                }
            }
        }
        w->timer++;
    }
    if (w->slideTimer <= 16) {
        if (w->slideTimer < 16) {
            t = 1.0f - VuSinAngle(w->slideTimer << 10);
            w->slideY = t * -128.0f;
            w->slideX = t * 640.0f;
        } else {
            w->slideY = w->slideX = 0.0f;
        }
        w->slideTimer++;
    }
}
