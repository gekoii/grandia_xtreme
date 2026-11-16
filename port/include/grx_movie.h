/* Movie playback (port/src/movie/).

   The game's player (MovPlay 4633B0 and the ezmovie-style code around it:
   readBuf, viBuf, voBuf, videoDec, the vblank and GIF-end handlers, the
   decoder thread) stays as it is. Three things under it are replaced:

   1. Sony's libmpeg decoder and libipu (src/main/4723B0.c-478FF7.c,
      479208.c-4795E0.c), which ran on the IPU: sceMpeg* in C on top of
      the software MPEG-2 decoder below (port/src/movie/libmpeg.c). The
      demuxer (sceMpegDemuxPssRing and its _sceMpegBs* bit reader) is
      plain C and stays.
   2. The IPU's input DMA (channel 4, toIPU): the game's viBuf code still
      builds its source chain and writes D4_MADR/TADR/QWC/CHCR into the
      mapped registers; the decoder reads its bitstream by walking that
      chain and writes the registers back as the DMAC would, so viBuf's
      bookkeeping (getFIFOindex, viBufAddDMA, viBufGetTs) works unchanged.
   3. GS_IOP.IRX's movie_server (the IOP side of command 0x300): the split
      of the .MOV file into the video stream (PSS ring) and the audio
      stream (VBS, handed to the sound driver), port/src/movie/iop_movie.c.

   The decoded frames go to the display the game's way: sceMpegGetPicture
   writes RGBA32 macroblocks (as the IPU's CSC did) into the voBuf entry,
   and the game's vblank handler sends its prebuilt GIF chain (image
   transfers, then the textured sprites) through GIF DMA (port/src/dma.c
   -> grx_gs_gif). The audio goes to the sound driver as VBS sectors
   (grx_snd_strm_feed, grx_snd.h), which plays them through the SPU2 as
   GS_IOP's own VBS player did.

   No game names in this header. */
#ifndef GRX_MOVIE_H
#define GRX_MOVIE_H

#include <stdint.h>

/* ------------------------------------------------------------------------ */
/* GS_IOP's movie commands (port/src/cdvd.c dispatches them)                */

/* One command from the EE's queue, as grx_snd_command takes them: cmd as
   queued, args its three int32 arguments (size 12). Returns 1 when the
   command was the movie side's and nothing else should handle it (0x300
   start, 0x310 stop), 0 otherwise. cdvd.c calls it for every command,
   before its own handling: the movie side also watches 0x101 (reads into
   a stream ring), 0x110 and 0x120 to know which disc sectors the file
   stream holds, and returns 0 for them. */
int grx_movie_command(uint32_t cmd, const void *args, uint32_t size);

/* From the game's thread whenever the EE reads the IOP status block
   (SndIopPoll), before grx_snd_poll: runs movie_server, which moves
   sectors from the file stream into the video stream's ring and the sound
   driver's VBS stream, and writes movieState into the status block. */
void grx_movie_poll(void);

/* 1 while the game's player is showing a movie (MovPlay, from the start of
   its audio stream to its end), port/src/movie/libmpeg.c. The frame loop
   then runs every late field, however many: the player shows a frame
   every two fields while the sound driver plays the audio by the clock,
   and a dropped field would leave the picture behind for the rest of the
   movie. */
int grx_movie_playing(void);

/* ------------------------------------------------------------------------ */
/* The .MOV container                                                       */

/* A .MOV is GS_IOP's interleave of a PSS stream (MPEG-2 program stream:
   pack headers and video PES packets, 0x4000-byte packs) and a VBS stream
   (PS-ADPCM, a header sector, stereo interleaved by 0x800 bytes). Its
   first sector says how, at +0x100 (int32 each):
     +0x100 PSS bytes        +0x104 VBS bytes
     +0x108 PSS sectors per chunk (intPSS)
     +0x10C VBS sectors per chunk (intVBS)
     +0x110 if +0x114 is 1..4096: PSS remainder << 16 | VBS remainder (s16);
            else the sectors of the first VBS chunk
     +0x114 chunks (time)    +0x118 last-chunk flags
   The file is: the first VBS chunk (65 sectors: this header sector, then
   64 sectors of audio), then PSS and VBS chunks alternately, each intPSS
   (intVBS) sectors plus a share of the remainder spread over the chunks
   (movie_server in GS_IOP.IRX). The first sector is also the VBS stream's
   own header: +0 channels, +4 sample rate, +8 interleave. */
typedef struct GrxMovSplit {
    int32_t pss_bytes, vbs_bytes;
    int32_t int_pss, int_vbs;
    int32_t pss_rem, vbs_rem;
    int32_t time, time_cnt;
    int32_t v1st;
    int32_t pss_add, vbs_add;
    int32_t vbs_2nd;
    int state;         /* 2: the first VBS chunk, 3: a PSS chunk, 4: a VBS chunk */
    int32_t left;      /* sectors left in the current chunk */
} GrxMovSplit;

/* Sets up the split from the file's first sector (2048 bytes) */
void grx_mov_split_init(GrxMovSplit *s, const uint8_t *hdr);
/* The stream of the next sector: 0 PSS (video), 1 VBS (audio). Advances. */
int grx_mov_split_next(GrxMovSplit *s);

/* ------------------------------------------------------------------------ */
/* The MPEG-2 video decoder (port/src/movie/mpeg2.c)                        */

/* MP@ML as the movies use it: 4:2:0, frame pictures, frame and field
   prediction and DCT, MPEG-2 quantisation, both VLC tables and scans.
   Not done: field pictures, dual prime, MPEG-1 streams, scalability. */
typedef struct GrxMpeg2 GrxMpeg2;

/* Where the decoder's bytes come from: up to max bytes into dst. Return
   the count (> 0); 0 for "nothing yet" (the decoder calls again, so the
   function should wait or yield for data itself); < 0 to abort. */
typedef int (*GrxMpeg2Read)(void *ctx, uint8_t *dst, int max);

typedef struct GrxMpeg2Frame {
    const uint8_t *y, *cb, *cr;
    int width, height;          /* coded size, multiples of 16 */
    int display_width, display_height;  /* horizontal_size, vertical_size */
    int y_stride, c_stride;
    int type;                   /* 1 I, 2 P, 3 B */
    int temporal_reference;
    int64_t pts, dts;           /* from the picture hook, -1 none */
} GrxMpeg2Frame;

GrxMpeg2 *grx_mpeg2_create(GrxMpeg2Read read, void *ctx);
void grx_mpeg2_destroy(GrxMpeg2 *m);

/* Called when a picture header has been read, before its slices: may set
   the picture's time stamps with grx_mpeg2_set_pts */
void grx_mpeg2_set_picture_hook(GrxMpeg2 *m, void (*hook)(void *ctx, int type), void *ctx);
void grx_mpeg2_set_pts(GrxMpeg2 *m, int64_t pts, int64_t dts);

/* Decodes until the next picture in display order is ready.
   1: *out is it (valid until the next call); 0: the sequence ended (a
   sequence end code, and every picture before it given out); -1: the
   read function aborted; -2: a stream the decoder can't decode. Errors
   inside a slice are concealed (the rest of the slice keeps the
   prediction, or grey), as the IPU's decoding would leave them. */
int grx_mpeg2_decode(GrxMpeg2 *m, GrxMpeg2Frame *out);

/* STREAM: forget the bitstream position, the bytes read ahead, the
   pending headers and the end (sceMpegReset). REFS: forget the reference
   pictures (sceMpegClearRefBuff). */
#define GRX_MPEG2_RESET_STREAM 1
#define GRX_MPEG2_RESET_REFS 2
void grx_mpeg2_reset(GrxMpeg2 *m, int what);

/* 1 while a decoded reference picture hasn't been given out */
int grx_mpeg2_has_pending(const GrxMpeg2 *m);
/* 1 once a sequence end code has been read and everything given out */
int grx_mpeg2_ended(const GrxMpeg2 *m);
/* Bytes taken from the read function but not decoded yet */
int grx_mpeg2_buffered(const GrxMpeg2 *m);
/* Corrupt slices so far (diagnostics) */
int grx_mpeg2_errors(const GrxMpeg2 *m);

/* The IPU's colour conversion (CSC, RGB32 with the thresholds at 0):
   RGBA bytes, alpha 0x80, chroma sampled per 2x2 as the IPU does.
   _mb: the IPU's layout, 16x16 macroblocks of 1024 bytes, column by
   column (macroblock (x, y) at (x * mb_rows + y) * 1024), at most max_mb
   of them, as sceMpegGetPicture leaves them for the movie's image tags.
   _linear: plain rows, for the tests. */
void grx_mpeg2_csc_mb(const GrxMpeg2Frame *f, uint8_t *dst, int max_mb);
void grx_mpeg2_csc_linear(const GrxMpeg2Frame *f, uint8_t *dst, int pitch);

#endif
