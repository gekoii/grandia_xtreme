#include "common.h"
#include "field/n26.h"

EvtPictureReq *EvtScnrGetPicture(s32 arg0) {
    return &evtPictureReqs[arg0];
}
