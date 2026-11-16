#include "common.h"
#include "protos/main.h"
#include "chr.h"


void ChrFaceAnimInit(ChrFaceAnim *arg0, ChrPackBinding *arg1, GphModel *arg2) {
    if ((arg0 != NULL) && (arg2 != NULL)) {
        arg0->faces = (s32 *)&arg1->faces;
        ChrFaceAnimSet(arg0, 0, 0);
    }
}
