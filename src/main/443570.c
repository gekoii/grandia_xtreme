#include "common.h"
#include "gph.h"
#include "main/n07.h"

void GphFbWarpMeshStep(f32 arg0) {
    s32 i;
    s32 j;
    f32 k;
    f32 sx;
    f32 sy;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            gphWarpPrevX[i][j] = gphWarpMeshX[i][j];
            gphWarpPrevY[i][j] = gphWarpMeshY[i][j];
        }
    }
    k = arg0 * 4.0f;
    for (i = 1; i < 8; i++) {
        for (j = 1; j < 8; j++) {
            sx = sy = 0.0f;
            sx += gphWarpPrevX[i + 1][j] * arg0;
            sy += gphWarpPrevY[i + 1][j] * arg0;
            sx += gphWarpPrevX[i][j + 1] * arg0;
            sy += gphWarpPrevY[i][j + 1] * arg0;
            sx += gphWarpPrevX[i][j - 1] * arg0;
            sy += gphWarpPrevY[i][j - 1] * arg0;
            sx += gphWarpPrevX[i - 1][j] * arg0;
            sy += gphWarpPrevY[i - 1][j] * arg0;
            sx -= gphWarpPrevX[i][j] * k;
            sy -= gphWarpPrevY[i][j] * k;
            gphWarpVelX[i][j] += sx;
            gphWarpVelY[i][j] += sy;
            gphWarpMeshX[i][j] += gphWarpVelX[i][j] * arg0;
            gphWarpMeshY[i][j] += gphWarpVelY[i][j] * arg0;
        }
    }
}
