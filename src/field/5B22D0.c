#include "common.h"
#include "field/event.h"
#include "field/n12.h"

s32 EvtAreaGetUse(void) {
    return evtAreaUse;
}

void EvtAreaSetUse(s32 arg0) {
    evtAreaUse = arg0;
}

void EvtAreaClearCount(void) {
    evtAreaTable->count = 0;
}

void EvtAreaSetCount(s32 arg0) {
    evtAreaTable->count = arg0;
}

s32 EvtAreaGetCount(void) {
    return evtAreaTable->count;
}
