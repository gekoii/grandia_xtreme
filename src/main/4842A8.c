/* Matches newlib's _strtol_r (strtol.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

struct _reent {
    s32 _errno;
};

extern const char ctypeTable[];

#define _U 01
#define _L 02
#define _N 04
#define _S 010

#define isspace(c) ((ctypeTable)[(int)(c)] & _S)
#define isdigit(c) ((ctypeTable)[(int)(c)] & _N)
#define isalpha(c) ((ctypeTable)[(int)(c)] & (_U | _L))
#define isupper(c) ((ctypeTable)[(int)(c)] & _U)

#define LONG_MAX 0x7FFFFFFFFFFFFFFFLL
#define LONG_MIN (-LONG_MAX - 1)
#define ERANGE 34


s64 _strtol_r(struct _reent *rptr, const char *nptr, char **endptr, int base) {
    register const char *s = nptr;
    register u64 acc;
    register int c;
    register u64 cutoff;
    register int neg = 0, any, cutlim;

    do {
        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+')
        c = *s++;
    if ((base == 0 || base == 16) &&
        c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;

    cutoff = neg ? -(u64)LONG_MIN : LONG_MAX;
    cutlim = cutoff % (u64)base;
    cutoff /= (u64)base;
    for (acc = 0, any = 0;; c = *s++) {
        if (isdigit(c))
            c -= '0';
        else if (isalpha(c))
            c -= isupper(c) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if (c >= base)
            break;
        if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if (any < 0) {
        acc = neg ? LONG_MIN : LONG_MAX;
        rptr->_errno = ERANGE;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = (char *)(any ? s - 1 : nptr);
    return (acc);
}
