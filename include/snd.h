#ifndef SND_H
#define SND_H

/* Sound (IOP sound/stream driver, EE side) and the movie player's buffers
 * in the main executable. Field names come from the debug pages
 * (iop_stat 425340, BGM page 425808), docs/notes/names_observations.md (n06,
 * n10, n11) and the code; Sony's playpss sample names are kept for the
 * movie player. */

#include "common.h"
#include "sdk/libmpeg.h"   /* sceMpeg */

/* ------------------------------------------------------------------------ */
/* IOP status block (0x21FF0100, uncached, written by the IOP driver).      */

/* One stream ring (iop_stat page, 425340): stat .. lsn. */
typedef struct SndIopStrmRing {
    /* 0x00 */ s16 stat;     /* >= 2 data ready (45FDF0) */
    /* 0x02 */ s16 secNum;   /* sectors in the ring (45FB38 "max") */
    /* 0x04 */ s16 getOfs;   /* first sector the EE may fetch (45FF60 "start") */
    /* 0x06 */ s16 bufOfs;
    /* 0x08 */ s16 bufLen;   /* (45FF60 "end") */
    /* 0x0A */ u16 dummy;
    /* 0x0C */ s32 readLen;
    /* 0x10 */ s32 bufAdr;   /* IOP address of the ring buffer; u8 * in 45FB38/45FF60,
                              * s32 in 4600E8/460380/4601E8/4604F0 */
    /* 0x14 */ s32 reqLen;
    /* 0x18 */ s32 lsn;
    /* 0x1C */ s32 unk1C;    /* read by the movie player (4633B0) */
} SndIopStrmRing; /* size = 0x20 */

/* Per stream index (BGM streams 0/1, 462078/461590). */
typedef struct SndIopStrmInfo {
    /* 0x0 */ s8 state;      /* 3..5 prepared, 0xB stopped (462078); u8 in 461590 */
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ s32 length;    /* play length, copied to the handle's duration */
} SndIopStrmInfo; /* size = 0x8 */

typedef struct SndIopStatus {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 batchSeq;      /* last command batch the IOP took (45F438) */
    /* 0x0E */ s16 doneSeq;       /* last batch the IOP processed */
    /* 0x10 */ s32 driveUnk10;    /* SndIopGetDriveStat 45FA68 */
    /* 0x14 */ u8 pad14[8];
    /* 0x1C */ s16 driveUnk1C;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 driveStat;     /* drive status */
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s32 driveUnk28;
    /* 0x2C */ s32 movieState;    /* movie stream state (4633B0 waits for >= 2) */
    /* 0x30 */ s8 bankState[16];  /* SE bank state per bank slot (460F60, 462030) */
    /* 0x40 */ SndIopStrmInfo strmInfo[2];
    /* 0x50 */ s32 ready;         /* sound driver ready (SndDriverInit 460B60 waits) */
    /* 0x54 */ u8 pad54[4];
    /* 0x58 */ u8 unk58[0x28];    /* SndIopGetStatus58 45F2B8 returns its address */
    /* 0x80 */ SndIopStrmRing strm[4]; /* the movie reads strm[2].stat/.secNum (4633B0, 463F00) */
} SndIopStatus; /* size = 0x100 */

/* ------------------------------------------------------------------------ */
/* Sound API argument blocks (SndPrepare 461B80, SndPlay 460F60,
 * SndGetStatus 461590, SndSetFade 461778). */

typedef struct SndPrepareReq {
    /* 0x0 */ s32 handle;    /* OBJNUM */
    /* 0x4 */ s32 index;     /* INDEXNUM - sound index id */
    /* 0x8 */ s32 unk8;      /* HANDLE on the BGM page */
    /* 0xC */ s32 unkC;      /* SECLEN on the BGM page */
} SndPrepareReq; /* size = 0x10 */

/* Filled by SndGetStatus 461590 from the sound handle (-1/0 when there is
 * no handle). */
typedef struct SndStatus {
    /* 0x0 */ s8 stat;       /* handle state: 0 stopped, >= 2 playing */
    /* 0x1 */ s8 atr;        /* handle's index type: 4 stream BGM, 5 SE, 10 soft voice ... */
    /* 0x2 */ s8 cate;       /* handle +6 */
    /* 0x3 */ s8 error;      /* type 5/10: voice; type 4: IOP stream state (10/11 in 440D88) */
    /* 0x4 */ s32 nowVol;    /* current volume */
    /* 0x8 */ s32 disVol;    /* fade target volume */
    /* 0xC */ s32 kTime;     /* handle timer while state 3, else -1 */
} SndStatus; /* size = 0x10 */

typedef struct SndPlayReq {
    /* 0x00 */ s32 handle;   /* OBJNUM */
    /* 0x04 */ s32 index;    /* INDEXNUM */
    /* 0x08 */ s32 dist;     /* attenuation */
    /* 0x0C */ s32 hPan;
    /* 0x10 */ s32 vPan;
} SndPlayReq; /* size = 0x14 */

typedef struct SndFadeReq {
    /* 0x0 */ s32 handle;
    /* 0x4 */ s32 volume;    /* target, 0..255 */
    /* 0x8 */ s32 speed;     /* fade speed index */
} SndFadeReq; /* size = 0xC */

/* ------------------------------------------------------------------------ */
/* BGM stream channels (sndBgmChannels, two, driven by SndBgmUpdate 440D88). */

typedef struct SndBgmChannel {
    /* 0x00 */ s32 unk0;          /* random 0..0xB3 (440BE8); void * in 440AC0/440BE8/440D88 */
    /* 0x04 */ s32 unk4;          /* 0 crossfade play (440AC0), 1 direct play (440BE8) */
    /* 0x08 */ s32 state;         /* PROCESS; u32 in 440CE8 (unsigned >= 12 compare) */
    /* 0x0C */ s32 handle;        /* channel + 1 */
    /* 0x10 */ s32 index;         /* index id base (+ channel for SndPrepare/SndPlay) */
    /* 0x14 */ s32 id;            /* BGM id */
    /* 0x18 */ s32 reqStop;       /* REQ STOP */
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 curVolume;     /* 0..255 from nowVol */
    /* 0x24 */ s32 volume;        /* VOLUME, target 0..255 */
    /* 0x28 */ s32 fade;          /* FADE speed, 0 = none */
    /* 0x2C */ s32 trackMask;     /* 8-track mask (440A78) */
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 fadeState;     /* 0 idle, 1 fading (440D88 "sub") */
    /* 0x38 */ u8 pad38[8];
    /* 0x40 */ s32 drvIndex;      /* INDEX - SndDriverInit arguments, channel 0 only */
    /* 0x44 */ s32 drvInitData;   /* INITDATA */
    /* 0x48 */ s32 drvInitSize;   /* INITSIZE */
    /* 0x4C */ SndPrepareReq prepare; /* VMSND OPEN */
    /* 0x5C */ SndStatus status;  /* VMSND STAT */
    /* 0x6C */ SndPlayReq play;   /* VMSND ON1 */
    /* 0x80 */ SndFadeReq fadeReq;
} SndBgmChannel; /* size = 0x8C */

/* ------------------------------------------------------------------------ */
/* Sound effect slots (sndSeSlots, 4 groups x 64 slots of 0x30). A slot is
 * its fields followed by its position, after a 0x10-byte group header
 * (SndSeSlot; SndSePlay3D 4416A0, SndSeSetPos 441898, SndPlayAtPos in
 * 4419B0). Most files (425DB0, 4415F0, 441648, 4416A0, 4417A0, 4419B0,
 * 441930, 441960, 441E18) index the slots from the group base instead, as
 * 0x10 bytes (the previous slot's position) then the fields
 * (SndSeSlotShifted), and only match that way: the fields' +0x10 has to
 * stay in the field offset, not in the address constant. Both are real
 * views of the same bytes, hence the union. */

typedef struct SndSeSlot {
    /* 0x00 */ u8 flags;     /* 8 positional (SndPlayAtPos), 0x10 hold after the end */
    /* 0x01 */ u8 state;     /* 0 free, 1 prepare, 2 start, 3 playing, 4 held, 5 moved */
    /* 0x02 */ u8 dist;      /* SndPlay attenuation */
    /* 0x03 */ u8 pan;
    /* 0x04 */ u16 timer;    /* s16 in 441E18 */
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ s32 wait;
    /* 0x0C */ s32 handle;   /* sound handle (SndGetStatus/SndStop) */
    /* 0x10 */ s32 index;    /* sound index id, or a data pointer (4417A0 stores (s32)data) */
    /* 0x14 */ u8 pad14[0xC];
    /* 0x20 */ f32 pos[4];   /* source position (u128 in 4419B0, 16-aligned in the data) */
} SndSeSlot; /* size = 0x30 */

typedef struct SndSeSlotShifted {
    /* 0x00 */ f32 prevPos[4]; /* the previous slot's position (the group header for slot 0) */
    /* 0x10 */ u8 flags;
    /* 0x11 */ u8 state;
    /* 0x12 */ u8 dist;
    /* 0x13 */ u8 pan;
    /* 0x14 */ u16 timer;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 wait;
    /* 0x1C */ s32 handle;
    /* 0x20 */ s32 index;    /* index id or data pointer (see SndSeSlot) */
    /* 0x24 */ u8 pad24[0xC];
} SndSeSlotShifted; /* size = 0x30 */

typedef struct SndSeGroup {
    union {
        /* 0x000 */ SndSeSlotShifted shifted[64]; /* + the last slot's position at 0xC00 */
        struct {
            /* 0x000 */ u8 hdr[0x10];
            /* 0x010 */ SndSeSlot slots[64];
        } s;
    } u;
} SndSeGroup; /* size = 0xC10 */

/* ------------------------------------------------------------------------ */
/* Stream slots (sndStrmSlots, 4: file reads 0/1, BGM streams 2/3). */

typedef struct SndStrmSlot {
    /* 0x0 */ s16 busy;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s32 lsn;       /* start sector */
    /* 0x8 */ s32 size;      /* bytes */
    /* 0xC */ s32 pos;       /* sectors fetched */
} SndStrmSlot; /* size = 0x10 */

/* ------------------------------------------------------------------------ */
/* Movie player buffers (Sony playpss: vobuf.c, vibuf.c, videodec.c).       */

#define MOV_N_VOBUF 5

/* Output tag of one decoded frame (0x36040): two image transfer packets. */
typedef struct VoTag {
    /* 0x00000 */ s32 status;     /* 2 = full (voBufIncCount) */
    /* 0x00004 */ u8 pad4[0x3C];
    /* 0x00040 */ u32 v[2][0x6C00]; /* image packets, sent at +0x40 / +0x1B040 */
} VoTag; /* size = 0x36040 */

typedef struct VoBuf {
    /* 0x00 */ s32 data;              /* frame data base (uncached), frame n at + n * 0x118000 */
    /* 0x04 */ VoTag *tag[MOV_N_VOBUF]; /* s32 in 464E50/465148 */
    /* 0x18 */ volatile s32 write;    /* put index */
    /* 0x1C */ volatile s32 count;    /* full frames */
    /* 0x20 */ s32 size;              /* frames (5) */
} VoBuf; /* size = 0x24 */

/* Video input time stamp (playpss TimeStamp); ViBuf.ts points to
 * movTimeStamps (0x280 of them). */
typedef struct MovTimeStamp {
    /* 0x00 */ s64 pts;           /* -1 none */
    /* 0x08 */ s64 dts;
    /* 0x10 */ s32 pos;           /* offset in the input ring */
    /* 0x14 */ s32 len;
} MovTimeStamp; /* size = 0x18 */

/* One DMA tag (0x10) of the video input ring's tag list (ViBuf.tag), as
 * scTag2 writes it. */
typedef struct MovDmaTag {
    /* 0x0 */ u64 tag;
    /* 0x8 */ u64 pad;
} MovDmaTag; /* size = 0x10 */

/* Video input ring (viBuf*). */
typedef struct ViBuf {
    /* 0x00 */ u8 *data;
    /* 0x04 */ MovDmaTag *tag;
    /* 0x08 */ s32 n;
    /* 0x0C */ s32 dmaStart;
    /* 0x10 */ s32 dmaN;
    /* 0x14 */ s32 readBytes;
    /* 0x18 */ s32 buffSize;
    /* 0x1C */ u32 env[9];            /* saved IPU/D3/D4 DMA state (sceIpuDmaEnv's registers;
                                       * 466A40/466B78 store them by index) */
    /* 0x40 */ s32 sema;
    /* 0x44 */ s32 isActive;
    /* 0x48 */ s64 totalBytes;
    /* 0x50 */ MovTimeStamp *ts;      /* timestamp table (movTimeStamps) */
    /* 0x54 */ s32 nTs;
    /* 0x58 */ s32 countTs;
    /* 0x5C */ s32 wtTs;
} ViBuf; /* size = 0x60 */

typedef struct VideoDec {
    /* 0x00 */ sceMpeg mpeg;
    /* 0x48 */ ViBuf vibuf;
    /* 0xA8 */ s32 state;             /* 0 normal, 1 abort, 2 flush, 3 end */
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ s32 dmacHandlerId;     /* GIF end handler (AddDmacHandler) */
    /* 0xB4 */ s32 intcHandlerId;     /* vblank handler (AddIntcHandler) */
} VideoDec; /* size = 0xB8 */

/* ------------------------------------------------------------------------ */
/* Globals */

extern SndIopStatus *sndIopStatus;       /* 0x004A94FC, = 0x21FF0100; s32 in 45EEB8/45F2B8,
                                          * u8 * in 4601E8/4604F0 (raw offsets) */
extern SndBgmChannel sndBgmChannels[2];  /* 0x004F5E50 */
extern SndSeGroup sndSeSlots[4];         /* 0x004F2E10 */
extern SndStrmSlot sndStrmSlots[4];      /* 0x005A0280 */

extern VoBuf movVoBuf;                   /* 0x005A6998 */
extern VideoDec movVideoDec;             /* 0x005A6A68 */
extern ViBuf movVideoDecViBuf;           /* 0x005A6AB0 = movVideoDec.vibuf */

#endif
