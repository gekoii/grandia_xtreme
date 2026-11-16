#include "common.h"
#include "protos/main.h"
#include "main/n03.h"

extern u8 dbgFileCntStrOps[];

void DbgFileCountsDraw(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        DbgPrintf(2, i + 4, dbgFileCntStrOps, fileReadWork.log[0][i], fileReadWork.log[1][i], fileReadWork.log[2][i],
                      fileReadWork.log[3][i], fileReadWork.log[4][i], fileReadWork.log[5][i]);
    }
}
