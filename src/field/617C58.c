#include "common.h"
#include "protos/field.h"
#include "field/n22.h"

/* void: 0x005FD000 reads v0 after the call (FldSndEmitterEnable's result, if found) through a cast. */
void FldSndEmitterEnableById(u16 key, u8 on) {
    u32 i;

    for (i = 0; i < fldSndEmitterCount; i++) {
        if (((FldSndEmitter *)((u32)fldSndEmitters + (i << 7)))->se == key) {
            FldSndEmitterEnable(i, on);
            break;
        }
    }
}
