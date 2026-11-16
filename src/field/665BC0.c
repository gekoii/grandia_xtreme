/* rodata: 0x00676298-0x006762A8 */
/* Matches newlib's sqrt error-handling wrapper (w_sqrt.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/newlib.h"
#include "protos/field.h"

struct exception {
    s32 type;
    char *name;
    double arg1;
    double arg2;
    double retval;
    s32 err;
};

s32 isnan(double) __attribute__((const));

double sqrt(double x) {
    double z;
    struct exception exc;

    z = __ieee754_sqrt(x);
    if (_LIB_VERSION == -1 || isnan(x)) return z;
    if (x < 0.0) {
        exc.type = 1;
        exc.name = "sqrt";
        exc.err = 0;
        exc.arg1 = exc.arg2 = x;
        if (_LIB_VERSION == 0)
            exc.retval = 0.0;
        else
            exc.retval = 0.0 / 0.0;
        if (_LIB_VERSION == 2)
            *__errno() = 33;
        else if (!matherr(&exc)) {
            *__errno() = 33;
        }
        if (exc.err != 0)
            *__errno() = exc.err;
        return exc.retval;
    } else
        return z;
}
