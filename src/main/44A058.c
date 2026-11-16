#include "common.h"
#include "protos/main.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Rot;

void VuMtxSetRotX(s32, s32);

void VuMtxSetRotXY(s32 m, f32 a, f32 b, f32 c) {
    Rot r;
    r.z = 0;
    r.x = a * 10430.378f;
    r.y = b * 10430.378f;
    VuMtxSetRotX(m, r.x);
    VuMtxRotY(m, r.y, m);
    VuMtxRotZ(m, r.z, m);
}
