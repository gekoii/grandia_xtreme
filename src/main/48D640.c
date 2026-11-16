/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"

typedef struct {
    u8 pad0[2];
    u16 unk2;
    u16 len;
    u8 tag0;
    u8 tag1;
    s32 unk8;
} TtyHdr;

typedef struct {
    volatile s32 socket;
    volatile s32 unk4;
    volatile s32 unk8;
    volatile s32 unkC;
    TtyHdr *wbuf;
    void *rbuf;
    void *q;
} TtyState;

extern TtyState _sceTtyState;
extern u8 _sceTtySendBuf[];
extern u8 _sceTtyRecvBuf[];
extern void FlushCache(s32);
extern s32 sceDeci2Open(s32, void *, void (*)(s32, s32, void *));
extern void sceTtyHandler(s32, s32, void *);
extern void *_sceTtyQueueInit(s32);

s32 sceTtyInit(void) {
    TtyHdr *h;

    FlushCache(0);
    _sceTtyState.socket = sceDeci2Open(0x210, &_sceTtyState, sceTtyHandler);
    if (_sceTtyState.socket < 0) {
        return 0;
    }
    _sceTtyState.unkC = 0;
    _sceTtyState.unk4 = 0;
    _sceTtyState.unk8 = 0;
    _sceTtyState.rbuf = (void *)((u32)_sceTtyRecvBuf | 0x20000000);
    h = (TtyHdr *)((u32)_sceTtySendBuf | 0x20000000);
    _sceTtyState.wbuf = h;
    h->unk2 = 0;
    h->len = 0x210;
    h->tag0 = 'E';
    h->tag1 = 'H';
    h->unk8 = 0;
    _sceTtyState.q = _sceTtyQueueInit(0x100);
    return 1;
}
