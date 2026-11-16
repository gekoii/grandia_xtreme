/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "gph.h"
#include "main/n07.h"
#include "protos/main.h"

void GphFbEffectInit(void) {
    s32 i, j, x, y;

    gphFbPacket = (GphFbPacket *)0x70003C00;
    for (y = 0; y < 9; y++) {
        for (x = 0; x < 9; x++) {
            if (x == 0 || x == 8 || y == 0 || y == 8) {
                gphWarpMeshX[y][x] = 0.0f;
                gphWarpVelX[y][x] = 0.0f;
            } else {
                gphWarpMeshX[y][x] = (f32)((rand() >> 16) % 100 - 50) / 10000.0f;
                gphWarpVelX[y][x] = (f32)((rand() >> 16) % 100 - 50) / 10000.0f;
            }
        }
    }
    GphFbVWaveSet(0, 0, 0x40, 0x20, 1, 0, 0, 1, 0x30);
    y = -112;
    for (i = 0; i < 3; i++) {
        x = -160;
        for (j = 0; j < 3; j++) {
            gphNineDivTilesV[j].x[i] = x;
            gphNineDivTilesV[j].y[i] = y;
            x += 320;
        }
        y += 224;
    }
    y = -112;
    for (j = 0; j < 3; j++) {
        x = -160;
        for (i = 0; i < 3; i++) {
            gphNineDivTilesH[j].x[i] = x;
            gphNineDivTilesH[j].y[i] = y;
            x += 320;
        }
        y += 224;
    }
}
