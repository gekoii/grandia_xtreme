#include "common.h"
#include "protos/main.h"
s32 videoDecIsFlushed(s32 arg0) {
    s32 ret = 0; if (videoDecInputCount(arg0) == 0) { ret = sceMpegIsRefBuffEmpty(arg0) != 0; } return ret;
}
