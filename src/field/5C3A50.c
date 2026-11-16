#include "common.h"
#include "protos/field.h"


s32 ChrTenantOpRestartAi(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    ChrTenantAiRestart(arg2, arg3);
    return 0;
}
