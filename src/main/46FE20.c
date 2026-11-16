/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libmc.h"
#include "sdk/eekernel.h"
#include "protos/main.h"


s32 CreateSema(SemaParam *);
s32 SignalSema(s32);
s32 WaitSema(s32);
s32 sceSifBindRpc(sceSifClientData *, u32, s32);
s32 sceSifCallRpc(sceSifClientData *, s32, s32, void *, s32, void *, s32, void *, void *);
extern char _sceMcBindErrStr[];
extern char _sceMcOldMcservStr[];
extern char _sceMcOldMcmanStr[];

s32 sceMcInit(void) {
    SemaParam sp;
    s32 ret;
    s32 i;

    if (_sceMcSema < 0) {
        sp.option = 0;
        sp.initCount = 1;
        sp.maxCount = 1;
        _sceMcSema = CreateSema(&sp);
    }
    sceMcSync(0, NULL, NULL);
    WaitSema(_sceMcSema);
    sceSifInitRpc(0);
    while (1) {
        if (sceSifBindRpc(&_sceMcClient, 0x80000400, 0) < 0) {
            printf(_sceMcBindErrStr);
            while (1) {
            }
        }
        if (_sceMcClient.serve != 0) {
            break;
        }
        for (i = 0x100000; i != 0; i--) {
        }
    }
    ret = sceSifCallRpc(&_sceMcClient, 0xFE, 0, &_sceMcCmdArg, 0x30, &_sceMcRecvBuf, 0xC, NULL, NULL);
    SignalSema(_sceMcSema);
    if (ret < 0) {
        _sceMcClient.serve = 0;
        return ret - 100;
    }
    if (_sceMcRecvBuf.mcservVersion < 0x20A) {
        printf(_sceMcOldMcservStr);
        _sceMcClient.serve = 0;
        return -120;
    }
    if (_sceMcRecvBuf.mcmanVersion < 0x20E) {
        printf(_sceMcOldMcmanStr);
        _sceMcClient.serve = 0;
        return -121;
    }
    return _sceMcRecvBuf.result;
}
