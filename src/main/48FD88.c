/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/fileio.h"
#include "protos/main.h"


typedef struct {
    u8 b[4];
} B4;



s32 DIntr(void);
s32 EIntr(void);
s32 sceSifBindRpc(sceSifClientData *, s32, s32);
s32 SignalSema(s32);
s32 WaitSema(s32);
s32 sceSifCallRpc(sceSifClientData *, s32, s32, void *, s32, void *, s32, s32, s32);
void _sceFsCallbackHandler(void);

s32 sceFsInit(void) {
    SceFsFd *p;
    s32 i;
    SceFsCallback *q;

    q = &_sceFsIopCallback;
    sceSifInitRpc(0);
    _sceFsIopCallback.func = 0;
    q->arg = NULL;
    DIntr();
    sceSifAddCmdHandler(0x80000011, _sceFsRpcEndHandler, _sceFsCmdHandlerArg);
    sceSifAddCmdHandler(0x80000013, _sceFsCallbackHandler, q);
    EIntr();
    while (1) {
        if (sceSifBindRpc(&_sceFsClient, 0x80000001, 0) < 0) {
            goto fail;
        }
        if (_sceFsClient.serve != 0) {
            break;
        }
        i = 0x100000;
        do {
            i--;
        } while (i != -1);
    }
    _sceFsSemaInit();
    WaitSema(_sceFsFdSema);
    for (p = _sceFsFdTable; p < &_sceFsFdTable[32]; p++) {
        p->flags = 0;
    }
    SignalSema(_sceFsFdSema);
    _sceFsInitArg = &_sceFsRetBuf;
    if (sceSifCallRpc(&_sceFsClient, 0xFF, 0, &_sceFsInitArg, 4, _sceFsRecvBuf, 4, 0, 0) < 0) {
        return 0xFFFEFFFF;
    }
    *(B4 *)_sceFsIopVersion = *(B4 *)((u32)_sceFsRecvBuf | 0x20000000);
    _sceFsInitDone = 1;
    return 0;
fail:
    return -1;
}
