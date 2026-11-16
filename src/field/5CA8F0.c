#include "common.h"
#include "field/n15.h"

f32 CamFollowGetDistance(s32 i) {
    f32 r = 0.0f;

    switch (i) {
    case 0:
        r = camFollow->dist[0];
        break;
    case 1:
        r = camFollow->dist[1];
        break;
    case 2:
        r = camFollow->dist[2];
        break;
    }
    return r;
}
