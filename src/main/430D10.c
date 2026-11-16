#include "common.h"
#include "main/n03.h"
#include "gph.h"

void GphClearRenderStats(void) {
    GphRenderCtx *p = gphRenderCtx;
    p->modelCount = 0;
    p->unk14 = 0;
    p->unk18 = 0;
    p->gaObjCount = 0;
    p->gaObjDrawn = 0;
    p->unk1C = 0;
    p->texUploads = 0;
    p->texUploadSize = 0;
    p->unk28 = 0;
}
