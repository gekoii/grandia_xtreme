#include "common.h"
#include "field/n22.h"

void *FldOverlayImgGetTex(u32 arg0) {
    if ((fldOverlayImg == NULL) || (arg0 >= (u32)fldOverlayImg->u.s.count)) {
        return NULL;
    }
    return fldOverlayImg->tex[arg0];
}
