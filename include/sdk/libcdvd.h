/*
 * Sony libcdvd: the EE side of the cdvdfsv servers (0x80000592 init,
 * 0x80000593 S commands, 0x80000595 N commands, 0x80000597 search file,
 * 0x8000059A disk ready) and its callback thread.
 *
 * sceCdlFILE is the SCE libcdvd.h type. The library's own statics have no
 * known names; they are named for what they do.
 */
#ifndef SDK_LIBCDVD_H
#define SDK_LIBCDVD_H

#include "common.h"
#include "sdk/sif.h"
#include "sdk/eekernel.h"

/* sceCdlFILE (0x24). */
typedef struct sceCdlFILE {
    /* 0x00 */ u32 lsn;
    /* 0x04 */ u32 size;
    /* 0x08 */ char name[16];
    /* 0x18 */ u8 date[8];
    /* 0x20 */ u32 flag;
} sceCdlFILE;

/* sceCdSearchFile request and result (0x128, 0x140 reserved). */
typedef struct SceCdSearchFileBuf {
    /* 0x000 */ sceCdlFILE file;
    /* 0x024 */ char name[256];
    /* 0x124 */ void *addr;        /* its own address */
} SceCdSearchFileBuf;

/* struct ThreadParam is in sdk/eekernel.h. */

/*
 * Volatility, checked file by file: the semaphores, the N-command busy flag
 * and sceCdCbfunc_num are volatile (files that declared some of them plain
 * match either way). _sceCdCbCmd is volatile in the interrupt-side end
 * handler 46EBF0 but must be plain in the callback thread 46EC90, so the
 * header keeps it plain and 46EBF0 needs volatile accesses; sceCdInit
 * (46F880) likewise needs volatile stores to _sceCdInitBusy and
 * _sceCdThreadId.
 */
extern s32 _sceCdDebug;
extern s32 _sceCdCbThreadId;
extern s32 _sceCdSCmdNum;
extern s32 _sceCdNCmdNum;
extern volatile s32 _sceCdCbSema;
extern s32 _sceCdInitBusy;
extern volatile s32 _sceCdNCmdSema;
extern volatile s32 _sceCdSCmdSema;
extern volatile s32 _sceCdNCmdBusy;
extern s32 _sceCdInitClear;
extern s32 _sceCdNCmdBind;
extern s32 _sceCdSifCmdBind;
extern s32 _sceCdSearchFileBind;
extern s32 _sceCdDiskReadyBind;
extern s32 _sceCdSCmdBind;
extern s32 _sceCdInitBind;
extern s32 _sceCdInitCount;
extern s32 _sceCdCbCmd;
extern volatile s32 sceCdCbfunc_num;
extern void (*sceCdCbfunc)(s32);        /* declared s32 in 46EB98 and 46EBF0 */

extern s32 _sceCdNCmdRecvBuf[0x454];    /* read uncached */
extern sceSifClientData _sceCdNCmdClient;
extern s32 _sceCdSCmdRecvBuf[0x110];    /* sceCdInit: result and 3 versions */
extern s32 _sceCdSCmdSendBuf[0x102];
extern sceSifClientData _sceCdSCmdClient;

extern void (*_sceCdSifCmdCbFunc)(void *);
extern void *_sceCdSifCmdCbArg;
extern s32 _sceCdCbThreadFlag;
extern s32 _sceCdThreadId;
extern struct ThreadParam _sceCdThreadStatus;
extern struct ThreadParam _sceCdCbThreadParam;
extern SceCdSearchFileBuf _sceCdSearchFileBuf;
extern s32 _sceCdSearchFileRecvBuf[0x10];
extern sceSifClientData _sceCdSearchFileClient;
extern sceSifClientData _sceCdInitClient;
extern sceSifClientData _sceCdDiskReadyClient;
extern s32 _sceCdInitSendBuf;
extern s32 _sceCdDiskReadySendBuf;

#endif
