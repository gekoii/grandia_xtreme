#include "common.h"
#include "field/n15.h"

void VuVecCopy(void *, void *);

void CamTrailPushFront(void *src) {
    s32 i = camTrail->head - 1;

    if (i < 0) {
        i = 31;
    }
    VuVecCopy(camTrail->points[i], src);
    camTrail->head = i;
}
