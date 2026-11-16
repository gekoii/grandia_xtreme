/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef s32 (*Cb)(void *, void *, void *);

typedef struct {
    Cb func;
    void *data;
} CbEnt;

typedef struct {
    u8 pad[0xC];
    CbEnt e[1];
} Inner728B0;

typedef struct {
    u8 pad[0x40];
    Inner728B0 *p40;
} Outer728B0;

Cb sceMpegAddCallback(Outer728B0 *arg0, s32 arg1, Cb arg2, void *arg3) {
    Inner728B0 *p = arg0->p40;
    Cb old;

    p->e[arg1].data = arg3;
    old = p->e[arg1].func;
    p->e[arg1].func = arg2;
    return old;
}
