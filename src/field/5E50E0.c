#include "common.h"
#include "field/event.h"

s32 MapHtaHitBox(s32, s32, EvtArea *);
s32 MapHtaHitCapsule(s32, s32, EvtArea *);
s32 MapHtaHitCylinder(s32, s32, EvtArea *);

s32 MapHtaHitArea(s32 arg0, s32 arg1, EvtArea *arg2, f32 r) {
    switch (arg2->shape) {
    case EVT_AREA_BOX:
        return MapHtaHitBox(arg0, arg1, arg2);
    case EVT_AREA_CAPSULE:
        return MapHtaHitCapsule(arg0, arg1, arg2);
    case EVT_AREA_CYLINDER:
        return MapHtaHitCylinder(arg0, arg1, arg2);
    case EVT_AREA_SPHERE:
        return 1;
    default:
        return 0;
    }
}
