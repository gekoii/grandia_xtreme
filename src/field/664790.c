#include "common.h"
#include "field/n27.h"

s32 EvtScnrExtFlagGet(u16 arg0) {
    return (evtScnrGameRefs->flags[arg0 >> 3] >> (arg0 & 7)) & 1;
}
