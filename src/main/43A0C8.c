#include "common.h"
#include "gph.h"
#include "protos/main.h"
#include "m2c_macros.h"


void GphModelDrawCopy(GphModel *arg0) {
    GphModelDrawCopyMtx(arg0, VuMtxStackGetTop());
}
