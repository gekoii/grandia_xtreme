/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n30.h"
#include "party.h"

void IpPanelSetMode(s32 arg0, s32 arg1, s32 arg2) {
    IpPanel *e;
    s32 i;
    f32 f;
    s32 t;

    if (arg0 < 4) {
        i = arg0;
        e = &ipPanels[arg0];
        arg0 = arg0 + 1;
    } else {
        i = 0;
        e = ipPanels;
        arg0 = 4;
    }
    for (; i < arg0; i++, e++) {
        if (e->mode != arg1) {
            e->mode = arg1;
            e->timer = 0;
            e->from = e->pos;
            f = btlUnits[i].params->maxSp;
            do {} while (0); /* empty barrier: without it t gets a0 instead of v0 */
            t = 100;
            if (!(f > 100.0f)) {
                t = f;
            }
            e->spMax = t;
        }
        e->unk4 = arg2;
    }
}
