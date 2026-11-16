/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"

s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);

s32 sceSifSearchModuleByAddress(s32 arg0) {
    if (_sceSifLoadFileInit() < 0) return 0xFFFF0000;
    if (_sceSifLoadFileVersionMismatch() != 0) return 0xFFFEFFFC;
    _sceSifLoadBuf.module.result = arg0;
    if (sceSifCallRpc(&_sceSifLoadClient, 0xA, 0, &_sceSifLoadBuf, 4, &_sceSifLoadBuf, 4, 0, 0) < 0) return 0xFFFEFFFF;
    return _sceSifLoadBuf.module.result;
}
