/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"

typedef struct {
    u8 b[0xFC];
} Blk;


s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);
unsigned int strncpy(u8 *, s32, s32);
void *memcpy(void *, void *, s32);

s32 _sceSifLoadModule(s32 arg0, s32 len, void *src, s32 *out, s32 cmd) {
    u8 *p;
    s32 r;

    if (_sceSifLoadFileInit() < 0) {
        return 0xFFFF0000;
    }
    if (_sceSifLoadFileVersionMismatch() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(_sceSifLoadBufPath, arg0, 0xFC);
    p = _sceSifLoadBufPath - 8;
    p[0x103] = 0;
    if (src != NULL) {
        if (len >= 0xFD) {
            *(Blk *)(_sceSifLoadBufPath + 0xFC) = *(Blk *)src;
            _sceSifLoadBuf.module.result = 0xFC;
        } else {
            memcpy(_sceSifLoadBufPath + 0xFC, src, len);
            *(s32 *)(_sceSifLoadBufPath - 8) = len;
        }
    } else {
        p[0x104] = 0;
        *(s32 *)(_sceSifLoadBufPath - 8) = 0;
    }
    if (sceSifCallRpc(&_sceSifLoadClient, cmd, 0, &_sceSifLoadBuf, 0x200, &_sceSifLoadBuf, 8, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    r = _sceSifLoadBuf.module.result;
    *out = _sceSifLoadBuf.module.argLen;
    return r;
}
