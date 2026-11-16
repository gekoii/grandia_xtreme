#include "common.h"
#include "m2c_macros.h"

extern s32 camFollowReqFlag;
extern s32 camFollowReqValue;

void CamFollowRequest(s32 arg0) {
    camFollowReqFlag = 1;
    camFollowReqValue = arg0;
}
