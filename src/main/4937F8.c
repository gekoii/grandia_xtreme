/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"
#include "protos/main.h"

unsigned int strncpy(u8 *, s32, s32);
s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, s32, s32);

s32 _sceSifLoadElfPart(s32 arg0, s32 arg1, s32 *arg2, s32 arg3) {
    u8 *p;

    if (_sceSifLoadFileInit() < 0) {
        return 0xFFFF0000;
    }
    if (_sceSifLoadFileVersionMismatch() != 0) {
        return 0xFFFEFFFC;
    }
    strncpy(_sceSifLoadBufPath, arg0, 0xFC);
    p = _sceSifLoadBufPath - 8;
    p[0x103] = 0;
    strncpy(_sceSifLoadBufPath + 0xFC, arg1, 0xFC);
    p[0x1FF] = 0;
    if (sceSifCallRpc(&_sceSifLoadClient, arg3, 0, p, 0x200, p, 0x10, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    if (*(s32 *)(_sceSifLoadBufPath - 8) == 0) {
        return 0xFFFEFFFD;
    }
    arg2[0] = *(s32 *)(_sceSifLoadBufPath - 8);
    arg2[1] = ((s32 *)p)[1];
    return 0;
}
