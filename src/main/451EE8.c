#include "common.h"
#include "gph.h"

void MemSet(void *, s32, s32);

void GphFadeClear(GphFade *p) {
    MemSet(p, 0, 0x130);
}
