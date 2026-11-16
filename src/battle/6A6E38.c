/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n30.h"

void IpIconBounce(s32 arg0, f32 arg1) {
    IpIcon *p = &ipGauge.icons[arg0];

    if (p->bounce <= 0.0f) {
        p->bounceVel = arg1;
        p->bounce = arg1 + 0.01f;
    } else if (p->bounce < arg1) {
        p->bounceVel += arg1;
    }
    if (p->bounceVel > 16.0f) {
        p->bounceVel = 16.0f;
    }
    p->flash = 0x40;
}
