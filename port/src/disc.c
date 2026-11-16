/*
 * The player's disc image: an ISO 9660 reader (port/docs/disc.md).
 *
 * The game's own disc is a single-layer DVD: plain 2048-byte sectors, ISO
 * 9660 with no Joliet or Rock Ridge that the game looks at (the PS2 reads
 * the primary volume descriptor). Raw CD sectors are recognised too, since
 * it costs little, but no such image of this game exists to test them on.
 *
 * Uses only the C library, so it builds unchanged for the browser later
 * (Emscripten's stdio over a file the player picks).
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grx_disc.h"

#ifdef _WIN32
#define disc_seek(f, off) _fseeki64((f), (__int64)(off), SEEK_SET)
#define disc_tell(f) _ftelli64(f)
#else
#define disc_seek(f, off) fseeko((f), (off_t)(off), SEEK_SET)
#define disc_tell(f) ftello(f)
#endif

struct GrxDisc {
    FILE *f;
    uint64_t base;          /* file offset of sector 0 */
    int sector_size;        /* bytes per sector in the file */
    int data_offset;        /* user data within a sector */
    uint64_t file_size;
    uint32_t volume_sectors;
    uint32_t root_lba, root_size;
    char volume_id[33];
};

static char disc_err[256];

const char *grx_disc_error(void) {
    return disc_err;
}

static uint32_t le32(const uint8_t *p) {
    return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint16_t le16(const uint8_t *p) {
    return p[0] | (p[1] << 8);
}

/* Reads user data of whole sectors; past the end of the file, zeros */
int grx_disc_read_sectors(GrxDisc *d, uint32_t lba, uint32_t count, void *dst) {
    uint8_t *out = dst;
    uint8_t raw[2448];
    uint32_t i;

    if (d->sector_size == GRX_DISC_SECTOR) {
        /* One read for the whole run, then zeros for what is past the end */
        uint64_t off = d->base + (uint64_t)lba * GRX_DISC_SECTOR;
        uint64_t want = (uint64_t)count * GRX_DISC_SECTOR;
        uint64_t have = off < d->file_size ? d->file_size - off : 0;
        if (have > want) {
            have = want;
        }
        if (have > 0) {
            if (disc_seek(d->f, off) != 0 || fread(out, 1, (size_t)have, d->f) != have) {
                return -1;
            }
        }
        memset(out + have, 0, (size_t)(want - have));
        return (int)count;
    }
    for (i = 0; i < count; i++, out += GRX_DISC_SECTOR) {
        uint64_t off = d->base + (uint64_t)(lba + i) * d->sector_size;
        if (off + d->sector_size > d->file_size) {
            memset(out, 0, GRX_DISC_SECTOR);
            continue;
        }
        if (disc_seek(d->f, off) != 0 || fread(raw, 1, d->sector_size, d->f) != (size_t)d->sector_size) {
            return -1;
        }
        memcpy(out, raw + d->data_offset, GRX_DISC_SECTOR);
    }
    return (int)count;
}

/* "CD001" of a volume descriptor at sector 16 with this layout */
static int probe(GrxDisc *d, int sector_size, int data_offset, uint8_t *pvd) {
    d->sector_size = sector_size;
    d->data_offset = data_offset;
    if (d->base + (uint64_t)17 * sector_size > d->file_size) {
        return 0;
    }
    if (grx_disc_read_sectors(d, 16, 1, pvd) != 1) {
        return 0;
    }
    return memcmp(pvd + 1, "CD001", 5) == 0;
}

/*
 * An .MDS (Alcohol 120%'s descriptor, v1) names the file with the sectors
 * and gives their size and where sector 0 starts. Only the first track is
 * used: the game's disc is a one-track DVD. The layout (after libmirage):
 * header +0x50 the session blocks; a session (0x18) +0x14 its track
 * blocks, +0xA/+0xB the number of blocks (lead-in entries A0-A2 first on
 * a CD); a track block (0x50) +4 the point (track number), +0x10 the
 * sector size, +0x28 the file offset (u64), +0x34 its footer; a footer
 * +0 the file name's offset, +4 nonzero when it is UTF-16. "*.mdf" means
 * the descriptor's own name with that extension.
 */
static int mds_parse(const char *mds, char *out, size_t outlen, int *sector_size, uint64_t *base) {
    uint8_t *b = NULL;
    long n;
    FILE *f = fopen(mds, "rb");
    uint32_t so, to, foot, fo;
    int t, nblocks, ok = 0;

    if (f == NULL) {
        return 0;
    }
    if (fseek(f, 0, SEEK_END) == 0 && (n = ftell(f)) > 0x58 && n < (1 << 20) && (b = malloc(n)) != NULL) {
        rewind(f);
        if (fread(b, 1, n, f) != (size_t)n) {
            n = 0;
        }
    } else {
        n = 0;
    }
    fclose(f);
#define IN(off, len) ((uint64_t)(off) + (len) <= (uint64_t)n)
    if (n == 0 || memcmp(b, "MEDIA DESCRIPTOR", 16) != 0 || !IN(0x50, 4)) {
        goto done;
    }
    so = le32(b + 0x50);
    if (!IN(so, 0x18)) {
        goto done;
    }
    to = le32(b + so + 0x14);
    nblocks = b[so + 0xA] + b[so + 0xB];
    /* In 64 bits: to + t * 0x50 mustn't wrap past the check */
    for (t = 0; t < nblocks && IN((uint64_t)to + (uint64_t)t * 0x50, 0x50); t++) {
        const uint8_t *tb = b + (size_t)((uint64_t)to + (uint64_t)t * 0x50);
        char name[512];
        size_t k = 0;

        if (tb[4] < 1 || tb[4] > 99) {
            continue;               /* a lead-in entry, not a track */
        }
        foot = le32(tb + 0x34);
        if (!IN(foot, 8) || !IN(le32(b + foot), 2)) {
            break;
        }
        fo = le32(b + foot);
        if (le32(b + foot + 4) != 0) {      /* UTF-16: ASCII names only */
            for (; IN(fo + 2 * k, 2) && k < sizeof(name) - 1 && b[fo + 2 * k] != 0; k++) {
                name[k] = (char)b[fo + 2 * k];
            }
        } else {
            for (; IN(fo + k, 1) && k < sizeof(name) - 1 && b[fo + k] != 0; k++) {
                name[k] = (char)b[fo + k];
            }
        }
        name[k] = 0;
        *sector_size = le16(tb + 0x10);
        *base = (uint64_t)le32(tb + 0x28) | ((uint64_t)le32(tb + 0x2C) << 32);
        {
            /* relative to the descriptor's directory; "*.ext" = its own name */
            const char *slash = mds + strlen(mds);
            while (slash > mds && slash[-1] != '/' && slash[-1] != '\\') {
                slash--;
            }
            if (name[0] == '*') {
                const char *dot = strrchr(mds, '.');
                size_t stem = dot != NULL && dot > slash ? (size_t)(dot - mds) : strlen(mds);
                snprintf(out, outlen, "%.*s%s", (int)stem, mds, name + 1);
            } else {
                snprintf(out, outlen, "%.*s%s", (int)(slash - mds), mds, name);
            }
        }
        ok = 1;
        break;
    }
#undef IN
done:
    free(b);
    return ok;
}

GrxDisc *grx_disc_open(const char *path) {
    static const int layouts[][2] = {
        {2048, 0},              /* .ISO, DVD .MDF */
        {2352, 16},             /* raw MODE1 (sync 12, header 4) */
        {2352, 24},             /* raw MODE2/XA form 1 (+ subheader 8) */
        {2448, 16},             /* raw + 96 bytes of subchannel (.MDF of a CD) */
        {2448, 24},
    };
    uint8_t pvd[GRX_DISC_SECTOR];
    char mdf[1024];
    GrxDisc *d;
    size_t n;
    int i, lba, found = 0, mds_sector = 0;
    uint64_t base = 0;

    /* An .MDS is only the descriptor; the sectors are in the file it names
       (an .MDF, or here the .ISO itself) */
    n = strlen(path);
    if (n >= 4 && (strcmp(path + n - 4, ".mds") == 0 || strcmp(path + n - 4, ".MDS") == 0)) {
        if (!mds_parse(path, mdf, sizeof(mdf), &mds_sector, &base)) {
            snprintf(disc_err, sizeof(disc_err), "%s: not a media descriptor this reader knows", path);
            return NULL;
        }
        path = mdf;
    }

    d = calloc(1, sizeof(*d));
    if (d == NULL) {
        snprintf(disc_err, sizeof(disc_err), "out of memory");
        return NULL;
    }
    d->f = fopen(path, "rb");
    if (d->f == NULL) {
        snprintf(disc_err, sizeof(disc_err), "%s: can't open", path);
        free(d);
        return NULL;
    }
    setvbuf(d->f, NULL, _IOFBF, 64 * 1024);
    if (fseek(d->f, 0, SEEK_END) == 0) {
        int64_t t = disc_tell(d->f);
        d->file_size = t > 0 ? (uint64_t)t : 0;
    }
    d->base = base;

    for (i = 0; i < (int)(sizeof(layouts) / sizeof(layouts[0])) && !found; i++) {
        if (mds_sector != 0 && layouts[i][0] != mds_sector) {
            continue;
        }
        found = probe(d, layouts[i][0], layouts[i][1], pvd);
    }
    if (!found) {
        snprintf(disc_err, sizeof(disc_err), "%s: not an ISO 9660 image (no CD001 at sector 16)", path);
        grx_disc_close(d);
        return NULL;
    }

    /* The primary volume descriptor: the first of type 1 from sector 16 */
    for (lba = 16; pvd[0] != 1; ) {
        if (pvd[0] == 255 || ++lba > 32 || grx_disc_read_sectors(d, lba, 1, pvd) != 1
            || memcmp(pvd + 1, "CD001", 5) != 0) {
            snprintf(disc_err, sizeof(disc_err), "%s: no primary volume descriptor", path);
            grx_disc_close(d);
            return NULL;
        }
    }
    if (le16(pvd + 128) != GRX_DISC_SECTOR) {
        snprintf(disc_err, sizeof(disc_err), "%s: logical block size %u", path, le16(pvd + 128));
        grx_disc_close(d);
        return NULL;
    }
    d->volume_sectors = le32(pvd + 80);
    d->root_lba = le32(pvd + 156 + 2);
    d->root_size = le32(pvd + 156 + 10);
    memcpy(d->volume_id, pvd + 40, 32);
    for (i = 31; i >= 0 && d->volume_id[i] == ' '; i--) {
        d->volume_id[i] = 0;
    }
    d->volume_id[32] = 0;
    return d;
}

void grx_disc_close(GrxDisc *d) {
    if (d != NULL) {
        if (d->f != NULL) {
            fclose(d->f);
        }
        free(d);
    }
}

uint32_t grx_disc_sector_count(const GrxDisc *d) {
    return d->volume_sectors;
}

void grx_disc_volume_id(const GrxDisc *d, char out[33]) {
    memcpy(out, d->volume_id, 33);
}

void grx_disc_layout(const GrxDisc *d, int *sector_size, int *data_offset) {
    *sector_size = d->sector_size;
    *data_offset = d->data_offset;
}

/* ---- directories ---- */

/* Directory record -> entry; 0 for "." and ".." (r[0] >= 34, the record
   within its sector: walk_dir) */
static int parse_record(const uint8_t *r, GrxDiscEntry *e) {
    int len = r[32], i;

    if (len > r[0] - 33) {
        len = r[0] - 33;    /* damaged: the name ends with the record */
    }

    if (len == 1 && (r[33] == 0 || r[33] == 1)) {
        return 0;
    }
    memset(e, 0, sizeof(*e));
    e->lba = le32(r + 2);
    e->size = le32(r + 10);
    e->flags = r[25];
    e->is_dir = (r[25] & 2) != 0;
    for (i = 0; i < len && i < (int)sizeof(e->name) - 1 && r[33 + i] != ';'; i++) {
        e->name[i] = (char)r[33 + i];
    }
    if (i > 0 && e->name[i - 1] == '.') {       /* "NAME." of a file without extension */
        i--;
    }
    e->name[i] = 0;
    return 1;
}

/* Walks a directory extent; fn returns nonzero to stop (and that value is
   returned). Records never cross a sector; a zero length byte pads to the
   next one. */
static int walk_dir(GrxDisc *d, uint32_t lba, uint32_t size,
                    int (*fn)(const GrxDiscEntry *, void *), void *user, int *count) {
    uint8_t sec[GRX_DISC_SECTOR];
    uint32_t s, nsec = (size + GRX_DISC_SECTOR - 1) / GRX_DISC_SECTOR;
    GrxDiscEntry e;
    int pos, r;

    for (s = 0; s < nsec; s++) {
        if (grx_disc_read_sectors(d, lba + s, 1, sec) != 1) {
            return -1;
        }
        for (pos = 0; pos < GRX_DISC_SECTOR && sec[pos] != 0; pos += sec[pos]) {
            if (sec[pos] < 34 || pos + sec[pos] > GRX_DISC_SECTOR) {
                break;              /* damaged record: skip the rest of the sector */
            }
            if (!parse_record(sec + pos, &e)) {
                continue;
            }
            if (count != NULL) {
                (*count)++;
            }
            r = fn(&e, user);
            if (r != 0) {
                return r;
            }
        }
    }
    return 0;
}

typedef struct {
    const char *name;
    size_t len;
    GrxDiscEntry *out;
} FindCtx;

static int find_cb(const GrxDiscEntry *e, void *user) {
    FindCtx *c = user;
    size_t i;

    for (i = 0; i < c->len; i++) {
        if (e->name[i] == 0 || toupper((unsigned char)e->name[i]) != toupper((unsigned char)c->name[i])) {
            return 0;
        }
    }
    if (e->name[i] != 0) {
        return 0;
    }
    *c->out = *e;
    return 1;
}

int grx_disc_find(GrxDisc *d, const char *path, GrxDiscEntry *out) {
    GrxDiscEntry cur;
    const char *p = path, *colon;
    FindCtx c;

    colon = strchr(p, ':');
    if (colon != NULL) {
        p = colon + 1;
    }
    memset(&cur, 0, sizeof(cur));
    cur.lba = d->root_lba;
    cur.size = d->root_size;
    cur.flags = 2;
    cur.is_dir = 1;

    for (;;) {
        size_t len;

        while (*p == '/' || *p == '\\') {
            p++;
        }
        if (*p == 0) {
            break;
        }
        for (len = 0; p[len] != 0 && p[len] != '/' && p[len] != '\\' && p[len] != ';'; len++) {
        }
        if (len > 0 && p[len - 1] == '.') {
            len--;                  /* "NAME." */
        }
        if (!cur.is_dir) {
            return -1;
        }
        c.name = p;
        c.len = len;
        c.out = &cur;
        if (walk_dir(d, cur.lba, cur.size, find_cb, &c, NULL) != 1) {
            return -1;
        }
        p += len;
        if (*p == '.') {
            p++;
        }
        if (*p == ';') {            /* a version: only the last component has one */
            while (*p != 0 && *p != '/' && *p != '\\') {
                p++;
            }
        }
    }
    *out = cur;
    return 0;
}

int grx_disc_list(GrxDisc *d, const char *dir,
                  int (*fn)(const GrxDiscEntry *e, void *user), void *user) {
    GrxDiscEntry e;
    int count = 0;

    if (grx_disc_find(d, dir, &e) != 0 || !e.is_dir) {
        return -1;
    }
    if (walk_dir(d, e.lba, e.size, fn, user, &count) < 0) {
        return -1;
    }
    return count;
}

/* ---- file contents ---- */

int64_t grx_disc_read_file(GrxDisc *d, const GrxDiscEntry *f, uint64_t offset,
                           void *dst, uint32_t len) {
    uint8_t sec[GRX_DISC_SECTOR];
    uint8_t *out = dst;
    uint32_t done = 0;

    if (offset >= f->size) {
        return 0;
    }
    if (len > f->size - offset) {
        len = (uint32_t)(f->size - offset);
    }
    while (done < len) {
        uint64_t pos = offset + done;
        uint32_t lba = f->lba + (uint32_t)(pos / GRX_DISC_SECTOR);
        uint32_t in = (uint32_t)(pos % GRX_DISC_SECTOR);
        uint32_t left = len - done;

        if (in == 0 && left >= GRX_DISC_SECTOR) {
            /* Whole sectors straight into the destination */
            uint32_t n = left / GRX_DISC_SECTOR;
            if (grx_disc_read_sectors(d, lba, n, out + done) != (int)n) {
                return -1;
            }
            done += n * GRX_DISC_SECTOR;
        } else {
            uint32_t n = GRX_DISC_SECTOR - in;
            if (n > left) {
                n = left;
            }
            if (grx_disc_read_sectors(d, lba, 1, sec) != 1) {
                return -1;
            }
            memcpy(out + done, sec + in, n);
            done += n;
        }
    }
    return done;
}
