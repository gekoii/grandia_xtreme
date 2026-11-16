/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"


extern s32 GetThreadId(void);
extern void sceSifWriteBackDCache(void *, s32);
extern s32 sceSifBindRpc(sceSifClientData *, s32, s32);
extern s32 sceSifCallRpc(sceSifClientData *, s32, s32, void *, s32, void *, s32, s32, s32);
extern char _sceCdInitBindErrFmt[];
extern char _sceCdExitStr[];

#define UNCACHED(p) (*(s32 *)((u32)(p) | 0x20000000))

s32 sceCdInit(s32 arg0) {
    s32 ret;
    s32 i;
    s32 r1;
    s32 r2;
    s32 r3;
    s32 result;
    s32 one;

    one = 1;
    if (sceCdSyncS(one) != 0) {
        return 0;
    }
    sceSifInitRpc(0);
    {
        s32 t = GetThreadId();
        *(volatile s32 *)&_sceCdInitBusy = 1;
        *(volatile s32 *)&_sceCdThreadId = t;
        _sceCdSearchFileBind = -1;
        _sceCdNCmdBind = -1;
        _sceCdSCmdBind = -1;
        _sceCdDiskReadyBind = -1;
        _sceCdInitBind = -1;
        _sceCdSifCmdBind = -1;
        _sceCdInitClear = 0;
        _sceCdInitCount++;
    }
    while (1) {
        ret = sceSifBindRpc(&_sceCdInitClient, 0x80000592, 0);
        if (ret < 0) {
            if (_sceCdDebug > 0) {
                scePrintf(_sceCdInitBindErrFmt, ret, _sceCdInitCount);
            }
            i = 0x100000;
            do {
                i--;
            } while (i != -1);
        } else if (_sceCdInitClient.serve != 0) {
            _sceCdInitSendBuf = arg0;
            _sceCdInitBind = 0;
            sceSifWriteBackDCache(&_sceCdInitSendBuf, 4);
            ret = sceSifCallRpc(&_sceCdInitClient, 0, 0, &_sceCdInitSendBuf, 4, _sceCdSCmdRecvBuf, 0x10, 0, 0);
            break;
        } else {
            i = 0x100000;
            do {
                i--;
            } while (i != -1);
        }
    }
    if (ret < 0) {
        *(volatile s32 *)&_sceCdInitBusy = 0;
        return 0;
    }
    r1 = UNCACHED(&_sceCdSCmdRecvBuf[1]);
    r2 = UNCACHED(&_sceCdSCmdRecvBuf[2]);
    r3 = UNCACHED(&_sceCdSCmdRecvBuf[3]);
    result = 1;
    if (r3 != 0xFF) {
        if (r3 == 0xFE) {
            _sceCdDebug = 1;
        } else if (r1 / 256 < 2 || r2 / 256 < 2) {
            result = 2;
        }
    }
    *(volatile s32 *)&_sceCdInitBusy = 0;
    switch (arg0) {
    case 0:
    case 1:
        break;
    case 5:
        if (_sceCdDebug > 0) {
            scePrintf(_sceCdExitStr);
        }
        _sceCdSemaExit();
        _sceCdNCmdSema = -1;
        _sceCdSCmdSema = -1;
        _sceCdCbSema = -1;
        goto out;
    }
    _sceCdSemaInit();
    _sceCdPOffInit();
out:
    return result;
}
