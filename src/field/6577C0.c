#include "common.h"
#include "field/n25.h"

void FldPictureShow(u8 arg0) {
    u8 v;

    fldPictureWork->req = arg0;
    v = fldPictureWork->slots[0];
    if (v != 0) {
        fldPictureWork->slots[1] = v;
        fldPictureWork->t[1] = 0;
    }
    fldPictureWork->slots[0] = arg0;
    fldPictureWork->shown = 1;
    fldPictureWork->t[0] = 0;
}
