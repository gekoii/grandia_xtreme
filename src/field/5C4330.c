#include "common.h"
#include "protos/field.h"


s32 ChrTenantOpStartOccs(s32 arg0, s32 arg1, s16 arg2) {
    OccsWake(arg2);
    return 0;
}
