/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x004A78C8-0x004A78D0 */
/* Matches newlib's atan2f error-handling wrapper (wf_atan2.c).
 * Third-party library code: it follows the upstream source and falls under
 * that library's license; see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

struct exception {
    s32 type;
    char *name;
    double arg1;
    double arg2;
    double retval;
    s32 err;
};

s32 isnanf(f32) __attribute__((const));
f32 func_00481AE0(double);
double func_004826D8(f32) __attribute__((const));
extern s32 _LIB_VERSION;

f32 atan2f(f32 y, f32 x) {
    f32 z;
    struct exception exc;

    z = __ieee754_atan2f(y, x);
    if (_LIB_VERSION == -1 || isnanf(x) || isnanf(y)) return z;
    if (x == 0.0f && y == 0.0f) {
        exc.arg1 = func_004826D8(y);
        exc.arg2 = func_004826D8(x);
        exc.type = 1;
        exc.name = "atan2f";
        exc.err = 0;
        exc.retval = 0.0;
        if (_LIB_VERSION == 2)
            *__errno() = 33;
        else if (!matherr(&exc)) {
            *__errno() = 33;
        }
        if (exc.err != 0)
            *__errno() = exc.err;
        return func_00481AE0(exc.retval);
    } else
        return z;
}
