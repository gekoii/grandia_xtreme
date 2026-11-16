/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/sif.h"


s32 sceSifBindRpc(sceSifClientData *, s32, s32);
s32 sceSifCallRpc(sceSifClientData *, s32, s32, s32, s32, void *, s32, s32, s32);
typedef struct {
    u8 b[4];
} B4;


s32 _sceSifLoadFileInit(void) {
    s32 i;

    if (_sceSifLoadBindState < 0) {
    retry:
        if (sceSifBindRpc(&_sceSifLoadClient, 0x80000006, 0) < 0) {
            return -1;
        }
        if (_sceSifLoadClient.serve != 0) {
            _sceSifLoadBindState = 0;
            if (sceSifCallRpc(&_sceSifLoadClient, 0xFF, 0, 0, 0, &_sceSifLoadBuf, 4, 0, 0) < 0) {
                return 0xFFFEFFFF;
            }
            *(B4 *)_sceSifLoadIopVersion = *(B4 *)&_sceSifLoadBuf;
            return 0;
        }
        i = 0x100000;
        do {
            i--;
        } while (i != -1);
        goto retry;
    }
    return 0;
}
