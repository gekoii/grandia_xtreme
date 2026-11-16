/*
 * Sony libkernel fileio: the EE side of the IOP fileio server (0x80000001).
 *
 * Every call fills one request in _sceFsSendBuf and calls sceSifCallRpc with
 * it. A request starts with a semaphore the result handler (48F878, command
 * 0x80000011) signals, the address the result goes to and its size; the rest
 * is per call. The SCE names of these structs are not known, so they are
 * named after the calls. Older SDKs' fileio has no such header.
 */
#ifndef SDK_FILEIO_H
#define SDK_FILEIO_H

#include "common.h"
#include "sdk/sif.h"

#define SCE_FS_PATH_MAX 0x400

/* Common request header (0xC). */
typedef struct SceFsReqHdr {
    /* 0x00 */ s32 sema;
    /* 0x04 */ s32 *result;
    /* 0x08 */ s32 resultSize;
} SceFsReqHdr;

typedef struct SceFsOpenReq { /* sceOpen (490018), 0x418 sent */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ s32 flags;
    /* 0x010 */ s32 mode;
    /* 0x014 */ char name[SCE_FS_PATH_MAX];
    /* 0x414 */ s32 fdIndex;
} SceFsOpenReq;

typedef struct SceFsCloseReq { /* sceClose (4902A0), sceDclose (4918C8) */
    /* 0x00 */ s32 sema;
    /* 0x04 */ s32 *result;
    /* 0x08 */ s32 resultSize;
    /* 0x0C */ s32 fd;
    /* 0x10 */ s32 fdIndex;
} SceFsCloseReq;

typedef struct SceFsLseekReq { /* sceLseek (490420) */
    /* 0x00 */ s32 sema;
    /* 0x04 */ s32 *result;
    /* 0x08 */ s32 resultSize;
    /* 0x0C */ s32 fd;
    /* 0x10 */ s32 offset;
    /* 0x14 */ s32 whence;
    /* 0x18 */ s32 fdIndex;
} SceFsLseekReq;

typedef struct SceFsLseek64Req { /* sceLseek64 (4925A0) */
    /* 0x00 */ s32 sema;
    /* 0x04 */ s64 *result;
    /* 0x08 */ s32 resultSize;
    /* 0x0C */ s32 fd;
    /* 0x10 */ s64 offset;
    /* 0x18 */ s32 whence;
    /* 0x1C */ s32 fdIndex;
} SceFsLseek64Req;

typedef struct SceFsReadReq { /* sceRead (490658) */
    /* 0x00 */ s32 sema;
    /* 0x04 */ s32 *result;
    /* 0x08 */ s32 resultSize;
    /* 0x0C */ s32 fd;
    /* 0x10 */ void *buf;
    /* 0x14 */ s32 len;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 fdIndex;
} SceFsReadReq;

typedef struct SceFsWriteReq { /* sceWrite (4908C8): the unaligned head goes inline in data */
    /* 0x00 */ s32 sema;
    /* 0x04 */ s32 *result;
    /* 0x08 */ s32 resultSize;
    /* 0x0C */ s32 fd;
    /* 0x10 */ u32 addr;
    /* 0x14 */ s32 len;
    /* 0x18 */ s32 headLen;
    /* 0x1C */ u8 head[0x10];
    /* 0x2C */ s32 fdIndex;
} SceFsWriteReq;

typedef struct SceFsIoctlReq { /* sceIoctl (490B88), sceIoctl2 (490ED8) */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ s32 fd;
    /* 0x010 */ s32 cmd;
    /* 0x014 */ u8 arg[0x400];
    /* 0x414 */ s32 buf;           /* sceIoctl2 only */
    /* 0x418 */ u32 bufLen;
    /* 0x41C */ u32 argLen;
} SceFsIoctlReq;

typedef struct SceFsPathReq { /* calls taking only a path: _sceFsPathCall (4910B8) */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ char name[SCE_FS_PATH_MAX];
} SceFsPathReq;

typedef struct SceFsArgPathReq { /* sceMkdir (491280) */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ s32 arg;
    /* 0x010 */ char name[SCE_FS_PATH_MAX];
} SceFsArgPathReq;

typedef struct SceFsGetstatReq { /* sceGetstat (491B88): u8 name, the copy loop zero-extends */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ void *stat;
    /* 0x010 */ u8 name[SCE_FS_PATH_MAX];
} SceFsGetstatReq;

typedef struct SceFsChstatReq { /* sceChstat (491D28): u8 name, as sceGetstat */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ s32 cbit;
    /* 0x010 */ u8 stat[0x40];
    /* 0x050 */ u8 name[SCE_FS_PATH_MAX];
} SceFsChstatReq;

typedef struct SceFsSyncReq { /* sceSync (492178), sceReadlink (492BF0: unkC = buffer size) */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 arg;
    /* 0x014 */ char name[SCE_FS_PATH_MAX];
} SceFsSyncReq;

typedef struct SceFsArgReq { /* sceAddDrv (4916C0), sceDread (491A30: fd, buffer) */
    /* 0x00 */ s32 sema;
    /* 0x04 */ s32 *result;
    /* 0x08 */ s32 resultSize;
    /* 0x0C */ s32 arg0;
    /* 0x10 */ s32 arg1;
} SceFsArgReq;

typedef struct SceFsPath2Req { /* sceRename (491F68), sceSymlink (492A10) */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ char name1[SCE_FS_PATH_MAX];
    /* 0x40C */ char name2[SCE_FS_PATH_MAX];
} SceFsPath2Req;

typedef struct SceFsMountReq { /* sceFormat (491450), sceMount (492310) */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ char name1[SCE_FS_PATH_MAX];
    /* 0x40C */ char name2[SCE_FS_PATH_MAX];
    /* 0x80C */ u8 arg[0x400];
    /* 0xC0C */ s32 flag;     /* sceMount only; sceFormat puts the length here */
    /* 0xC10 */ s32 argLen;
} SceFsMountReq;

typedef struct SceFsDevctlReq { /* sceDevctl (4927D8) */
    /* 0x000 */ s32 sema;
    /* 0x004 */ s32 *result;
    /* 0x008 */ s32 resultSize;
    /* 0x00C */ char name[SCE_FS_PATH_MAX];
    /* 0x40C */ u8 arg[0x400];
    /* 0x80C */ s32 cmd;
    /* 0x810 */ u32 argLen;
    /* 0x814 */ s32 buf;
    /* 0x818 */ u32 bufLen;
} SceFsDevctlReq;

/* _sceFsSendBuf (0xC40 reserved): one request per call. Files take a pointer
 * to their member (`SceFsOpenReq *req = &_sceFsSendBuf.open;`), so the
 * accesses keep the member struct's alias set. */
typedef union SceFsRpcBuf {
    SceFsReqHdr hdr;
    SceFsOpenReq open;
    SceFsCloseReq close;
    SceFsLseekReq lseek;
    SceFsLseek64Req lseek64;
    SceFsReadReq read;
    SceFsWriteReq write;
    SceFsIoctlReq ioctl;
    SceFsPathReq path;
    SceFsArgPathReq argPath;
    SceFsGetstatReq getstat;
    SceFsChstatReq chstat;
    SceFsSyncReq sync;
    SceFsArgReq arg;
    SceFsPath2Req path2;
    SceFsMountReq mount;
    SceFsDevctlReq devctl;
    u8 raw[0xC40];
} SceFsRpcBuf;

/* Result block the IOP writes before sending command 0x80000011 (read
 * uncached by 48F878): the head and tail bytes of an unaligned read, or a
 * getstat / dread entry, are copied from data to dst. */
typedef struct SceFsRetBuf {
    /* 0x00 */ s32 result;
    /* 0x04 */ s32 function;
    /* 0x08 */ void *dst;
    /* 0x0C */ u32 size;
    /* 0x10 */ u8 data[0x430];
} SceFsRetBuf;

/* File descriptor record (0x10); flags 0x10000000 marks it in use. */
typedef struct SceFsFd {
    /* 0x00 */ s32 fd;
    /* 0x04 */ s32 flags;
    /* 0x08 */ u8 pad8[8];
} SceFsFd;

/* {func, arg} called by the 0x80000013 command handler. */
typedef struct SceFsCallback {
    /* 0x00 */ void (*func)(void *);
    /* 0x04 */ void *arg;
} SceFsCallback;

/* Volatile (docs/matching-tips.md), but sceWrite (4908C8) declares it plain
 * and only matches with plain accesses through a (s32 *) cast, as 48F878
 * already does for its reads. */
extern volatile s32 _sceFsAsyncSemaTable[32];
extern s32 _sceFsInitDone;
extern s32 _sceFsCallSema;
extern s32 _sceFsFdSema;
extern s32 _sceFsAsyncSema;
extern char *_sceFsWildVersion;   /* "...." */
extern void *_sceFsInitArg;
extern s32 *_sceFsIoctlArg;
extern SceFsRpcBuf _sceFsSendBuf;
extern u8 _sceFsRecvBuf[0x40];    /* +0: result or IOP version, read uncached */
extern SceFsRetBuf _sceFsRetBuf;
extern u8 _sceFsRetBufData[];     /* _sceFsRetBuf + 0x10 */
extern u8 _sceFsRetBufExtra[];    /* _sceFsRetBuf + 0x14 */
extern SceFsFd _sceFsFdTable[32];
extern sceSifClientData _sceFsClient;
extern u8 _sceFsIopVersion[4];    /* "2340" */
extern u8 _sceFsCmdHandlerArg[0x40];
extern SceFsCallback _sceFsIopCallback;

#endif
