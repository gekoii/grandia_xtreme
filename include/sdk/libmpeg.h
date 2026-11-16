/*
 * Sony libmpeg (and the libipu DMA environment it saves), whose decoder is
 * MSSG mpeg2decode adapted to the IPU.
 *
 * sceMpeg and sceIpuDmaEnv are the SCE libmpeg.h / libipu.h types. The
 * library's internal state behind sceMpeg.sys, its picture records and its
 * macroblock slots have no known names; their fields are named for what the
 * code does with them. The MSSG globals keep their mpeg2decode names and
 * types (int). 4732C0 and 473908 declare some of them u32 (horizontal_size,
 * vertical_size, bit_rate_value, vbv_buffer_size, the sequence_extension
 * fields, the load_*_quantizer_matrix flags); both also match with int.
 */
#ifndef SDK_LIBMPEG_H
#define SDK_LIBMPEG_H

#include "common.h"

/* sceIpuDmaEnv (0x24): IPU_TO (ch 4) and IPU_FROM (ch 3) DMA registers and
 * the IPU state saved by sceIpuStopDMA, restored by sceIpuRestartDMA. */
typedef struct sceIpuDmaEnv {
    /* 0x00 */ u32 d4madr;
    /* 0x04 */ u32 d4tadr;
    /* 0x08 */ u32 d4qwc;
    /* 0x0C */ u32 d4chcr;
    /* 0x10 */ u32 d3madr;
    /* 0x14 */ u32 d3qwc;
    /* 0x18 */ u32 d3chcr;
    /* 0x1C */ u32 ipubp;
    /* 0x20 */ u32 ipuctrl;
} sceIpuDmaEnv;

/* sceMpegAddCallback slot (8), indexed by sceMpegCbType: 0 error, 1 no data,
 * 2 stop DMA, 3 restart DMA, 4 background, 5 time stamp, 6 stream. */
typedef struct SceMpegCallback {
    /* 0x00 */ s32 (*func)(void *mp, void *cbdata, void *anyData);
    /* 0x04 */ void *data;
} SceMpegCallback;

/* sceMpegAddStrCallback entry (0x18): a stream matches when
 * (stream id & mask) == key. */
typedef struct SceMpegStrCallback {
    /* 0x00 */ u64 key;
    /* 0x08 */ u64 mask;
    /* 0x10 */ s32 (*func)(void *mp, void *cbdata, void *anyData);
    /* 0x14 */ void *data;
} SceMpegStrCallback;

/* Internal decoder state behind sceMpeg.sys (0x108), set up by sceMpegCreate
 * at the start of the work area; the work area heap follows it. */
typedef struct SceMpegSys {
    /* 0x00 */ s32 isEnd;                 /* sceMpegIsEnd */
    /* 0x04 */ s32 sequenceFramenum;      /* frames decoded in the sequence (MSSG Sequence_Framenum); 0 = sceMpegIsRefBuffEmpty */
    /* 0x08 */ s32 outputState;           /* 2 once a picture is ready (478430) */
    /* 0x0C */ SceMpegCallback callbacks[7];
    /* 0x44 */ SceMpegStrCallback *strCallbacks;  /* 0x600-byte heap block */
    /* 0x48 */ s32 strCallbackCount;
    /* 0x4C */ sceIpuDmaEnv ipuDmaEnv;    /* used by the default stop/restart DMA callbacks */
    /* 0x70 */ s32 ptsGapSet;             /* sceMpegSetDefaultPtsGap / ResetDefaultPtsGap */
    /* 0x74 */ u8 pad74[4];
    /* 0x78 */ s64 ptsGap;
    /* 0x80 */ s32 lastPts;               /* pts of the last picture output, -1 none */
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ s64 fieldCount;            /* display fields of the last picture */
    /* 0x90 */ s32 ptsInterpCount;
    /* 0x94 */ s32 decodeModeI;           /* sceMpegSetDecodeMode */
    /* 0x98 */ s32 decodeModeP;
    /* 0x9C */ s32 decodeModeB;
    /* 0xA0 */ s32 countI;                /* pictures decoded by type, reset per picture call */
    /* 0xA4 */ s32 countP;
    /* 0xA8 */ s32 countB;
    /* 0xAC */ s32 readyFramenum;         /* _sceMpegBitstreamFramenum when the last picture was ready */
    /* 0xB0 */ s32 outputCsc;             /* 1 sceMpegGetPicture (RGB32 via CSC), 0 the RAW8 variants */
    /* 0xB4 */ s32 frameCenterOffset[6];  /* [0..2] horizontal, [3..5] vertical */
    /* 0xCC */ s32 displayWidth;
    /* 0xD0 */ s32 displayHeight;
    /* 0xD4 */ s32 firstFieldStructure;   /* picture_structure of the first field, 0 after a sequence header */
    /* 0xD8 */ u32 imageAddr;             /* output buffer of the picture call */
    /* 0xDC */ s32 imageWidth;
    /* 0xE0 */ s32 imageHeight;
    /* 0xE4 */ s32 imageSize;             /* in macroblocks */
    /* 0xE8 */ s32 unkE8;                 /* cleared by the GOP header, drops B pictures of an open GOP (477DC0) */
    /* 0xEC */ u8 padEC[4];
    /* 0xF0 */ s64 nextPts;               /* _sceMpegSetNextPts, -1 none */
    /* 0xF8 */ s32 nextPtsState;
    /* 0xFC */ void *frameBuf[3];         /* the three frame buffers, from the work area heap */
} SceMpegSys;

/* Work area heap header (0x10), right after SceMpegSys. */
typedef struct SceMpegHeap {
    /* 0x00 */ u32 base;
    /* 0x04 */ u32 size;
    /* 0x08 */ u32 cur;
    /* 0x0C */ u32 mark;
} SceMpegHeap;

/* sceMpeg (0x48), the public decoder handle. */
typedef struct sceMpeg {
    /* 0x00 */ s32 width;
    /* 0x04 */ s32 height;
    /* 0x08 */ s32 frameCount;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s64 pts;
    /* 0x18 */ s64 dts;
    /* 0x20 */ s64 flags;
    /* 0x28 */ s64 pts2nd;
    /* 0x30 */ s64 dts2nd;
    /* 0x38 */ s64 flags2nd;
    /* 0x40 */ SceMpegSys *sys;
} sceMpeg;

/* Picture record (0x68): one frame buffer, or its top or bottom field.
 * 477DC0 fills the picture fields from the MSSG globals when decoding starts;
 * the output code reads them back. */
typedef struct SceMpegPicture {
    /* 0x00 */ u32 addr;                  /* buffer base (uncached / UCAB) */
    /* 0x04 */ s32 width;
    /* 0x08 */ s32 height;
    /* 0x0C */ s32 mbWidth;
    /* 0x10 */ s32 mbHeight;              /* also the macroblock column stride */
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ s64 pts;
    /* 0x20 */ s64 dts;
    /* 0x28 */ s32 ref;                   /* 1 once fully decoded, 0 while decoding */
    /* 0x2C */ s32 pictureCodingType;
    /* 0x30 */ s32 pictureStructure;
    /* 0x34 */ s32 progressiveSequence;
    /* 0x38 */ s32 progressiveFrame;
    /* 0x3C */ s32 topFieldFirst;
    /* 0x40 */ s32 repeatFirstField;
    /* 0x44 */ s32 frameCenterOffset[6];  /* [0..2] horizontal, [3..5] vertical */
    /* 0x5C */ s32 displayHorizontalSize;
    /* 0x60 */ s32 displayVerticalSize;
    /* 0x64 */ u8 pad64[4];
} SceMpegPicture;

/* Argument block of one prediction step (0x1C), filled by _formPrediction. */
typedef struct SceMpegPredArg {
    /* 0x00 */ u8 data[0x1C];
} SceMpegPredArg;

/* Macroblock slot (0x140); two are used alternately while the IPU decodes
 * the next macroblock. */
typedef struct SceMpegMbSlot {
    /* 0x000 */ u8 *predBuf;               /* scratchpad: toSPR destination of the reference blocks; a pointer for 473018 */
    /* 0x004 */ u32 blockBuf;             /* scratchpad: IPU_FROM destination of the residual */
    /* 0x008 */ s32 predSrcA[4];          /* reference addresses fetched for each prediction */
    /* 0x018 */ s32 predSrcB[4];
    /* 0x028 */ void (*predFuncA[4])(void *);
    /* 0x038 */ void (*predFuncB[4])(void *);
    /* 0x048 */ SceMpegPredArg predArgA[4];
    /* 0x0B8 */ SceMpegPredArg predArgB[4];
    /* 0x128 */ u8 *dst;                   /* macroblock address in the frame buffer; a pointer: 473B70 only matches with one */
    /* 0x12C */ s32 predCount;
    /* 0x130 */ s32 intra;
    /* 0x134 */ s32 dctType;
    /* 0x138 */ s32 predPending;          /* reference DMA started */
    /* 0x13C */ s32 skipped;
} SceMpegMbSlot;

typedef struct SceMpegMbSlots {
    /* 0x000 */ SceMpegMbSlot slot[2];
    /* 0x280 */ s32 cur;
    /* 0x284 */ s32 unk284;
} SceMpegMbSlots;

/* Stream type pattern (0x10), indexed by the sceMpegAddStrCallback type. */
typedef struct SceMpegStrType {
    /* 0x00 */ u64 pattern;
    /* 0x08 */ u64 mask;
} SceMpegStrType;

/* One IPU_TO DMA tag (0x10). */
typedef struct SceMpegDmaTag {
    /* 0x00 */ u64 tag;
    /* 0x08 */ u64 pad;
} SceMpegDmaTag;

/* ---- libmpeg globals ---- */

extern SceMpegStrType _sceMpegStrTypeTable[10];
extern u8 _sceMpegPesFlagBits[0x10];
extern u8 *_sceMpegSprWork;            /* 0x70000000 */
extern u32 _sceMpegRefDmaTags;
extern u32 _sceMpegSprPred;            /* 0x70003600 */
extern sceMpeg *_sceMpegCurMp;
extern u32 _sceMpegInputBuf;
extern u32 _sceMpegInputSize;
extern SceMpegPicture *_sceMpegFwdRefFrame;
extern SceMpegPicture *_sceMpegBwdRefFrame;
extern SceMpegPicture *_sceMpegAuxFrame;
extern SceMpegPicture *_sceMpegFwdRefTop;
extern SceMpegPicture *_sceMpegBwdRefTop;
extern SceMpegPicture *_sceMpegAuxTop;
extern SceMpegPicture *_sceMpegFwdRefBottom;
extern SceMpegPicture *_sceMpegBwdRefBottom;
extern SceMpegPicture *_sceMpegAuxBottom;
extern s32 _sceMpegBitsStale;
extern u8 _sceMpegDefNonIntraQuantMatrix[64];
extern s32 _sceMpegBitstreamFramenum;
extern s32 _sceMpegGopSec;              /* MSSG sec */
extern s32 _sceMpegGopFrame;            /* MSSG frame */
extern s32 _sceMpegDcReset;
extern SceMpegPicture *_sceMpegCurFrame;
extern SceMpegPicture *_sceMpegCurTop;
extern SceMpegPicture *_sceMpegCurBottom;
extern SceMpegPicture _sceMpegPicFrame0;
extern SceMpegPicture _sceMpegPicFrame1;
extern SceMpegPicture _sceMpegPicFrame2;
extern SceMpegPicture _sceMpegPicTop0;
extern SceMpegPicture _sceMpegPicTop1;
extern SceMpegPicture _sceMpegPicTop2;
extern SceMpegPicture _sceMpegPicBottom0;
extern SceMpegPicture _sceMpegPicBottom1;
extern SceMpegPicture _sceMpegPicBottom2;
extern SceMpegMbSlots _sceMpegMbSlots;
extern s32 _sceMpegPictureDone;
extern s64 _sceMpegPicPts;
extern s64 _sceMpegPicDts;
extern u32 _sceMpegBfr;                 /* MSSG ld->Bfr */
extern s32 _sceMpegIncnt;               /* MSSG ld->Incnt */
extern s32 _sceMpegIpuCmdInvalidatesBfr[16];
extern void (*_sceMpegExtensionTable[11])(void);
extern u32 _sceMpegFieldCountTable[16];
extern s32 _sceMpegCscError;
extern volatile s32 _sceMpegCscChunkDone;
extern SceMpegDmaTag _sceMpegIpuToTags[0x81];
extern s32 _sceMpegCscMbLeft;
extern s32 _sceMpegCscDstAddr;
extern s32 _sceMpegCscChunks;
extern s32 _sceMpegIpuToIntrCount;
extern u32 _sceMpegIpuToQwcLeft;
extern u32 _sceMpegIpuToSrcAddr;

/* ---- MSSG mpeg2decode globals (global.h), all int ---- */

extern u8 default_intra_quantizer_matrix[64];
extern s32 Fault_Flag;
extern s32 Coded_Picture_Width;
extern s32 Coded_Picture_Height;
extern s32 Chroma_Width;
extern s32 Chroma_Height;
extern s32 Second_Field;
extern s32 horizontal_size;
extern s32 vertical_size;
extern s32 mb_width;
extern s32 mb_height;
extern s32 aspect_ratio_information;
extern s32 frame_rate_code;
extern s32 bit_rate_value;
extern s32 vbv_buffer_size;
extern s32 constrained_parameters_flag;
extern s32 profile_and_level_indication;
extern s32 progressive_sequence;
extern s32 chroma_format;
extern s32 low_delay;
extern s32 frame_rate_extension_n;
extern s32 frame_rate_extension_d;
extern s32 video_format;
extern s32 color_description;
extern s32 color_primaries;
extern s32 transfer_characteristics;
extern s32 matrix_coefficients;
extern s32 display_horizontal_size;
extern s32 display_vertical_size;
extern s32 temporal_reference;
extern s32 picture_coding_type;
extern s32 vbv_delay;
extern s32 full_pel_forward_vector;
extern s32 forward_f_code;
extern s32 full_pel_backward_vector;
extern s32 backward_f_code;
extern s32 f_code[2][2];                /* 4775A8 declares it f_code[4] */
extern s32 intra_dc_precision;
extern s32 picture_structure;
extern s32 top_field_first;
extern s32 frame_pred_frame_dct;
extern s32 concealment_motion_vectors;
extern s32 intra_vlc_format;
extern s32 repeat_first_field;
extern s32 chroma_420_type;
extern s32 progressive_frame;
extern s32 composite_display_flag;
extern s32 v_axis;
extern s32 field_sequence;
extern s32 sub_carrier;
extern s32 burst_amplitude;
extern s32 sub_carrier_phase;
extern s32 frame_center_horizontal_offset[3];
extern s32 frame_center_vertical_offset[3];
extern s32 copyright_flag;
extern s32 copyright_identifier;
extern s32 original_or_copy;
extern s32 copyright_number_1;
extern s32 copyright_number_2;
extern s32 copyright_number_3;
extern s32 drop_flag;
extern s32 hour;
extern s32 minute;
extern s32 closed_gop;
extern s32 broken_link;
extern s32 True_Framenum;
extern s32 quantizer_scale_code;
/* MSSG ld fields, flattened into globals */
extern s32 load_intra_quantizer_matrix;
extern s32 load_non_intra_quantizer_matrix;
extern s32 MPEG2_Flag;
extern s32 q_scale_type;
extern s32 alternate_scan;
extern s32 intra_slice;
extern s32 Temporal_Reference_Base;
extern s32 True_Framenum_max;
extern s32 Temporal_Reference_GOP_Reset;
extern s32 temporal_reference_wrap;
extern s32 temporal_reference_old;

#endif
