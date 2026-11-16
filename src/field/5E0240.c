#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"


s32 EffObjDraw(PtclEmitter *arg0) {
    if (arg0 == NULL) {
        return 0x42;
    }
    if (arg0->active == 0) {
        return 0x41;
    }
    PtclEmitterDraw(arg0, camField);
    return arg0->unk12B9;
}
