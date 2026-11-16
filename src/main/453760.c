#include "common.h"
#include "main/n07.h"

s32 FilePacGetModel(FilePacModelPack *arg0, u32 arg1) {
    if (arg0 == NULL || arg1 >= arg0->modelCount) {
        return 0;
    }
    return arg0->models[arg1] + 0x10;
}
