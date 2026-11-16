/*
 * Checks the disc reader (port/src/disc.c) and the game's disc drivers on
 * it (port/src/cdvd.c) against an extracted copy of the disc.
 *
 *   disc_test [image] [extracted dir]     (default: "disc/Grandia Xtreme.ISO" iso)
 *
 * Built and run by hand (port/docs/disc.md). Prints names, sizes,
 * sector numbers and hashes only, never the data itself.
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_disc.h"

/* cdvd.c's game-named functions (their prototypes in the game's headers) */
typedef struct {
    uint32_t lsn, size;
    char name[16];
    uint8_t date[8];
    uint32_t flag;
} CdlFILE;
int32_t sceCdInit(int32_t);
int32_t sceCdSearchFile(CdlFILE *, const char *);
int32_t sceCdRead(uint32_t, uint32_t, void *, void *);
int32_t sceCdSync(int32_t);
typedef struct { uint32_t data, addr, size, mode; } SifDmaData;
typedef struct { void *paddr; uint32_t pid; int32_t tid; uint32_t mode; void *src, *dest; int32_t size; } SifReceiveData;
uint32_t sceSifSetDma(SifDmaData *, int32_t);
int32_t sceSifGetOtherData(SifReceiveData *, int32_t, int32_t, int32_t, uint32_t);

/* What cdvd.c needs from the platform layer */
void grx_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
}

void grx_fatal(const char *fmt, ...) {
    va_list ap;
    fputs("disc_test: ", stderr);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(2);
}

static int failures;

#define CHECK(cond, ...) do { if (!(cond)) { failures++; printf("  FAIL: " __VA_ARGS__); printf("\n"); } } while (0)

static uint64_t fnv(const void *p, size_t n, uint64_t h) {
    const uint8_t *b = p;
    while (n--) {
        h = (h ^ *b++) * 0x100000001B3ull;
    }
    return h;
}
#define FNV0 0xCBF29CE484222325ull

static uint8_t *xmalloc(size_t n) {
    uint8_t *p = malloc(n ? n : 1);
    if (p == NULL) {
        grx_fatal("out of memory");
    }
    return p;
}

/* Hash of [offset, offset+len) of the extracted copy; -1 size if missing */
static int64_t ref_hash(const char *dir, const char *path, uint64_t offset, uint32_t len, uint64_t *h) {
    char full[1024];
    FILE *f;
    int64_t size;
    uint8_t *buf;
    size_t got;

    snprintf(full, sizeof(full), "%s/%s", dir, path);
    f = fopen(full, "rb");
    if (f == NULL) {
        return -1;
    }
    _fseeki64(f, 0, SEEK_END);
    size = _ftelli64(f);
    if (offset > (uint64_t)size) {
        offset = size;
    }
    if (len > size - offset) {
        len = (uint32_t)(size - offset);
    }
    buf = xmalloc(len);
    _fseeki64(f, offset, SEEK_SET);
    got = fread(buf, 1, len, f);
    fclose(f);
    *h = fnv(buf, got, FNV0);
    free(buf);
    return size;
}

static int list_cb(const GrxDiscEntry *e, void *user) {
    (void)user;
    printf("  %-14s %s %10u  lba %7u\n", e->name, e->is_dir ? "<dir>" : "     ", e->size, e->lba);
    return 0;
}

static void check_file(GrxDisc *d, const char *dir, const char *path) {
    GrxDiscEntry e;
    uint64_t h_disc, h_ref, h_sec;
    uint32_t len;
    int64_t size;
    uint8_t *buf;

    if (grx_disc_find(d, path, &e) != 0) {
        CHECK(0, "%s not found", path);
        return;
    }
    /* The first MB (or the whole file) */
    len = e.size < (1u << 20) ? e.size : (1u << 20);
    buf = xmalloc(len);
    CHECK(grx_disc_read_file(d, &e, 0, buf, len) == len, "%s: short read", path);
    h_disc = fnv(buf, len, FNV0);
    size = ref_hash(dir, path, 0, len, &h_ref);
    printf("  %-18s size %10u (copy %10lld)  first %7u bytes %016llx (copy %016llx)  %s\n",
           path, e.size, (long long)size, len, (unsigned long long)h_disc, (unsigned long long)h_ref,
           size == e.size && h_disc == h_ref ? "ok" : "MISMATCH");
    CHECK(size == e.size && h_disc == h_ref, "%s differs from the copy", path);
    free(buf);

    /* The first sector by LBA (zero-padded past the end of the file) */
    buf = xmalloc(GRX_DISC_SECTOR);
    CHECK(grx_disc_read_sectors(d, e.lba, 1, buf) == 1, "%s: sector read", path);
    len = e.size < GRX_DISC_SECTOR ? e.size : GRX_DISC_SECTOR;
    h_sec = fnv(buf, len, FNV0);
    ref_hash(dir, path, 0, len, &h_ref);
    CHECK(h_sec == h_ref, "%s: sector %u differs", path, e.lba);
    free(buf);

    /* An unaligned range across sectors */
    if (e.size > 3000) {
        uint64_t off = 1001;
        len = e.size - off < 70000 ? (uint32_t)(e.size - off) : 70000;
        buf = xmalloc(len);
        CHECK(grx_disc_read_file(d, &e, off, buf, len) == len, "%s: range read", path);
        h_disc = fnv(buf, len, FNV0);
        ref_hash(dir, path, off, len, &h_ref);
        CHECK(h_disc == h_ref, "%s: bytes %llu-%llu differ", path, (unsigned long long)off,
              (unsigned long long)(off + len));
        free(buf);
    }
}

/* ---- GS_IOP stream: the game's side of it, as FileReadUpdate (42EDE0) ---- */

typedef struct {
    int16_t stat, secNum, getOfs, bufOfs, bufLen;
    uint16_t dummy;
    int32_t readLen, bufAdr, reqLen, lsn, unk1C;
} Ring;

static int32_t iop_hdr[8];
static uint8_t iop_status[0x100];
static uint8_t cmd_buf[0x200];
static int ncmd;
static uint16_t cmd_seq = 1;

static void push(int16_t cmd, int32_t a0, int32_t a1, int32_t a2) {
    uint8_t *q = cmd_buf + ncmd * 16;
    memcpy(q, &cmd_seq, 2);
    memcpy(q + 2, &cmd, 2);
    memcpy(q + 4, &a0, 4);
    memcpy(q + 8, &a1, 4);
    memcpy(q + 12, &a2, 4);
    memset(q + 16, 0, 16);
    ncmd++;
    cmd_seq++;
}

/* SndIopPoll: flush the commands, then fetch the status block */
static void poll_iop(void) {
    SifReceiveData rd;
    SifDmaData dma;

    if (ncmd > 0) {
        dma.data = (uint32_t)(uintptr_t)cmd_buf;
        dma.addr = (uint32_t)iop_hdr[3];
        dma.size = 0x200;
        dma.mode = 0;
        sceSifSetDma(&dma, 1);
        ncmd = 0;
    }
    sceSifGetOtherData(&rd, iop_hdr[1], (int32_t)(uintptr_t)iop_status, iop_hdr[2], 1);
}

static Ring *ring(int slot) {
    return (Ring *)(iop_status + 0x80 + 0x20 * slot);
}

/* Reads `sectors` sectors from lsn through stream slot, fetching at most
   chunk at a time (split at the ring's wrap, as SndStrmFetchStart does) */
static uint64_t stream_read(int slot, uint32_t lsn, uint32_t sectors, uint32_t chunk, uint8_t *dst, int *wraps) {
    SifReceiveData rd;
    uint32_t done = 0;
    uint64_t h = FNV0;

    push(0x101, slot, lsn, sectors);
    poll_iop();
    while (done < sectors) {
        Ring *r = ring(slot);
        uint32_t n = r->secNum, first;
        if (n == 0) {
            CHECK(0, "stream %d: nothing ready after %u sectors", slot, done);
            break;
        }
        if (n > chunk) {
            n = chunk;
        }
        first = r->bufLen - r->getOfs;
        if (first > n) {
            first = n;
        } else if (first < n) {
            (*wraps)++;
        }
        sceSifGetOtherData(&rd, r->bufAdr + r->getOfs * GRX_DISC_SECTOR, (int32_t)(uintptr_t)dst, first * GRX_DISC_SECTOR, 1);
        if (n > first) {
            sceSifGetOtherData(&rd, r->bufAdr, (int32_t)(uintptr_t)(dst + first * GRX_DISC_SECTOR),
                               (n - first) * GRX_DISC_SECTOR, 1);
        }
        CHECK(rd.paddr == NULL, "receive left pending");
        h = fnv(dst, (size_t)n * GRX_DISC_SECTOR, h);
        push(0x110, slot, n, 0);
        poll_iop();
        done += n;
    }
    push(0x120, slot, 0, 0);
    poll_iop();
    CHECK(ring(slot)->secNum == 0 && ring(slot)->stat == 0, "stream %d not idle after close", slot);
    return h;
}

static void check_stream(GrxDisc *d, const char *dir, const char *path, uint32_t max_sectors, uint32_t chunk) {
    GrxDiscEntry e;
    uint32_t sectors;
    uint64_t h, h_ref;
    uint8_t *buf = xmalloc((size_t)chunk * GRX_DISC_SECTOR);
    int wraps = 0;

    if (grx_disc_find(d, path, &e) != 0) {
        CHECK(0, "%s not found", path);
        free(buf);
        return;
    }
    sectors = (e.size + GRX_DISC_SECTOR - 1) / GRX_DISC_SECTOR;
    if (sectors > max_sectors) {
        sectors = max_sectors;
    }
    h = stream_read(1, e.lba, sectors, chunk, buf, &wraps);
    /* the copy, zero-padded to whole sectors like the ring */
    {
        uint64_t bytes = (uint64_t)sectors * GRX_DISC_SECTOR;
        uint32_t len = bytes > e.size ? e.size : (uint32_t)bytes;
        uint8_t *ref = xmalloc((size_t)bytes);
        char full[1024];
        FILE *f;
        snprintf(full, sizeof(full), "%s/%s", dir, path);
        f = fopen(full, "rb");
        memset(ref, 0, (size_t)bytes);
        if (f != NULL) {
            if (fread(ref, 1, len, f) != len) {
                CHECK(0, "%s: copy short", path);
            }
            fclose(f);
        }
        h_ref = fnv(ref, (size_t)bytes, FNV0);
        free(ref);
    }
    printf("  stream %-18s %6u sectors in chunks of %4u, %d wrap(s): %016llx (copy %016llx)  %s\n",
           path, sectors, chunk, wraps, (unsigned long long)h, (unsigned long long)h_ref,
           h == h_ref ? "ok" : "MISMATCH");
    CHECK(h == h_ref, "%s: stream differs", path);
    free(buf);
}

int main(int argc, char **argv) {
    const char *image = argc > 1 ? argv[1] : "disc/Grandia Xtreme.ISO";
    const char *dir = argc > 2 ? argv[2] : "iso";
    static const char *files[] = {
        "SYSTEM.CNF", "SLUS_204.17", "FIELD.BIN", "BATTLE.BIN", "GS_IOP.IRX",
        "SYS/GBX.PAC", "SYS/GRXSYS.PAC", "SYS/PARTY.PAC", "SYS/FACE.DAT",
        "MUSIC/GRX.IDX", "MUSIC/6207.SE", "BTL_DATA/E001S1.DAT", "FIELD/A086.MDT",
        "MOVIE/GRM00.MOV",
    };
    static const char *aliases[] = {
        "cdrom0:\\SYS\\GBX.PAC;1", "sys/gbx.pac", "/SYS/GBX.PAC", "\\Sys\\Gbx.Pac;1", "SYS//GBX.PAC",
    };
    GrxDisc *d;
    GrxDiscEntry e, ref;
    char vol[33];
    int i, n, ss, off;

    d = grx_disc_open(image);
    if (d == NULL) {
        printf("open: %s\n", grx_disc_error());
        return 1;
    }
    grx_disc_volume_id(d, vol);
    grx_disc_layout(d, &ss, &off);
    printf("image: %u sectors of %d bytes (data at +%d), volume \"%s\"\n",
           grx_disc_sector_count(d), ss, off, vol);

    /* The .MDS beside the image, if there is one, names the same sectors */
    {
        char mds[1024];
        size_t len = strlen(image);
        GrxDisc *m;
        if (len > 4 && len < sizeof(mds) && image[len - 4] == '.') {
            memcpy(mds, image, len - 3);
            strcpy(mds + len - 3, "MDS");
            m = grx_disc_open(mds);
            if (m != NULL) {
                uint8_t a[GRX_DISC_SECTOR], b[GRX_DISC_SECTOR];
                grx_disc_read_sectors(m, 16, 1, a);
                grx_disc_read_sectors(d, 16, 1, b);
                printf("descriptor %s: %u sectors, %s\n", mds, grx_disc_sector_count(m),
                       grx_disc_sector_count(m) == grx_disc_sector_count(d) && memcmp(a, b, sizeof(a)) == 0
                       ? "same volume" : "DIFFERENT");
                CHECK(grx_disc_sector_count(m) == grx_disc_sector_count(d) && memcmp(a, b, sizeof(a)) == 0,
                      "the .MDS opens a different volume");
                grx_disc_close(m);
            } else {
                printf("descriptor %s: %s\n", mds, grx_disc_error());
            }
        }
    }
    {
        GrxDisc *bad = grx_disc_open("port/tests/disc_test.c");
        printf("a text file as an image: %s\n", bad == NULL ? grx_disc_error() : "OPENED");
        CHECK(bad == NULL, "a text file opened as an image");
        grx_disc_close(bad);
    }

    printf("root:\n");
    n = grx_disc_list(d, "/", list_cb, NULL);
    printf("  (%d entries)\n", n);
    CHECK(n > 0, "root listing");
    printf("SYS/:\n");
    n = grx_disc_list(d, "SYS", list_cb, NULL);
    printf("  (%d entries)\n", n);
    CHECK(n > 0, "SYS listing");

    printf("files against %s/:\n", dir);
    for (i = 0; i < (int)(sizeof(files) / sizeof(files[0])); i++) {
        check_file(d, dir, files[i]);
    }

    printf("lookups:\n");
    CHECK(grx_disc_find(d, "SYS/GBX.PAC", &ref) == 0, "SYS/GBX.PAC");
    for (i = 0; i < (int)(sizeof(aliases) / sizeof(aliases[0])); i++) {
        int r = grx_disc_find(d, aliases[i], &e);
        printf("  %-24s %s\n", aliases[i], r == 0 && e.lba == ref.lba ? "same file" : "FAIL");
        CHECK(r == 0 && e.lba == ref.lba, "%s", aliases[i]);
    }
    CHECK(grx_disc_find(d, "SYS/NO_SUCH.BIN", &e) != 0, "missing file found");
    CHECK(grx_disc_find(d, "SYS/GBX.PAC/X", &e) != 0, "file used as a directory");
    CHECK(grx_disc_find(d, "SYS", &e) == 0 && e.is_dir, "SYS is a directory");
    CHECK(grx_disc_list(d, "SYS/GBX.PAC", list_cb, NULL) < 0, "listing a file");
    printf("  missing files and file-as-directory: %s\n", failures ? "see failures" : "ok");

    /* Past the end of the image: zeros, not an error */
    {
        uint8_t sec[GRX_DISC_SECTOR];
        uint32_t k, nz = 0;
        memset(sec, 0xAA, sizeof(sec));
        CHECK(grx_disc_read_sectors(d, grx_disc_sector_count(d) + 10, 1, sec) == 1, "read past the end");
        for (k = 0; k < sizeof(sec); k++) {
            nz += sec[k] != 0;
        }
        CHECK(nz == 0, "past the end is not zero");
    }

    printf("libcdvd (cdvd.c):\n");
    grx_cdvd_set_disc(d);
    sceCdInit(0);
    {
        CdlFILE fp;
        uint8_t a[4 * GRX_DISC_SECTOR], b[4 * GRX_DISC_SECTOR];
        int r = sceCdSearchFile(&fp, "\\SYS\\GRXSYS.PAC;1");
        printf("  sceCdSearchFile(\\SYS\\GRXSYS.PAC;1) = %d: lsn %u size %u name %s\n", r, fp.lsn, fp.size, fp.name);
        CHECK(r == 1 && grx_disc_find(d, "SYS/GRXSYS.PAC", &e) == 0 && e.lba == fp.lsn && e.size == fp.size,
              "sceCdSearchFile");
        CHECK(sceCdSearchFile(&fp, "\\SYS\\NOPE.PAC;1") == 0, "sceCdSearchFile of a missing file");
        r = sceCdRead(e.lba, 4, a, NULL);
        grx_disc_read_sectors(d, e.lba, 4, b);
        printf("  sceCdRead(%u, 4) = %d, sceCdSync(1) = %d, %s\n", e.lba, r, sceCdSync(1),
               memcmp(a, b, sizeof(a)) == 0 ? "same data" : "DIFFERENT");
        CHECK(r == 1 && sceCdSync(1) == 0 && memcmp(a, b, sizeof(a)) == 0, "sceCdRead");
    }

    printf("GS_IOP stream driver (cdvd.c):\n");
    grx_gsiop_boot(iop_hdr);
    CHECK((uint32_t)iop_hdr[0] == 0x5A5AA5A5u, "header magic");
    push(0x100, 0, 0, 0);
    poll_iop();
    CHECK(*(int16_t *)(iop_status + 0xE) == (int16_t)(cmd_seq - 1), "doneSeq follows the commands");
    CHECK(*(int32_t *)(iop_status + 0x50) != 0, "driver ready");
    /* the volume descriptor through slot 0, as SndFsMount does */
    {
        uint8_t pvd[GRX_DISC_SECTOR], ref_pvd[GRX_DISC_SECTOR];
        int wraps = 0;
        uint64_t h = stream_read(0, 16, 1, 1, pvd, &wraps);
        grx_disc_read_sectors(d, 16, 1, ref_pvd);
        CHECK(h == fnv(ref_pvd, sizeof(ref_pvd), FNV0), "volume descriptor through the stream");
        printf("  sector 16 through stream 0: %s\n", h == fnv(ref_pvd, sizeof(ref_pvd), FNV0) ? "ok" : "MISMATCH");
    }
    check_stream(d, dir, "SYS/GRXSYS.PAC", 1u << 20, 4096);
    check_stream(d, dir, "MUSIC/6207.SE", 1u << 20, 128);
    /* past the ring's 16384 sectors: wraps */
    check_stream(d, dir, "MOVIE/GRM01.MOV", 24000, 1000);

    grx_disc_close(d);
    printf("%s (%d failure%s)\n", failures ? "FAILED" : "all ok", failures, failures == 1 ? "" : "s");
    return failures != 0;
}
