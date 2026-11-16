/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

extern f32 fldMenuJunctionMeshX[5][5];
extern f32 fldMenuJunctionMeshVelX[5][5];
extern s32 fldMenuJunctionWarpAmp;

void FldMenuJunctionMeshInit(void) {
    s32 i, j;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (j == 0 || j == 4 || i == 0 || i == 4) {
                fldMenuJunctionMeshX[i][j] = 0.0f;
                fldMenuJunctionMeshVelX[i][j] = 0.0f;
            } else {
                fldMenuJunctionMeshX[i][j] = (f32)((rand() >> 16) % 100 - 50) / 10000.0f;
                fldMenuJunctionMeshVelX[i][j] = (f32)((rand() >> 16) % 100 - 50) / 10000.0f;
            }
        }
    }
    fldMenuJunctionWarpAmp = 0;
    fldMenuJunction.unk24 = 0;
}
