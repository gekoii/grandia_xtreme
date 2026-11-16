#include "common.h"
#include "field/event.h"

void EvtScnrBlock790Reset(void) {
    s32 i;

    evtScnrBlock790->unk0 = 0;
    evtScnrBlock790->unk2 = 0;
    evtScnrBlock790->unk4 = 0;
    for (i = 0; i < 4; i++) {
        evtScnrBlock790->unk6[i] = 0;
        evtScnrBlock790->unkA[i] = 0;
    }
}
