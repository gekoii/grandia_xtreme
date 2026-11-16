#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n17.h"


void FldSkyLoad(s32 arg0, s32 arg1, s32 arg2) {
    GphModel *o = ChrModelCreate(FilePacGetModel(arg0, arg1));
    fldSkyTable[0].follow = arg2;
    fldSkyTable[0].enable = 1;
    fldSkyTable[0].model = o;
    o->flags |= 0x80;
}
