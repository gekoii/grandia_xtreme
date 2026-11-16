#ifndef MAIN_N11_H
#define MAIN_N11_H

/* Movie player (main executable 0x004633B0-0x0046CF80): MovPlay/MovPlayList
 * and their read loops, Sony's playpss sample code (readbuf.c, videodec.c,
 * vibuf.c, vobuf.c, audiodec.c; sample names kept) and the display code.
 * The decoder handle, the video input ring and the frame queue (VideoDec,
 * ViBuf, VoBuf, VoTag) and their globals are in snd.h. Layouts from
 * docs/notes/names_observations.md (n11) and the code. */

#include "common.h"
#include "snd.h"
#include "sdk/eekernel.h"

/* struct ThreadParam is in sdk/eekernel.h. */

/* Read ring buffer (playpss ReadBuf), at movReadBuf in the work memory. */
typedef struct MovReadBuf {
    /* 0x000000 */ u8 data[0x180000];
    /* 0x180000 */ s32 put;       /* write offset */
    /* 0x180004 */ s32 count;     /* bytes waiting */
    /* 0x180008 */ s32 size;      /* 0x180000 */
} MovReadBuf; /* size = 0x18000C */

/* A picture's time stamps, as viBufGetTs returns them to mpegTS. */
typedef struct MovPts {
    /* 0x0 */ s64 pts;
    /* 0x8 */ s64 dts;
} MovPts; /* size = 0x10 */

/* MovTimeStamp and MovDmaTag are in snd.h (ViBuf). */

/* Audio decoder (playpss AudioDec), movAudioDec. */
typedef struct MovAudioDec {
    /* 0x00 */ s32 state;         /* 2 once started (audioDecStart) */
    /* 0x04 */ u8 unk4[0x28];
    /* 0x2C */ s32 hdrCount;
    /* 0x30 */ u8 *data;          /* movAudioBuf */
    /* 0x34 */ s32 put;
    /* 0x38 */ s32 count;
    /* 0x3C */ s32 size;          /* 0x2400 */
    /* 0x40 */ s32 totalBytes;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 iopBuffSize;   /* 0x1000 */
    /* 0x4C */ s32 iopLastPos;
    /* 0x50 */ s32 totalBytesSent; /* preset once >= iopBuffSize (audioDecIsPreset) */
    /* 0x54 */ s32 unk54;
} MovAudioDec; /* size = 0x58 */

/* Display (movDisp, set up by MovDispInit): sceGsDispEnv, the two frame
 * buffers and the image size the transfer packets are built for. The 64-bit
 * GS registers are u32 pairs and DISPFB is flat bit-fields: with u64 members
 * (8-byte alignment) or a nested struct gcc reads and writes the DISPFB
 * fields as one u64 (467FD8). */
typedef struct MovDisp {
    /* 0x00 */ u32 pmode[2];
    /* 0x08 */ u32 smode2[2];
    /* 0x10 */ u32 fbp : 9;        /* DISPFB */
    /* 0x10 */ u32 fbw : 6;
    /* 0x10 */ u32 psm : 5;
    /* 0x10 */ u32 pad10 : 12;
    /* 0x14 */ u32 dbx : 11;
    /* 0x14 */ u32 dby : 11;
    /* 0x14 */ u32 pad14 : 10;
    /* 0x18 */ u32 display[2];
    /* 0x20 */ u32 bgcolor[2];
    /* 0x28 */ s32 fbp0;          /* DISPFB.FBP of buffer 0 (MovDispSetBuffer) */
    /* 0x2C */ s32 fbp1;          /* buffer 1, after a 640x448 frame */
    /* 0x30 */ s32 width;         /* frame buffer width (FBW = (width + 63) / 64) */
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 imageWidth;    /* decoded image, sent in 16x16 blocks */
    /* 0x4C */ s32 imageHeight;
} MovDisp; /* size = 0x50 */

/* MovPlayList playlist (movPlaylistTable, 7 types). */
typedef struct MovPlaylist {
    /* 0x00 */ s32 count;         /* files */
    /* 0x04 */ char name[6][0x16]; /* "MOV/SOLONT.PSS", ... */
    /* 0x88 */ s32 repeat[6];     /* times each file is played, the repeats from the
                                   * read buffer (MovListReadLoop) */
    /* 0xA0 */ void *base[6];     /* where it is read to (the read buffer) */
    /* 0xB8 */ s32 size[6];       /* file sizes */
} MovPlaylist; /* size = 0xD0 */

/* Sprite rectangle of the GS packet helpers (MovGsSprite*), 12.4 fixed. */
typedef struct MovRect {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 w;
    /* 0xC */ s32 h;
} MovRect; /* size = 0x10 */

/* ---- globals ---- */

extern s8 sndMovieStreaming;            /* 0x0049B821 */
extern s32 movPadVblank;                /* 0x0049D6A0 */
extern s32 movPlayStatus;               /* 0x0049D6A4 */
extern MovPlaylist movPlaylistTable[7]; /* 0x0049D6A8 */
extern s32 movAudioHeld;                /* 0x0049DC74 */
extern volatile s32 movFrameStarve;     /* 0x0049DD30 */
extern s32 movOvlFieldCount;            /* 0x0049DD34 */
extern s32 movOvlScrollStep;            /* 0x0049DD38 */
extern s32 movOvlStripStep;             /* 0x0049DD3C */
extern s32 movOvlPictureStep;           /* 0x0049DD40 */
extern s32 movOvlLabelFadeStep;         /* 0x0049DD44 */
extern s32 movOvlDimFadeStep;           /* 0x0049DD48 */
extern s32 movSkipFadeStep;             /* 0x0049DD4C */
extern volatile s32 movDispActive;      /* 0x0049DD50 */
extern volatile s32 movVblankCount;     /* 0x0049DD54 */
extern volatile s32 movFrameSent;       /* 0x0049DD58 */
extern volatile s32 movOddField;        /* 0x0049DD5C */
extern volatile s32 movPathBusy;        /* 0x0049DD60 */
extern s32 movSkipButtons;              /* 0x004A9558 */
extern s32 movMpegWork;                 /* 0x004A955C */
extern s32 movDataStream;               /* 0x004A9564 */
extern s32 movFileInfo[];               /* 0x004A9568 */
extern s32 movTrpScroll;                /* 0x004A9570 */
extern s32 movTrpClut;                  /* 0x004A9574 */
extern s32 movVoBufData;                /* 0x004A9578 */
extern s32 movTrpLabel;                 /* 0x004A957C */
extern s32 movMainPrio;                 /* 0x004A9580 */
extern s32 movViBufData;                /* 0x004A9584 */
extern s32 movFileSize;                 /* 0x004A958C */
extern s32 movVoBufTag;                 /* 0x004A9590 */
extern s32 movTrpPicture;               /* 0x004A9594 */
extern s32 movAudioStream;              /* 0x004A9598 */
extern s32 movSkipRequest;              /* 0x004A95A0 */
extern s32 movPadPort;                  /* 0x004A95A4 */
extern s32 movDefThreadId;              /* 0x004A95A8 */
extern s32 movDecThreadId;              /* 0x004A95AC */
extern s32 movFrameMissCount;           /* 0x004A95B0 */
extern s32 movTrpStrip;                 /* 0x004A95B4 */
extern s32 movListType;                 /* 0x004A95B8 */
extern s32 movPadSlot;                  /* 0x004A95BC */
extern s32 movPadButtons;               /* 0x004A95C4 */
extern MovReadBuf *movReadBuf;          /* 0x004A95C8 */
extern s32 movPktIpartStripL;           /* 0x004A95CC */
extern s32 movPktIpartLabelAlpha;       /* 0x004A95D0 */
extern s32 movPktIpartPicture;          /* 0x004A95DC */
extern s32 movPktFadeAlpha;             /* 0x004A95E0 */
extern s32 movPktIpartColumn;           /* 0x004A95E4 */
extern volatile s32 movFadeDone;        /* 0x004A95EC */
extern volatile s32 movOvlEnd;          /* 0x004A95F0 */
extern s32 movPktIpartDimAlpha;         /* 0x004A95F4 */
extern s32 movPktIpartPictureSrc;       /* 0x004A95F8 */
extern s32 movPktIpartStripR;           /* 0x004A95FC */
extern s32 movPktStripSrc;              /* 0x004A9600 */
extern u8 threadGp[];                   /* 0x004B1470 */
extern u8 movViBufTag[];                /* 0x004DBE40 */
extern MovTimeStamp movTimeStamps[0x280]; /* 0x004DEA80 */
extern u8 movDefStack[];                /* 0x004E2680 */
extern u8 movDecStack[];                /* 0x004E2E80 */
extern MovDisp movDisp;                 /* 0x005A69C0 */
extern MovAudioDec movAudioDec;         /* 0x005A6A10 */

#endif
