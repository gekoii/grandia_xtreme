/*
 * GS_IOP.IRX's sound driver, rewritten (port/include/grx_snd.h,
 * port/docs/sound.md): spu2_command (the 0x2xx commands), spu2_server
 * (run once a millisecond: SE volume ramps, the music streams) and
 * vmsound_init, on the software SPU2. Sound effects are here, the music
 * streams in vbs.c.
 *
 * Function and variable names follow the IRX's symbols. Where the
 * original has a quirk that changes what is heard (volume decreases
 * jump instead of ramping), the port keeps it.
 */

#include <string.h>
#include <time.h>

#include "grx_snd.h"
#include "grx_disc.h"
#include "grx_platform.h"
#include "snd_internal.h"
#include "grx_state_parts.h"

SndState grx_snd_state;

static void (*lock_fn)(void *);
static void (*unlock_fn)(void *);
static void *lock_ctx;
static int realtime;
static struct timespec last_poll;
static int have_last_poll;

void grx_snd_set_lock(void (*lock)(void *), void (*unlock)(void *), void *ctx) {
    lock_fn = lock;
    unlock_fn = unlock;
    lock_ctx = ctx;
}

void grx_snd_lock(void) {
    if (lock_fn != NULL) {
        lock_fn(lock_ctx);
    }
}

void grx_snd_unlock(void) {
    if (unlock_fn != NULL) {
        unlock_fn(lock_ctx);
    }
}

void grx_snd_set_realtime(int on) {
    realtime = on;
    have_last_poll = 0;
}

GrxSpu2 *grx_snd_spu2(void) {
    return snd.spu;
}

int grx_snd_init(void) {
    if (snd.spu == NULL) {
        snd.spu = grx_spu2_create();
        if (snd.spu == NULL) {
            return -1;
        }
        snd.pause = -1;       /* pause_flag's initial value */
        snd.tick = 0;
    }
    return 0;
}

void grx_snd_quit(void) {
    grx_snd_lock();
    grx_spu2_destroy(snd.spu);
    snd.spu = NULL;
    snd.sdinit = 0;
    grx_snd_unlock();
}

/* ------------------------------------------------------------------------ */
/* libsd, the calls the IRX makes                                           */

static void sd_set_param(uint32_t e, uint16_t v) { grx_spu2_set_param(snd.spu, e, v); }
static uint16_t sd_get_param(uint32_t e) { return grx_spu2_get_param(snd.spu, e); }
static void sd_set_switch(uint32_t e, uint32_t v) { grx_spu2_set_switch(snd.spu, e, v); }
static void sd_set_addr(uint32_t e, uint32_t a) { grx_spu2_set_addr(snd.spu, e, a); }

/* ------------------------------------------------------------------------ */
/* The status block                                                         */

static void put32(uint8_t *p, int32_t v) {
    memcpy(p, &v, 4);
}

/* The driver's fields into the IOP status block (cdvd.c's), as the IRX
   keeps them there. Lock held. */
static void snd_publish(void) {
    uint8_t *st = grx_gsiop_status();
    int i;

    if (st == NULL) {
        return;
    }
    if (snd.sdinit) {
        st[ST_MONO] = (uint8_t)snd.mono;
        put32(st + ST_VAB_HEADER, (int32_t)SND_IOP_VAB_HEADER);
        put32(st + ST_PCM_BUF, (int32_t)SND_IOP_PCM_BUF);
    }
    for (i = 0; i < SND_BANKS; i++) {
        st[ST_BANK_STATE + i] = (uint8_t)snd.bank_state[i];
    }
    for (i = 0; i < SND_STREAMS; i++) {
        SndVbs *v = &snd.vbs[i];
        uint8_t *p = st + ST_STRM_INFO + i * 8;
        uint16_t vol = (uint16_t)v->vol;
        p[0] = (uint8_t)v->state;
        p[1] = (uint8_t)v->ch;
        memcpy(p + 2, &vol, 2);
        put32(p + 4, v->length);
    }
}

/* ------------------------------------------------------------------------ */
/* vmsound_init (0x200)                                                     */

static void vmsound_init(void) {
    GrxSpu2EffectAttr a0;
    int i;

    grx_spu2_reset(snd.spu);
    grx_spu2_set_core_attr(snd.spu, GRX_SD_C_EFFECT_ENABLE | 1, 1);
    sd_set_param(GRX_SD_P_MVOLL | 0, 0);
    sd_set_param(GRX_SD_P_MVOLR | 0, 0);
    sd_set_param(GRX_SD_P_MVOLL | 1, 0);
    sd_set_param(GRX_SD_P_MVOLR | 1, 0);
    sd_set_param(GRX_SD_P_BVOLL | 0, 0);
    sd_set_param(GRX_SD_P_BVOLR | 0, 0);
    sd_set_param(GRX_SD_P_BVOLL | 1, 0);
    sd_set_param(GRX_SD_P_BVOLR | 1, 0);
    sd_set_param(GRX_SD_P_EVOLL | 0, 0);
    sd_set_param(GRX_SD_P_EVOLR | 0, 0);
    sd_set_param(GRX_SD_P_EVOLL | 1, 0);
    sd_set_param(GRX_SD_P_EVOLR | 1, 0);
    /* the bank entry tables: vab_header[16], 2 KB each */
    memset(snd.vab_header, 0, sizeof(snd.vab_header));
    /* core 0: voices dry only; core 1: voices dry and to the effect,
       sound data input and core 0 dry */
    sd_set_param(GRX_SD_P_MMIX | 0, 0x0C00);
    sd_set_param(GRX_SD_P_MMIX | 1, 0x0FCC);
    sd_set_switch(GRX_SD_S_VMIXL | 0, 0xFFFFFF);
    sd_set_switch(GRX_SD_S_VMIXR | 0, 0xFFFFFF);
    sd_set_switch(GRX_SD_S_VMIXL | 1, 0xFFFFFF);
    sd_set_switch(GRX_SD_S_VMIXR | 1, 0xFFFFFF);
    sd_set_switch(GRX_SD_S_VMIXEL | 0, snd.rev_switch0);
    sd_set_switch(GRX_SD_S_VMIXER | 0, snd.rev_switch0);
    sd_set_switch(GRX_SD_S_VMIXEL | 1, snd.rev_switch1);
    sd_set_switch(GRX_SD_S_VMIXER | 1, snd.rev_switch1);
    for (i = 0; i < SND_SE_VOICES; i++) {
        int c;
        for (c = 0; c < 2; c++) {
            uint32_t v = (uint32_t)GRX_SD_VOICE(i) | (uint32_t)c;
            sd_set_param(GRX_SD_VP_VOLL | v, 0);
            sd_set_param(GRX_SD_VP_VOLR | v, 0);
            sd_set_param(GRX_SD_VP_PITCH | v, 0x1000);
            sd_set_param(GRX_SD_VP_ADSR1 | v, 0x1EFF);
            sd_set_param(GRX_SD_VP_ADSR2 | v, 0xDFFF);
        }
    }
    /* core 0: effect off, work area ending at 0x1FFFFF; core 1: mode 7
       (echo, | 0x100 clear), ending at 0xFFFFF */
    sd_set_addr(GRX_SD_A_EEA | 0, 0x1FFFFF);
    memset(&a0, 0, sizeof(a0));
    grx_spu2_set_effect_attr(snd.spu, &a0);
    sd_set_addr(GRX_SD_A_EEA | 1, 0xFFFFF);
    snd.core1attr.core = 1;
    snd.core1attr.mode = 0x107;
    snd.core1attr.depth_l = 1;
    snd.core1attr.depth_r = 1;
    snd.core1attr.delay = 1;
    snd.core1attr.feedback = 1;
    grx_spu2_set_effect_attr(snd.spu, &snd.core1attr);
    /* the IRX starts an AutoDMA of silence into core 1's sound data input
       here (sceSdBlockTrans, 0x8000 bytes, looped): nothing to do */
    sd_set_param(GRX_SD_P_MVOLL | 0, 0x3FFF);
    sd_set_param(GRX_SD_P_MVOLR | 0, 0x3FFF);
    sd_set_param(GRX_SD_P_MVOLL | 1, 0x3FFF);
    sd_set_param(GRX_SD_P_MVOLR | 1, 0x3FFF);
    sd_set_param(GRX_SD_P_BVOLL | 1, 0x7FFF);
    sd_set_param(GRX_SD_P_BVOLR | 1, 0x7FFF);
    sd_set_param(GRX_SD_P_EVOLL | 1, 0x3FFF);
    sd_set_param(GRX_SD_P_EVOLR | 1, 0x3FFF);
    memset(snd.slot_vol, 0, sizeof(snd.slot_vol));
    snd.pause = 0;
    snd.sdinit = 1;
    snd.tick = 0;
}

/* ------------------------------------------------------------------------ */
/* Sound effects                                                            */

/* A bank entry (16 bytes, vab_header[bank] + slot * 16; sound.md) */
static const uint8_t *vab_entry(int bank, int slot) {
    return snd.vab_header[bank & (SND_BANKS - 1)] + (slot & 0x7F) * 16;
}

static uint16_t rd16(const uint8_t *p) {
    return (uint16_t)(p[0] | p[1] << 8);
}

static uint32_t rd32(const uint8_t *p) {
    return (uint32_t)p[0] | (uint32_t)p[1] << 8 | (uint32_t)p[2] << 16 | (uint32_t)p[3] << 24;
}

/* vms_setvol: the target of an SE voice's volume ramp, 3/4 of what was
   asked, mono as the mean */
static void vms_setvol(int voice, int32_t l, int32_t r) {
    if (voice < 0 || voice >= SND_SE_VOICES) {
        return;
    }
    if (snd.mono) {
        l = r = (l + r) >> 1;
    }
    snd.slot_vol[voice].tgt_l = (l >> 1) + (l >> 2);
    snd.slot_vol[voice].tgt_r = (r >> 1) + (r >> 2);
}

static void vms_keyon(int voice, int bank, int slot, int32_t l, int32_t r) {
    const uint8_t *e = vab_entry(bank, slot);
    uint32_t v = (uint32_t)GRX_SD_VOICE(voice) | 1;
    uint16_t adsr1, adsr2;

    if ((int16_t)rd16(e + 6) == 0) {
        return;
    }
    sd_set_switch(GRX_SD_S_KOFF | 1, 1u << voice);
    vms_setvol(voice, l, r);
    snd.slot_vol[voice].cur_l = snd.slot_vol[voice].tgt_l - 1;
    snd.slot_vol[voice].cur_r = snd.slot_vol[voice].tgt_r - 1;
    sd_set_param(GRX_SD_VP_PITCH | v, rd16(e + 4));
    sd_set_addr(GRX_SD_VA_SSA | v, snd.vab_address[bank & (SND_BANKS - 1)] + rd32(e));
    adsr1 = (uint16_t)((e[8] & 0x7F) << 8 | (e[9] & 0x0F) << 4 | (rd16(e + 12) & 0x0F));
    adsr2 = (uint16_t)(0xC020 | (e[10] & 0x7F) << 6 | (e[11] & 0x1F));
    sd_set_param(GRX_SD_VP_ADSR1 | v, adsr1);
    sd_set_param(GRX_SD_VP_ADSR2 | v, adsr2);
    snd.rev_switch1 = (snd.rev_switch1 & ~(1u << voice)) | (uint32_t)((rd16(e + 14) >> 15) & 1) << voice;
    sd_set_switch(GRX_SD_S_VMIXEL | 1, snd.rev_switch1);
    sd_set_switch(GRX_SD_S_VMIXER | 1, snd.rev_switch1);
    sd_set_switch(GRX_SD_S_KON | 1, 1u << voice);
}

/* set_pitch: the entry's pitch scaled by a 16.16 factor */
static void set_pitch(int voice, int bank, int slot, int32_t factor) {
    const uint8_t *e = vab_entry(bank, slot);

    if ((int16_t)rd16(e + 6) == 0) {
        return;
    }
    /* (in 64 bits: a 16-bit pitch times a 24-bit factor overflows 32) */
    sd_set_param(GRX_SD_VP_PITCH | GRX_SD_VOICE(voice) | 1, (uint16_t)(((int64_t)(int16_t)rd16(e + 4) * factor) >> 16));
}

/* set_vab: a bank's entry table into vab_header[bank], its samples into
   sound memory at vab_address[bank]. data is the bank file from its
   second sector on, size bytes - 1 (as the IRX passes it). */
static void set_vab(const uint8_t *data, uint32_t avail, int32_t size, int bank) {
    uint8_t *h = snd.vab_header[bank];
    uint32_t hsize = avail >= 4 ? rd32(data) : 0;
    uint32_t i, n;

    if (hsize > 0x7F0) {
        grx_log("snd: bank %d: entry table too big (0x%X)", bank, hsize);
        hsize = 0x7F0;
    }
    for (i = 0; i < hsize; i += 16) {
        uint32_t off = (i + 16 <= avail) ? rd32(data + i) - hsize : 0;
        if (i + 16 <= avail) {
            memcpy(h + i, data + i, 16);
        } else {
            memset(h + i, 0, 16);
        }
        memcpy(h + i, &off, 4);
    }
    /* the rest: only each entry's offset word is cleared, as in the IRX */
    for (; i < SND_VAB_HEADER; i += 16) {
        memset(h + i, 0, 4);
    }
    n = (uint32_t)((size - (int32_t)hsize + 63) & 0xFFFC0);
    if (hsize < avail) {
        uint32_t have = avail - hsize;
        grx_spu2_ram_write(snd.spu, snd.vab_address[bank], data + hsize, have < n ? have : n);
        if (have < n) {
            /* past the file the IRX moves whatever its ring held */
            static const uint8_t zero[64];
            uint32_t a;
            for (a = have; a < n; a += 64) {
                grx_spu2_ram_write(snd.spu, snd.vab_address[bank] + a, zero, n - a < 64 ? n - a : 64);
            }
        }
    }
}

/* ------------------------------------------------------------------------ */
/* spu2_server                                                              */

static void server_tick(void) {
    int i;

    if (!snd.sdinit) {
        return;
    }
    for (i = 0; i < SND_STREAMS; i++) {
        grx_snd_vbs_server(i);
    }
    /* SE volume ramps: up by (both targets) / 64 + 1 a tick; going down,
       the IRX's test lands on the target at once */
    for (i = 0; i < SND_SE_VOICES; i++) {
        int32_t *cl = &snd.slot_vol[i].cur_l, *cr = &snd.slot_vol[i].cur_r;
        int32_t tl = snd.slot_vol[i].tgt_l, tr = snd.slot_vol[i].tgt_r;
        uint32_t v = (uint32_t)GRX_SD_VOICE(i) | 1;

        if (*cl < tl) {
            *cl += ((tl + tr) >> 6) + 1;
            if (*cl > tl) {
                *cl = tl;
            }
            sd_set_param(GRX_SD_VP_VOLL | v, (uint16_t)*cl);
        }
        if (tl < *cl) {
            *cl -= ((*cl - tl) >> 6) + 1;
            if (tl < *cl) {
                *cl = tl;
            }
            sd_set_param(GRX_SD_VP_VOLL | v, (uint16_t)*cl);
        }
        if (*cr < tr) {
            *cr += ((tl + tr) >> 6) + 1;
            if (*cr > tr) {
                *cr = tr;
            }
            sd_set_param(GRX_SD_VP_VOLR | v, (uint16_t)*cr);
        }
        if (tr < *cr) {
            *cr -= ((*cr - tr) >> 6) + 1;
            if (tr < *cr) {
                *cr = tr;
            }
            sd_set_param(GRX_SD_VP_VOLR | v, (uint16_t)*cr);
        }
    }
}

/* The IOP's main loop runs spu2_server about once a millisecond */
#define SND_TICK_FRAMES (GRX_SPU2_RATE / 1000)

void grx_snd_render(int16_t *out, int frames) {
    if (snd.spu == NULL) {
        memset(out, 0, (size_t)frames * 4);
        return;
    }
    while (frames > 0) {
        int n;
        if (snd.tick <= 0) {
            server_tick();
            snd.tick = SND_TICK_FRAMES;
        }
        n = frames < snd.tick ? frames : snd.tick;
        grx_spu2_mix(snd.spu, out, n);
        out += 2 * n;
        frames -= n;
        snd.tick -= n;
    }
}

/* ------------------------------------------------------------------------ */
/* spu2_command                                                             */

static void pause_all(void) {
    int i;

    if (snd.pause == 0) {
        for (i = 0; i < SND_SE_VOICES; i++) {
            snd.kpt_pitch[0][i] = sd_get_param(GRX_SD_VP_PITCH | GRX_SD_VOICE(i) | 0);
            snd.kpt_pitch[1][i] = sd_get_param(GRX_SD_VP_PITCH | GRX_SD_VOICE(i) | 1);
        }
    }
    for (i = 0; i < SND_SE_VOICES; i++) {
        sd_set_param(GRX_SD_VP_PITCH | GRX_SD_VOICE(i) | 0, 0);
        sd_set_param(GRX_SD_VP_PITCH | GRX_SD_VOICE(i) | 1, 0);
    }
    sd_set_param(GRX_SD_P_BVOLL | 0, 0);
    sd_set_param(GRX_SD_P_BVOLR | 0, 0);
    snd.pause = 1;
}

static void resume_all(void) {
    int i;

    if (snd.pause == 1) {
        for (i = 0; i < SND_SE_VOICES; i++) {
            sd_set_param(GRX_SD_VP_PITCH | GRX_SD_VOICE(i) | 0, snd.kpt_pitch[0][i]);
            sd_set_param(GRX_SD_VP_PITCH | GRX_SD_VOICE(i) | 1, snd.kpt_pitch[1][i]);
        }
    }
    sd_set_param(GRX_SD_P_BVOLL | 0, 0x7FFF);
    sd_set_param(GRX_SD_P_BVOLR | 0, 0x7FFF);
    snd.pause = 0;
}

/* set_revptr (0x260): the reverb's mode, depth, delay and feedback */
static void set_revptr(int32_t mode, int32_t depth, int32_t dly) {
    if (mode >= 0) {
        snd.core1attr.mode = mode;
        if (mode & 0x100) {
            sd_set_param(GRX_SD_P_EVOLL | 1, 0);
            sd_set_param(GRX_SD_P_EVOLR | 1, 0);
        }
    }
    snd.core1attr.depth_l = (int16_t)(depth >> 16);
    snd.core1attr.depth_r = (int16_t)depth;
    snd.core1attr.delay = (dly >> 16) & 0x7F;
    snd.core1attr.feedback = dly & 0x7F;
    grx_spu2_set_effect_attr(snd.spu, &snd.core1attr);
    if (mode >= 0 && (mode & 0x100)) {
        sd_set_param(GRX_SD_P_EVOLL | 1, 0x3FFF);
        sd_set_param(GRX_SD_P_EVOLR | 1, 0x3FFF);
    }
}

/* 0x20A (slot, bank, sectors): the bank file, open on a stream ring with
   all its sectors read and the first (the EE's header) already fetched,
   goes to vab_header[bank] and sound memory. The ring is cdvd.c's. */
static void cmd_bank_load(int32_t slot, int32_t bank, int32_t sectors) {
    static uint8_t buf[SND_BANK_MAX];
    int32_t size;
    int got;

    if (bank < 0 || bank >= SND_BANKS) {
        return;
    }
    grx_snd_lock();
    snd.bank_state[bank] = 1;
    snd_publish();
    grx_snd_unlock();

    size = sectors * SND_SECTOR;
    if (size > SND_BANK_MAX) {
        size = SND_BANK_MAX;
    }
    /* the IRX reads from the ring's second sector and releases the
       sectors it covered, less the one the EE took */
    got = grx_gsiop_strm_take(slot, (size + SND_SECTOR - 1) / SND_SECTOR - 1, buf);

    grx_snd_lock();
    if (snd.spu != NULL) {
        set_vab(buf, (uint32_t)got * SND_SECTOR, size - 1, bank);
    }
    snd.bank_state[bank] = 2;
    snd_publish();
    grx_snd_unlock();
}

int grx_snd_command(uint32_t cmd, const void *args, uint32_t size) {
    int32_t a[3] = { 0, 0, 0 };

    if ((cmd & 0xF00) != 0x200) {
        return 0;
    }
    if (args != NULL) {
        memcpy(a, args, size < sizeof(a) ? size : sizeof(a));
    }
    if (grx_snd_init() != 0) {
        return 1;
    }
    switch (cmd & 0xFF) {
    case 0x0A:
        cmd_bank_load(a[0], a[1], a[2]);
        return 1;
    case 0x08:
        /* (stream, lsn, sectors): reads the disc, so outside the lock */
        if ((a[0] & 0xFF) < SND_STREAMS) {
            grx_snd_lock();
            grx_snd_vbs_prepare(a[0] & 0xFF, (uint32_t)a[1], (uint32_t)a[2]);
            snd_publish();
            grx_snd_unlock();
            grx_snd_vbs_fill(a[0] & 0xFF);
        }
        return 1;
    default:
        break;
    }

    grx_snd_lock();
    switch (cmd & 0xFF) {
    case 0x00:              /* init */
        vmsound_init();
        break;
    case 0x02:              /* (bank, SPU address) */
        if (a[0] >= 0 && a[0] < SND_BANKS) {
            snd.vab_address[a[0]] = (uint32_t)a[1];
        }
        break;
    case 0x04:              /* (stream, SPU address, bytes): music buffers */
        if (a[0] >= 0 && a[0] < 4) {
            snd.streambuf[a[0]] = (uint32_t)a[1];
            snd.streambuflen[a[0]] = (uint32_t)a[2];
        }
        break;
    case 0x0B:              /* (-, bank): bank released */
        if (a[1] >= 0 && a[1] < SND_BANKS) {
            snd.bank_state[a[1]] = 0;
        }
        break;
    case 0x10: {            /* (voice, bank << 16 | slot, l << 16 | r): key on */
        int bank = (a[1] >> 16) & 0x0F;
        if (snd.bank_state[bank] >= 2 && a[0] >= 0 && a[0] < SND_SE_VOICES) {
            vms_keyon(a[0], bank, a[1] & 0x7F, (a[2] >> 16) & 0x3FFF, a[2] & 0x3FFF);
        }
        break;
    }
    case 0x18:              /* (stream): play once ready */
        if (a[0] >= 0 && a[0] < SND_STREAMS) {
            grx_snd_vbs_play(a[0]);
        }
        break;
    case 0x20:              /* (voice): key off */
        if (a[0] >= 0 && a[0] < SND_SE_VOICES) {
            sd_set_switch(GRX_SD_S_KOFF | 1, 1u << a[0]);
        }
        break;
    case 0x28:              /* (stream): stop */
        if (a[0] >= 0 && a[0] < SND_STREAMS) {
            grx_snd_vbs_stop(a[0]);
        }
        break;
    case 0x30:              /* (voice, l, r): SE volume */
        if (a[0] >= 0) {
            vms_setvol(a[0], a[1], a[2]);
        }
        break;
    case 0x34:              /* (stream, volume 0..10000) */
        if (a[0] >= 0 && a[0] < SND_STREAMS) {
            snd.vbs[a[0]].vol = a[1];
            if (snd.vbs[a[0]].state == 5) {
                grx_snd_vbs_setvol(&snd.vbs[a[0]]);
            }
        }
        break;
    case 0x38:              /* (voice << 24 | pitch factor, bank << 16 | slot, l << 16 | r) */
        if (a[0] >= 0 && (a[0] >> 24) < SND_SE_VOICES) {
            int voice = a[0] >> 24;
            int bank = (a[1] >> 16) & 0x0F;
            set_pitch(voice, bank, a[1] & 0x7F, a[0] & 0xFFFFFF);
            vms_setvol(voice, (a[2] >> 16) & 0xFFFF, a[2] & 0xFFFF);
        }
        break;
    case 0x40:              /* pause */
        pause_all();
        break;
    case 0x41:              /* resume */
        resume_all();
        break;
    case 0x50:              /* (mono) */
        snd.mono = (int8_t)a[0];
        break;
    case 0x60:              /* (mode, depth L << 16 | R, delay << 16 | feedback): reverb */
        set_revptr(a[0], a[1], a[2]);
        break;
    case 0x80:              /* (stream, track, l << 16 | r): a track's volume */
        if (a[0] >= 0 && a[0] < SND_STREAMS && a[1] >= 0 && a[1] < SND_TRACKS) {
            snd.vbs[a[0]].voll[a[1]] = (a[2] >> 16) & 0x3FFF;
            snd.vbs[a[0]].volr[a[1]] = a[2] & 0x3FFF;
            grx_snd_vbs_setvol(&snd.vbs[a[0]]);
        }
        break;
    case 0xC0:              /* (on): capture of the output for the EE's level meter */
        snd.pcm_monitor = a[0] != 0;
        break;
    default:
        break;
    }
    snd_publish();
    grx_snd_unlock();
    return 1;
}

/* ------------------------------------------------------------------------ */

int grx_snd_strm_feed(int stream, const void *sectors, int n) {
    int r;

    if (stream < 0 || stream >= SND_STREAMS) {
        return 0;
    }
    grx_snd_lock();
    r = grx_snd_vbs_feed(stream, sectors, n);
    grx_snd_unlock();
    return r;
}

void grx_snd_poll(void) {
    int i;

    if (snd.spu == NULL) {
        return;
    }
    if (!realtime) {
        /* no device: keep the driver's time with the wall clock */
        struct timespec now;
        if (timespec_get(&now, TIME_UTC) == TIME_UTC) {
            if (have_last_poll) {
                double dt = (double)(now.tv_sec - last_poll.tv_sec) + (now.tv_nsec - last_poll.tv_nsec) * 1e-9;
                int frames = dt <= 0 ? 0 : dt > 0.25 ? GRX_SPU2_RATE / 4 : (int)(dt * GRX_SPU2_RATE);
                static int16_t scratch[2 * 1024];
                grx_snd_lock();
                while (frames > 0) {
                    int k = frames > 1024 ? 1024 : frames;
                    grx_snd_render(scratch, k);
                    frames -= k;
                }
                grx_snd_unlock();
            }
            last_poll = now;
            have_last_poll = 1;
        }
    }
    for (i = 0; i < SND_STREAMS; i++) {
        grx_snd_vbs_fill(i);
    }
    grx_snd_lock();
    snd_publish();
    grx_snd_unlock();
}

/* ------------------------------------------------------------------------ */
/* Save states (grx_state_parts.h): the driver's state and the SPU2's,
   under the lock the audio thread mixes under; the SPU2 object is this
   run's */
void grx_snd_state_io(GrxState *s) {
    GrxSpu2 *spu;
    grx_snd_lock();
    spu = snd.spu;
    GRX_STATE_VAR(s, snd);
    snd.spu = spu;
    if (spu != NULL) {
        grx_spu2_state_io(spu, s); /* (without one the part's size differs: not loaded) */
    }
    grx_snd_unlock();
}
