#include "common.h"
#include "protos/main.h"
#include "gph.h"


void ChrModelSetMotionFrame(GphModel *arg0, GphMotion *arg1, f32 arg2) {
    if (arg1->flags & 0xFF0) {
        ((void (*)(GphModel *, GphMotion *))ChrModelClearDeformBufs)(arg0, arg1);
    }
    arg0->motion[0].motion = arg1;
    arg0->motion[0].frame = arg2;
}
