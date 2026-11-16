#include "common.h"
#include "field/n22.h"

void FldSndEmitterToggle(u16 id) {
    u16 flags;
    u32 idx;
    FldSndEmitter *e;

    idx = id;
    if (idx < fldSndEmitterCount) {
        e = &fldSndEmitters[idx];
        flags = e->flags;
        if (flags & 1) {
            e->flags = flags ^ 1;
            return;
        }
        e->flags = flags | 1;
    }
}
