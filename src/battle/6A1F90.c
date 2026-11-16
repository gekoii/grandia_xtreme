/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"


void FxWallRegister(BtlUnit *arg0) {
    FxWall *p = &fxWall[arg0->id];
    GphModel *m;
    VuVec4 v;
    s32 n;
    f32 f;

    m = p->model;
    if (m != NULL) {
        n = (u8)btlTickCount & 0x7F;
        m->rot[0] = 0x6800 - (n << 9);
        m->rot[1] = 0x8000 - arg0->rot[1];
        BtlChrGetPartPos(&v, arg0, 1);
        p->model->pos[0] = v.x;
        p->model->pos[1] = v.y;
        p->model->pos[2] = v.z;
        if (p->flash > 1.0f) {
            f = 1.0f;
        } else {
            if ((u32)n < 0x40) {
                f = (1.0f / 64.0f) * n;
            } else {
                f = (0x80 - n) * (1.0f / 64.0f);
            }
            f = f + (1.0f - f) * p->flash;
        }
        ChrModelSetAlpha(p->model, (f * arg0->alpha - 1.0f) * 128.0f);
        BtlChrModelCalcDraw(p->model);
    }
}
