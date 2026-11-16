/* cflags: -fno-builtin */
/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x004A78C0-0x004A78C8 */
/* Matches newlib's asinf error-handling wrapper (wf_asin.c). Third-party library code: see README.md, "Third-party library code". */
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

f32 fabsf(f32) __attribute__((const));
s32 isnanf(f32) __attribute__((const));
f32 func_00481AE0(double);
double func_004826D8(f32) __attribute__((const));
extern s32 _LIB_VERSION;

f32 asinf(f32 x) {
    f32 z;
    struct exception exc;

    z = __ieee754_asinf(x);
    if (_LIB_VERSION == -1 || isnanf(x)) return z;
    if (fabsf(x) > 1.0f) {
        exc.type = 1;
        exc.name = "asinf";
        exc.err = 0;
        exc.arg1 = exc.arg2 = func_004826D8(x);
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
