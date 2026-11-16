#include "common.h"
#include "protos/main.h"


u32 *MovGsSpriteTex(u32 *arg0, s32 *arg1, s32 *arg2) {
    return MovGsXyz2(MovGsUv(MovGsXyz2(MovGsUv(MovGsPrim(arg0, 6, 0, 1, 0, 0, 0, 1, 0, 0), arg2[0], arg2[1]), arg1[0], arg1[1], 0), arg2[0] + arg2[2], arg2[1] + arg2[3]), arg1[0] + arg1[2], arg1[1] + arg1[3], 0);
}
