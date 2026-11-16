/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct S48F4C0b {
    u8 pad0[0x3C];
    struct S48F4C0b *x3C;
} S48F4C0b;

typedef struct {
    u8 pad0[4];
    s32 x4;
    u8 pad8[4];
    S48F4C0b *xC;
} S48F4C0a;

extern void DIntr(void);
extern void EIntr(void);

S48F4C0b *sceSifGetNextRequest(S48F4C0a *arg0) {
    S48F4C0b *p;

    DIntr();
    p = arg0->xC;
    if (p == NULL) {
        arg0->x4 = 0;
    } else {
        arg0->x4 = 1;
        arg0->xC = p->x3C;
    }
    EIntr();
    return p;
}
