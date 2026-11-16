#include "common.h"
#include "protos/main.h"

/* s32 size: not the builtin's prototype, so gcc keeps the call (the builtin
 * would store the 4 bytes inline) */
void *memset(void *, s32, s32);

s32 DbgTextParseEscape(char *s, s32 *out) {
    char buf[4];
    s32 n;
    s32 i;

    n = 1;
    memset(buf, 0, 4);
    if (s[0] == 'c' || s[0] == 'w' || s[0] == 'h') {
        for (i = 0; i < 3 && s[n] != ' '; i++) {
            buf[i] = s[n];
            n++;
            *out = atoi(buf);
            if (s[0] == 'c') {
                *out |= 0x80000000;
            }
            if (s[0] == 'w') {
                *out |= 0x40000000;
            }
            if (s[0] == 'h') {
                *out |= 0x20000000;
            }
        }
    } else {
        *out = 0;
    }
    return n;
}
