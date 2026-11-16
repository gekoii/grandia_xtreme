#include "common.h"
#include "protos/field.h"
#include "field/n27.h"

void EvtScnrMoneyToVar(void) {
    EvtScnrVarSet(2, evtScnrSysVarIds->resultVar, *evtScnrGameRefs->money);
}
