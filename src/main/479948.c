/* rodata: 0x004A7748-0x004A7798 */
/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libpad.h"
#include "protos/main.h"


s32 sceSifBindRpc(sceSifClientData *, u32, s32);

s32 scePadInit(s32 mode) {
    s32 i;
    s32 ver;

    _scePadInitDone = 1;
    while (1) {
        sceSifBindRpc(&_scePadClient[0], 0x80000100, 0);
        if (_scePadClient[0].serve != 0)
            break;
        i = 0x10000;
        do {
            i--;
        } while (i != -1);
    }
    while (1) {
        sceSifBindRpc(&_scePadClient[1], 0x80000101, 0);
        if (_scePadClient[1].serve != 0)
            break;
        i = 0x10000;
        do {
            i--;
        } while (i != -1);
    }
    ver = scePadGetModVersion();
    if ((ver >> 8) != 4) {
        if (_scePadDebugPrint != 0) {
            printf("libpad: Module version mismatch ");
            printf("[libpad.a = %d.%d, padman.irx = %d.%d]\n", 4, 0, ver >> 8, ver & 0xFF);
        }
        return 0;
    }
    return _scePadPortInit(mode);
}
