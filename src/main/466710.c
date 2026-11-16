#include "common.h"
#include "snd.h"

void WaitSema(s32);
void SignalSema(s32);

void viBufBeginPut(ViBuf *arg0, u8 **ptr1, s32 *len1, u8 **ptr2, s32 *len2) {
    s32 off;
    s32 avail;
    s32 room;
    s32 r;
    s32 t;
    s32 b;
    s32 s;
    s32 b2;

    WaitSema(arg0->sema);
    r = arg0->dmaN;
    t = r + 2;
    avail = ((arg0->n - t) << 11) - arg0->readBytes;
    off = (((arg0->dmaStart + r) << 11) + arg0->readBytes) % arg0->buffSize;
    room = arg0->buffSize - off;
    if (room >= avail) {
        b2 = (s32)arg0->data;
        *len1 = avail;
        *len2 = 0;
        *ptr1 = (u8 *)(b2 + off);
        *ptr2 = NULL;
    } else {
        *len1 = room;
        b = (s32)arg0->data;
        s = arg0->buffSize;
        *ptr1 = (u8 *)(b + off);
        *ptr2 = (u8 *)b;
        *len2 = avail - (s - off);
    }
    SignalSema(arg0->sema);
}
