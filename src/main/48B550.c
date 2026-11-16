/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "m2c_macros.h"

extern s32 errno;

s32 _close_r(s32 *arg0, s32 arg1) {
    s32 temp_v0;

    errno = 0;
    temp_v0 = close(arg1);
    if (temp_v0 == -1) {
        if (errno != 0) {
            *arg0 = errno;
        }
    }
    return temp_v0;
}

s32 _fstat_r(s32 *arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;

    errno = 0;
    temp_v0 = fstat(arg1, arg2);
    if (temp_v0 == -1) {
        if (errno != 0) {
            *arg0 = errno;
        }
    }
    return temp_v0;
}

s32 _lseek_r(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;

    errno = 0;
    temp_v0 = lseek(arg1, arg2, arg3);
    if (temp_v0 == -1) {
        if (errno != 0) {
            *arg0 = errno;
        }
    }
    return temp_v0;
}

s64 _read_r(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;

    errno = 0;
    temp_v0 = read(arg1, arg2, arg3);
    if (temp_v0 == -1) {
        if (errno != 0) {
            *arg0 = errno;
        }
    }
    return temp_v0;
}
