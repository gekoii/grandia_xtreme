#include "common.h"
#include "sys.h"
#include "protos/main.h"

extern void MemSet(void *, s32, s32);

void SysDemoReset(void) {
    MemSet(&sysDemo, 0, 0x440);
    GphFadeClear(&sysDemo.fade);
    GphFadeClear(&sysDemo.fade2);
}
