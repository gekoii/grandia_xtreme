/* cflags: -fno-schedule-insns */
/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"

typedef struct {
    volatile s32 h;
    volatile s32 len;
    s32 _8;
    volatile s32 busy;
    u8 *buf;
} State;

void DIntr(void);
void EIntr(void);
extern State _sceTtyState;
extern u8 _sceTtySendBuf[];

s32 sceTtyWrite(s8 *src, s32 n) {
    s32 count;
    s32 i;
    u8 *buf;
    u8 *dst;
    s32 k;
    s32 j;
    s32 m1, nl, cr;

    i = 0;
    count = 0;
    if (_sceTtyState.busy != 0) {
        return -1;
    }
    DIntr();
    buf = (u8 *)((u32)_sceTtySendBuf | 0x20000000);
    _sceTtyState.busy = 1;
    m1 = -1;
    nl = 10;
    cr = 13;
    dst = buf + 0xC;
    _sceTtyState.buf = buf;
    for (;;) {
        if (--n == m1) {
            break;
        }
        if (*src == nl) {
            *dst++ = cr;
            i++;
            if (i >= 0x100) {
                break;
            }
        }
        *dst++ = *src++;
        i++;
        count++;
        if (i >= 0x100) {
            break;
        }
    }
    _sceTtyState.len = i + 0xC;
    j = _sceTtyState.len;
    k = (s8)buf[7];
    *(s16 *)buf = j;
    if (sceDeci2ReqSend(_sceTtyState.h, k, buf) < 0) {
        _sceTtyState.busy = 0;
        EIntr();
        return -1;
    }
    while (_sceTtyState.busy != 0) {
        sceDeci2Poll(_sceTtyState.h);
    }
    EIntr();
    return count;
}
