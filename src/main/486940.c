/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"

char *__sccl(char *tab, char *fmt) {
    s32 c;
    s32 n;
    s32 v;
    char *p;

    c = *fmt++;
    if (c == '^') {
        v = 1;
        c = *fmt++;
    } else {
        v = 0;
    }
    n = 255;
    p = tab + 255;
    for (; n >= 0; n--) {
        *p-- = v;
    }
    if (c == 0) {
        return fmt - 1;
    }
    v = 1 - v;
    for (;;) {
        tab[c] = v;
    doswitch:
        n = *fmt++;
        switch (n) {
        case 0:
            return fmt - 1;
        case '-':
            n = *fmt;
            if (n == ']' || n < c) {
                c = '-';
                break;
            }
            fmt++;
            do {
                tab[++c] = v;
            } while (c < n);
            goto doswitch;
        case ']':
            return fmt;
        default:
            c = n;
            break;
        }
    }
}
