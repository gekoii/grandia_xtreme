#include "common.h"
#include "field/n15.h"

void MemSet(void *, s32, s32);

void CamTrailInit(void) {
    MemSet(&camTrailBuf, 0, 0x210);
}
