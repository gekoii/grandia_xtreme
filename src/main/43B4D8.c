#include "common.h"
#include "protos/main.h"
#include "gph.h"

f32 ChrModelGetAlphaRaw(GphModel *arg0) {
    return ChrPartGetAlphaRaw(arg0->parts);
}
