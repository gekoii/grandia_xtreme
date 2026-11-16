/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    s32 unk0;
    volatile s32 count;
    u8 *rp;
} TtyQueue;

typedef struct {
    volatile s32 socket;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    void *wbuf;
    void *rbuf;
    TtyQueue *q;
} TtyState;

extern TtyState _sceTtyState;
extern void _sceTtyQueuePop(TtyQueue *);

s32 sceTtyRead(char *buf, s32 len) {
    s32 i;

    for (i = 0; i < len; i++) {
        while (_sceTtyState.q->count == 0) {
        }
        buf[i] = *_sceTtyState.q->rp;
        _sceTtyQueuePop(_sceTtyState.q);
        if (buf[i] == '\n' || buf[i] == '\r') {
            return i + 1;
        }
    }
    return i;
}
