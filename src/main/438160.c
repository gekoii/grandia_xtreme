#include "common.h"
#include "main/n05.h"

void *ChrModelPoolAlloc(s32 arg0) {
    ChrModelPool *p = chrModelPool;
    s32 t = p->cur;
    t = (t + 0xF) & 0xFFFFFFF0;
    p->cur = t + arg0;
    return (void *)t;
}
