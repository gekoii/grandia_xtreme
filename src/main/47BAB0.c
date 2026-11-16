/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x004A7900-0x004A7920 */
/* Matches newlib's powf error-handling wrapper (wf_pow.c). Third-party library code: see README.md, "Third-party library code". */
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

double rint(double) __attribute__((const));
s32 finitef(f32) __attribute__((const));
s32 isnanf(f32) __attribute__((const));
f32 func_00481AE0(double);
extern const s32 _LIB_VERSION;
extern const double __infinity;

f32 powf(f32 x, f32 y) {
    f32 z;
    struct exception exc;

    z = __ieee754_powf(x, y);
    if (_LIB_VERSION == -1 || isnanf(y)) return z;
    if (isnanf(x)) {
        if (y == 0.0f) {
            exc.type = 1;
            exc.name = "powf";
            exc.err = 0;
            exc.arg1 = (double)x;
            exc.arg2 = (double)y;
            exc.retval = x;
            if (_LIB_VERSION == -1 || _LIB_VERSION == 2) exc.retval = 1.0;
            else if (!matherr(&exc)) {
                *__errno() = 33;
            }
            if (exc.err != 0)
                *__errno() = exc.err;
            return func_00481AE0(exc.retval);
        } else
            return z;
    }
    if (x == 0.0f) {
        if (y == 0.0f) {
            exc.type = 1;
            exc.name = "powf";
            exc.err = 0;
            exc.arg1 = (double)x;
            exc.arg2 = (double)y;
            exc.retval = 0.0;
            if (_LIB_VERSION != 0) exc.retval = 1.0;
            else if (!matherr(&exc)) {
                *__errno() = 33;
            }
            if (exc.err != 0)
                *__errno() = exc.err;
            return func_00481AE0(exc.retval);
        }
        if (finitef(y) && y < 0.0f) {
            exc.type = 1;
            exc.name = "powf";
            exc.err = 0;
            exc.arg1 = (double)x;
            exc.arg2 = (double)y;
            if (_LIB_VERSION == 0)
                exc.retval = 0.0;
            else
                exc.retval = -__infinity;
            if (_LIB_VERSION == 2)
                *__errno() = 33;
            else if (!matherr(&exc)) {
                *__errno() = 33;
            }
            if (exc.err != 0)
                *__errno() = exc.err;
            return func_00481AE0(exc.retval);
        }
        return z;
    }
    if (!finitef(z)) {
        if (finitef(x) && finitef(y)) {
            if (isnanf(z)) {
                exc.type = 1;
                exc.name = "powf";
                exc.err = 0;
                exc.arg1 = (double)x;
                exc.arg2 = (double)y;
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
                return func_00481AE0(exc.retval);
            } else {
                exc.type = 3;
                exc.name = "powf";
                exc.err = 0;
                exc.arg1 = (double)x;
                exc.arg2 = (double)y;
                if (_LIB_VERSION == 0) {
                    exc.retval = 3.40282347e+38F;
                    y *= 0.5;
                    if (x < 0.0 && rint(y) != y) exc.retval = -3.40282347e+38F;
                } else {
                    exc.retval = __infinity;
                    y *= 0.5;
                    if (x < 0.0 && rint(y) != y) exc.retval = -__infinity;
                }
                if (_LIB_VERSION == 2)
                    *__errno() = 34;
                else if (!matherr(&exc)) {
                    *__errno() = 34;
                }
                if (exc.err != 0)
                    *__errno() = exc.err;
                return func_00481AE0(exc.retval);
            }
        }
    }
    if (z == 0.0f && finitef(x) && finitef(y)) {
        exc.type = 4;
        exc.name = "powf";
        exc.err = 0;
        exc.arg1 = (double)x;
        exc.arg2 = (double)y;
        exc.retval = 0.0;
        if (_LIB_VERSION == 2)
            *__errno() = 34;
        else if (!matherr(&exc)) {
            *__errno() = 34;
        }
        if (exc.err != 0)
            *__errno() = exc.err;
        return func_00481AE0(exc.retval);
    }
    return z;
}
