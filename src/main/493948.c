/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"

s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);

s32 sceSifIopGetVal(s32 arg0, void *arg1, u32 arg2) {
    if (_sceSifLoadFileInit() < 0) {
        return 0xFFFF0000;
    }
    if (arg2 >= 3) {
        goto err;
    }
    _sceSifLoadBuf.val.addr = arg0;
    _sceSifLoadBuf.val.type = arg2;
    if (sceSifCallRpc(&_sceSifLoadClient, 3, 0, &_sceSifLoadBuf, 0x20, &_sceSifLoadBuf, 0x20, NULL, NULL) < 0) {
        return 0xFFFEFFFF;
    }
    if (arg2 == 0) {
        *(u8 *)arg1 = *(u8 *)&_sceSifLoadBuf;
    } else if (arg2 == 1) {
        *(u16 *)arg1 = *(u16 *)&_sceSifLoadBuf;
    } else if (arg2 != 2) {
    err:
        return 0xFFFEFFFE;
    } else {
        *(u32 *)arg1 = _sceSifLoadBuf.val.addr;
    }
    return 0;
}
