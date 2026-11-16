/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"


s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);

s32 sceSifIopSetVal(s32 arg0, void *arg1, s32 arg2) {
    s32 r;

    if (_sceSifLoadFileInit() < 0) {
        return 0xFFFF0000;
    }
    _sceSifLoadBuf.val.addr = arg0;
    _sceSifLoadBuf.val.type = arg2;
    if (arg2 == 0) {
        _sceSifLoadBuf.val.value.b = *(u8 *)arg1;
    } else if (arg2 == 1) {
        _sceSifLoadBuf.val.value.h = *(u16 *)arg1;
    } else if (arg2 == 2) {
        _sceSifLoadBuf.val.value.w = *(u32 *)arg1;
    } else {
        return 0xFFFEFFFE;
    }
    r = sceSifCallRpc(&_sceSifLoadClient, 2, 0, &_sceSifLoadBuf, 0x20, &_sceSifLoadBuf, 0x10, 0, 0);
    if (r < 0) return 0xFFFEFFFF; return 0;
}
