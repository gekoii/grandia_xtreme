#include "common.h"
#include "field/n27.h"

void EvtScnrItemInc(s16 arg0) {
    u8 *temp_a0;
    u8 temp_v1;

    temp_a0 = evtScnrGameRefs->itemCounts + arg0;
    temp_v1 = *temp_a0;
    if (temp_v1 < 0x63U) {
        *temp_a0 = temp_v1 + 1;
    }
}

void EvtScnrItemDec(s16 arg0) {
    u8 *temp_v1;
    u8 temp_v0;

    temp_v1 = evtScnrGameRefs->itemCounts + arg0;
    temp_v0 = *temp_v1;
    if (temp_v0 != 0) {
        *temp_v1 = temp_v0 - 1;
    }
}
