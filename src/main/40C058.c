#include "common.h"
#include "protos/main.h"
#include "gph.h"

u8 UtilTexRegister(void *arg0) {
    GphTex *tex = arg0;

    GphTexDataRelocate(tex);
    tex->id = GphTexRegister(tex);
    return tex->id;
}
