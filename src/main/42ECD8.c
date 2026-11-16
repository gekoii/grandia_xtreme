#include "common.h"
#include "protos/main.h"


s32 FileGetSize(s32 arg0) {
    s32 buf[4];
    s32 r;
    r = SndFsFind(arg0, buf);
    return (r < 1) ? 0 : buf[1];
}
