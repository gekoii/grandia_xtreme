#include "common.h"
#include "main/n07.h"

s32 FilePacGetMotion(FilePacMotionPack *arg0, u32 arg1) {
    if (arg0 == NULL || arg1 >= arg0->count) {
        return 0;
    }
    return arg0->entries[arg1].offset + 0x10;
}
