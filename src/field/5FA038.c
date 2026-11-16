/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/n12.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void ChrStepSoundUpdate(ChrObj *c) {
    f32 d;
    s32 lim;
    s32 cnt;

    if (EvtSystemFlagGet(0x84) == 1) {
        return;
    }
    if (!(c->flags & 1)) {
        return;
    }

    d = fsqrt(c->vel[0] * c->vel[0] + c->vel[2] * c->vel[2]);
    if (d < 0.1f) {
        c->unk14 = 0x10;
        return;
    }
    lim = 0x16;
    if (d < 11.99f) {
        lim = 0x1D;
    }

    if (c->ctrlMode == 2) {
        lim = 0x16;
    }
    if (c->actFlags & 8) {
        lim = 0x15;
    }
    cnt = (s8)c->unk14;
    if (lim < cnt) {
        cnt = 0;
        SndSePlay3D(FldSndGetStepSe(evtAreaStepSound), 3, 0, c->model->pos);
        if (lim >= 0x15 && c->ctrlMode != 2 && camCtl.unk5 >= 2 && !ChrPlayerIsBusy() && c->ctrlMode == 0) {
            FldMapIsNoKamae();
        }
    }
    c->unk14 = cnt + 1;
}
