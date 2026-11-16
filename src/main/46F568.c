/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"

extern void SignalSema(s32);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceCdGetReadPos(void) {
    s32 r;

    if (_sceCdCheckNCmd(2) == 0) {
        return 0;
    }
    if (sceSifCallRpc(&_sceCdNCmdClient, 0xE, 0, NULL, 0, &_sceCdNCmdRecvBuf, 4, NULL, NULL) < 0) {
        SignalSema(_sceCdNCmdSema);
        return 0;
    }
    r = *(s32 *)((u32)&_sceCdNCmdRecvBuf | 0x20000000);
    SignalSema(_sceCdNCmdSema);
    return r;
}
