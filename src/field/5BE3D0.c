/* cc: ee-gcc2.9-991111-01 */
/* rodata: 0x00671170-0x00671188 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/event.h"
#include "field/n13.h"


void OccAlphaPulseForward(u8 *arg0) {
    OccAlphaPulse *a = OCC_PARAMS(arg0, OccAlphaPulse);
    f32 t;
    s32 i;
    GphModel *m;

    t = 0.0f;
    switch (a->phase) {
    case 0:
        if (++a->frame >= a->timeIn) {
            a->phase = 1;
            a->frame = a->hold;
        } else {
            t = OccEase(a->ease, (f32)a->frame / (f32)a->timeIn);
        }
        break;
    case 1:
        a->phase = 2;
    case 2:
        if (a->frame > 0) {
            a->frame--;
            break;
        }
        a->phase = 3;
        a->frame = 0;
        for (i = 0; i <= a->last; i++) {
            a->delta[i] = a->end[i] - a->alpha;
        }
    case 3:
        if (++a->frame >= a->timeOut) {
            a->phase = 4;
        } else {
            t = OccEase(a->ease, (f32)a->frame / (f32)a->timeOut);
        }
        break;
    case 4:
        break;
    }
    for (i = 0; i <= a->last; i++) {
        m = 0;
        switch (a->target) {
        case 0:
            m = FldObjGet(a->id + i);
            break;
        case 1:
            m = (s32)ChrTenantGet(a->id + i)->model;
            break;
        case 2:
            m = (s32)ChrGetByNo(a->id + i)->model;
            break;
        }
        if (m != 0) {
            switch (a->phase) {
            case 0:
                ChrModelSetAlpha(m, a->start[i] + a->delta[i] * t);
                break;
            case 1:
                ChrModelSetAlpha(m, a->alpha);
                break;
            case 3:
                ChrModelSetAlpha(m, a->alpha + a->delta[i] * t);
                break;
            case 4:
                ChrModelSetAlpha(m, a->end[i]);
                break;
            }
        }
    }
}
