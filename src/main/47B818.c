/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x004A78E0-0x004A78F0 */
/* Matches newlib's logf error-handling wrapper (wf_log.c).
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
double func_004812A8(double, double);
f32 func_00481AE0(double);
double func_004826D8(f32) __attribute__((const));
extern double __infinity;
extern s32 _LIB_VERSION;

f32 logf(f32 x) {
    f32 z;
    struct exception exc;

    z = __ieee754_logf(x);
    if (_LIB_VERSION == -1 || isnanf(x) || x > 0.0f) return z;
    exc.name = "logf";
    exc.err = 0;
    exc.arg1 = exc.arg2 = func_004826D8(x);
    if (_LIB_VERSION == 0)
        exc.retval = -3.4028234663852886e+38;
    else
        exc.retval = func_004812A8(0.0, __infinity);
    if (x == 0.0f) {
        exc.type = 2;
        if (_LIB_VERSION == 2)
            *__errno() = 34;
        else if (!matherr(&exc)) {
            *__errno() = 33;
        }
    } else {
        exc.type = 1;
        if (_LIB_VERSION == 2)
            *__errno() = 33;
        else if (!matherr(&exc)) {
            *__errno() = 33;
        }
    }
    if (exc.err != 0)
        *__errno() = exc.err;
    return func_00481AE0(exc.retval);
}
