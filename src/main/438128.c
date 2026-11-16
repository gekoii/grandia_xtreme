#include "common.h"
#include "main/n05.h"

s32 ChrModelPoolInit(ChrModelPool *arg0, s32 arg1, s32 arg2) {
    arg0->cur = arg1;
    arg0->end = arg1 + arg2;
    arg0->start = arg1;
    return 0;
}
