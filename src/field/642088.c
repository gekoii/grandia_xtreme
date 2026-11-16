#include "common.h"
#include "protos/field.h"
#include "field/n24.h"

void FldMenuJunctionScatterInit(void) {
    FldMenuJunctionPiece (*base)[5];
    FldMenuJunctionPiece *p;
    s32 i, j;
    f32 k320, k5;

    base = FldMenuGetParticleBuf();
    k320 = 320.0f;
    k5 = 5.0f;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            p = &base[i][j];
            p->pts[0][0] = VuRandSigned(1280.0f) + k320;
            p->pts[0][1] = VuRandSigned(896.0f) + 224.0f;
            p->pts[0][2] = 10.0f;
            p->pts[1][0] = VuRandSigned(5120.0f);
            p->pts[1][1] = VuRandSigned(3584.0f);
            p->pts[1][2] = k5;
            p->pts[2][0] = VuRandSigned(2560.0f);
            p->pts[2][1] = VuRandSigned(1792.0f);
            p->pts[2][2] = k5;
            p->pts[3][0] = j * 24;
            p->pts[3][1] = i * 24;
            p->pts[3][2] = 1.0f;
        }
    }
}
