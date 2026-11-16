#include "common.h"
#include "protos/field.h"

void MemSet(void *, s32, s32);

s32 MapGetMdtSizeMB(s32 arg0, s32 arg1) {
    u8 buf[0x20];

    MemSet(buf, 0, 0x20);
    MapMakeMdtPath(arg0 & 0xFFFF, buf);
    return FileGetSizeAligned(buf) / 0x100000;
}
