#include "common.h"
#include "field/n21.h"

s32 AmapPathsHaveRepeat(AmapWork *o) {
    s32 i;
    s32 j;
    s32 x;
    s32 z;
    s32 px;
    s32 pz;
    s32 found;
    s32 end;
    AmapPathRange *ranges;
    VuVec3 *pts;

    found = 0;
    ranges = o->pathRanges;
    pts = o->pathPoints;
    for (i = 0; i < o->pathCount; i++) {
        px = -1;
        pz = -1;
        end = ranges[i].end;
        for (j = ranges[i].start; j <= end; j++) {
            x = pts[j].x;
            z = pts[j].z;
            if (px == x && pz == z) {
                found = 1;
                break;
            }
            px = x;
            pz = z;
        }
    }
    return found;
}
