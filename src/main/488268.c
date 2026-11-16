/* Matches newlib's __sinit (findfp.c). Third-party library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct __sFILE {
    u8 pad[0x58];
} FILE;

struct _glue {
    struct _glue *_next;
    s32 _niobs;
    FILE *_iobs;
};

struct _reent {
    u8 pad0[0x38];
    s32 __sdidinit;
    void (*__cleanup)(struct _reent *);
    u8 pad40[0x1D8 - 0x40];
    struct _glue __sglue;
    FILE __sf[3];
};

extern void std(FILE *, s32, s32, struct _reent *);

void __sinit(struct _reent *s) {
    s->__cleanup = _cleanup_r;
    s->__sdidinit = 1;

    std(s->__sf + 0, 4, 0, s);
    std(s->__sf + 1, 9, 1, s);
    std(s->__sf + 2, 0xA, 2, s);
    s->__sglue._next = 0;
    s->__sglue._niobs = 3;
    s->__sglue._iobs = &s->__sf[0];
}
