#include "common.h"
#include "gph.h"

s32 GphAnimIsDone(u8 *arg0) {
    if (gphCurAnimSet == NULL) {
        return 1;
    }
    return *arg0;
}
