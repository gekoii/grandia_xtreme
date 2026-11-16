#include "common.h"
#include "battle/n28.h"

void BtlResultBannerUpdate(BtlResult *o) {
    BtlResultBanner *f = &o->banner;

    switch (f->state) {
    case 1:
        f->x += f->vel;
        f->vel += 4.0f;
        if (f->vel >= 0.0f) {
            f->timer = 0;
            f->state++;
        }
        break;
    case 2:
        if (f->timer >= 50) {
            f->state++;
        }
        break;
    case 3:
        f->x += f->vel;
        f->vel -= 4.0f;
        if (f->x <= -127.0f) {
            f->state = 0;
        }
        break;
    }
    f->timer++;
}
