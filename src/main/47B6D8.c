/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x004A78D0-0x004A78E0 */
/* Matches newlib's fmodf error-handling wrapper (wf_fmod.c). Third-party library code: see README.md, "Third-party library code". */
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
extern s32 _LIB_VERSION;

f32 fmodf(f32 x, f32 y) {
    f32 z;
    struct exception exc;

    z = __ieee754_fmodf(x, y);
    if (_LIB_VERSION == -1 || isnanf(y) || isnanf(x)) return z;
    if (y == 0.0f) {
        exc.type = 1;
        exc.name = "fmodf";
        exc.err = 0;
        exc.arg1 = (double)x;
        exc.arg2 = (double)y;
        if (_LIB_VERSION == 0)
            exc.retval = x;
        else
            exc.retval = 0.0 / 0.0;
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
