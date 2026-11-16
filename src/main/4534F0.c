#include "common.h"

s32 FilePacGetTextureCount(s32 *arg0) {
    if (arg0 == NULL) {
        return 0;
    }
    return arg0[1];
}
