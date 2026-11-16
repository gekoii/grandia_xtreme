#include "common.h"
#include "gph.h"

void ChrModelSetSubMotion(void *arg0, s32 arg1) {
    GphModel *m = arg0;

    m->motion[1].motion = (GphMotion *)arg1;
    m->motion[1].frame = 0;
}
