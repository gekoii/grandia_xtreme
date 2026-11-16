#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n25.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

extern void WmapVecApproach(void *, void *, void *, f32, f32);
extern void CamCalcMatrix(Camera *);
extern void VuVecCopy(void *, void *);

void WmapCamUpdate(s32 arg0, s32 arg1) {
    Camera *cam;
    s32 one;

    cam = &camField[0];
    VuVecCopy(wmapCursorTrail[2], wmapCursorTrail[1]);
    VuVecCopy(wmapCursorTrail[1], wmapCursorTrail[0]);
    VuVecCopy(wmapCursorTrail[0], wmapCursorPos);
    one = 1;
    switch (arg0) {
    case 0:
        WmapVecApproach(&wmapWork->camTargetVel, wmapCamTarget, wmapCamTargetGoal, 20.0f, 2.0f);
        WmapVecApproach(&wmapWork->camPosVel, wmapCamPos, &wmapCamPosGoal, 80.0f, 2.0f);
        VuVecCopy(cam->eye, wmapCamPos);
        VuVecCopy(cam->lookAt, wmapCamTarget);
        break;
    case 1:
        WmapVecApproach(&wmapWork->camTargetVel, wmapCamTarget, wmapCamTargetGoal, 20.0f, 2.0f);
        WmapVecApproach(&wmapWork->camPosVel, wmapCamPos, &wmapCamPosGoal, 20.0f, 2.0f);
        VuVecCopy(cam->eye, wmapCamPos);
        VuVecCopy(cam->lookAt, wmapCamTarget);
        break;
    case 2:
        if (arg1 == 0) {
            copyq(wmapCamTargetGoal, wmapCursorGoal);
            copyq(&wmapCamPosGoal, wmapCursorGoal);
            wmapCamPosGoal[0] *= 0.8f;
            wmapCamPosGoal[1] += 1000.0f;
            wmapCamPosGoal[2] -= 1200.0f;
        }
        if (EvtScnrIsRunning() != one) {
            WmapVecApproach(&wmapWork->camTargetVel, wmapCamTarget, wmapCamTargetGoal, 20.0f, 2.0f);
            WmapVecApproach(&wmapWork->camPosVel, wmapCamPos, &wmapCamPosGoal, 20.0f, 2.0f);
            VuVecCopy(cam->eye, wmapCamPos);
            VuVecCopy(cam->lookAt, wmapCamTarget);
        }
        break;
    }
    cam->mode = 0x23;
    cam->roll = 0;
    CamCalcMatrix(cam);
}
