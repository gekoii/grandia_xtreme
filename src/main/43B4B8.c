#include "common.h"
#include "protos/main.h"
#include "gph.h"

f32 ChrModelGetAlpha(GphModel *arg0) {
    return ChrPartGetAlpha(arg0->parts);
}
