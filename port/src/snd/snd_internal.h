#ifndef GRX_SND_INTERNAL_H
#define GRX_SND_INTERNAL_H

/* Shared state of port/src/snd/ (GS_IOP.IRX's sound side). Names follow
   the IRX's symbols (port/docs/sound.md). */

#include <stdint.h>
#include "grx_spu2.h"

#define SND_BANKS        16
#define SND_SE_VOICES    24          /* core 1, voices 0-23 */
#define SND_STREAMS      2
#define SND_TRACKS       8           /* channels of a VBS stream */
#define SND_SECTOR       2048
#define SND_FIFO_SECTORS 256         /* music read-ahead per stream (512 KB) */
#define SND_VAB_HEADER   2048        /* vab_header[]: one bank's entry table */
#define SND_BANK_MAX     0x40000     /* the most 0x20A moves into sound memory */

/* Fake IOP addresses written into the status block. Nothing reads them
   back; they only have to be nonzero where the EE waits for that. */
#define SND_IOP_VAB_HEADER 0x001E0000u   /* +0x28 */
#define SND_IOP_PCM_BUF    0x001E8000u   /* +0x50 ("ready") */

/* Status block offsets the driver writes (SndIopStatus, include/snd.h) */
#define ST_MONO       0x1E
#define ST_VAB_HEADER 0x28
#define ST_BANK_STATE 0x30
#define ST_STRM_INFO  0x40
#define ST_PCM_BUF    0x50
#define ST_PCM_POS    0x54

/* One music stream: the IRX's vagst[] entry (0xA8 bytes there; the
   offsets in the comments) plus the port's read-ahead */
typedef struct SndVbs {
    int state;            /* +0x00: 0 idle, 1 prepared, 2 header read, 3-4 buffering,
                             5 playing, 10 ending, 11 ended */
    int flags;            /* +0x01: bit 1 play requested */
    int ch;               /* +0x02: tracks (interleaved channels) */
    int slot;             /* +0x08: stream ring slot on the IOP (index + 2) */
    int index;            /* +0x0C */
    uint32_t lsn;         /* +0x10: first sector of the track */
    uint32_t sectors;     /* +0x14: the track's sectors (GRX.IDX) */
    int first_voice;      /* +0x18: core 0 voice of track 0 (index * 8) */
    uint16_t pitch;       /* +0x1C: rate * 4096 / 48000 */
    uint32_t loop_file;   /* +0x20: loop start, byte in the file */
    uint32_t loop_pc;     /* +0x24: loop start, byte in each channel */
    uint32_t end_file;    /* +0x28: end, byte in the file */
    uint32_t end_pc;      /* +0x2C: end, byte in each channel */
    uint32_t rpos;        /* +0x30: next byte of each channel to move */
    uint32_t wpos;        /* +0x34: next byte of each channel's SPU buffer */
    uint32_t data_ofs;    /* +0x38: where the sound starts in the first sector */
    uint32_t bufsize;     /* +0x3C: SPU buffer per channel */
    int32_t length;       /* +0x40: play length in frames, 0 = until stopped */
    int32_t vol;          /* +0x44: 0..10000 */
    uint32_t spu[SND_TRACKS];   /* +0x48: SPU buffer of each channel */
    int32_t voll[SND_TRACKS];   /* +0x68 */
    int32_t volr[SND_TRACKS];   /* +0x88 */

    /* read-ahead: the track's sectors in the order the IOP's ring gets
       them (all of them, then the loop region over and over) */
    uint8_t fifo[SND_FIFO_SECTORS][SND_SECTOR];
    int head, count;
    int from_disc;        /* 0: fed by grx_snd_strm_feed */
    uint32_t next;        /* next sector of the file to read */
    int looping;          /* next is in the loop region */
    uint32_t loop_sec, end_sec;  /* the loop region, once the header is read */
    int have_region;
    int exhausted;        /* nothing more will come */
    unsigned gen;         /* changes when the stream is restarted */
} SndVbs;

typedef struct SndState {
    GrxSpu2 *spu;
    int sdinit;                       /* sdinit_flag */
    int8_t pause;                     /* pause_flag */
    uint32_t vab_address[SND_BANKS];  /* SPU address of each bank's samples */
    uint8_t vab_header[SND_BANKS][SND_VAB_HEADER];
    uint32_t streambuf[4], streambuflen[4];
    struct { int32_t cur_l, cur_r, tgt_l, tgt_r; } slot_vol[SND_SE_VOICES];
    uint32_t rev_switch0, rev_switch1;
    uint16_t kpt_pitch[2][SND_SE_VOICES];
    GrxSpu2EffectAttr core1attr;
    int pcm_monitor;

    /* the driver's status fields, written to the block by snd_publish */
    int8_t bank_state[SND_BANKS];
    int8_t mono;

    SndVbs vbs[SND_STREAMS];
    int tick;                         /* frames until the next server tick */
} SndState;

extern SndState grx_snd_state;
#define snd grx_snd_state

/* vbs.c */
void grx_snd_vbs_reset(void);
void grx_snd_vbs_prepare(int idx, uint32_t lsn, uint32_t sectors);
void grx_snd_vbs_play(int idx);
void grx_snd_vbs_stop(int idx);
void grx_snd_vbs_setvol(SndVbs *v);
void grx_snd_vbs_server(int idx);
void grx_snd_vbs_fill(int idx);     /* game thread, takes the lock itself */
int grx_snd_vbs_feed(int idx, const void *sectors, int n);

/* snd.c */
void grx_snd_lock(void);
void grx_snd_unlock(void);

#endif
