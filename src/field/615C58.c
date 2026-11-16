#include "common.h"
#include "protos/field.h"
#include "field/n22.h"


void AmapLoadObjPack(s32 arg0) {
    s32 *p;

    p = FilePacModelRelocate(arg0);
    amapState->objPack = p;
    FilePacModelBindTextures(p);
    amapState->objPackCount = *amapState->objPack;
}
