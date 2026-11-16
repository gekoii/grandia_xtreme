#include "common.h"
#include "field/n22.h"

void FldOverlayImgShow(u8 arg0) {
    FldOverlayImg *t = fldOverlayImg;

    if (t != NULL && t->u.s.count != 0) {
        t->u.s.show = arg0;
        if (arg0 != 0) {
            t->u.s.alpha = 0;
        }
    }
}
