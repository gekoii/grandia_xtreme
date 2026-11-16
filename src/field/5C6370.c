#include "common.h"
#include "protos/field.h"
#include "field/n14.h"


void FldDmgPopDraw(void) {
    if (fldDmgPopWork->count != 0) {
        GphBatchSetOffset(0, 0);
        GphTexBind(0xE9);
        GphBatchDrawSprites(fldDmgPopWork->bars);
        GphTexBind(0xEB);
        GphBatchDrawSprites(fldDmgPopWork->digits);
    }
}
