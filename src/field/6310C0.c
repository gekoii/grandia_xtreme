#include "common.h"
#include "protos/field.h"
#include "field/n23.h"

void *MemSet(void *, s32, s32);

void FldMenuStatusInit(s32 arg0) {
    MemSet(&fldMenuStatus, 0, sizeof(FldMenuStatus));
    FldMenuWinOpen(8, 0x140, 0x10E, 0x21C, 0x118, 0, 0, 0x89001);
    FldMenuGuideSet(5);
    fldMenuStatus.chr = arg0;
    fldMenuStatus.chrNext = arg0;
}
