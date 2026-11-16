#include "common.h"
#include "field/menu.h"

void FldMenuGuideSet(s32 arg0) {
    u8 temp_a0;

    temp_a0 = arg0 & 0xFF;
    if (fldMenuWork->guideMode != temp_a0) {
        fldMenuWork->guideMode = temp_a0;
        fldMenuWork->unkD = 0;
        fldMenuWork->unkC = 0;
    }
}
