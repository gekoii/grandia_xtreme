/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "protos/main.h"
#include "sdk/libpad.h"



s32 scePadGetButtonMask(s32 arg0, s32 arg1) {
    u8 *p;

    if (_scePadPortTable[arg0][arg1].open == 0) {
        return 0;
    }
    p = _scePadGetDmaStr(arg0, arg1);
    if (p[0x72] != 1) {
        return 0;
    }
    if (p[0x64] < 2) {
        return 0;
    }
    if (p[0x66] < 2) {
        return 0;
    }
    return (s64)p[0x79] + ((s64)p[0x7A] << 8) + ((s64)p[0x7B] << 16) + ((s64)p[0x7C] << 24);
}
