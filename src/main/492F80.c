/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"

typedef struct {
    u8 b[0xFC];
} Blk;


s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);
void *memcpy(void *, void *, s32);

s32 _sceSifLoadModuleBuffer(s32 arg0, s32 len, void *src, s32 *out) {
    s32 r;

    if (_sceSifLoadFileInit() < 0) {
        return 0xFFFF0000;
    }
    if (_sceSifLoadFileVersionMismatch() != 0) {
        return 0xFFFEFFFC;
    }
    _sceSifLoadBuf.module.result = arg0;
    if (src != NULL) {
        if (len >= 0xFD) {
            *(Blk *)_sceSifLoadBuf.module.args = *(Blk *)src;
            _sceSifLoadBuf.module.argLen = 0xFC;
        } else {
            memcpy(_sceSifLoadBuf.module.args, src, len);
            _sceSifLoadBuf.module.argLen = len;
        }
    } else {
        _sceSifLoadBuf.module.argLen = 0;
    }
    if (sceSifCallRpc(&_sceSifLoadClient, 6, 0, &_sceSifLoadBuf, 0x200, &_sceSifLoadBuf, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    r = _sceSifLoadBuf.module.result;
    *out = _sceSifLoadBuf.module.argLen;
    return r;
}
