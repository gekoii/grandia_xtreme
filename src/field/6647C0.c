#include "common.h"
#include "protos/field.h"
#include "field/n27.h"

void EvtScnrItemTestToVar(s16 arg0) {
    if (evtScnrGameRefs->itemCounts[arg0] != 0) {
        EvtScnrVarSet(2, evtScnrSysVarIds->resultVar, 1);
    } else {
        EvtScnrVarSet(2, evtScnrSysVarIds->resultVar, 0);
    }
}
