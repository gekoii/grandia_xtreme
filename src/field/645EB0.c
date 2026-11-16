#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n24.h"


void MiniConcertCarrosCreate(void) {
    s32 i, j;

    if (miniCtl->dataFile != 0 && miniCtl->kind != 2) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 5; j++) {
                miniCtl->carros[i][j] = ChrTenantAddFromPack(i);
            }
        }
    }
}
