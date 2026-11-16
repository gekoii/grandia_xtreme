#include "common.h"
#include "protos/main.h"
#include "main/n10.h"


s32 SndUnused0(void) {
    return 0;
}

s32 SndStop(s32 arg0) {
    SndHandle *temp_v0;

    temp_v0 = SndFindHandle(arg0);
    if (temp_v0 == 0) {
        return -3;
    }
    return SndHandleStop(temp_v0);
}
