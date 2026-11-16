/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n29.h"

void BtlCmdListCellSlideOut(BtlCmdListEntry *p) {
    f32 v;
    f32 t;

    v = p->unk10[4] * 1.25f;
    p->unk10[4] = v;
    t = p->alpha - 1.0f / (f32)p->unk2;
    p->x = p->x + v;
    p->alpha = t;
    if (t <= 0.0f) {
        p->alpha = 0.0f;
        p->type = 0;
    }
}
