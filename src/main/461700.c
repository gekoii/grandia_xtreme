#include "common.h"
#include "main/n10.h"
#include "protos/main.h"

s32 SndSetListener(VuVec3 *arg0, VuVec3 *arg1) {
    sndListenerPos.x = arg0->x;
    sndListenerPos.y = arg0->y;
    sndListenerPos.z = arg0->z;
    sndListenerDir.x = arg1->x;
    sndListenerDir.y = arg1->y;
    sndListenerDir.z = arg1->z;
    /* s32 return: the u8 result is stored without masking */
    sndListenerAngle = ((s32 (*)(f32, f32))SndCalcAngle)(sndListenerDir.x, sndListenerDir.z);
    return 0;
}
