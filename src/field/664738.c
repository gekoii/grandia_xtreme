#include "common.h"
#include "field/n27.h"

void EvtScnrExtFlagSet(u16 arg0, s32 arg1) {
    EvtScnrGameRefs *f = evtScnrGameRefs;

    f->flags[arg0 >> 3] &= ~(1 << (arg0 & 7));
    f->flags[arg0 >> 3] |= (arg1 & 1) << (arg0 & 7);
}
