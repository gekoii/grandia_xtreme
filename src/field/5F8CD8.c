#include "common.h"
#include "save.h"

extern u16 fldNoKamaeMapIds[14];

s32 FldMapIsNoKamae(void) {
    s32 i;

    for (i = 0; i < 14; i++) {
        if (fldNoKamaeMapIds[i] == sysSaveData.mapNo) {
            return 1;
        }
    }
    return 0;
}
