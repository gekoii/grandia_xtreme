#include "common.h"
#include "protos/main.h"
#include "main/n11.h"

s32 videoDecPutTs(VideoDec *arg0, s64 arg1, s64 arg2, s32 arg3, s32 arg4) {
    MovTimeStamp s;

    s.pts = arg1;
    s.dts = arg2;
    s.pos = arg3 - (s32)arg0->vibuf.data;
    s.len = arg4;
    return viBufPutTs(&movVideoDecViBuf, &s);
}
