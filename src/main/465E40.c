#include "common.h"
#include "protos/main.h"


s32 videoDecInputSpaceCount(s32 arg0) {
    s32 buf[4];
    videoDecBeginPut(arg0, &buf[0], &buf[1], &buf[2], &buf[3]);
    return buf[1] + buf[3];
}
