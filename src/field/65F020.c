#include "common.h"
#include "field/n26.h"
#include "protos/field.h"

void McLoadFlowInit(void) {
    mcLoadState = 0;
    mcLoadPrevState = 0xFFFF;
    mcLoadStateChanged = 0;
    FldMenuSaveSetLoadMode(1);
    mcLoadPort = mcLastPort;
    mcLoadSlot = mcLastSlot;
}
