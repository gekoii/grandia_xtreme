#include "common.h"
#include "field/n15.h"

void CamFollowSetDistance(s32 arg0, f32 fparg0) {
    switch (arg0) {                                 /* irregular */
    case 0:
        camFollow->dist[0] = fparg0;
        return;
    case 1:
        camFollow->dist[1] = fparg0;
        return;
    case 2:
        camFollow->dist[2] = fparg0;
        return;
    }
}
