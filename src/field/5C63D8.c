#include "common.h"

void *memset(void *, s32, u32);
extern s32 ChrTrailFill(s32, void *, s32);

void ChrTrailReset(s32 arg0) {
    u8 buf[16];
    memset(buf, 0, 0x10);
    ChrTrailFill(arg0, buf, 0);
}
