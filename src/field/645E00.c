#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniLoad(s32 arg0, s32 arg1) {
    miniCtl->dataFile = arg0;
    if (arg0 != 0) {
        MiniUiAlloc();
        if (arg1 != 0) {
            miniCtl->kind = 2;
            MiniBallAlloc();
            ((s64 (*)(void))MiniBallDataLoad)();
            return;
        }
        miniCtl->kind = 0;
        MiniConcertAlloc();
        MiniConcertDataLoad();
    }
}
