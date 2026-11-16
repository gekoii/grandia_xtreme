#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void ChrFindHeadPart(void *arg0) {
    ChrModelFindPart(((ChrObj *)arg0)->model, 0x280);
}
