#include "common.h"
#include "protos/field.h"
#include "field/event.h"

s32 EvtAreaHitBoundSphere(f32 *, f32, EvtArea *);
s32 EvtAreaHitBox(f32 *, f32, EvtArea *);
s32 EvtAreaHitCylinder(f32 *, f32, EvtArea *);

s32 EvtAreaHitTest(f32 *arg0, f32 f, EvtArea *arg1) {
    s32 ret = 0;

    if (EvtAreaHitBoundSphere(arg0, f, arg1) != 0) {
        switch (arg1->shape) {
        case EVT_AREA_BOX:
            ret = EvtAreaHitBox(arg0, f, arg1);
            break;
        case EVT_AREA_CAPSULE:
            ret = EvtAreaHitCapsule(arg0, f, arg1);
            break;
        case EVT_AREA_CYLINDER:
            ret = EvtAreaHitCylinder(arg0, f, arg1);
            break;
        case EVT_AREA_SPHERE:
            ret = 1;
            break;
        }
    }
    return ret;
}
