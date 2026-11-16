/*
 * The game's disc reads, from the player's disc image (disc.c). See
 * port/docs/disc.md.
 *
 * The game doesn't read its files through Sony's libcdvd or fileio: those
 * are only initialised (LoadIopModules 45EEB8 calls sceCdInit and
 * sceCdMmode). Every file is read by sector through the stream side of
 * its own IOP module GS_IOP.IRX: the EE queues commands (0x101 read
 * sectors into a stream ring, 0x110 release them, 0x120 close), watches
 * the four rings in the IOP status block and copies ring sectors with
 * sceSifGetOtherData. The directory tree is parsed by the game itself
 * (SndFsMount 45F610 reads the volume descriptor at sector 16).
 *
 * So this file has two parts:
 *
 *  1. libcdvd, the calls the main executable links (all of src/main/46EB30.c
 *     to 46FD58.c can go), done synchronously over disc.c. Only sceCdInit
 *     and sceCdMmode are called by the game.
 *  2. GS_IOP's stream driver, IOP side, with virtual rings: a fetch reads
 *     straight from the disc. Plus the SIF calls that carry it:
 *     sceSifGetOtherData, and weak sceSifSetDma/sceSifDmaStat for the
 *     standalone test (the game links sif.c's, which call grx_gsiop_dma;
 *     sif.c's GS_IOP RPC server, called once at boot, calls grx_gsiop_boot).
 *
 * Every game-named function here is weak: while the game's own version is
 * still linked (the port build compiles all of src/), that one wins, so
 * this file can be built before the build leaves those sources out. The
 * build now leaves out libcdvd and the SIF libraries (build.py,
 * port/tools/fixups/sif.py), so these are the ones linked.
 *
 * No <windows.h> here: it would clash with the game's names.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_disc.h"
#include "grx_platform.h"
#include "grx_movie.h"
#include "grx_sif.h"
#include "grx_snd.h"
#include "grx_state_parts.h"

#define WEAK __attribute__((weak))

/* ------------------------------------------------------------------------ */
/* The EE/IOP shared block                                                  */

/* EE 0x01FF0000-0x02000000 on the PS2, where GS_IOP.IRX writes its header
   (+0) and the game keeps the status copy (+0x100) and the directory tree
   (+0x1000). In the port it is an ordinary object: it is in the exe's
   image, which is linked below 0x08000000, as the game's address masks
   need. The game's literal uses of it (LoadIopModules 45EEB8, SysBoot
   42FB00, SndIopStatusRecv 45F578) are rewritten to grx_iop_block() by
   port/tools/fixups/sif.py, so nothing needs memory at 0x01FF0000. */
static uint8_t iop_block[GRX_IOP_BLOCK_SIZE] __attribute__((aligned(64)));

void *grx_iop_block(void) {
    return iop_block;
}

uintptr_t grx_iop_addr(uint32_t ps2) {
    uint32_t top = ps2 >> 28;

    if ((ps2 & 0x0FFF0000u) == GRX_IOP_BLOCK_PS2 && (top == 0 || top == 2 || top == 3)) {
        return (uintptr_t)iop_block + (ps2 & 0xFFFFu);
    }
    return (uintptr_t)ps2;
}

/* EE addresses the game passes around are host pointers in the port (it
   is built for i686), except a PS2 address in the IOP block (or its
   uncached aliases), should one still arrive untranslated (hwmem.c keeps
   host memory off those addresses) */
static void *ee_ptr(uint32_t addr) {
    return (void *)grx_iop_addr(addr);
}

/* ------------------------------------------------------------------------ */
/* The disc                                                                 */

static GrxDisc *cd_disc;

void grx_cdvd_set_disc(GrxDisc *d) {
    cd_disc = d;
}

/* The image the player chose; GRX_DISC names it until the front end
   passes one in */
GrxDisc *grx_cdvd_disc(void) {
    if (cd_disc == NULL) {
        const char *path = getenv("GRX_DISC");
        if (path == NULL || *path == 0) {
            grx_fatal("no disc image: set GRX_DISC to the game's .ISO");
        }
        cd_disc = grx_disc_open(path);
        if (cd_disc == NULL) {
            grx_fatal("%s", grx_disc_error());
        }
    }
    return cd_disc;
}

/* ------------------------------------------------------------------------ */
/* 1. libcdvd (include/sdk/libcdvd.h, include/protos/main.h)                */

/* sceCdlFILE as in sdk/libcdvd.h (that header needs the game's include
   path, which the port's own sources don't have) */
typedef struct {
    uint32_t lsn;
    uint32_t size;
    char name[16];
    uint8_t date[8];
    uint32_t flag;
} CdlFILE;
_Static_assert(sizeof(CdlFILE) == 0x24, "sceCdlFILE");

/* sceCdRMode (libcdvd.h): trycount, spindlctrl, datapattern, pad */
typedef struct {
    uint8_t trycount;
    uint8_t spindlctrl;
    uint8_t datapattern;   /* 0 = 2048 bytes per sector, 1 = 2328, 2 = 2340 */
    uint8_t pad;
} CdRMode;

enum {
    SCECdErNO = 0x00,
    SCECdErREAD = 0x30,
    SCECdErEOM = 0x32,      /* past the end of the media */
};
enum { SCECdComplete = 2 };
enum { SCECdFuncRead = 1 };

static int cd_error;
static void (*cd_callback)(int32_t);

/* (mode) 0 init, 1 init without waiting for the drive, 5 exit: all done */
WEAK int32_t sceCdInit(int32_t mode) {
    (void)mode;
    cd_error = SCECdErNO;
    return 1;
}

/* (media) 1 CD, 2 DVD: the image is whatever it is */
WEAK int32_t sceCdMmode(int32_t media) {
    (void)media;
    return 1;
}

WEAK int32_t sceCdDiskReady(int32_t mode) {
    (void)mode;
    return SCECdComplete;
}

/* Commands finish before they return, so nothing is ever busy: 0 */
WEAK int32_t sceCdSync(int32_t mode) {
    (void)mode;
    return 0;
}

WEAK int32_t sceCdSyncS(int32_t mode) {
    (void)mode;
    return 0;
}

WEAK int32_t sceCdGetError(void) {
    return cd_error;
}

/* Bytes of the current read transferred so far: none is ever running */
WEAK int32_t sceCdGetReadPos(void) {
    return 0;
}

/* Swaps the end-of-command callback, returns the old one (the game's
   prototype has s32 for the function) */
WEAK int32_t sceCdCallback(int32_t func) {
    void (*old)(int32_t) = cd_callback;
    cd_callback = (void (*)(int32_t))(uintptr_t)func;
    return (int32_t)(uintptr_t)old;
}

/* The callback thread's setup and the power-off callback: nothing to do */
WEAK int32_t sceCdInitEeCB(int32_t prio, void *stack, int32_t size) {
    (void)prio; (void)stack; (void)size;
    return 0;
}

WEAK int32_t sceCdPOffCallback(int32_t func, int32_t arg) {
    (void)func; (void)arg;
    return 0;
}

/* (file, "\\DIR\\NAME;1") - 1 found, 0 not */
WEAK int32_t sceCdSearchFile(CdlFILE *fp, const char *name) {
    GrxDiscEntry e;
    const char *base;
    size_t n;

    if (grx_disc_find(grx_cdvd_disc(), name, &e) != 0 || e.is_dir) {
        return 0;
    }
    memset(fp, 0, sizeof(*fp));
    fp->lsn = e.lba;
    fp->size = e.size;
    /* libcdvd gives the record's name with its version */
    base = name + strlen(name);
    while (base > name && base[-1] != '\\' && base[-1] != '/' && base[-1] != ':') {
        base--;
    }
    n = strcspn(base, ";");
    snprintf(fp->name, sizeof(fp->name), "%.*s;1", (int)(n < 13 ? n : 13), base);
    fp->flag = e.flags;
    return 1;
}

/* (lsn, sectors, buf, mode) - 1 when the command was taken. Synchronous:
   the data is there when it returns, and the callback (if any) has run */
WEAK int32_t sceCdRead(uint32_t lsn, uint32_t sectors, void *buf, CdRMode *mode) {
    GrxDisc *d = grx_cdvd_disc();

    if (mode != NULL && mode->datapattern != 0) {
        /* 2328/2340-byte reads need the raw sectors: no 2048-byte image has them */
        grx_log("sceCdRead: data pattern %d not supported", mode->datapattern);
        cd_error = SCECdErREAD;
        return 0;
    }
    if (lsn + sectors > grx_disc_sector_count(d)) {
        cd_error = SCECdErEOM;
    } else if (grx_disc_read_sectors(d, lsn, sectors, buf) != (int)sectors) {
        cd_error = SCECdErREAD;
    } else {
        cd_error = SCECdErNO;
    }
    if (cd_callback != NULL) {
        cd_callback(SCECdFuncRead);
    }
    return cd_error == SCECdErNO;
}

/* ------------------------------------------------------------------------ */
/* 2. GS_IOP.IRX's stream driver                                            */

/* The IOP status block (SndIopStatus, include/snd.h) */
typedef struct {
    int16_t stat;          /* 0 idle, 1 reading; >= 2 (a BGM/movie stream) refuses 0x101 */
    int16_t secNum;        /* sectors in the ring the EE may fetch */
    int16_t getOfs;        /* ring sector the next fetch starts at */
    int16_t bufOfs;        /* ring sector the next read goes to */
    int16_t bufLen;        /* ring size in sectors */
    uint16_t dummy;
    int32_t readLen;       /* sectors read since the slot was opened */
    int32_t bufAdr;        /* IOP address of the ring */
    int32_t reqLen;        /* sectors requested and not yet read */
    int32_t lsn;           /* next sector to read */
    int32_t unk1C;
} IopStrmRing;

typedef struct {
    uint8_t pad0[0xC];
    int16_t batchSeq;      /* first command of the last batch taken */
    int16_t doneSeq;       /* last command processed */
    int32_t driveUnk10;
    uint8_t pad14[8];
    int16_t driveUnk1C;
    uint8_t pad1E[2];
    int32_t driveStat;     /* & 0x21: the movie player waits */
    uint8_t pad24[4];
    int32_t driveUnk28;
    int32_t movieState;
    int8_t bankState[16];  /* 2 = SE bank loaded */
    struct { int8_t state; uint8_t pad[3]; int32_t length; } strmInfo[2];
    int32_t ready;         /* SndDriverInit waits for nonzero */
    uint8_t pad54[4];
    uint8_t unk58[0x28];
    IopStrmRing strm[4];
} IopStatus;
_Static_assert(sizeof(IopStatus) == 0x100, "SndIopStatus");
_Static_assert(offsetof(IopStatus, strm) == 0x80, "SndIopStatus.strm");

/* IOP addresses handed to the game. Nothing but this file interprets
   them; the rings are far above the IOP's 2 MB so they can't be confused
   with anything real. */
#define GSIOP_STATUS_ADDR 0x001F0000u
#define GSIOP_CMDBUF_ADDR 0x001F0200u
#define GSIOP_CMDBUF_SIZE 0x200u
#define GSIOP_RING_ADDR   0x10000000u
#define GSIOP_RING_STRIDE 0x04000000u
/* Ring size in sectors (32 MB, s16 in the status block). The rings are
   virtual, so this costs nothing; it only has to hold the biggest file the
   game waits to have whole (an SE bank: SndSeBankLoadUpdate waits for
   secNum == its size, 128 sectors at most on this disc). */
#define GSIOP_RING_LEN    0x4000
#define GSIOP_SLOTS       4

/* A run of sectors requested by one 0x101, at ring position start */
typedef struct {
    uint32_t start;
    uint32_t lsn;
    uint32_t count;
} Seg;

typedef struct {
    uint32_t head;         /* ring position of the first unreleased sector */
    uint32_t end;          /* ring position after the last requested one */
    Seg seg[32];
    int nseg;
} Slot;

static IopStatus gs_status;
static Slot gs_slot[GSIOP_SLOTS];

void *grx_gsiop_status(void) {
    return &gs_status;
}

static uint32_t slot_avail(const Slot *s) {
    uint32_t n = s->end - s->head;
    return n > GSIOP_RING_LEN ? GSIOP_RING_LEN : n;
}

/* The status block's view of a slot */
static void slot_publish(int i) {
    Slot *s = &gs_slot[i];
    IopStrmRing *r = &gs_status.strm[i];
    uint32_t avail = slot_avail(s);
    uint32_t pos = s->head + avail;
    int k;

    r->bufAdr = (int32_t)(GSIOP_RING_ADDR + i * GSIOP_RING_STRIDE);
    r->bufLen = GSIOP_RING_LEN;
    r->secNum = (int16_t)avail;
    r->getOfs = (int16_t)(s->head % GSIOP_RING_LEN);
    r->bufOfs = (int16_t)(pos % GSIOP_RING_LEN);
    r->reqLen = (int32_t)(s->end - pos);
    r->stat = s->end != s->head;
    r->readLen = (int32_t)pos;      /* positions count from 0 at open */
    r->lsn = 0;
    for (k = 0; k < s->nseg; k++) {
        if (pos >= s->seg[k].start && pos < s->seg[k].start + s->seg[k].count) {
            r->lsn = (int32_t)(s->seg[k].lsn + (pos - s->seg[k].start));
        }
    }
}

static void slot_reset(int i) {
    memset(&gs_slot[i], 0, sizeof(gs_slot[i]));
    slot_publish(i);
}

/* Drops the runs that are wholly released */
static void slot_trim(Slot *s) {
    int k = 0;

    while (k < s->nseg && s->seg[k].start + s->seg[k].count <= s->head) {
        k++;
    }
    if (k > 0) {
        memmove(s->seg, s->seg + k, (s->nseg - k) * sizeof(Seg));
        s->nseg -= k;
    }
}

/* Disc sector of ring position pos, or -1 */
static int64_t slot_lsn(const Slot *s, uint32_t pos) {
    int k;

    for (k = 0; k < s->nseg; k++) {
        if (pos - s->seg[k].start < s->seg[k].count) {
            return s->seg[k].lsn + (pos - s->seg[k].start);
        }
    }
    return -1;
}

/* Copies ring sectors [pos, pos + n) to dst */
static void slot_copy(const Slot *s, uint32_t pos, uint32_t n, uint8_t *dst) {
    GrxDisc *d = grx_cdvd_disc();

    while (n > 0) {
        int64_t lsn = slot_lsn(s, pos);
        uint32_t run = 1;

        if (lsn < 0) {
            memset(dst, 0, GRX_DISC_SECTOR);
        } else {
            /* as many as are contiguous on the disc */
            while (run < n && slot_lsn(s, pos + run) == lsn + run) {
                run++;
            }
            if (grx_disc_read_sectors(d, (uint32_t)lsn, run, dst) != (int)run) {
                grx_fatal("disc image: read error at sector %lld", (long long)lsn);
            }
        }
        pos += run;
        n -= run;
        dst += run * GRX_DISC_SECTOR;
    }
}

int grx_gsiop_strm_take(int slot, int n, void *dst) {
    Slot *s;
    uint32_t avail;

    if (slot < 0 || slot >= GSIOP_SLOTS || n <= 0) {
        return 0;
    }
    s = &gs_slot[slot];
    avail = slot_avail(s);
    if ((uint32_t)n > avail) {
        n = (int)avail;
    }
    if (dst != NULL) {
        slot_copy(s, s->head, n, dst);
    }
    s->head += n;
    slot_trim(s);
    slot_publish(slot);
    return n;
}

void grx_gsiop_command(uint16_t seq, int16_t cmd, int32_t a0, int32_t a1, int32_t a2) {
    int i;

    /* The movie driver (port/src/movie/) sees every command first: it takes
       0x3xx, and watches 0x101/0x110/0x120 (returning 0, so they go on)
       to know which sectors the file stream holds */
    if (grx_movie_command((uint16_t)cmd, (int32_t[3]){a0, a1, a2}, 12)) {
        gs_status.doneSeq = (int16_t)seq;
        return;
    }
    switch (cmd) {
    case 0x100:                 /* SndFsMount: reset the stream side */
        for (i = 0; i < GSIOP_SLOTS; i++) {
            slot_reset(i);
        }
        break;
    case 0x101:                 /* (slot, lsn, sectors): read into the ring */
        if (a0 >= 0 && a0 < GSIOP_SLOTS && a2 > 0) {
            Slot *s = &gs_slot[a0];
            Seg *last = s->nseg > 0 ? &s->seg[s->nseg - 1] : NULL;
            if (last != NULL && last->start + last->count == s->end && last->lsn + last->count == (uint32_t)a1) {
                last->count += a2;
            } else if (s->nseg < (int)(sizeof(s->seg) / sizeof(s->seg[0]))) {
                s->seg[s->nseg].start = s->end;
                s->seg[s->nseg].lsn = (uint32_t)a1;
                s->seg[s->nseg].count = (uint32_t)a2;
                s->nseg++;
            } else {
                grx_fatal("GS_IOP: too many reads queued on stream %d", a0);
            }
            s->end += a2;
            slot_publish(a0);
        }
        break;
    case 0x110:                 /* (slot, sectors, wrapped): release ring sectors */
        grx_gsiop_strm_take(a0, a1, NULL);
        break;
    case 0x120:                 /* (slot): close */
        if (a0 >= 0 && a0 < GSIOP_SLOTS) {
            slot_reset(a0);
        }
        break;
    case 0x160:                 /* (slot): EE data uploaded into the ring (unused) */
        break;
    default:
        if (cmd >= 0x200 && cmd <= 0x2FF) {
            /* sound (port/src/snd/): 0x20A takes an SE bank from an open
               stream with grx_gsiop_strm_take and sets bankState itself */
            grx_snd_command((uint16_t)cmd, (int32_t[3]){a0, a1, a2}, 12);
        }
        /* 0x3xx: the movie driver, above */
        break;
    }
    gs_status.doneSeq = (int16_t)seq;
}

void grx_gsiop_boot(void *ee_header) {
    int32_t *h = ee_header;
    int i;

    memset(&gs_status, 0, sizeof(gs_status));
    for (i = 0; i < GSIOP_SLOTS; i++) {
        slot_reset(i);
    }
    gs_status.ready = 1;
    h[1] = (int32_t)GSIOP_STATUS_ADDR;   /* status block: IOP address, size */
    h[2] = (int32_t)sizeof(gs_status);
    h[3] = (int32_t)GSIOP_CMDBUF_ADDR;   /* command buffer */
    h[4] = 0;
    h[5] = 0;
    h[0] = (int32_t)0x5A5AA5A5;          /* last: LoadIopModules polls it */
}

/* One command batch (SndIopCmdFlush 45F438): 16-byte commands until cmd 0 */
int grx_gsiop_dma(uint32_t iop_addr, const void *src, uint32_t size) {
    const uint8_t *p = src;
    uint32_t off;
    int first = 1;

    if (iop_addr != GSIOP_CMDBUF_ADDR) {
        return 0;
    }
    if (size > GSIOP_CMDBUF_SIZE) {
        size = GSIOP_CMDBUF_SIZE;
    }
    for (off = 0; off + 16 <= size && off < 0x180; off += 16) {
        uint16_t seq;
        int16_t cmd;
        int32_t a[3];

        memcpy(&seq, p + off, 2);
        memcpy(&cmd, p + off + 2, 2);
        memcpy(a, p + off + 4, 12);
        if (cmd == 0) {
            break;
        }
        if (first) {
            gs_status.batchSeq = (int16_t)seq;
            first = 0;
        }
        grx_gsiop_command(seq, cmd, a[0], a[1], a[2]);
    }
    return 1;
}

int grx_gsiop_read(uint32_t iop_addr, void *dst, uint32_t size) {
    uint32_t ring;

    if (iop_addr >= GSIOP_STATUS_ADDR && iop_addr + size <= GSIOP_STATUS_ADDR + sizeof(gs_status)) {
        /* the movie and sound drivers top up their read-ahead and put
           their status fields in the block */
        grx_movie_poll();
        grx_snd_poll();
        memcpy(dst, (uint8_t *)&gs_status + (iop_addr - GSIOP_STATUS_ADDR), size);
        return 1;
    }
    if (iop_addr < GSIOP_RING_ADDR || iop_addr >= GSIOP_RING_ADDR + GSIOP_SLOTS * GSIOP_RING_STRIDE) {
        return 0;
    }
    ring = (iop_addr - GSIOP_RING_ADDR) / GSIOP_RING_STRIDE;
    {
        Slot *s = &gs_slot[ring];
        uint32_t off = (iop_addr - GSIOP_RING_ADDR) % GSIOP_RING_STRIDE;
        uint32_t sec = off / GRX_DISC_SECTOR, in = off % GRX_DISC_SECTOR;
        /* ring sector -> ring position: the one among the available sectors */
        uint32_t pos = s->head + (sec + GSIOP_RING_LEN - s->head % GSIOP_RING_LEN) % GSIOP_RING_LEN;
        uint8_t *out = dst;

        if (off + size > (uint32_t)GSIOP_RING_LEN * GRX_DISC_SECTOR) {
            grx_fatal("GS_IOP: fetch past the end of ring %u", ring);
        }
        if (in == 0 && size % GRX_DISC_SECTOR == 0) {
            slot_copy(s, pos, size / GRX_DISC_SECTOR, out);
        } else {
            /* not sector-aligned (the game never does this) */
            uint8_t sector[GRX_DISC_SECTOR];
            while (size > 0) {
                uint32_t n = GRX_DISC_SECTOR - in;
                if (n > size) {
                    n = size;
                }
                slot_copy(s, pos, 1, sector);
                memcpy(out, sector + in, n);
                out += n;
                size -= n;
                pos++;
                in = 0;
            }
        }
    }
    return 1;
}

/* ------------------------------------------------------------------------ */
/* The SIF calls that carry it                                              */

/* sceSifDmaData (sdk/sif.h) */
typedef struct {
    uint32_t data;         /* EE source */
    uint32_t addr;         /* IOP destination */
    uint32_t size;
    uint32_t mode;
} SifDmaData;

/* sceSifReceiveData (sdk/sif.h) */
typedef struct {
    uint32_t paddr;        /* the request packet; 0 = done for sceSifCheckStatRpc */
    uint32_t pid;
    int32_t tid;
    uint32_t mode;
    uint32_t src;          /* addresses: 32 bits, as the game's view of it on every target (port/docs/win64.md) */
    uint32_t dest;
    int32_t size;
} SifReceiveData;
_Static_assert(sizeof(SifReceiveData) == 0x1C, "sceSifReceiveData");

/* Kernel call 0x77. The game links sif.c's (strong) one, which passes
   GS_IOP's command buffer to grx_gsiop_dma and other IOP addresses to
   other targets; these weak ones only stand in for it where cdvd.c is
   linked without sif.c (port/tests/disc_test.c). */
WEAK uint32_t sceSifSetDma(SifDmaData *sdd, int32_t len) {
    static uint32_t id;
    int32_t i;

    for (i = 0; i < len; i++) {
        if (!grx_gsiop_dma(sdd[i].addr, ee_ptr(sdd[i].data), sdd[i].size)) {
            grx_log("sceSifSetDma: %u bytes to IOP %08X not handled", sdd[i].size, sdd[i].addr);
        }
    }
    if (++id == 0) {
        id = 1;
    }
    return id;
}

/* Transfers finish inside sceSifSetDma: negative = done (sif.c's wins too) */
WEAK int32_t sceSifDmaStat(uint32_t id) {
    (void)id;
    return -1;
}

/* (receive data, IOP source, EE destination, size, mode) - copies at once
   and leaves no packet pending, so the game's sceSifCheckStatRpc (48F1F0)
   reports it done without being replaced. 0 = taken. */
WEAK int32_t sceSifGetOtherData(SifReceiveData *rd, int32_t src, int32_t dst, int32_t size, uint32_t mode) {
    (void)mode;
    rd->paddr = 0;
    rd->pid = 0;
    rd->tid = -1;
    rd->src = (uint32_t)src;
    rd->dest = (uint32_t)(uintptr_t)ee_ptr((uint32_t)dst);
    rd->size = size;
    if (size <= 0) {
        return 0;
    }
    {
        /* into the IOP block: it must fit (hwmem.c keeps host memory off
           its PS2 address, so a buffer can't be mistaken for it) */
        uintptr_t d = (uintptr_t)ee_ptr((uint32_t)dst), blk = (uintptr_t)iop_block;
        if (d >= blk && d < blk + sizeof(iop_block) && d + (uint32_t)size > blk + sizeof(iop_block)) {
            grx_fatal("sceSifGetOtherData: %08X+0x%X runs past the IOP block (from IOP %08X)", (uint32_t)dst,
                      (uint32_t)size, (uint32_t)src);
        }
    }
    if (!grx_gsiop_read((uint32_t)src, ee_ptr((uint32_t)dst), (uint32_t)size)) {
        grx_fatal("sceSifGetOtherData: IOP address %08X not emulated", (uint32_t)src);
    }
    return 0;
}

/* ------------------------------------------------------------------------ */
/* Save states (grx_state_parts.h): the IOP block, GS_IOP's status block and
   its streams' rings. cd_disc is this run's. */
void grx_cdvd_state_io(GrxState *s) {
    GRX_STATE_VAR(s, iop_block);
    GRX_STATE_VAR(s, cd_error);
    GRX_STATE_VAR(s, cd_callback);
    GRX_STATE_VAR(s, gs_status);
    GRX_STATE_VAR(s, gs_slot);
}
