/*
 * Sony libmc: the EE side of the mcserv memory card server (0x80000400).
 *
 * The RPC argument blocks have ps2sdk libmc.c's layouts and field names
 * (mcDescParam_t, mcNameParam_t); sceMcTblGetDir and sceMcStDateTime are the
 * SCE libmc.h types.
 */
#ifndef SDK_LIBMC_H
#define SDK_LIBMC_H

#include "common.h"
#include "sdk/sif.h"

/* sceMcStDateTime (8). */
typedef struct sceMcStDateTime {
    /* 0x00 */ u8 Resv2;
    /* 0x01 */ u8 Sec;
    /* 0x02 */ u8 Min;
    /* 0x03 */ u8 Hour;
    /* 0x04 */ u8 Day;
    /* 0x05 */ u8 Month;
    /* 0x06 */ u16 Year;
} sceMcStDateTime;

/* sceMcTblGetDir (0x40), one directory entry. */
typedef struct sceMcTblGetDir {
    /* 0x00 */ sceMcStDateTime _Create;
    /* 0x08 */ sceMcStDateTime _Modify;
    /* 0x10 */ u32 FileSizeByte;
    /* 0x14 */ u16 AttrFile;
    /* 0x16 */ u16 Reserve1;
    /* 0x18 */ u32 Reserve2;
    /* 0x1C */ u32 PdaAplNo;
    /* 0x20 */ char EntryName[32];
} sceMcTblGetDir;

/* Argument block of the fd/port calls (0x30), ps2sdk mcDescParam_t. GetInfo
 * sends 0/1 flags in size/offset/origin for the type/free/format results it
 * wants; Write puts the unaligned head bytes in data and their count in
 * origin. */
typedef struct SceMcDescParam {
    /* 0x00 */ s32 fd;
    /* 0x04 */ s32 port;
    /* 0x08 */ s32 slot;
    /* 0x0C */ s32 size;
    /* 0x10 */ s32 offset;
    /* 0x14 */ s32 origin;
    /* 0x18 */ void *buffer;
    /* 0x1C */ void *param;
    /* 0x20 */ u8 data[16];
} SceMcDescParam;

/* Argument block of the path calls (0x414), ps2sdk mcNameParam_t: Open,
 * Chdir (table = the result directory buffer), GetDir, SetFileInfo, Delete,
 * Rename. */
typedef struct SceMcNameParam {
    /* 0x000 */ s32 port;
    /* 0x004 */ s32 slot;
    /* 0x008 */ s32 flags;
    /* 0x00C */ s32 maxent;
    /* 0x010 */ void *table;
    /* 0x014 */ char name[0x400];
} SceMcNameParam;

/* RPC receive buffer (0x240 reserved). */
typedef struct SceMcRecvBuf {
    /* 0x00 */ s32 result;
    /* 0x04 */ s32 mcservVersion;   /* sceMcInit (RPC 0xFE) */
    /* 0x08 */ s32 mcmanVersion;
    /* 0x0C */ u8 padC[0x30];
    /* 0x3C */ s32 sema;            /* stored by 46FFD8 */
    /* 0x40 */ u8 pad40[0x200];
} SceMcRecvBuf;

extern s32 _sceMcCmdNo;                 /* sceMcFunc number in flight, 0 when idle */
extern s32 _sceMcSema;
extern sceSifClientData _sceMcClient;
extern s32 *_sceMcGetInfoType;
extern s32 *_sceMcGetInfoFree;
extern s32 *_sceMcGetInfoFormat;
extern sceMcTblGetDir _sceMcFileInfoBuf;
extern SceMcDescParam _sceMcCmdArg;
extern SceMcNameParam _sceMcNameArg;
extern u8 _sceMcRecvInfoBuf[0xC0];      /* read uncached by the end functions */
extern char _sceMcCurDirBuf[0x400];
extern SceMcRecvBuf _sceMcRecvBuf;

#endif
