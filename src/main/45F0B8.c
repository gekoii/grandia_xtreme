#include "common.h"
#include "sdk/sif.h"

s32 sceSifBindRpc(sceSifClientData *, s32, s32);
s32 sceSifCallRpc(sceSifClientData *, s32, s32, void *, s32, void *, s32, void *, void *);

extern u8 sysIopRpcSendData[];
extern u8 sysIopRpcRecvBuf[];
extern sceSifClientData sysIopRpcClient;

s32 SysIopRpcBind(void) {
    s32 i;

    while (1) {
        if (sceSifBindRpc(&sysIopRpcClient, 0x415249, 0) < 0) {
            while (1) {
            }
        }
        i = 10000;
        do {
            i--;
        } while (i != -1);
        if (sysIopRpcClient.serve != 0) {
            break;
        }
    }
    i = 10000;
    do {
        i--;
    } while (i != -1);
    i = 3;
    sceSifCallRpc(&sysIopRpcClient, 0, i, sysIopRpcSendData, 0x20, sysIopRpcRecvBuf, 0x20, 0, 0);
    return 1;
}
