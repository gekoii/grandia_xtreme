/* as: ee-gcc2.9-991111-01 */
/* rodata: 0x00703448-0x00703450 */
/* Matches newlib's asinf error-handling wrapper (wf_asin.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/newlib.h"

struct exception {
    s32 type;
    char *name;
    double arg1;
    double arg2;
    double retval;
    s32 err;
};

f32 vwr___ieee754_asinf(f32);
f32 vwr_fabsf(f32) __attribute__((const));
s32 vwr_matherr(struct exception *);
s32 vwr_isnanf(f32) __attribute__((const));
f32 dptofp(double);
double fptodp(f32) __attribute__((const));
s32 *__errno(void);

f32 vwr_asinf(f32 x) {
    f32 z;
    struct exception exc;

    z = vwr___ieee754_asinf(x);
    if (_LIB_VERSION_vwr == -1 || vwr_isnanf(x)) return z;
    if (vwr_fabsf(x) > 1.0f) {
        exc.type = 1;
        exc.name = "asinf";
        exc.err = 0;
        exc.arg1 = exc.arg2 = fptodp(x);
        exc.retval = 0.0;
        if (_LIB_VERSION_vwr == 2)
            *__errno() = 33;
        else if (!vwr_matherr(&exc)) {
            *__errno() = 33;
        }
        if (exc.err != 0)
            *__errno() = exc.err;
        return dptofp(exc.retval);
    } else
        return z;
}
