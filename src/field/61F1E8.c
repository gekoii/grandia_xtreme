#include "common.h"
#include "field/menu.h"

FldMenuQuad *FldMenuGetSprBuf(u8 arg0) {
    return fldMenuWork->sprBufs[arg0];
}
