/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

extern s32 func_00481838(double, double);

union double_union {
    double d;
    s32 i[2];
};

char *cvt(void *data, double value, s32 ndigits, s32 flags, char *sign, s32 *decpt, s32 ch, s32 *length) {
    s32 mode;
    s32 dsgn;
    char *rve;
    char *digits;
    char *bp;
    union double_union tmp;

    if (ch == 'f') {
        mode = 3;
    } else {
        if (ch == 'e' || ch == 'E') {
            ndigits++;
        }
        mode = 2;
    }
    tmp.d = value;
    if (tmp.i[1] & 0x80000000) {
        value = dpsub(0, value);
        *sign = '-';
    } else {
        *sign = '\0';
    }
    digits = _dtoa_r(data, value, mode, ndigits, decpt, &dsgn, &rve);
    if ((ch != 'g' && ch != 'G') || (flags & 1)) {
        bp = digits + ndigits;
        if (ch == 'f') {
            if (*digits == '0' && func_00481838(value, 0) != 0) {
                *decpt = -ndigits + 1;
            }
            bp += *decpt;
        }
        if (func_00481838(value, 0) == 0) {
            rve = bp;
        }
        while (rve < bp) {
            *rve++ = '0';
        }
    }
    *length = rve - digits;
    return digits;
}
