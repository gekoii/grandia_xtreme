#include "common.h"
#include "gph.h"
#include "protos/main.h"
#include "m2c_macros.h"


void GphModelDrawSimpleInit(void) {

}

void GphModelDrawSimple(GphModel *arg0) {
    GphModelDrawSimpleMtx(arg0, VuMtxStackGetTop());
}
