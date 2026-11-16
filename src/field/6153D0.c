#include "common.h"
#include "field/n22.h"

void FldOverlayImgUpdate(void) {
    FldOverlayImg *t = fldOverlayImg;

    if (t != NULL && t->u.s.count != 0 && (t->u.all & 0xFFFFFFFF00000000ULL)) {
        if (t->u.s.show == 0) {
            if (t->u.s.alpha >= 0) {
                t->u.s.alpha--;
            }
        } else if (t->u.s.alpha < 0x7F) {
            t->u.s.alpha++;
        }
    }
}
