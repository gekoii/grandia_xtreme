#include "common.h"
#include "protos/main.h"
#include "gph.h"


void ChrModelSetMotion(GphModel *arg0, GphMotion *arg1) {
    if (arg1 == NULL) {
        ChrModelClearDeformBufs(arg0);
        return;
    }
    if (arg1->flags & 0xFF0) {
        ChrModelClearDeformBufs(arg0);
    }
    arg0->motion[0].motion = arg1;
    arg0->motion[0].frame = 0;
}
