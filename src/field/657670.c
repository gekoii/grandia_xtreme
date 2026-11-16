#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n25.h"

void FldTitleCamFixedUpdate(void) {
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(camField[0].lookAt) : "memory");
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(camField[0].eye) : "memory");
    camField[0].eye[1] = 100.0f;
    camField[0].eye[2] = -800.0f;
    camField[0].lookAt[1] = 100.0f;
    camField[0].fov = 10.0f;
    camField[0].dist = 800.0f;
    camField[0].clipFar = 30000.0f;
    camField[0].pitch = 0.0f;
    camField[0].yaw = 0.0f;
    camField[0].roll = 0.0f;
    camField[0].zoom = 1.0f;
    if (EvtGetMapNo() == 0x1820) {
        if (fldTitleWork->frame == 0) {
            GphScreenFxSetHold(fldScreenFx, 1, 1, 1, 80.0f);
        }
        if (fldTitleWork->frame == 0x78) {
            GphScreenFxSet(fldScreenFx, 0, 0, 0, 0.0f);
        }
        fldTitleWork->frame++;
    }
}
