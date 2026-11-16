#include "common.h"
#include "protos/main.h"


s32 SndFsLoadFile(s32 arg0, s32 arg1) {
    u8 buf[16];
    s32 n;
    s32 blocks;

    n = SndFsFind(arg0, buf);
    if (n >= 0) {
        blocks = n >> 11;
        if (n & 0x7FF) {
            blocks++;
        }
        n = SndFsReadSectors(arg0, arg1, 0, blocks) << 11;
    }
    return n;
}
