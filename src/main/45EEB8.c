#include "common.h"
#include "snd.h"
#include "main/n10.h"
#include "protos/main.h"

typedef struct {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fC;
    s32 f10;
    s32 f14;
} IopHdr;

void sceSifSetDChain(void);
extern char sysIoprpImagePath[];
extern char sysSio2manPath[];
extern char sysPadmanPath[];
extern char sysMcmanPath[];
extern char sysMcservPath[];
extern char sysLibsdPath[];
extern char sysGsIopPath[];
extern s32 sndIopCmdAddr;
extern s32 sndIopHeaderWord14;
extern s32 sndIopStatusAddr;
extern IopHdr *sndIopHeader;
extern SndIopCmd sndIopCmdBuf[];

void LoadIopModules(char *path) {
    s32 i;

    sceSifInitRpc(0);
    sceCdInit(0);
    sceCdMmode(2);
    while (!sceSifRebootIop(sysIoprpImagePath));
    while (!sceSifSyncIop());
    sceSifInitRpc(0);
    sceCdInit(0);
    sceCdMmode(2);
    sceFsReset();
    SysLoadIopModule(sysSio2manPath);
    SysLoadIopModule(sysPadmanPath);
    SysLoadIopModule(sysMcmanPath);
    SysLoadIopModule(sysMcservPath);
    SysLoadIopModule(sysLibsdPath);
    SysLoadIopModule(sysGsIopPath);
    for (i = 0; i < 500; i++);
    sceSifInitRpc(0);
    SndBusyWait();
    SysIopRpcBind();
    for (i = 0; i < 500; i++);
    sndIopCmdBufPtr = sndIopCmdBuf;
    for (i = 0; i < 0x8000; i++) {
        ((u8 *)sndPcmBuf)[i] = 0; /* cleared bytewise */
    }
    sndIopHeader = (IopHdr *)0x21FF0000;
    do {
        for (i = 0; i < 500; i++);
    } while (*(volatile s32 *)0x21FF0000 != 0x5A5AA5A5);
    for (i = 0; i < 500; i++);
    sndIopStatusAddr = sndIopHeader->f4;
    sndIopCmdAddr = sndIopHeader->fC;
    sndIopHeaderWord14 = sndIopHeader->f14;
    sndIopStatus = (SndIopStatus *)0x21FF0100;
    sceSifSetDChain();
}
