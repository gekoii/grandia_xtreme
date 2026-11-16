#include "common.h"
#include "protos/main.h"
#include "camera.h"
#include "main/n06.h"


void SndListenerSetFromCamera(Camera *arg0) {
    VuVec3 a;
    VuVec3 b;

    a.x = arg0->lookAt[0];
    a.y = arg0->lookAt[1];
    a.z = arg0->lookAt[2];
    b.x = arg0->dir[0];
    b.y = arg0->dir[1];
    b.z = arg0->dir[2];
    SndSetListener(&a, &b);
}
