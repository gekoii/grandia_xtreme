#include "common.h"
#include "protos/field.h"

typedef struct {
    u8 pad0[0x15];
    u8 unk15;
    u8 pad16[0x180 - 0x16];
    f32 unk180[4];
} Obj5F9C40;


void ChrPlayerScriptModeUpdate(void) {
    Obj5F9C40 *o = ChrGetByNo(1);

    if (o != NULL) {
        if (EvtScnrIsRunning() != 0) {
            if (o->unk15 == 0) {
                o->unk15 = 1;
                __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(o->unk180) : "memory");
            }
        } else if (o->unk15 == 1) {
            o->unk15 = 0;
        }
    }
}
