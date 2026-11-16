/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "m2c_macros.h"

s32 sbrk(s32);                             /* extern */
extern s32 errno;

s32 _sbrk_r(s32 *arg0, s32 arg1) {
    s32 temp_v0;

    errno = 0;
    temp_v0 = sbrk(arg1);
    if (temp_v0 == 0xFFFFFFFF) {
        if (errno != 0) {
            *arg0 = errno;
        }
    }
    return temp_v0;
}
