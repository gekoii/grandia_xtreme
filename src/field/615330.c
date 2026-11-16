#include "common.h"
#include "field/n22.h"

void *FldMemAlloc(s32);
void *MemSet(void *, s32, s32);

void FldOverlayImgInit(void) {
    fldOverlayImg = FldMemAlloc(sizeof(FldOverlayImg));
    MemSet(fldOverlayImg, 0, sizeof(FldOverlayImg));
}
