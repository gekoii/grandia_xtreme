/* Port-only C for CamStateCalcEye (0x005CAD90), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/field.h"
#include "field/n15.h"

void CamStateCalcEye(CamState *c);

/* eye = lookAt - dir * dist. The asm stores the whole quadword, so eye.w
   takes dir.w. */
void CamStateCalcEye(CamState *c) {
    f32 d;

    CamStateCalcDir(c);
    d = -c->dist;
    c->eye[0] = c->dir[0] * d + c->lookAt[0];
    c->eye[1] = c->dir[1] * d + c->lookAt[1];
    c->eye[2] = c->dir[2] * d + c->lookAt[2];
    c->eye[3] = c->dir[3];
}
