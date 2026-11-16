/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    u8 pad[0xC];
    u8 *out;
} Sink;

typedef struct {
    volatile s32 fd;
    volatile s32 remain;
    volatile u32 count;
    volatile s32 active;
    u8 *ptr;
    u8 *buf;
    Sink *sink;
} Stream;

void _sceTtyQueuePush(Sink *);
extern char _sceTtyPacketSizeErrStr[];
extern char _sceTtyRecvErrStr[];
extern char _sceTtySendErrFmt[];
extern char _sceTtyWlenErrFmt[];

void sceTtyHandler(s32 mode, s32 n, Stream *s) {
    s32 r2;
    u16 *hdr;

    switch (mode) {
    case 1:
    case 2:
        if (n != 0) {
            if (s->count + n > 320) {
                _sceSioPrintf(_sceTtyPacketSizeErrStr);
            }
            n = sceDeci2ExRecv(s->fd, s->count + s->buf, n);
            if (n < 0) {
                _sceSioPrintf(_sceTtyRecvErrStr);
            }
            s->count += n;
        } else {
            hdr = (u16 *)s->buf;
            for (n = 12; n < *hdr; n++) {
                *s->sink->out = s->buf[n];
                _sceTtyQueuePush(s->sink);
            }
            s->count = 0;
        }
        break;
    case 3:
        r2 = sceDeci2ExSend(s->fd, s->ptr, s->remain);
        if (r2 < 0) {
            _sceSioPrintf(_sceTtySendErrFmt, r2);
            s->active = 0;
        } else {
            s->ptr += r2;
            s->remain -= r2;
        }
        break;
    case 4:
        if (s->remain != 0) {
            _sceSioPrintf(_sceTtyWlenErrFmt, s->remain);
        }
        s->active = 0;
        break;
    }
}
