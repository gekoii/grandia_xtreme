#include "common.h"
#include "protos/main.h"
#include "grxsys.h"
#include "main/n02.h"


s32 SysCommonModelGet(u32 arg0) {
    s32 v;

    if (arg0 >= 100) {
        return 0;
    }
    v = sysCommonModelGroups[arg0];
    if (v == -1) {
        v = 0;
    }
    return FilePacGetModel(sysGrxsysPac->modelPackB0, v);
}
