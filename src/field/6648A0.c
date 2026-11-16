#include "common.h"
#include "field/n27.h"

void EvtScnrMoneyAdd(s32 arg0) {
    s32 *temp_a1;

    temp_a1 = evtScnrGameRefs->money;
    *temp_a1 += arg0;
}

void EvtScnrMoneySub(s32 arg0) {
    s32 *temp_a1;

    temp_a1 = evtScnrGameRefs->money;
    *temp_a1 -= arg0;
}
