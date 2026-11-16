#include "common.h"
#include "field/n22.h"

void FldSndEmitterEnable(u16 idx, u8 on) {
    FldSndEmitter *o;

    if (idx < fldSndEmitterCount) {
        o = &fldSndEmitters[idx];
        if (on == 1) {
            o->flags |= 1;
        } else if (o->flags & 1) {
            o->flags ^= 1;
        }
    }
}
