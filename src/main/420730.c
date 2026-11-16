#include "common.h"
#include "main/n02.h"

void VuVecCopy(DbgLightAngle *);

void DbgLightAngleSet(u8 arg0, void *arg1) {
    VuVecCopy(&dbgLightAngles[arg0]);
}
