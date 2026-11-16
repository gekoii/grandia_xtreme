#include "common.h"

s32 UtilReadInt(char **pp, s32 skipline) {
    s32 n;
    char *c;
    s32 d;
    s32 t;

    n = 0;
    while (**pp == ' ' || **pp == '\t') {
        (*pp)++;
    }
    while ((u32)((u8)**pp - '0') < 10) {
        c = *pp;
        d = *c;
        *pp = c + 1;
        n *= 10;
        t = n - '0';
        n = t + d;
    }
    if (skipline) {
        while (**pp != '\n') {
            (*pp)++;
        }
        (*pp)++;
    }
    return n;
}
