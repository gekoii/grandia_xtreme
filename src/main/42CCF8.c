#include "common.h"
#include "main/n03.h"

s32 McSaveSizeKB(void) {
    s32 a = (mcIconSysSize + 0x3FF) >> 10;
    s32 b = (mcIconFileSize + 0x3FF) >> 10;
    s32 c = (mcSaveDataSize[0] + 0x3FF) >> 10;
    return a + b + c + 4;
}
