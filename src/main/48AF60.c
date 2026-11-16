/* Matches newlib's _strtoul_r (strtoul.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"

struct _reent {
    s32 _errno;
};

extern u8 ctypeTable[];
#define _U 01
#define _L 02
#define _N 04
#define _S 010
#define isspace(c) (ctypeTable[c] & _S)
#define isdigit(c) (ctypeTable[c] & _N)
#define isalpha(c) (ctypeTable[c] & (_U | _L))
#define isupper(c) (ctypeTable[c] & _U)
#define ULONG_MAX 0xFFFFFFFFFFFFFFFFUL

u64 _strtoul_r(struct _reent *rptr, const char *nptr, char **endptr, s32 base) {
    register const char *s = nptr;
    register u64 acc;
    register s32 c;
    register u64 cutoff;
    register s32 neg = 0, any, cutlim;

    do {
        c = *s++;
    } while (isspace(c));
    if (c == '-') {
        neg = 1;
        c = *s++;
    } else if (c == '+')
        c = *s++;
    if ((base == 0 || base == 16) && c == '0' && (*s == 'x' || *s == 'X')) {
        c = s[1];
        s += 2;
        base = 16;
    }
    if (base == 0)
        base = c == '0' ? 8 : 10;
    cutoff = (u64)ULONG_MAX / (u64)base;
    cutlim = (u64)ULONG_MAX % (u64)base;
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
        acc = ULONG_MAX;
        rptr->_errno = 34;
    } else if (neg)
        acc = -acc;
    if (endptr != 0)
        *endptr = (char *)(any ? s - 1 : nptr);
    return (acc);
}
