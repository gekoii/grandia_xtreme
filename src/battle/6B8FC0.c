#include "common.h"
#include "protos/battle.h"

s32 BtlFileRelocate(s32 arg0) {
    if (arg0 == 0) {
        return 0;
    }
    FileChunkRelocate(arg0);
    return arg0 + 0x10;
}
