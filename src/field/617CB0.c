#include "common.h"
#include "field/n22.h"

void FldSndEmitterToggleById(u16 key) {
    u32 i;
    FldSndEmitter *o;

    for (i = 0; i < fldSndEmitterCount; i++) {
        o = &fldSndEmitters[i];
        if (o->se == key) {
            if (o->flags & 1) {
                o->flags ^= 1;
            } else {
                o->flags |= 1;
            }
            break;
        }
    }
}
