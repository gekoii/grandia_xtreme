#include "common.h"
#include "field/n15.h"

void VuVecCopy(void *, void *);

void CamTrailPushBack(void *src) {
    s32 i = camTrail->tail + 1;

    if (i >= 32) {
        i = 0;
    }
    VuVecCopy(camTrail->points[i], src);
    camTrail->tail = i;
}
