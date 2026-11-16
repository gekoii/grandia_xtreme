/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 x0;
    s32 x4;
    void *x8;
    void *xC;
    u8 data[0x10];
} S48D1E0;

extern S48D1E0 _sceTtyRecvQueue;

S48D1E0 *_sceTtyQueueInit(s32 arg0) {
    S48D1E0 *p = &_sceTtyRecvQueue;
    void *d = p->data;
    p->x0 = arg0;
    p->x4 = 0;
    p->x8 = p->xC = d;
    return p;
}
