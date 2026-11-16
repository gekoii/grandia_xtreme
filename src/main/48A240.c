/* Third-party library code (newlib): see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct Bigint {
    struct Bigint *_next;
} Bigint;

typedef struct {
    u8 pad[0x48];
    Bigint *_p5s;
} Reent;

Bigint *multadd(Reent *, Bigint *, s32, s32);
Bigint *i2b(Reent *, s32);
Bigint *mult(Reent *, Bigint *, Bigint *);
extern s32 p05[];

Bigint *pow5mult(Reent *ptr, Bigint *b, s32 k) {
    Bigint *b1, *p5, *p51;
    s32 i;

    if ((i = k & 3)) {
        b = multadd(ptr, b, p05[i - 1], 0);
    }
    if (!(k >>= 2)) {
        return b;
    }
    if (!(p5 = ptr->_p5s)) {
        p5 = ptr->_p5s = i2b(ptr, 625);
        p5->_next = 0;
    }
    for (;;) {
        if (k & 1) {
            b1 = mult(ptr, b, p5);
            Bfree(ptr, b);
            b = b1;
        }
        if (!(k >>= 1)) {
            break;
        }
        if (!(p51 = p5->_next)) {
            p51 = p5->_next = mult(ptr, p5, p5);
            p51->_next = 0;
        }
        p5 = p51;
    }
    return b;
}
