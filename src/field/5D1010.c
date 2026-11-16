#include "common.h"
#include "field/state.h"
#include "field/n15.h"

void MemMove128(void *, void *, s32);

void CamScrSaveCamera(s32 arg0) {
    Camera *p;

    switch (arg0) {
    case 0:
        p = &camScrSaveStart;
        break;
    case 1:
        p = &camScrSaveMap;
        break;
    default:
        return;
    }
    MemMove128(p, camField, 0x18);
}
