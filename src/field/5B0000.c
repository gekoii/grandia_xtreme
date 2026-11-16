#include "common.h"
#include "protos/field.h"


s32 FldEntry(void) {
    SysProgRegister(FldInit, FldResume, FldUseDataHeap, FldUpdate,
                  FldBuildDrawLists, FldDraw, FldModelCalcPass, FldDebugHook);
    return 0;
}
