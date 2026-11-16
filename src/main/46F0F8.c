/* Sony PS2 SDK library code: see README.md, "Third-party library code". */
#include "common.h"
#include "sdk/libcdvd.h"
#include "protos/main.h"




void *memcpy(void *, const void *, u32);
s32 ReferThreadStatus(s32, void *);
s32 SignalSema(s32);
s32 PollSema(s32);
void sceSifWriteBackDCache(void *, s32);
s32 sceSifBindRpc(sceSifClientData *, u32, s32);
s32 sceSifCallRpc(sceSifClientData *, s32, s32, void *, s32, void *, s32, void *, void *);
extern char _sceCdSearchFileBindErrStr[];
extern char _sceCdSearchCallFmt[];
extern char _sceCdSearchNameFmt[];
extern char _sceCdSearchSizeFmt[];
extern char _sceCdSearchLbnFmt[];

s32 sceCdSearchFile(sceCdlFILE *fp, const char *name) {
    s32 i;
    s32 ret;
    s32 j;

    _sceCdSemaInit();
    if (_sceCdNCmdSema != PollSema(_sceCdNCmdSema)) {
        return 0;
    }
    _sceCdNCmdNum = 1;
    ReferThreadStatus(_sceCdThreadId, &_sceCdThreadStatus);
    if (sceCdSync(1) != 0) {
        goto fail;
    }
    do {
        sceSifInitRpc(0);
        j = _sceCdSearchFileBind;
    } while (0);
    if (j < 0) {
        while (1) {
            if (sceSifBindRpc(&_sceCdSearchFileClient, 0x80000597, 0) < 0) {
                if (_sceCdDebug > 0) {
                    scePrintf(_sceCdSearchFileBindErrStr);
                }
                for (j = 0x100000; j != -1; j--) {
                }
                continue;
            }
            if (_sceCdSearchFileClient.serve != 0) {
                break;
            }
            for (j = 0x100000; j != -1; j--) {
            }
        }
        _sceCdSearchFileBind = 0;
    }
    for (i = 0; i < 256; i++) {
        if ((_sceCdSearchFileBuf.name[i] = name[i]) == 0) {
            break;
        }
    }
    if (i == 256) {
        _sceCdSearchFileBuf.name[255] = 0;
    }
    _sceCdSearchFileBuf.addr = &_sceCdSearchFileBuf;
    if (_sceCdDebug > 0) {
        scePrintf(_sceCdSearchCallFmt, _sceCdSearchFileBuf.name);
    }
    sceSifWriteBackDCache(&_sceCdSearchFileBuf, sizeof(SceCdSearchFileBuf));
    if (sceSifCallRpc(&_sceCdSearchFileClient, 0, 0, &_sceCdSearchFileBuf, sizeof(SceCdSearchFileBuf), _sceCdSearchFileRecvBuf, 4, NULL, NULL) < 0) {
    fail:
        SignalSema(_sceCdNCmdSema);
        return 0;
    }
    memcpy(fp, (void *)((u32)&_sceCdSearchFileBuf | 0x20000000), sizeof(sceCdlFILE));
    if (_sceCdDebug > 0) {
        scePrintf(_sceCdSearchNameFmt, fp->name);
    }
    if (_sceCdDebug > 0) {
        scePrintf(_sceCdSearchSizeFmt, fp->size);
    }
    do {
        if (_sceCdDebug > 0) {
            scePrintf(_sceCdSearchLbnFmt, fp->lsn);
        }
        ret = *(s32 *)((u32)_sceCdSearchFileRecvBuf | 0x20000000);
        SignalSema(_sceCdNCmdSema);
    } while (0);
    return ret;
}
