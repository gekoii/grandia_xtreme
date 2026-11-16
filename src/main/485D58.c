/* Matches newlib's exponent (libc/stdio/vfprintf.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

#define MAXEXP 308
#define to_char(n) ((n) + '0')

s32 exponent(char *p0, s32 exp, s32 fmtch) {
    register char *p, *t;
    char expbuf[MAXEXP];

    p = p0;
    *p++ = fmtch;
    if (exp < 0) {
        exp = -exp;
        *p++ = '-';
    } else
        *p++ = '+';
    t = expbuf + MAXEXP;
    if (exp > 9) {
        do {
            *--t = to_char(exp % 10);
        } while ((exp /= 10) > 9);
        *--t = to_char(exp);
        for (; t < expbuf + MAXEXP; *p++ = *t++);
    } else {
        *p++ = '0';
        *p++ = to_char(exp);
    }
    return (p - p0);
}
