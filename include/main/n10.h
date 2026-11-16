#ifndef MAIN_N10_H
#define MAIN_N10_H

/* Sound driver, EE side (0x0045F1B0-0x004633B0): the IOP command queue and
 * SIF transfers, the driver's ISO9660 lookup (SndFs*), the stream slots
 * (SndStrm*), the sound handle API (Snd*) and the unused EE soft mixer
 * (SndSoft*). The IOP status block, the stream slots and the API argument
 * blocks are in snd.h. */

#include "common.h"
#include "vu.h"   /* VuVec3 */
#include "snd.h"
#include "sdk/sif.h"

/* ------------------------------------------------------------------------ */
/* IOP command queue (SndIopCmdPush 45F398, SndIopCmdFlush 45F438).         */

/* One queued command; a command with cmd 0 ends the batch. The buffer is
 * 0x200 bytes: 24 commands, then a string area at +0x180
 * (SndIopCmdPushString 45F228). */
typedef struct SndIopCmd {
    /* 0x0 */ u16 seq;       /* sndIopCmdSeq when queued; the first one is the batch seq */
    /* 0x2 */ s16 cmd;       /* 0x1xx stream, 0x2xx sound commands */
    /* 0x4 */ s32 arg0;
    /* 0x8 */ s32 arg1;
    /* 0xC */ s32 arg2;
} SndIopCmd; /* size = 0x10 */

/* sceSifDmaData (sceSifSetDma descriptor) is in sdk/sif.h. */

/* ------------------------------------------------------------------------ */
/* Driver file system (SndFsMount 45F610, SndFsFind 45F938,
 * SndFsParseDirRecord 460890).                                             */

/* Directory tree entry, parsed from an ISO9660 directory record. An entry
 * with lsn -1 ends a directory. */
typedef struct SndFsDirEntry {
    /* 0x00 */ s8 name[14];  /* without the ";1" version */
    /* 0x0E */ s16 flags;    /* ISO9660 file flags, 2 = directory */
    /* 0x10 */ s32 lsn;
    /* 0x14 */ s32 size;
    /* 0x18 */ struct SndFsDirEntry *child; /* a directory's entries */
    /* 0x1C */ s32 unk1C;
} SndFsDirEntry; /* size = 0x20 */

/* ------------------------------------------------------------------------ */
/* Sound index (GRX_SND.IDX, sndIndexTable), one entry per index id.        */

typedef struct SndIndexEntry {
    /* 0x0 */ s8 type;       /* 4 stream BGM, 5 SE, 9 SE bank, 10 soft voice, 11 channel
                              * value; | 0x80 once loaded (SndPrepare), -1 ends the table */
    /* 0x1 */ u8 unk1;       /* copied to SndHandle.unk6 */
    /* 0x2 */ s8 bank;       /* SE: bank channel (& 0xF) | volume mode (& 0xE0, SndPlay);
                              * stream: stream index; type 11: channel */
    /* 0x3 */ u8 slot;       /* SE: entry in its bank */
    /* 0x4 */ union {
        s32 packed;          /* SndPrepare 461B80 writes both halves at once */
        struct {
            /* 0x4 */ u16 duration; /* frames until key off (SndPlay) */
            /* 0x6 */ s16 volume;   /* & 0x7FFF, scale /10000 (SndCalcVolPan) */
        } s;
    } u;
} SndIndexEntry; /* size = 0x8 */

/* ------------------------------------------------------------------------ */
/* Sound handles (sndHandles, 64; SndFindHandle 462BE0).                    */

typedef struct SndHandle {
    /* 0x00 */ s32 id;        /* handle id given by the caller */
    /* 0x04 */ s8 state;      /* 0 free, 1/2 preparing (stream), 3 playing, 4 stream
                               * prepared, -1 stream ended (SndUpdate 462078) */
    /* 0x05 */ s8 type;       /* index type: 4 stream, 5 SE, 6, 10 soft voice */
    /* 0x06 */ u8 unk6;       /* SndIndexEntry.unk1 */
    /* 0x07 */ s8 unk7;       /* SE: bank slot; stream: 1 play requested, 2 started */
    /* 0x08 */ s16 voice;     /* SE voice / soft voice index; stream: length latched (462078) */
    /* 0x0A */ s16 chan;      /* SE: bank channel; stream: stream index (0/1) */
    /* 0x0C */ s16 dist;      /* SndPlayReq.dist */
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ s16 pan;       /* SndPlayReq.hPan */
    /* 0x12 */ u8 pad12[2];
    /* 0x14 */ s16 volL;      /* last L/R volumes sent (SndCalcVolPan) */
    /* 0x16 */ s16 volR;
    /* 0x18 */ s32 baseVol;   /* SndIndexEntry volume & 0x7FFF */
    /* 0x1C */ s32 fadeSpeed; /* index into sndFadeInStep / sndFadeOutRate (SndSetFade) */
    /* 0x20 */ s32 index;     /* sound index id */
    /* 0x24 */ s32 curVol;    /* 0..10000; SndStatus.nowVol */
    /* 0x28 */ s32 targetVol; /* SndStatus.disVol */
    /* 0x2C */ s32 duration;  /* frames, 0 = until stopped */
    /* 0x30 */ s32 timer;     /* frames played; SndStatus.kTime */
    /* 0x34 */ s32 unk34;     /* last value sent with command 0x238, 0x10000 at start */
    /* 0x38 */ s32 unk38;     /* 0x100 at start, set by SndControl(1) */
    /* 0x3C */ u8 pad3C[0x24];
    /* 0x60 */ s32 strmSlot;  /* stream: sndStrmSlots index (2/3) */
} SndHandle; /* size = 0x64 */

/* ------------------------------------------------------------------------ */
/* EE software voices (sndSoftVoices, 8) of the unused mixer
 * (SndSoftVoiceFetch 462398, SndSoftMix 462528, SndSoftVoiceAlloc 4631A0). */

typedef struct SndSoftVoice {
    /* 0x00 */ s8 state;      /* 0 free, 1 playing, 2 released (decaying) */
    /* 0x01 */ s8 loop;       /* 0 = stop at the end */
    /* 0x02 */ s8 delayL;     /* current per-ear sample delays (sndPanDelayTable) */
    /* 0x03 */ s8 delayR;
    /* 0x04 */ SndHandle *handle;
    /* 0x08 */ s32 pos;       /* sample address, +0x40 per fetch */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 loopStart;
    /* 0x14 */ s32 end;
    /* 0x18 */ s32 lastL;     /* last sample of the previous block per ear */
    /* 0x1C */ s32 lastR;
    /* 0x20 */ s16 pan;       /* 0..255, steps towards panTarget */
    /* 0x22 */ s16 panTarget;
    /* 0x24 */ s16 unk24;     /* steps towards unk26; SndSoftVoiceAlloc steals the highest */
    /* 0x26 */ s16 unk26;
    /* 0x28 */ f32 env;       /* envelope, * decay while released */
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 decay;
} SndSoftVoice; /* size = 0x34 */

/* ------------------------------------------------------------------------ */
/* Globals                                                                  */

/* IOP command queue and SIF transfers */
extern s8 sndIopCmdDmaBusy;              /* 0x0049B822 */
extern s8 sndIopStringCmdPending;        /* 0x0049B823 */
extern s16 sndIopCmdSeq;                 /* 0x0049B824 */
extern s16 sndIopCmdDmaId;               /* 0x0049B826 */
extern s32 sndIopCmdCount;               /* 0x0049B828 */
extern s16 sndIopWaitSeq;                /* 0x0049B844 */
extern SndIopCmd *sndIopCmdBufPtr;       /* 0x004A9500 */
extern s8 sndIopStatusRecvBusy;          /* 0x0049B820 */
extern s8 sndIopRecvBusy;                /* 0x0049B83C */
extern sceSifReceiveData sndIopStatusRecvData; /* 0x004DB980 */
extern sceSifReceiveData sndIopRecvData; /* 0x005A02C0 */

/* Driver file system */
extern SndFsDirEntry *sndFsRootDir;      /* 0x004A951C */
extern SndFsDirEntry *sndFsCurDir;       /* 0x004A9524 */
extern s32 sndFsDirStackDepth;           /* 0x004A9528 */
extern SndFsDirEntry *sndFsDirStack[7];  /* 0x005A02F0 */

/* Stream fetches (IOP -> EE) and uploads (EE -> IOP) */
extern s8 sndStrmFetchWrapped;           /* 0x0049B83D */
extern s32 sndStrmFetchSlot;             /* 0x0049B840 */
extern s32 sndStrmFetchRemain;           /* 0x004A952C */
extern s32 sndStrmFetchCount;            /* 0x004A9530 */
extern u8 *sndStrmFetchDst2;             /* 0x004A953C */
extern s16 sndStrmUploadSeq;             /* 0x0049B846 */
extern s16 sndStrmUploadDmaId;           /* 0x0049B848 */
extern s32 sndStrmUploadSlot;            /* 0x0049B84C */
extern s32 sndStrmUploadCount;           /* 0x0049B850 */
extern s32 sndStrmUploadRemain;          /* 0x0049B854 */
extern s32 sndStrmUploadSrc2;            /* 0x004A9538 */
extern sceSifDmaData sndStrmUploadDmaDesc; /* 0x005A02E0 */
extern s32 sndStrmDataLsn;               /* 0x004A9540 */
extern s32 sndStrmIndex[0x400];          /* 0x005A0C80 */

/* Sound handles and voices */
extern s8 sndPaused;                     /* 0x0049D689 */
extern s32 sndInitDone;                  /* 0x0049D68C */
extern SndIndexEntry *sndIndexTable;     /* 0x004A9550 */
extern SndHandle sndHandles[64];         /* 0x005A4B68 */
extern s8 sndSeVoiceState[24];           /* 0x005A4B40 */
extern s32 sndSeVoiceTimer[24];          /* 0x005A64C8 */
extern s32 sndSeVoiceVol[24];            /* 0x005A6938 */
extern s8 sndBankSlot[16];               /* 0x005A2FD0 */
extern s32 sndVolumeCurve[256];          /* 0x005A2BD0 */
extern s32 sndFadeInStep[256];           /* 0x005A4740 */
extern s32 sndFadeOutRate[256];          /* 0x005A26C0 */

/* Listener */
extern s32 sndListenerAngle;             /* 0x0049D690 */
extern f32 sndDistanceScale;             /* 0x0049D698 */
extern VuVec3 sndListenerDir;           /* 0x005A2BC0 */
extern VuVec3 sndListenerPos;           /* 0x005A4B58 */

/* PCM monitor and soft mixer */
extern s8 sndPcmMonitorOff;              /* 0x0049D688 */
extern s16 sndPcmBuf[0x4000];            /* 0x00597780, L/R halves */
extern SndSoftVoice sndSoftVoices[8];    /* 0x005A2100 */

#endif
