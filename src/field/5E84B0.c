#include "common.h"
#include "field/n17.h"

void MemSet(void *, s32, s32);

void ChrPartyModelsClear(void) {
    MemSet(chrPartyModels, 0, sizeof(chrPartyModels));
}
