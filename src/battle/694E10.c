/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n29.h"
#include "protos/battle.h"

void BtlCmdListCellSlideIn(BtlCmdListEntry *p, s8 dir) {
    f32 t;

    if (p->alpha < 1.0f) {
        p->alpha = p->alpha + 0.125f;
    }
    if (dir != 0) {
        if (p->unk10[4] > -20.0f) {
            p->unk10[4] = p->unk10[4] - 4.0f;
        }
    } else if (p->unk10[4] < 0.0f) {
        p->unk10[4] = p->unk10[4] + 4.0f;
    }
    if (p->unk2 <= 0) {
        p->y = p->unk10[3];
        p->x = p->unk10[2] + p->unk10[4];
        return;
    }
    t = VuCosAngle(p->unk2 << 10);
    p->unk2--;
    p->y = (p->unk10[3] - p->unk10[1]) * t + p->unk10[1];
    p->x = (p->unk10[2] - p->unk10[0]) * t + p->unk10[0] + p->unk10[4];
}
