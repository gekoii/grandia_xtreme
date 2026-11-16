#include "common.h"
#include "protos/main.h"
#include "gph.h"


s32 GphTexRestoreMode(u8 arg0) {
    GphTexSlot *p = GphTexGetSlot(arg0);

    if (p == NULL) {
        return 0;
    }
    p->src->uploadMode = p->savedMode;
    return 1;
}
