#include "common.h"

extern f32 fldMenuJunctionMeshX[5][5];
extern f32 fldMenuJunctionMeshY[5][5];
extern f32 fldMenuJunctionMeshVelX[5][5];
extern f32 fldMenuJunctionMeshVelY[5][5];
extern f32 fldMenuJunctionMeshPrevX[5][5];
extern f32 fldMenuJunctionMeshPrevY[5][5];

void FldMenuJunctionMeshStep(f32 dt) {
    s32 i, j;
    f32 k;
    f32 a, b;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            fldMenuJunctionMeshPrevX[i][j] = fldMenuJunctionMeshX[i][j];
            fldMenuJunctionMeshPrevY[i][j] = fldMenuJunctionMeshY[i][j];
        }
    }
    k = dt * 4.0f;
    for (i = 1; i < 4; i++) {
        for (j = 1; j < 4; j++) {
            a = b = 0.0f;
            a += fldMenuJunctionMeshPrevX[i + 1][j] * dt;
            b += fldMenuJunctionMeshPrevY[i + 1][j] * dt;
            a += fldMenuJunctionMeshPrevX[i][j + 1] * dt;
            b += fldMenuJunctionMeshPrevY[i][j + 1] * dt;
            a += fldMenuJunctionMeshPrevX[i][j - 1] * dt;
            b += fldMenuJunctionMeshPrevY[i][j - 1] * dt;
            a += fldMenuJunctionMeshPrevX[i - 1][j] * dt;
            b += fldMenuJunctionMeshPrevY[i - 1][j] * dt;
            a -= fldMenuJunctionMeshPrevX[i][j] * k;
            b -= fldMenuJunctionMeshPrevY[i][j] * k;
            fldMenuJunctionMeshVelX[i][j] += a;
            fldMenuJunctionMeshVelY[i][j] += b;
            fldMenuJunctionMeshX[i][j] += fldMenuJunctionMeshVelX[i][j] * dt;
            fldMenuJunctionMeshY[i][j] += fldMenuJunctionMeshVelY[i][j] * dt;
        }
    }
}
