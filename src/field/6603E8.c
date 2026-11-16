#include "common.h"
#include "field/n27.h"

void EvtScnrTempFlagsReset(void) {
    evtScnrGameRefs->unk0 = 0;
    evtScnrGameRefs->unk4 = 0;
    evtScnrGameRefs->unk8 = 0;
}
