/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 x0;
    s32 x4;
    s32 x8;
} S84518;

extern s32 __sfvwrite(s32, S84518 *);

s32 __sprint(s32 arg0, S84518 *arg1) {
    s32 ret;

    if (arg1->x8 == 0) {
        arg1->x4 = 0;
        return 0;
    }
    ret = __sfvwrite(arg0, arg1);
    arg1->x8 = 0;
    arg1->x4 = 0;
    return ret;
}
