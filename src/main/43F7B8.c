#include "common.h"
#include "gph.h"
#include "protos/main.h"

s32 GphTexSetMode(u8 arg0, s32 arg1) {
    GphTex *p = (GphTex *)GphTexGetSource(arg0);
    if (p == NULL) {
        return 0;
    }
    p->uploadMode = arg1;
    return 1;
}
