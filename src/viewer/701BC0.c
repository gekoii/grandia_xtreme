#include "common.h"
#include "viewer/n35.h"

void VwrFlockGetWaypointNeg(VuVec4 *out) {
    VwrVec4 *p = &vwrFlockWaypoints.v[vwrFlockWaypoints.cur];
    out->x = -p->x;
    out->y = -p->y;
    out->z = -p->z;
}
