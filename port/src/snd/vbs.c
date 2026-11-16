/*
 * GS_IOP.IRX's music streams ("VBS": interleaved PS-ADPCM channels, one
 * 2048-byte sector per channel in turn), rewritten: the vagst[] state
 * machine in spu2_server, trans_vbs/trans_vbs0 (stream data into looping
 * SPU buffers, kept ahead of the voices' play position), keyon_vbs,
 * setvol_vbs, keyoff_vbs, stop_vbs. port/docs/sound.md has the format.
 *
 * The IOP reads the track into a stream ring (slot index + 2) with its
 * own disc driver and, once the read has finished, asks for the loop
 * region again. The port keeps a read-ahead of the same sequence of
 * sectors (the whole track, then its loop region over and over), filled
 * from the disc image on the game's thread (grx_snd_poll) and drained on
 * the audio thread.
 */

#include <string.h>

#include "grx_snd.h"
#include "grx_disc.h"
#include "grx_platform.h"
#include "snd_internal.h"

static uint32_t rd32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

static int16_t rd16s(const uint8_t *p) {
    return (int16_t)(uint16_t)(p[0] | p[1] << 8);
}

/* ------------------------------------------------------------------------ */
/* The read-ahead                                                           */

static void fifo_clear(SndVbs *v) {
    v->head = 0;
    v->count = 0;
}

static void fifo_pop(SndVbs *v) {
    if (v->count > 0) {
        v->head = (v->head + 1) % SND_FIFO_SECTORS;
        v->count--;
    }
}

static uint8_t *fifo_at(SndVbs *v, int i) {
    return v->fifo[(v->head + i) % SND_FIFO_SECTORS];
}

static int fifo_push(SndVbs *v, const uint8_t *sectors, int n) {
    int i;

    if (n > SND_FIFO_SECTORS - v->count) {
        n = SND_FIFO_SECTORS - v->count;
    }
    for (i = 0; i < n; i++) {
        memcpy(v->fifo[(v->head + v->count) % SND_FIFO_SECTORS], sectors + i * SND_SECTOR, SND_SECTOR);
        v->count++;
    }
    return n;
}

/* The loop region in file sectors, from the track's first sector */
static void read_region(SndVbs *v, const uint8_t *h) {
    int ch = h[0] & 0x7F;
    uint32_t loop_file = rd32(h + 0x10);
    uint32_t end_file;

    if (ch < 1) {
        ch = 1;
    } else if (ch > SND_TRACKS) {
        ch = SND_TRACKS;
    }
    end_file = rd32(h + 0x18) + (uint32_t)(ch - 1) * SND_SECTOR;
    v->loop_sec = loop_file >> 11;
    v->end_sec = (end_file + SND_SECTOR - 1) >> 11;
    v->have_region = 1;
}

void grx_snd_vbs_fill(int idx) {
    static uint8_t buf[64 * SND_SECTOR];
    SndVbs *v = &snd.vbs[idx];
    int rounds;

    for (rounds = 0; rounds < 8; rounds++) {
        unsigned gen;
        uint32_t lsn, first;
        int run, space, got;
        GrxDisc *d;

        grx_snd_lock();
        if (snd.spu == NULL || !v->from_disc || v->exhausted || v->state <= 0 || v->state >= 10) {
            grx_snd_unlock();
            return;
        }
        space = SND_FIFO_SECTORS - v->count;
        if (space < 32 && v->count > 0) {
            grx_snd_unlock();
            return;
        }
        /* where the sequence goes on: the whole track, then the loop region */
        if (!v->looping && v->next >= v->sectors) {
            if (!v->have_region) {
                grx_snd_unlock();
                return;
            }
            if (v->loop_sec >= v->end_sec) {
                v->exhausted = 1;
                grx_snd_unlock();
                return;
            }
            v->looping = 1;
            v->next = v->loop_sec;
        } else if (v->looping && v->next >= v->end_sec) {
            v->next = v->loop_sec;
        }
        first = v->next;
        run = (int)((v->looping ? v->end_sec : v->sectors) - first);
        if (run > space) {
            run = space;
        }
        if (run > 64) {
            run = 64;
        }
        gen = v->gen;
        lsn = v->lsn + first;
        grx_snd_unlock();

        d = grx_cdvd_disc();
        got = grx_disc_read_sectors(d, lsn, (uint32_t)run, buf);
        if (got < run) {
            memset(buf + (got > 0 ? got : 0) * SND_SECTOR, 0, (size_t)(run - (got > 0 ? got : 0)) * SND_SECTOR);
        }

        grx_snd_lock();
        if (v->gen == gen) {
            if (first == 0 && !v->looping) {
                read_region(v, buf);
            }
            fifo_push(v, buf, run);
            v->next = first + (uint32_t)run;
        }
        grx_snd_unlock();
    }
}

int grx_snd_vbs_feed(int idx, const void *sectors, int n) {
    SndVbs *v = &snd.vbs[idx];

    if (v->from_disc || v->state <= 0 || n <= 0) {
        return 0;
    }
    return fifo_push(v, sectors, n);
}

/* ------------------------------------------------------------------------ */
/* The voices                                                               */

static void sd_set_param(uint32_t e, uint16_t v) { grx_spu2_set_param(snd.spu, e, v); }

void grx_snd_vbs_setvol(SndVbs *v) {
    int t;

    for (t = 0; t < v->ch; t++) {
        uint32_t e = (uint32_t)GRX_SD_VOICE(v->first_voice + t);
        int32_t l = v->voll[t] * v->vol / 10000;
        int32_t r = v->volr[t] * v->vol / 10000;
        if (snd.mono) {
            l = r = (l + r) >> 1;
        }
        sd_set_param(GRX_SD_VP_VOLL | e, (uint16_t)l);
        sd_set_param(GRX_SD_VP_VOLR | e, (uint16_t)r);
    }
}

static void keyon_vbs(SndVbs *v) {
    uint32_t mask = 0;
    int t;

    grx_snd_vbs_setvol(v);
    for (t = 0; t < v->ch; t++) {
        int voice = v->first_voice + t;
        uint32_t e = (uint32_t)GRX_SD_VOICE(voice);
        sd_set_param(GRX_SD_VP_PITCH | e, v->pitch);
        grx_spu2_set_addr(snd.spu, GRX_SD_VA_SSA | e, v->spu[t]);
        sd_set_param(GRX_SD_VP_ADSR1 | e, 0x0FFF);
        sd_set_param(GRX_SD_VP_ADSR2 | e, 0xDFEA);
        mask |= 1u << voice;
    }
    grx_spu2_set_switch(snd.spu, GRX_SD_S_KON | 0, mask);
}

static void keyoff_vbs(SndVbs *v) {
    (void)v;
    grx_spu2_set_switch(snd.spu, GRX_SD_S_KOFF | 0, 0xFFu << (v->index * 8));
}

/* ------------------------------------------------------------------------ */
/* trans_vbs: n bytes of each channel from the stream into the SPU
   buffers, a sector of each channel per round. The block at a buffer's
   start gets the loop-start flags (6) and the one at its end the jump
   back (3), so the voices loop over their buffers. */

static void trans_vbs(SndVbs *v, int32_t n) {
    uint8_t tmp[SND_SECTOR];

    /* (a buffer smaller than a chunk can't be filled; the server doesn't
       start such a stream, this holds for one from a save state too) */
    if (n < 16 || v->bufsize < SND_SECTOR) {
        return;
    }
    n &= 0xFFFF0;
    v->rpos += (uint32_t)n;
    while (n > 0) {
        int32_t chunk = 0x800;
        int32_t k = n & 0x7F0;
        int t;

        if (n < chunk) {
            chunk = n;
        }
        if (k < chunk && k != 0) {
            chunk = k;
        }
        for (t = 0; t < v->ch; t++) {
            const uint8_t *src;
            uint32_t len = (uint32_t)chunk;
            uint32_t ofs = v->data_ofs;

            if (v->count <= 0) {
                break;
            }
            src = fifo_at(v, 0);
            if (ofs + len > SND_SECTOR) {
                len = SND_SECTOR - ofs;
            }
            memset(tmp, 0, sizeof(tmp));
            memcpy(tmp, src + ofs, len);
            if (v->wpos == 0) {
                tmp[1] = 6;
            }
            if (v->wpos + (uint32_t)chunk >= v->bufsize && v->bufsize - v->wpos >= 16) {
                tmp[v->bufsize - v->wpos - 15] = 3;
            }
            if (v->wpos + (uint32_t)chunk > v->bufsize) {
                uint32_t first = v->bufsize - v->wpos;
                grx_spu2_ram_write(snd.spu, v->spu[t] + v->wpos, tmp, first);
                tmp[first + 1] = 6;
                grx_spu2_ram_write(snd.spu, v->spu[t], tmp + first, (uint32_t)chunk - first);
            } else {
                grx_spu2_ram_write(snd.spu, v->spu[t] + v->wpos, tmp, (uint32_t)chunk);
            }
            fifo_pop(v);
        }
        v->data_ofs = 0;
        v->wpos += (uint32_t)chunk;
        if (v->wpos >= v->bufsize) {
            v->wpos -= v->bufsize;
        }
        n -= chunk;
    }
}

/* trans_vbs0: moves what the voices have played past, as far as there is
   stream data; at the end of the data the read position goes back to the
   loop start. -1 when the stream has run dry. */
static int trans_vbs0(SndVbs *v) {
    int32_t play = 0, free, avail, remain;

    if (v->state >= 5) {
        play = (int32_t)(grx_spu2_get_addr(snd.spu, GRX_SD_VA_NAX | GRX_SD_VOICE(v->first_voice)) & 0x7FFFFF)
               - (int32_t)v->spu[0];
    }
    free = play - (int32_t)v->wpos - 256;
    if (free < 0) {
        free += (int32_t)v->bufsize;
    }
    avail = (v->count / v->ch) * 0x800;
    free &= 0xFFF800;
    /* the IRX ends a stream whose data has run out with nothing queued;
       the port's read-ahead only runs out for good when there is no more
       (a stream fed from outside), not while the disc reads catch up */
    if (free > (int32_t)v->bufsize - 0x2000 && avail == 0 && v->exhausted && v->length <= 0) {
        return -1;
    }
    if (avail < free) {
        free = avail;
    }
    remain = (int32_t)v->end_pc - (int32_t)v->rpos;
    if (remain <= 0) {
        v->rpos = v->loop_pc;
        remain = 0x800;
    }
    if (remain >= 0x800) {
        remain &= 0xFFF800;
    }
    if (remain < free) {
        trans_vbs(v, remain);
    } else if (free >= 0x800) {
        trans_vbs(v, free);
    }
    return 0;
}

/* ------------------------------------------------------------------------ */
/* Commands and the server                                                  */

void grx_snd_vbs_reset(void) {
    int i;

    for (i = 0; i < SND_STREAMS; i++) {
        SndVbs *v = &snd.vbs[i];
        unsigned gen = v->gen + 1;
        memset(v, 0, sizeof(*v) - sizeof(v->fifo));
        fifo_clear(v);
        v->gen = gen;
    }
}

void grx_snd_vbs_stop(int idx) {
    SndVbs *v = &snd.vbs[idx];

    if (snd.spu != NULL) {
        keyoff_vbs(v);
    }
    /* the IRX closes the stream ring here (0x120) */
    fifo_clear(v);
    v->exhausted = 1;
    v->gen++;
    if (v->state != 0) {
        v->flags &= ~3;
        v->state = 0;
    }
}

/* 0x208 (stream, lsn, sectors): sectors 0 prepares a stream that is fed
   from outside (the movie player does that) */
void grx_snd_vbs_prepare(int idx, uint32_t lsn, uint32_t sectors) {
    SndVbs *v = &snd.vbs[idx];

    if (v->state > 0) {
        grx_snd_vbs_stop(idx);
    }
    v->slot = idx + 2;
    v->index = idx;
    v->lsn = lsn;
    v->sectors = sectors;
    v->first_voice = idx * 8;
    v->state = 1;
    v->vol = 10000;
    v->flags = 0;
    fifo_clear(v);
    v->from_disc = sectors > 0;
    v->next = 0;
    v->looping = 0;
    v->have_region = 0;
    v->exhausted = 0;
    v->gen++;
}

/* 0x218 (stream): starts once buffered */
void grx_snd_vbs_play(int idx) {
    SndVbs *v = &snd.vbs[idx];

    if (v->state > 0 && v->state < 5) {
        v->flags |= 2;
    }
}

/* State 1: the track's first sector, its header (sound.md) */
static void read_header(SndVbs *v, int idx) {
    uint8_t *h = fifo_at(v, 0);
    int t;
    int32_t rate;

    v->ch = h[0] & 0x7F;
    if (v->ch < 1) {
        v->ch = 1;
    } else if (v->ch > SND_TRACKS) {
        v->ch = SND_TRACKS;
    }
    v->loop_file = rd32(h + 0x10);
    v->data_ofs = rd32(h + 0x08) & 0x7F0;
    v->end_file = rd32(h + 0x18) + (uint32_t)(v->ch - 1) * SND_SECTOR;
    v->loop_pc = (v->loop_file - SND_SECTOR) / (uint32_t)v->ch;
    v->end_pc = (((rd32(h + 0x18) - SND_SECTOR) / (uint32_t)v->ch) & 0x7FFF800) + (v->end_file & 0x7F0);
    v->length = (int32_t)rd32(h + 0x20);
    rate = (int32_t)rd32(h + 0x04);
    v->pitch = (uint16_t)((int64_t)rate * 4096 / 48000);
    v->bufsize = (snd.streambuflen[idx] / (uint32_t)v->ch) & 0x7FFF00;
    if (v->ch == 1) {
        if (rd16s(h + 0x24) == 0) { h[0x24] = 0x00; h[0x25] = 0x38; }
        if (rd16s(h + 0x34) == 0) { h[0x34] = 0x00; h[0x35] = 0x38; }
    }
    if (v->ch == 2) {
        if (rd16s(h + 0x24) == 0) { h[0x24] = 0x00; h[0x25] = 0x3C; }
        h[0x26] = h[0x27] = 0;
        if (rd16s(h + 0x36) == 0) { h[0x36] = 0x00; h[0x37] = 0x3C; }
        h[0x34] = h[0x35] = 0;
    }
    for (t = 0; t < v->ch; t++) {
        v->voll[t] = rd16s(h + 0x24 + 2 * t);
        v->volr[t] = rd16s(h + 0x34 + 2 * t);
        v->spu[t] = snd.streambuf[idx] + (v->bufsize + 128) * (uint32_t)t;
    }
    v->wpos = 0;
    v->rpos = v->data_ofs;
    fifo_pop(v);
}

void grx_snd_vbs_server(int idx) {
    SndVbs *v = &snd.vbs[idx];

    switch (v->state) {
    case 1:
        if (v->count >= 8 || (v->from_disc && v->count > 0 && (uint32_t)v->count >= v->sectors)) {
            read_header(v, idx);
            v->state = 2;
            /* a buffer smaller than a chunk (the game sent no 0x204 for the
               stream, or a small one for many channels): the stream ends
               before it starts rather than overrun it */
            if (v->bufsize < SND_SECTOR) {
                static int warned;
                if (!warned) {
                    warned = 1;
                    grx_log("snd: stream %d's buffer (%u bytes, %d channels) is too small: not played", idx,
                            snd.streambuflen[idx], v->ch);
                }
                v->state = 10;
            }
        }
        break;
    case 2:
        if (v->ch * 4 < v->count) {
            trans_vbs(v, 0x2000 - (int32_t)v->data_ofs);
            v->state = 3;
        }
        break;
    case 3:
        v->state = 4;
        v->flags += 1;
        break;
    case 4:
        trans_vbs0(v);
        if (v->flags & 2) {
            keyon_vbs(v);
            v->state = 5;
            v->flags += 4;
        }
        break;
    case 5:
        if (trans_vbs0(v) < 0) {
            keyoff_vbs(v);
            v->state = 10;
        }
        break;
    case 10:
        /* the IRX closes the stream ring here (0x120) */
        v->state = 11;
        break;
    default:
        break;
    }
}
