/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"


void IpIconUpdate(BtlUnit *arg0) {
    IpIcon *s = &ipGauge.icons[arg0->id];
    s32 n;
    f32 lim;
    f32 t;
    f32 d;
    f32 v;
    f32 t2;
    f32 z;

    if (btlPhase == 0x80) {
        if (arg0->ipMode == 1) {
            v = s->pos;
            z = 0.0f;
            /* goto loop: subtract 100 until <= 0; while/do-while/for(;;) forms
             * all lay the loop out differently */
            if (v > z) {
                goto body;
            loop:
                v = t2;
            body:
                t2 = v - 100.0f;
                s->pos = t2;
                if (t2 > z) {
                    goto loop;
                }
                v = t2;
            }

            s->pos = v + 1.0f;
            if (s->pos >= -12.5f) {
                arg0->ipMode = 0;
            }
        } else {
            t = 0.0f;
            switch (arg0->ipMode) {
            case 0:
                t = 50.0f - (arg0->ipPos + arg0->ipDamage) * 50.0f;
                break;
            case 2:
                t = 75.0f - arg0->ipPos * 25.0f - arg0->ipDamage * 50.0f;
                break;
            }
            d = t - s->pos;
            if (d > 1.0f) {
                s->pos += 1.0f;
            } else if (d < -1.0f) {
                s->pos -= 1.0f;
            } else {
                s->pos = t;
            }
        }
    }
    if (s->bounce > 0.0f) {
        s->bounceVel -= 1.0f;
        s->bounce += s->bounceVel;
        if (s->bounce <= 0.0f) {
            s->bounceVel *= -0.5f;
            if (s->bounceVel < 2.0f) {
                s->bounce = 0.0f;
            } else {
                s->bounce = -s->bounce;
            }
        }
    }
    if (s->flash > 0) {
        s->flash--;
    }
    if (arg0->comboPartners[0] >= 0 || (arg0->comboLeader >= 0 && arg0->action == 6)) {
        IpIconGetComboSpread(arg0, &n, &lim);
        s->spread += 0.5f;
        if (s->spread > lim) {
            s->spread = lim;
        }
        s->spreadAngle = BtlAngleApproach(s->spreadAngle, n, 0x400);
    } else if (s->spread > 0.0f) {
        s->spread -= 1.0f;
        if (s->spread < 0.0f) {
            s->spread = 0.0f;
            s->spreadAngle = 0;
        }
    }
}
