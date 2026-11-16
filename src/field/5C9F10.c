#include "common.h"
#include "field/n15.h"

void MemSet(void *, s32, s32);

void CamBlendInit(void) {
    MemSet(&camBlend, 0, 0x320);
}
