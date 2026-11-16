/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"

#define UNCACHED(p) ((void *)((u32)(p) | 0x20000000))


typedef struct {
    s32 n0;
    s32 n1;
    u8 *d0;
    u8 *d1;
    u8 b0[0x40];
    u8 b1[0x40];
} Pair;

typedef struct {
    u8 data[0x144];
} Big;

typedef struct {
    u8 data[0x40];
} Small;

extern void *memcpy(void *, const void *, u32);
extern void iSignalSema(s32);

void _sceFsRpcEndHandler(void) {
    struct {
        s32 res;
        s32 cmd;
        void *dst;
        u32 size;
    } h;
    struct {
        void *dst;
        u32 size;
    } h2;
    Pair *pr;
    u8 *d;
    s32 i;
    s64 r;
    s32 x;
    u32 n;

    memcpy(&h.res, UNCACHED(&_sceFsRetBuf.result), 4);
    memcpy(&h.cmd, UNCACHED(&_sceFsRetBuf.function), 4);
    memcpy(&h.dst, UNCACHED(&_sceFsRetBuf.dst), 4);
    memcpy(&h.size, UNCACHED(&_sceFsRetBuf.size), 4);
    if (h.res >= 0) {
        memcpy(h.dst, UNCACHED(_sceFsRetBuf.data), h.size);
    }
    switch (h.cmd) {
    case 2:
        pr = UNCACHED(_sceFsRetBufExtra);
        if (pr->n0 > 0) {
            d = pr->d0;
            for (i = 0; i < pr->n0; i++) {
                d[i] = pr->b0[i];
            }
        }
        if (pr->n1 > 0) {
            d = pr->d1;
            for (i = 0; i < pr->n1; i++) {
                d[i] = pr->b1[i];
            }
        }
        break;
    case 11:
        memcpy(&h2.dst, UNCACHED(_sceFsRetBufExtra), 4);
        memcpy(h2.dst, UNCACHED(_sceFsRetBufExtra + 4), sizeof(Big));
        break;
    case 12:
        memcpy(&h2.dst, UNCACHED(_sceFsRetBufExtra), 4);
        memcpy(h2.dst, UNCACHED(_sceFsRetBufExtra + 4), sizeof(Small));
        break;
    case 23:
    case 25:
    case 26:
        memcpy(&h2.dst, UNCACHED(_sceFsRetBufExtra), 4);
        memcpy(&h2.size, UNCACHED(_sceFsRetBufExtra + 4), 4);
        n = h2.size;
        if (n > 0x400) {
            h2.size = 0x400;
            n = 0x400;
        }
        memcpy(h2.dst, UNCACHED(_sceFsRetBufExtra + 8), n);
        break;
    }
    if (h.res < 0) {
        do {
            x = -h.res;
            r = x;
            h.res = x;
            i = 0;
            if (((s32 *)_sceFsAsyncSemaTable)[i] == r) {
                _sceFsAsyncSemaTable[i] = -1;
                return;
            }
        } while (0);
    loop:
        i++;
        if (i < 32) {
            if (((s32 *)_sceFsAsyncSemaTable)[i] == r) {
                _sceFsAsyncSemaTable[i] = -1;
                return;
            }
            goto loop;
        }
    } else {
        iSignalSema(h.res);
    }
}
