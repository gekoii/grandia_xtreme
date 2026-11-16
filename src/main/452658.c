#include "common.h"
#include "sys.h"
#include "main/n07.h"

extern void MemSet(s32, s32, s32);

void TxtFontImageClear(TxtFontImage *arg0) {
    if (txtFontImageEnable != 0 && arg0 != NULL) {
        MemSet((s32)arg0->pixels, 0, arg0->pixelSize);
    }
}
