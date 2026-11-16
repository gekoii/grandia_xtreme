#include "common.h"
#include "protos/main.h"
#include "gph.h"


s32 ChrModelUpdate(GphModel *arg0, f32 f) {
    ChrModelUvScrollUpdate(arg0, f);
    return ChrModelMotionUpdate(arg0, f);
}
