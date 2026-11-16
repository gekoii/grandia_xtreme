#include "common.h"
#include "main/n03.h"
#include "protos/main.h"
s32 FileDecompress(s32 arg0, s32 arg1, s32 arg2) {
    FileCmpHeader h;
    if (FileCmpReadHeader(&h, arg0) == 0) return 0; arg0 += 0x10; if (arg2 != 0) { if (h.magic != 0x15246468) return 0; FileLzssDecode(arg0, arg1, h.unkC); } return 1;
}
