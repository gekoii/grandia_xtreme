/*
 * libmc over host directories (port/include/grx_mc.h).
 *
 * Sony's libmc (src/main 46FE20-4714C0, left out by port/tools/fixups/mc.py)
 * sends each call to mcserv on the IOP; mcman runs it on the card. Here the
 * card is a host directory, and these are the sceMc* calls the game links,
 * with libmc's protocol and mcman's results:
 *
 *  - A call checks that libmc is initialised (-100) and that no command is
 *    in flight (-200: libmc's semaphore), copies its arguments as libmc
 *    copies them into the RPC block (names up to 0x3FF bytes, -210 for a
 *    NULL or empty one), and returns 0. The command runs at the next
 *    sceMcSync, which returns 1 with libmc's command number and mcman's
 *    result, as a finished RPC does; with nothing in flight it returns -1.
 *    sceMcInit and sceMcGetSlotMax answer at once, as libmc's do.
 *  - Results are XMCMAN's (ps2sdk's mcman reimplementation is the
 *    reference for what it returns when): sceMcGetInfo (XMCSERV's GetInfo2
 *    over McDetectCard2) gives -1 for a newly inserted formatted card, -2
 *    for a newly inserted unformatted one, 0 for the card it has seen
 *    before, and -12 (sceMcResFailDetect) without one; free and format are
 *    0 when the result is below -1. Every other card call (McDetectCard)
 *    gives -12 without a card, -13 before sceMcGetInfo has seen the card
 *    (mcman only knows a card after that full probe), -2 on an unformatted
 *    one. The game's driver (McUpdate 454BE8) takes < -10 as "no card".
 *  - Space is counted in 1 KB clusters like an 8 MB card's: 8001
 *    allocatable clusters (mcman_format2 rounds the card down to 8000 and
 *    adds one), the root directory's first one used, so an empty card has
 *    8000 free. A file takes ceil(size / 1 KB); a directory of n entries
 *    (with "." and "..") ceil(n / 2), two 512-byte entries per cluster.
 *    Writing past the free space gives -3; an entry that needs a new
 *    directory cluster too.
 *  - Entries: a new file is 0x8417 (no "closed" bit); a write clears 0x80,
 *    and closing or flushing after a write (or a zero-byte write) sets it,
 *    as mcman_close2 does: the game checks icon.sys and icon0.ico for 0x80
 *    (McSaveCheckGameFiles 65DD40). Directories are 0x8427. Timestamps are
 *    the PS2's: Japan time, from the host clock. Listings (sceMcGetDir)
 *    are sceMcTblGetDir records in entry order: "." and ".." first except
 *    in the root, FileSizeByte only for files, '*' and '?' as mcman
 *    matches them. Names are compared case-sensitively, as on the card.
 *  - fds are mcman's: the lowest free of 3 (-7 when all are open). The game
 *    relies on that: it reads and writes through the fd sceMcOpen returned
 *    at the call, which is 0, not the one the sync reports (McExecNext).
 *  - PS1CardFlag is mcman's default (1): hidden entries are listed, delete
 *    doesn't check the write bit, sceMcSetFileInfo's attribute mask is
 *    0x7FCF.
 *
 * Host layout: <base>/cardN/ is the card's root; a PS2 name is stored as a
 * host name with the bytes Windows can't hold as %XX. <base>/cardN.meta
 * keeps each entry's mode and PS2 timestamps in entry order; entries it
 * doesn't list (files copied in by hand) get default ones.
 *
 * All host access goes through SDL3's filesystem and I/O calls. No
 * <windows.h> here: it would clash with the game's names.
 */
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_time.h>
#undef NULL
#include "sdk/libmc.h"
#include "grx_mc.h"
#include "grx_platform.h"
#include "grx_state_parts.h"

/* mcman's results (sceMcRes*) */
#define RES_OK            0
#define RES_CHANGED      -1
#define RES_NOFORMAT     -2
#define RES_FULL         -3
#define RES_NOENTRY      -4
#define RES_DENIED       -5
#define RES_NOTEMPTY     -6
#define RES_UPLIMIT      -7
#define RES_FAILDETECT  -12
#define RES_FAILDETECT2 -13
/* libmc's own, before anything is sent */
#define LIB_NOINIT     -100
#define LIB_BUSY       -200
#define LIB_BADNAME    -210

/* Entry mode bits (sceMcFileAttr*) and open flags */
#define ATTR_R       0x0001
#define ATTR_W       0x0002
#define ATTR_X       0x0004
#define ATTR_FILE    0x0010
#define ATTR_SUBDIR  0x0020
#define ATTR_CLOSED  0x0080
#define ATTR_PDAEXEC 0x0800
#define ATTR_PS1     0x1000
#define ATTR_HIDDEN  0x2000
#define ATTR_EXISTS  0x8000
#define MODE_DIR     0x8427   /* EXISTS | 0400 | SUBDIR | rwx */
#define MODE_NEWFILE 0x8417   /* EXISTS | 0400 | FILE | rwx */
#define MODE_FILE    0x8497   /* a closed file */
#define OPEN_CREAT_DIR  0x0040
#define OPEN_CREAT_FILE 0x0200
/* sceMcSetFileInfo's attribute mask with PS1CardFlag set: all but EXISTS,
   SUBDIR and FILE */
#define SETINFO_ATTR_MASK 0x7FCF

/* libmc command numbers (what sceMcSync reports) */
enum {
    CMD_GETINFO = 1, CMD_OPEN = 2, CMD_CLOSE = 3, CMD_SEEK = 4, CMD_READ = 5, CMD_WRITE = 6,
    CMD_FLUSH = 0xA, CMD_MKDIR = 0xB, CMD_CHDIR = 0xC, CMD_GETDIR = 0xD, CMD_SETINFO = 0xE,
    CMD_DELETE = 0xF, CMD_FORMAT = 0x10, CMD_UNFORMAT = 0x11, CMD_ENTSPACE = 0x12,
    CMD_RENAME = 0x13, CMD_THREADPRIO = 0x14,
};

#define CARD_CLUSTERS 8001    /* allocatable clusters of an 8 MB card */
#define CLUSTER 1024
#define MAX_FD 3
#define PS2_PATH 0x400        /* libmc's name buffers */
#define HOST_PATH 4096
#define NAME_MAX_PS2 31       /* sceMcTblGetDir.EntryName / McFsEntry.name */

/* ------------------------------------------------------------------------ */
/* Time                                                                     */

/* The card's clock is the PS2's RTC, which keeps Japan time */
static void time_from_sdl(SDL_Time t, sceMcStDateTime *o) {
    SDL_DateTime dt;
    memset(o, 0, sizeof *o);
    if (!SDL_TimeToDateTime(t + (SDL_Time)9 * 3600 * 1000000000, &dt, false)) {
        /* mcman's date when the RTC can't be read */
        o->Year = 2000, o->Month = 3, o->Day = 4;
        return;
    }
    o->Sec = (u8)dt.second;
    o->Min = (u8)dt.minute;
    o->Hour = (u8)dt.hour;
    o->Day = (u8)dt.day;
    o->Month = (u8)dt.month;
    o->Year = (u16)dt.year;
}

static void time_now(sceMcStDateTime *o) {
    SDL_Time t;
    if (!SDL_GetCurrentTime(&t)) {
        memset(o, 0, sizeof *o);
        o->Year = 2000, o->Month = 3, o->Day = 4;
        return;
    }
    time_from_sdl(t, o);
}

static void time_fmt(const sceMcStDateTime *t, char out[15]) {
    snprintf(out, 15, "%04u%02u%02u%02u%02u%02u", t->Year % 10000, t->Month % 100, t->Day % 100,
             t->Hour % 100, t->Min % 100, t->Sec % 100);
}

static int time_parse(const char *s, sceMcStDateTime *t) {
    unsigned y, mo, d, h, mi, se;
    if (strlen(s) != 14 || sscanf(s, "%4u%2u%2u%2u%2u%2u", &y, &mo, &d, &h, &mi, &se) != 6) {
        return 0;
    }
    memset(t, 0, sizeof *t);
    t->Year = (u16)y, t->Month = (u8)mo, t->Day = (u8)d;
    t->Hour = (u8)h, t->Min = (u8)mi, t->Sec = (u8)se;
    return 1;
}

/* ------------------------------------------------------------------------ */
/* Host names                                                               */

static int host_bad_char(unsigned char ch) {
    return ch < 0x20 || ch == 0x7F || (ch && strchr("\"*/:<>?\\|%", ch) != NULL);
}

/* CON, PRN, AUX, NUL, COM1-9, LPT1-9, with or without an extension */
static int host_reserved(const char *name) {
    static const char *const dev[] = { "CON", "PRN", "AUX", "NUL" };
    size_t n = strcspn(name, ".");
    size_t i;
    for (i = 0; i < 4; i++) {
        if (n == 3 && SDL_strncasecmp(name, dev[i], 3) == 0) {
            return 1;
        }
    }
    return n == 4 && (SDL_strncasecmp(name, "COM", 3) == 0 || SDL_strncasecmp(name, "LPT", 3) == 0) &&
           name[3] >= '1' && name[3] <= '9';
}

/* A PS2 name as a host name: the bytes a Windows name can't hold (and a
   reserved device name's first letter, a trailing dot or space) as %XX */
static void name_encode(const char *name, char *out, size_t n) {
    size_t len = strlen(name), i, k = 0;
    int reserved = host_reserved(name);
    for (i = 0; i < len && k + 4 < n; i++) {
        unsigned char ch = (unsigned char)name[i];
        if (host_bad_char(ch) || (i == 0 && reserved) || (i == len - 1 && (ch == '.' || ch == ' '))) {
            k += (size_t)snprintf(out + k, n - k, "%%%02X", ch);
        } else {
            out[k++] = (char)ch;
        }
    }
    out[k] = 0;
}

static int hexval(char c) {
    return c >= '0' && c <= '9' ? c - '0' : c >= 'A' && c <= 'F' ? c - 'A' + 10 : c >= 'a' && c <= 'f' ? c - 'a' + 10 : -1;
}

static void name_decode(const char *host, char *out, size_t n) {
    size_t k = 0;
    while (*host && k + 1 < n) {
        if (host[0] == '%' && hexval(host[1]) >= 0 && hexval(host[2]) >= 0) {
            out[k++] = (char)(hexval(host[1]) * 16 + hexval(host[2]));
            host += 3;
        } else {
            out[k++] = *host++;
        }
    }
    out[k] = 0;
}

/* The PS2 name a host name stands for; 0 for host names a card can't
   have (too long, or not what encoding that name gives) */
static int host_name_ps2(const char *host, char out[NAME_MAX_PS2 + 1]) {
    char dec[256], enc[256 * 3 + 1];
    if (!strcmp(host, ".") || !strcmp(host, "..") || strlen(host) >= sizeof dec) {
        return 0;
    }
    name_decode(host, dec, sizeof dec);
    if (dec[0] == 0 || strlen(dec) > NAME_MAX_PS2) {
        return 0;
    }
    name_encode(dec, enc, sizeof enc);
    if (strcmp(enc, host) != 0) {
        return 0;
    }
    strcpy(out, dec);
    return 1;
}

/* A directory's host names */
typedef struct Names {
    char **v;
    int n, cap;
} Names;

static SDL_EnumerationResult SDLCALL names_add(void *ud, const char *dirname, const char *fname) {
    Names *nl = ud;
    (void)dirname;
    if (nl->n == nl->cap) {
        int cap = nl->cap ? nl->cap * 2 : 16;
        char **v = realloc(nl->v, (size_t)cap * sizeof *v);
        if (!v) {
            return SDL_ENUM_FAILURE;
        }
        nl->v = v, nl->cap = cap;
    }
    nl->v[nl->n] = SDL_strdup(fname);
    if (!nl->v[nl->n]) {
        return SDL_ENUM_FAILURE;
    }
    nl->n++;
    return SDL_ENUM_CONTINUE;
}

static void names_free(Names *nl) {
    int i;
    for (i = 0; i < nl->n; i++) {
        SDL_free(nl->v[i]);
    }
    free(nl->v);
    memset(nl, 0, sizeof *nl);
}

static int names_get(const char *hostdir, Names *nl) {
    memset(nl, 0, sizeof *nl);
    if (!SDL_EnumerateDirectory(hostdir, names_add, nl)) {
        names_free(nl);
        return 0;
    }
    return 1;
}

static int host_rmtree(const char *path) {
    SDL_PathInfo pi;
    int ok = 1;
    if (!SDL_GetPathInfo(path, &pi)) {
        return 1;
    }
    if (pi.type == SDL_PATHTYPE_DIRECTORY) {
        Names nl;
        int i;
        if (names_get(path, &nl)) {
            for (i = 0; i < nl.n; i++) {
                char sub[HOST_PATH];
                if (!strcmp(nl.v[i], ".") || !strcmp(nl.v[i], "..")) {
                    continue;
                }
                snprintf(sub, sizeof sub, "%s/%s", path, nl.v[i]);
                ok &= host_rmtree(sub);
            }
            names_free(&nl);
        }
    }
    return SDL_RemovePath(path) && ok;
}

/* ------------------------------------------------------------------------ */
/* Cards                                                                    */

/* An entry's attributes kept beside the host tree (cardN.meta), keyed by
   its host path from the card's root ("/" is the root) */
typedef struct Meta {
    char *key;
    u16 mode;
    sceMcStDateTime ctime, mtime;
} Meta;

typedef struct Card {
    int state;                 /* GRX_MC_* */
    int probed;                /* sceMcGetInfo has seen this card (mcman's cardtype) */
    char root[HOST_PATH];      /* host directory, no trailing separator */
    char rootslash[HOST_PATH];
    char meta_file[HOST_PATH];
    char cwd[PS2_PATH];        /* sceMcChdir's directory, absolute */
    Meta *meta;
    int nmeta, capmeta;
} Card;

/* An entry as a listing gives it */
typedef struct Ent {
    char name[NAME_MAX_PS2 + 1];
    int is_dir;
    u32 size;
    u16 mode;
    sceMcStDateTime ctime, mtime;
    int order;                 /* position in the meta list, INT_MAX if not in it */
} Ent;

typedef struct Fd {
    int used, port;
    int is_dir, rd, wr;
    int dirty;                 /* written since the last close2 (mcman's fh->unknown2) */
    u32 pos, size;
    char raw[PS2_PATH];
    SDL_IOStream *io;
} Fd;

static Card cards[2];
static Fd fds[MAX_FD];
static int cfg_done;
static char base_dir[HOST_PATH];

/* sceMcGetDir's listing in progress (mcman_getdir2 continues one when
   called with flags != 0) */
static struct {
    int valid, port;
    Ent *v;
    int n, pos;
} listing;

static int card_index(int port, int slot) {
    return (port == 0 || port == 1) && slot == 0 ? port : -1;
}

/* "/A/B" -> "/a%3Ab/..." (the host path from the card's root) */
static void path_key(const char *raw, char *key, size_t n) {
    const char *p = raw;
    size_t k = 0;
    if (strcmp(raw, "/") == 0) {
        snprintf(key, n, "/");
        return;
    }
    key[0] = 0;
    while (*p == '/') {
        char comp[PS2_PATH], enc[PS2_PATH * 3 + 1];
        size_t len;
        p++;
        len = strcspn(p, "/");
        if (len >= sizeof comp) {
            len = sizeof comp - 1;
        }
        memcpy(comp, p, len);
        comp[len] = 0;
        p += strcspn(p, "/");
        name_encode(comp, enc, sizeof enc);
        k += (size_t)snprintf(key + k, n > k ? n - k : 0, "/%s", enc);
    }
}

static void host_of_key(const Card *c, const char *key, char *out, size_t n) {
    if (strcmp(key, "/") == 0) {
        snprintf(out, n, "%s", c->root);
    } else {
        snprintf(out, n, "%s%s", c->root, key);
    }
}

/* "/A/B" -> "/A" and "B"; "/A" -> "/" and "A" */
static void path_split(const char *raw, char *parent, char *base) {
    const char *s = strrchr(raw, '/');
    size_t n = (size_t)(s - raw);
    if (n == 0) {
        strcpy(parent, "/");
    } else {
        memcpy(parent, raw, n);
        parent[n] = 0;
    }
    strcpy(base, s + 1);
}

static void path_join(const char *dir, const char *name, char *out, size_t n) {
    snprintf(out, n, strcmp(dir, "/") == 0 ? "%s%s" : "%s/%s", dir, name);
}

/* A name from the game, relative to the current directory or absolute,
   as a normalised absolute path ("." and ".." resolved, ".." at the root
   staying there) */
static int path_resolve(const Card *c, const char *name, char *out) {
    char buf[PS2_PATH * 2 + 2];
    char *comp[PS2_PATH];
    int n = 0, i;
    char *tok, *save = NULL;
    size_t k = 0;
    snprintf(buf, sizeof buf, "%s/%s", name[0] == '/' ? "" : c->cwd, name);
    for (tok = SDL_strtok_r(buf, "/", &save); tok; tok = SDL_strtok_r(NULL, "/", &save)) {
        if (!strcmp(tok, ".")) {
            continue;
        }
        if (!strcmp(tok, "..")) {
            if (n > 0) {
                n--;
            }
            continue;
        }
        comp[n++] = tok;
    }
    if (n == 0) {
        strcpy(out, "/");
        return 1;
    }
    for (i = 0; i < n; i++) {
        size_t len = strlen(comp[i]);
        if (k + 1 + len >= PS2_PATH) {
            return 0;
        }
        out[k++] = '/';
        memcpy(out + k, comp[i], len);
        k += len;
    }
    out[k] = 0;
    return 1;
}

/* ---- the attribute list ------------------------------------------------ */

static Meta *meta_find(Card *c, const char *key) {
    int i;
    for (i = 0; i < c->nmeta; i++) {
        if (strcmp(c->meta[i].key, key) == 0) {
            return &c->meta[i];
        }
    }
    return NULL;
}

/* Sets an entry's attributes, keeping its place if it has one */
static void meta_set(Card *c, const char *key, u16 mode, const sceMcStDateTime *ctime, const sceMcStDateTime *mtime) {
    Meta *m = meta_find(c, key);
    if (!m) {
        if (c->nmeta == c->capmeta) {
            int cap = c->capmeta ? c->capmeta * 2 : 16;
            Meta *v = realloc(c->meta, (size_t)cap * sizeof *v);
            if (!v) {
                grx_log("mc: out of memory");
                return;
            }
            c->meta = v, c->capmeta = cap;
        }
        m = &c->meta[c->nmeta++];
        m->key = SDL_strdup(key);
    }
    m->mode = mode;
    m->ctime = *ctime;
    m->mtime = *mtime;
}

/* Drops an entry's attributes and its children's */
static void meta_del(Card *c, const char *key) {
    size_t len = strlen(key);
    int i, k = 0;
    for (i = 0; i < c->nmeta; i++) {
        Meta *m = &c->meta[i];
        if (!strcmp(m->key, key) || (!strncmp(m->key, key, len) && m->key[len] == '/')) {
            SDL_free(m->key);
            continue;
        }
        c->meta[k++] = *m;
    }
    c->nmeta = k;
}

static void meta_rename(Card *c, const char *from, const char *to) {
    size_t len = strlen(from);
    int i;
    for (i = 0; i < c->nmeta; i++) {
        Meta *m = &c->meta[i];
        if (!strcmp(m->key, from) || (!strncmp(m->key, from, len) && m->key[len] == '/')) {
            char buf[HOST_PATH];
            snprintf(buf, sizeof buf, "%s%s", to, m->key + len);
            SDL_free(m->key);
            m->key = SDL_strdup(buf);
        }
    }
}

static void meta_clear(Card *c) {
    int i;
    for (i = 0; i < c->nmeta; i++) {
        SDL_free(c->meta[i].key);
    }
    c->nmeta = 0;
}

static void meta_load(Card *c) {
    size_t size;
    char *data, *line, *save = NULL;
    meta_clear(c);
    data = SDL_LoadFile(c->meta_file, &size);
    if (!data) {
        return;
    }
    for (line = SDL_strtok_r(data, "\r\n", &save); line; line = SDL_strtok_r(NULL, "\r\n", &save)) {
        unsigned mode;
        char ct[16], mt[16];
        int off = 0;
        sceMcStDateTime ctime, mtime;
        if (line[0] == '#' || sscanf(line, "%4x %15s %15s %n", &mode, ct, mt, &off) != 3 || off == 0 ||
            line[off] != '/' || !time_parse(ct, &ctime) || !time_parse(mt, &mtime)) {
            continue;
        }
        meta_set(c, line + off, (u16)mode, &ctime, &mtime);
    }
    SDL_free(data);
}

/* Writes the list back, without the entries no longer on the card */
static void meta_save(Card *c) {
    size_t cap = 256, len = 0;
    char *out = malloc(cap), tmp[HOST_PATH];
    int i, k = 0;
    if (!out) {
        return;
    }
    len += (size_t)snprintf(out, cap, "# grandia_extreme memory card: mode, created, modified (PS2 time), entry\n");
    for (i = 0; i < c->nmeta; i++) {
        Meta *m = &c->meta[i];
        char host[HOST_PATH], ct[15], mt[15];
        SDL_PathInfo pi;
        size_t need;
        host_of_key(c, m->key, host, sizeof host);
        if (!SDL_GetPathInfo(host, &pi)) {
            SDL_free(m->key);
            continue;
        }
        c->meta[k++] = *m;
        time_fmt(&m->ctime, ct);
        time_fmt(&m->mtime, mt);
        need = strlen(m->key) + 64;
        if (len + need > cap) {
            char *o;
            cap = (len + need) * 2;
            o = realloc(out, cap);
            if (!o) {
                free(out);
                return;
            }
            out = o;
        }
        len += (size_t)snprintf(out + len, cap - len, "%04X %s %s %s\n", m->mode, ct, mt, m->key);
    }
    c->nmeta = k;
    snprintf(tmp, sizeof tmp, "%s.tmp", c->meta_file);
    if (!SDL_SaveFile(tmp, out, len) || !SDL_RenamePath(tmp, c->meta_file)) {
        grx_log("mc: can't write %s: %s", c->meta_file, SDL_GetError());
    }
    free(out);
}

/* ---- entries ----------------------------------------------------------- */

static void ent_fill(Card *c, const char *key, const SDL_PathInfo *pi, Ent *e) {
    Meta *m = meta_find(c, key);
    e->is_dir = pi->type == SDL_PATHTYPE_DIRECTORY;
    e->size = e->is_dir ? 0 : pi->size > 0xFFFFFFFFu ? 0xFFFFFFFFu : (u32)pi->size;
    e->mode = e->is_dir ? MODE_DIR : MODE_FILE;
    time_from_sdl(pi->create_time ? pi->create_time : pi->modify_time, &e->ctime);
    time_from_sdl(pi->modify_time, &e->mtime);
    e->order = INT_MAX;
    if (m) {
        /* the host decides what the entry is */
        e->mode = (u16)((m->mode & ~(ATTR_FILE | ATTR_SUBDIR)) | (e->is_dir ? ATTR_SUBDIR : ATTR_FILE) | ATTR_EXISTS);
        e->ctime = m->ctime;
        e->mtime = m->mtime;
        e->order = (int)(m - c->meta);
    }
}

/* Is <encname> in the host directory, exactly (the host's names may not
   be case-sensitive; the card's are) */
static int host_has(const char *hostdir, const char *encname) {
    Names nl;
    int i, found = 0;
    if (!names_get(hostdir, &nl)) {
        return 0;
    }
    for (i = 0; i < nl.n && !found; i++) {
        found = strcmp(nl.v[i], encname) == 0;
    }
    names_free(&nl);
    return found;
}

/* Looks an absolute path up: 1 and the entry if it exists */
static int ent_get(Card *c, const char *raw, Ent *e) {
    char key[HOST_PATH], host[HOST_PATH];
    SDL_PathInfo pi;
    memset(e, 0, sizeof *e);
    path_key(raw, key, sizeof key);
    host_of_key(c, key, host, sizeof host);
    if (strcmp(raw, "/") != 0) {
        char praw[PS2_PATH], base[PS2_PATH], pkey[HOST_PATH], phost[HOST_PATH], enc[PS2_PATH * 3 + 1];
        path_split(raw, praw, base);
        if (strlen(base) > NAME_MAX_PS2) {
            return 0;
        }
        path_key(praw, pkey, sizeof pkey);
        host_of_key(c, pkey, phost, sizeof phost);
        name_encode(base, enc, sizeof enc);
        if (!host_has(phost, enc)) {
            return 0;
        }
        strcpy(e->name, base);
    } else {
        strcpy(e->name, ".");
    }
    if (!SDL_GetPathInfo(host, &pi) || (pi.type != SDL_PATHTYPE_FILE && pi.type != SDL_PATHTYPE_DIRECTORY)) {
        return 0;
    }
    ent_fill(c, key, &pi, e);
    return 1;
}

static int ent_cmp(const void *a, const void *b) {
    const Ent *x = a, *y = b;
    if (x->order != y->order) {
        return x->order < y->order ? -1 : 1;
    }
    return strcmp(x->name, y->name);
}

/* A directory's entries in entry order (without "." and ".."); -1 if it
   can't be read */
static int ent_list(Card *c, const char *raw, Ent **out) {
    char key[HOST_PATH], host[HOST_PATH];
    Names nl;
    Ent *v;
    int i, n = 0;
    *out = NULL;
    path_key(raw, key, sizeof key);
    host_of_key(c, key, host, sizeof host);
    if (!names_get(host, &nl)) {
        return -1;
    }
    v = calloc((size_t)nl.n + 1, sizeof *v);
    if (!v) {
        names_free(&nl);
        return -1;
    }
    for (i = 0; i < nl.n; i++) {
        char name[NAME_MAX_PS2 + 1], ckey[HOST_PATH], chost[HOST_PATH];
        SDL_PathInfo pi;
        if (!host_name_ps2(nl.v[i], name)) {
            continue;
        }
        snprintf(ckey, sizeof ckey, "%s/%s", strcmp(key, "/") ? key : "", nl.v[i]);
        host_of_key(c, ckey, chost, sizeof chost);
        if (!SDL_GetPathInfo(chost, &pi) || (pi.type != SDL_PATHTYPE_FILE && pi.type != SDL_PATHTYPE_DIRECTORY)) {
            continue;
        }
        strcpy(v[n].name, name);
        ent_fill(c, ckey, &pi, &v[n]);
        n++;
    }
    names_free(&nl);
    qsort(v, (size_t)n, sizeof *v, ent_cmp);
    *out = v;
    return n;
}

static int dir_count(Card *c, const char *raw) {
    Ent *v;
    int n = ent_list(c, raw, &v);
    free(v);
    return n < 0 ? 0 : n;
}

/* Clusters a directory and everything under it take */
static int clusters_used(Card *c, const char *raw, int depth) {
    Ent *v;
    int n = ent_list(c, raw, &v), i, used;
    if (n < 0) {
        return 0;
    }
    used = (n + 2 + 1) / 2;
    for (i = 0; i < n; i++) {
        if (v[i].is_dir) {
            char sub[PS2_PATH];
            path_join(raw, v[i].name, sub, sizeof sub);
            if (depth < 64) {
                used += clusters_used(c, sub, depth + 1);
            }
        } else {
            used += (int)((v[i].size + CLUSTER - 1) / CLUSTER);
        }
    }
    free(v);
    return used;
}

static int card_free(Card *c) {
    int f = CARD_CLUSTERS - clusters_used(c, "/", 0);
    return f < 0 ? 0 : f;
}

/* Sets an entry's modified time to now (a directory, when an entry in it
   is created or a file in it closed: mcman_open2, mcman_close2) */
static void ent_touch(Card *c, const char *raw) {
    Ent e;
    char key[HOST_PATH];
    if (!ent_get(c, raw, &e)) {
        return;
    }
    path_key(raw, key, sizeof key);
    time_now(&e.mtime);
    meta_set(c, key, e.mode, &e.ctime, &e.mtime);
}

/* ---- card state ------------------------------------------------------- */

static void fd_drop(Fd *f) {
    if (f->io) {
        SDL_CloseIO(f->io);
    }
    memset(f, 0, sizeof *f);
}

/* mcman forgets the card (invhandles, clearcache, cardtype = 0) */
static void card_lose(int port) {
    int i;
    cards[port].probed = 0;
    for (i = 0; i < MAX_FD; i++) {
        if (fds[i].used && fds[i].port == port) {
            fd_drop(&fds[i]);
        }
    }
    if (listing.valid && listing.port == port) {
        listing.valid = 0;
    }
}

/* An empty formatted card: the root and its attributes */
static int card_make_empty(Card *c) {
    sceMcStDateTime now;
    if (!SDL_CreateDirectory(c->root)) {
        grx_log("mc: can't create %s: %s", c->root, SDL_GetError());
        return 0;
    }
    meta_clear(c);
    time_now(&now);
    meta_set(c, "/", MODE_DIR, &now, &now);
    meta_save(c);
    return 1;
}

static int host_exists(const char *path) {
    SDL_PathInfo pi;
    return SDL_GetPathInfo(path, &pi);
}

static void card_setup(int i) {
    Card *c = &cards[i];
    card_lose(i);
    snprintf(c->root, sizeof c->root, "%scard%d", base_dir, i + 1);
    snprintf(c->rootslash, sizeof c->rootslash, "%s/", c->root);
    snprintf(c->meta_file, sizeof c->meta_file, "%scard%d.meta", base_dir, i + 1);
    strcpy(c->cwd, "/");
    meta_load(c);
    if (c->state == GRX_MC_FORMATTED && !host_exists(c->root)) {
        card_make_empty(c);
    }
}

static int parse_state(const char *s, int dflt) {
    if (!s || !*s) {
        return dflt;
    }
    if (!SDL_strcasecmp(s, "formatted") || !SDL_strcasecmp(s, "present") || !SDL_strcasecmp(s, "1") ||
        !SDL_strcasecmp(s, "on") || !SDL_strcasecmp(s, "yes")) {
        return GRX_MC_FORMATTED;
    }
    if (!SDL_strcasecmp(s, "unformatted")) {
        return GRX_MC_UNFORMATTED;
    }
    if (!SDL_strcasecmp(s, "absent") || !SDL_strcasecmp(s, "none") || !SDL_strcasecmp(s, "0") ||
        !SDL_strcasecmp(s, "off") || !SDL_strcasecmp(s, "no")) {
        return GRX_MC_ABSENT;
    }
    grx_log("mc: unknown card setting \"%s\" (formatted, unformatted or absent)", s);
    return dflt;
}

static void set_base(const char *dir) {
    size_t n;
    if (!dir || !*dir) {
        dir = SDL_getenv("GRX_MC_DIR");
    }
    if (dir && *dir) {
        snprintf(base_dir, sizeof base_dir, "%s", dir);
    } else {
        char *pref = SDL_GetPrefPath("grandia_extreme", "memcard");
        snprintf(base_dir, sizeof base_dir, "%s", pref ? pref : "memcard/");
        if (!pref) {
            grx_log("mc: no pref path (%s), cards in ./memcard", SDL_GetError());
        }
        SDL_free(pref);
    }
    n = strlen(base_dir);
    if (n && base_dir[n - 1] != '/' && base_dir[n - 1] != '\\' && n + 1 < sizeof base_dir) {
        base_dir[n] = '/';
        base_dir[n + 1] = 0;
    }
    SDL_CreateDirectory(base_dir);
}

/* What the slots hold, from the environment */
static void cfg_states(void) {
    static int done;
    if (done) {
        return;
    }
    done = 1;
    cards[0].state = parse_state(SDL_getenv("GRX_MC1"), GRX_MC_FORMATTED);
    cards[1].state = parse_state(SDL_getenv("GRX_MC2"), GRX_MC_ABSENT);
}

static void cfg_once(void) {
    if (cfg_done) {
        return;
    }
    cfg_done = 1;
    cfg_states();
    set_base(NULL);
    card_setup(0);
    card_setup(1);
}

void grx_mc_init(void) {
    cfg_once();
}

bool grx_mc_set_root(const char *dir) {
    cfg_states();
    cfg_done = 1;
    set_base(dir);
    card_setup(0);
    card_setup(1);
    return host_exists(base_dir);
}

void grx_mc_set_card(int port, int state) {
    Card *c;
    cfg_once();
    if (port != 0 && port != 1) {
        return;
    }
    c = &cards[port];
    card_lose(port);
    c->state = state == GRX_MC_FORMATTED || state == GRX_MC_UNFORMATTED ? state : GRX_MC_ABSENT;
    strcpy(c->cwd, "/");
    meta_load(c);
    if (c->state == GRX_MC_FORMATTED && !host_exists(c->root)) {
        card_make_empty(c);
    }
}

int grx_mc_card_state(int port) {
    cfg_once();
    return port == 0 || port == 1 ? cards[port].state : GRX_MC_ABSENT;
}

const char *grx_mc_card_dir(int port) {
    cfg_once();
    return port == 0 || port == 1 ? cards[port].rootslash : "";
}

int grx_mc_free_clusters(int port) {
    cfg_once();
    if ((port != 0 && port != 1) || cards[port].state != GRX_MC_FORMATTED) {
        return 0;
    }
    return card_free(&cards[port]);
}

void grx_mc_shutdown(void) {
    int i;
    for (i = 0; i < MAX_FD; i++) {
        if (fds[i].used) {
            fd_drop(&fds[i]);
        }
    }
}

/* ------------------------------------------------------------------------ */
/* mcman's calls                                                            */

/* McDetectCard: the card for a call, or why there isn't one */
static int card_ready(int port, int slot, Card **out) {
    int i = card_index(port, slot);
    Card *c;
    if (i < 0) {
        return RES_FAILDETECT;
    }
    c = &cards[i];
    if (c->state == GRX_MC_ABSENT) {
        card_lose(i);
        return RES_FAILDETECT;
    }
    if (!c->probed) {
        /* mcman_detectcard's quick probe fails on a card the full probe
           (McDetectCard2) hasn't set up, and it forgets the card */
        card_lose(i);
        return RES_FAILDETECT2;
    }
    if (c->state == GRX_MC_UNFORMATTED) {
        return RES_NOFORMAT;
    }
    if (!host_exists(c->root)) {
        card_make_empty(c);
    }
    *out = c;
    return RES_OK;
}

/* XMCSERV GetInfo2: McDetectCard2, then type, free and format as asked */
static int mc_getinfo(int port, int slot, s32 *type, s32 *freep, s32 *format) {
    int i = card_index(port, slot), r, t = 0;
    Card *c = i >= 0 ? &cards[i] : NULL;
    if (!c || c->state == GRX_MC_ABSENT) {
        if (c) {
            card_lose(i);
        }
        r = RES_FAILDETECT;
    } else {
        t = 2;  /* sceMcTypePS2 */
        if (!c->probed) {
            c->probed = 1;
            strcpy(c->cwd, "/");
            r = c->state == GRX_MC_FORMATTED ? RES_CHANGED : RES_NOFORMAT;
        } else {
            r = RES_OK;
        }
    }
    if (type) {
        *type = t;
    }
    if (r < -1) {
        if (freep) {
            *freep = 0;
        }
        if (format) {
            *format = 0;
        }
        return r;
    }
    if (freep) {
        *freep = c->state == GRX_MC_FORMATTED ? card_free(c) : 0;
    }
    if (format) {
        *format = c->state == GRX_MC_FORMATTED;
    }
    return r;
}

static int fd_open_host(Fd *f, Card *c, const char *raw, const char *mode) {
    char key[HOST_PATH], host[HOST_PATH];
    path_key(raw, key, sizeof key);
    host_of_key(c, key, host, sizeof host);
    f->io = SDL_IOFromFile(host, mode);
    if (!f->io) {
        grx_log("mc: can't open %s: %s", host, SDL_GetError());
        return 0;
    }
    return 1;
}

/* mcman_open2 (and sceMcMkdir: flags 0x40) */
static int mc_open(int port, int slot, const char *name, int flags) {
    Card *c;
    Ent e, pe;
    Fd *f;
    char raw[PS2_PATH], praw[PS2_PATH], base[PS2_PATH], key[HOST_PATH];
    int r = card_ready(port, slot, &c), fd, i, exists;
    if (r != RES_OK) {
        return r;
    }
    if (flags & OPEN_CREAT_FILE) {
        flags |= ATTR_W;
    }
    if (name[0] == 0) {
        return RES_NOENTRY;
    }
    for (fd = 0; fd < MAX_FD && fds[fd].used; fd++) {
    }
    if (fd == MAX_FD) {
        return RES_UPLIMIT;
    }
    if (!path_resolve(c, name, raw)) {
        return RES_NOENTRY;
    }
    path_key(raw, key, sizeof key);
    exists = ent_get(c, raw, &e);
    f = &fds[fd];
    if (exists) {
        if (flags & OPEN_CREAT_DIR) {
            return RES_NOENTRY;
        }
        if ((flags & ATTR_W) && !(e.mode & ATTR_W)) {
            return RES_DENIED;
        }
        memset(f, 0, sizeof *f);
        f->port = port;
        snprintf(f->raw, sizeof f->raw, "%s", raw);
        if (e.is_dir) {
            /* a directory handle: no reading or writing */
            f->used = 1;
            f->is_dir = 1;
            f->size = (u32)dir_count(c, raw) + 2;
            return fd;
        }
        if (flags & ATTR_W) {
            for (i = 0; i < MAX_FD; i++) {
                if (fds[i].used && fds[i].port == port && fds[i].wr && !strcmp(fds[i].raw, raw)) {
                    return RES_DENIED;
                }
            }
        }
        if (flags & OPEN_CREAT_FILE) {
            /* the old entry goes, a new one takes its place */
            sceMcStDateTime now;
            path_split(raw, praw, base);
            if (!fd_open_host(f, c, raw, "w+b")) {
                return RES_DENIED;
            }
            time_now(&now);
            meta_set(c, key, (u16)(MODE_NEWFILE | (flags & (ATTR_HIDDEN | ATTR_PS1 | ATTR_PDAEXEC))), &now, &now);
            ent_touch(c, praw);
            meta_save(c);
            f->used = 1;
            f->rd = flags & ATTR_R;
            f->wr = 1;
            return fd;
        }
        f->rd = (flags & ATTR_R) && (e.mode & ATTR_R);
        f->wr = (flags & ATTR_W) && (e.mode & ATTR_W);
        if (!fd_open_host(f, c, raw, f->wr ? "r+b" : "rb")) {
            memset(f, 0, sizeof *f);
            return RES_DENIED;
        }
        f->used = 1;
        f->size = e.size;
        return fd;
    }

    /* A new entry */
    if (!strcmp(raw, "/")) {
        return RES_NOENTRY;
    }
    path_split(raw, praw, base);
    if (!ent_get(c, praw, &pe) || !pe.is_dir) {
        return RES_NOENTRY;
    }
    if (!(flags & (OPEN_CREAT_FILE | OPEN_CREAT_DIR)) || strlen(base) > NAME_MAX_PS2) {
        return RES_NOENTRY;
    }
    {
        /* the parent's new entry may need a cluster, a directory its own */
        int need = ((dir_count(c, praw) + 2) % 2 == 0) + ((flags & OPEN_CREAT_DIR) != 0);
        sceMcStDateTime now;
        u16 extra = (u16)(flags & (ATTR_HIDDEN | ATTR_PS1 | ATTR_PDAEXEC));
        char host[HOST_PATH];
        if (card_free(c) < need) {
            return RES_FULL;
        }
        time_now(&now);
        if (flags & OPEN_CREAT_DIR) {
            host_of_key(c, key, host, sizeof host);
            if (!SDL_CreateDirectory(host)) {
                grx_log("mc: can't create %s: %s", host, SDL_GetError());
                return RES_DENIED;
            }
            meta_set(c, key, (u16)(MODE_DIR | extra), &now, &now);
            ent_touch(c, praw);
            meta_save(c);
            return RES_OK;
        }
        memset(f, 0, sizeof *f);
        f->port = port;
        snprintf(f->raw, sizeof f->raw, "%s", raw);
        if (!fd_open_host(f, c, raw, "w+b")) {
            return RES_DENIED;
        }
        meta_set(c, key, (u16)(MODE_NEWFILE | extra), &now, &now);
        ent_touch(c, praw);
        meta_save(c);
        f->used = 1;
        f->rd = flags & ATTR_R;
        f->wr = 1;
        return fd;
    }
}

/* An open fd, or NULL (mcman: -5) */
static Fd *fd_get(int fd) {
    return fd >= 0 && fd < MAX_FD && fds[fd].used ? &fds[fd] : NULL;
}

/* McDetectCard for an fd's card */
static int fd_card(Fd *f, Card **c) {
    return card_ready(f->port, 0, c);
}

/* mcman_close2: the entry's length, modified time and closed bit; the
   parent's modified time */
static void fd_close2(Fd *f, Card *c) {
    Ent e;
    char key[HOST_PATH], praw[PS2_PATH], base[PS2_PATH];
    if (f->io) {
        SDL_FlushIO(f->io);
    }
    if (!ent_get(c, f->raw, &e)) {
        return;
    }
    path_key(f->raw, key, sizeof key);
    e.mode = f->dirty ? (u16)(e.mode & ~ATTR_CLOSED) : (u16)(e.mode | ATTR_CLOSED);
    time_now(&e.mtime);
    meta_set(c, key, e.mode, &e.ctime, &e.mtime);
    path_split(f->raw, praw, base);
    ent_touch(c, praw);
    meta_save(c);
}

static int mc_close(int fd) {
    Fd *f = fd_get(fd);
    Card *c;
    int r;
    if (!f) {
        return RES_DENIED;
    }
    r = fd_card(f, &c);
    if (r == RES_OK && f->dirty) {
        f->dirty = 0;
        fd_close2(f, c);
    }
    fd_drop(f);
    return r;
}

static int mc_flush(int fd) {
    Fd *f = fd_get(fd);
    Card *c;
    int r;
    if (!f) {
        return RES_DENIED;
    }
    r = fd_card(f, &c);
    if (r != RES_OK) {
        return r;
    }
    if (f->dirty) {
        f->dirty = 0;
        fd_close2(f, c);
    }
    return RES_OK;
}

static int mc_seek(int fd, int offset, int origin) {
    Fd *f = fd_get(fd);
    Card *c;
    int r;
    int64_t p;
    if (!f) {
        return RES_DENIED;
    }
    r = fd_card(f, &c);
    if (r != RES_OK) {
        return r;
    }
    p = origin == 0 ? offset : origin == 2 ? (int64_t)f->size + offset : (int64_t)f->pos + offset;
    if (p < 0) {
        p = 0;
    }
    if (p > INT_MAX) {
        p = INT_MAX;
    }
    f->pos = (u32)p;
    return (int)p;
}

static int mc_read(int fd, void *buf, int size) {
    Fd *f = fd_get(fd);
    Card *c;
    int r;
    u32 n;
    if (!f || !f->rd) {
        return RES_DENIED;
    }
    r = fd_card(f, &c);
    if (r != RES_OK) {
        return r;
    }
    if (size <= 0 || f->pos >= f->size) {
        return 0;
    }
    n = f->size - f->pos < (u32)size ? f->size - f->pos : (u32)size;
    if (SDL_SeekIO(f->io, f->pos, SDL_IO_SEEK_SET) < 0 || SDL_ReadIO(f->io, buf, n) != n) {
        grx_log("mc: read failed: %s", SDL_GetError());
        fd_drop(f);
        return RES_DENIED;
    }
    f->pos += n;
    return (int)n;
}

static int mc_write(int fd, const void *buf, int size) {
    Fd *f = fd_get(fd);
    Card *c;
    int r;
    if (!f || !f->wr) {
        return RES_DENIED;
    }
    r = fd_card(f, &c);
    if (r != RES_OK) {
        return r;
    }
    if (size > 0) {
        u32 end = f->pos + (u32)size;
        u32 have = (f->size + CLUSTER - 1) / CLUSTER;
        u32 want = ((end > f->size ? end : f->size) + CLUSTER - 1) / CLUSTER;
        if (!f->dirty) {
            /* the entry loses its closed bit until the file is closed */
            f->dirty = 1;
            fd_close2(f, c);
        }
        if (want > have && (int)(want - have) > card_free(c)) {
            /* mcman writes until the clusters run out, then gives up on
               the fd; the entry keeps the length it was last closed with */
            if (f->pos < f->size) {
                u32 n = f->size - f->pos < (u32)size ? f->size - f->pos : (u32)size;
                if (SDL_SeekIO(f->io, f->pos, SDL_IO_SEEK_SET) >= 0) {
                    SDL_WriteIO(f->io, buf, n);
                }
            }
            fd_drop(f);
            return RES_FULL;
        }
        if (SDL_SeekIO(f->io, f->pos, SDL_IO_SEEK_SET) < 0 || SDL_WriteIO(f->io, buf, (size_t)size) != (size_t)size) {
            grx_log("mc: write failed: %s", SDL_GetError());
            fd_drop(f);
            return RES_DENIED;
        }
        f->pos = end;
        if (end > f->size) {
            f->size = end;
        }
    }
    fd_close2(f, c);
    return size > 0 ? size : 0;
}

static int mc_chdir(int port, int slot, const char *name, char *out) {
    Card *c;
    Ent e;
    char raw[PS2_PATH];
    int r = card_ready(port, slot, &c);
    if (r != RES_OK) {
        return r;
    }
    if (!path_resolve(c, name, raw) || !ent_get(c, raw, &e) || !e.is_dir) {
        return RES_NOENTRY;
    }
    strcpy(c->cwd, raw);
    if (out) {
        size_t n = strlen(raw) < 0x400 ? strlen(raw) : 0x3FF;
        memcpy(out, raw, n);
        out[n] = 0;
    }
    return RES_OK;
}

/* mcman_checkdirpath: '?' matches one character (or the end of the
   name), '*' any run */
static int wild_match(const char *name, const char *pat) {
    for (;;) {
        size_t k = strcspn(pat, "?*");
        if (pat[k] == 0) {
            return strcmp(pat, name) == 0;
        }
        if (strncmp(pat, name, k) != 0) {
            return 0;
        }
        pat += k, name += k;
        while (*pat == '?') {
            if (*name == 0) {
                return 1;
            }
            pat++, name++;
        }
        if (*pat == '*') {
            break;
        }
    }
    while (*pat == '*' || *pat == '?') {
        pat++;
    }
    if (*pat == 0) {
        return 1;
    }
    for (;;) {
        const char *q = strchr(name, *pat);
        if (!q) {
            return 0;
        }
        if (wild_match(q, pat)) {
            return 1;
        }
        name = q + 1;
    }
}

static void tbl_from_ent(const Ent *e, sceMcTblGetDir *t) {
    memset(t, 0, sizeof *t);
    t->_Create = e->ctime;
    t->_Modify = e->mtime;
    t->FileSizeByte = (e->mode & ATTR_SUBDIR) ? 0 : e->size;
    t->AttrFile = e->mode;
    strncpy(t->EntryName, e->name, sizeof t->EntryName);
}

/* mcman_getdir2 */
static int mc_getdir(int port, int slot, const char *name, int mode, int maxent, sceMcTblGetDir *table) {
    Card *c;
    int r = card_ready(port, slot, &c), n = 0;
    if (r != RES_OK) {
        return r;
    }
    if ((mode & 0xFFFF) == 0) {
        char dir[PS2_PATH * 2], raw[PS2_PATH], pat[PS2_PATH];
        const char *slash = strrchr(name, '/');
        Ent self, *v = NULL;
        int cnt, i, k = 0;
        free(listing.v);
        memset(&listing, 0, sizeof listing);
        if (!slash) {
            dir[0] = 0;
            snprintf(pat, sizeof pat, "%s", name);
        } else {
            size_t len = (size_t)(slash - name);
            if (len == 0) {
                strcpy(dir, "/");
            } else {
                memcpy(dir, name, len);
                dir[len] = 0;
            }
            snprintf(pat, sizeof pat, "%s", slash + 1);
        }
        if (!path_resolve(c, dir, raw) || !ent_get(c, raw, &self) || !self.is_dir) {
            return RES_NOENTRY;
        }
        cnt = ent_list(c, raw, &v);
        if (cnt < 0) {
            return RES_NOENTRY;
        }
        listing.v = calloc((size_t)cnt + 2, sizeof *listing.v);
        if (!listing.v) {
            free(v);
            return RES_NOENTRY;
        }
        if (strcmp(raw, "/") != 0) {
            /* "." is the directory's own entry, ".." its parent's */
            char praw[PS2_PATH], base[PS2_PATH];
            Ent parent;
            if (wild_match(".", pat)) {
                listing.v[k] = self;
                strcpy(listing.v[k++].name, ".");
            }
            path_split(raw, praw, base);
            if (wild_match("..", pat) && ent_get(c, praw, &parent)) {
                listing.v[k] = parent;
                strcpy(listing.v[k++].name, "..");
            }
        }
        for (i = 0; i < cnt; i++) {
            if (wild_match(v[i].name, pat)) {
                listing.v[k++] = v[i];
            }
        }
        free(v);
        listing.n = k;
        listing.valid = 1;
        listing.port = port;
    } else if (!listing.valid || listing.port != port) {
        return RES_NOENTRY;
    }
    while (maxent != 0 && listing.pos < listing.n) {
        tbl_from_ent(&listing.v[listing.pos++], table++);
        n++;
        maxent--;
    }
    return n;
}

static int mc_delete(int port, int slot, const char *name) {
    Card *c;
    Ent e;
    char raw[PS2_PATH], key[HOST_PATH], host[HOST_PATH];
    int r = card_ready(port, slot, &c), i;
    if (r != RES_OK) {
        return r;
    }
    if (!path_resolve(c, name, raw) || !strcmp(raw, "/") || !ent_get(c, raw, &e)) {
        return RES_NOENTRY;
    }
    if (e.is_dir && dir_count(c, raw) > 0) {
        return RES_NOTEMPTY;
    }
    /* mcman leaves fds on a deleted file pointing at nothing */
    for (i = 0; i < MAX_FD; i++) {
        if (fds[i].used && fds[i].port == port && !strcmp(fds[i].raw, raw)) {
            fd_drop(&fds[i]);
        }
    }
    path_key(raw, key, sizeof key);
    host_of_key(c, key, host, sizeof host);
    if (!SDL_RemovePath(host)) {
        grx_log("mc: can't delete %s: %s", host, SDL_GetError());
        return RES_DENIED;
    }
    meta_del(c, key);
    meta_save(c);
    return RES_OK;
}

/* mcman_setinfo2: flags 1 created, 2 modified, 4 attributes, 0x10 name */
static int mc_setinfo(int port, int slot, const char *name, const sceMcTblGetDir *info, int flags) {
    Card *c;
    Ent e;
    char raw[PS2_PATH], key[HOST_PATH];
    int r = card_ready(port, slot, &c);
    if (r != RES_OK) {
        return r;
    }
    if (!path_resolve(c, name, raw) || !ent_get(c, raw, &e)) {
        return RES_NOENTRY;
    }
    path_key(raw, key, sizeof key);
    if (flags & ATTR_FILE) {
        char praw[PS2_PATH], base[PS2_PATH], nraw[PS2_PATH], nkey[HOST_PATH], host[HOST_PATH], nhost[HOST_PATH];
        char newname[NAME_MAX_PS2 + 1];
        Ent other;
        snprintf(newname, sizeof newname, "%.*s", NAME_MAX_PS2, info->EntryName);
        if (!strcmp(raw, "/") || !newname[0] || !strcmp(newname, ".") || !strcmp(newname, "..") ||
            strchr(newname, '/')) {
            return RES_NOENTRY;
        }
        path_split(raw, praw, base);
        path_join(praw, newname, nraw, sizeof nraw);
        if (ent_get(c, nraw, &other)) {
            return RES_NOENTRY;
        }
        path_key(nraw, nkey, sizeof nkey);
        host_of_key(c, key, host, sizeof host);
        host_of_key(c, nkey, nhost, sizeof nhost);
        if (!SDL_RenamePath(host, nhost)) {
            grx_log("mc: can't rename %s: %s", host, SDL_GetError());
            return RES_DENIED;
        }
        meta_set(c, key, e.mode, &e.ctime, &e.mtime);
        meta_rename(c, key, nkey);
        strcpy(key, nkey);
        for (r = 0; r < MAX_FD; r++) {
            if (fds[r].used && fds[r].port == port && !strcmp(fds[r].raw, raw)) {
                snprintf(fds[r].raw, sizeof fds[r].raw, "%s", nraw);
            }
        }
    }
    if (flags & ATTR_X) {
        e.mode = (u16)((e.mode & ~SETINFO_ATTR_MASK) | (info->AttrFile & SETINFO_ATTR_MASK));
    }
    if (flags & ATTR_R) {
        e.ctime = info->_Create;
    }
    if (flags & ATTR_W) {
        e.mtime = info->_Modify;
    }
    meta_set(c, key, e.mode, &e.ctime, &e.mtime);
    meta_save(c);
    return RES_OK;
}

/* mcman_getentspace: free entries in the directory's last cluster */
static int mc_entspace(int port, int slot, const char *name) {
    Card *c;
    Ent e;
    char raw[PS2_PATH];
    int r = card_ready(port, slot, &c);
    if (r != RES_OK) {
        return r;
    }
    if (!path_resolve(c, name, raw) || !ent_get(c, raw, &e)) {
        return RES_NOENTRY;
    }
    return e.is_dir ? (dir_count(c, raw) + 2) & 1 : 0;
}

static int mc_format(int port, int slot, int unformat) {
    int i = card_index(port, slot), j;
    Card *c;
    if (i < 0) {
        return RES_FAILDETECT;
    }
    c = &cards[i];
    for (j = 0; j < MAX_FD; j++) {
        if (fds[j].used && fds[j].port == i) {
            fd_drop(&fds[j]);
        }
    }
    if (c->state == GRX_MC_ABSENT) {
        card_lose(i);
        return RES_FAILDETECT;
    }
    if (!c->probed) {
        card_lose(i);
        return RES_FAILDETECT2;
    }
    if (listing.valid && listing.port == i) {
        listing.valid = 0;
    }
    host_rmtree(c->root);
    strcpy(c->cwd, "/");
    if (unformat) {
        meta_clear(c);
        SDL_RemovePath(c->meta_file);
        c->state = GRX_MC_UNFORMATTED;
        c->probed = 0;
        return RES_OK;
    }
    if (!card_make_empty(c)) {
        return RES_FAILDETECT2;
    }
    c->state = GRX_MC_FORMATTED;
    return RES_OK;
}

/* ------------------------------------------------------------------------ */
/* libmc                                                                    */

static int lib_inited;

/* The command in flight: what libmc put in its RPC block */
static struct {
    int cmd;
    int port, slot, fd, a, b;
    void *p;
    s32 *type, *freep, *format;
    char name[PS2_PATH];
    sceMcTblGetDir info;
} pend;

static int lib_begin(int cmd, int port, int slot) {
    if (pend.cmd) {
        return LIB_BUSY;
    }
    if (!lib_inited) {
        return LIB_NOINIT;
    }
    memset(&pend, 0, sizeof pend);
    pend.port = port;
    pend.slot = slot;
    (void)cmd;
    return 0;
}

static int lib_name(const char *name) {
    if (!name || !*name) {
        return LIB_BADNAME;
    }
    strncpy(pend.name, name, PS2_PATH - 1);
    pend.name[PS2_PATH - 1] = 0;
    return 0;
}

static int lib_send(int cmd) {
    pend.cmd = cmd;
    return 0;
}

static int lib_run(void) {
    switch (pend.cmd) {
    case CMD_GETINFO:
        return mc_getinfo(pend.port, pend.slot, pend.type, pend.freep, pend.format);
    case CMD_OPEN:
        return mc_open(pend.port, pend.slot, pend.name, pend.a);
    case CMD_MKDIR:
        return mc_open(pend.port, pend.slot, pend.name, OPEN_CREAT_DIR);
    case CMD_CLOSE:
        return mc_close(pend.fd);
    case CMD_SEEK:
        return mc_seek(pend.fd, pend.a, pend.b);
    case CMD_READ:
        return mc_read(pend.fd, pend.p, pend.a);
    case CMD_WRITE:
        return mc_write(pend.fd, pend.p, pend.a);
    case CMD_FLUSH:
        return mc_flush(pend.fd);
    case CMD_CHDIR:
        return mc_chdir(pend.port, pend.slot, pend.name, pend.p);
    case CMD_GETDIR:
        return mc_getdir(pend.port, pend.slot, pend.name, pend.a, pend.b, pend.p);
    case CMD_SETINFO:
    case CMD_RENAME:
        return mc_setinfo(pend.port, pend.slot, pend.name, &pend.info, pend.a);
    case CMD_DELETE:
        return mc_delete(pend.port, pend.slot, pend.name);
    case CMD_FORMAT:
        return mc_format(pend.port, pend.slot, 0);
    case CMD_UNFORMAT:
        return mc_format(pend.port, pend.slot, 1);
    case CMD_ENTSPACE:
        return mc_entspace(pend.port, pend.slot, pend.name);
    case CMD_THREADPRIO:
        return RES_OK;
    }
    return RES_NOENTRY;
}

/* The game's prototypes (include/protos/main.h) */
s32 sceMcInit(void);
s32 sceMcSync(s32 mode, s32 *cmd, s32 *result);
s32 sceMcGetSlotMax(s32 port);
s32 sceMcChangeThreadPriority(s32 prio);
s32 sceMcGetInfo(s32 port, s32 slot, s32 *type, s32 *free, s32 *format);
s32 sceMcOpen(s32 port, s32 slot, const char *name, s32 mode);
s32 sceMcMkdir(s32 port, s32 slot, s32 name);
s32 sceMcClose(s32 fd);
s32 sceMcSeek(s32 fd, s32 offset, s32 origin);
s32 sceMcRead(s32 fd, void *buf, s32 size);
s32 sceMcWrite(s32 fd, u8 *buf, s32 size);
s32 sceMcFlush(s32 fd);
s32 sceMcChdir(s32 port, s32 slot, const char *name, void *curdir);
s32 sceMcGetDir(s32 port, s32 slot, const char *name, s32 mode, s32 maxent, void *table);
s32 sceMcSetFileInfo(s32 port, s32 slot, const char *name, struct sceMcTblGetDir *info, u32 flags);
s32 sceMcDelete(s32 port, s32 slot, const char *name);
s32 sceMcFormat(s32 port, s32 slot);
s32 sceMcUnformat(s32 port, s32 slot);
s32 sceMcGetEntSpace(s32 port, s32 slot, const char *name);
s32 sceMcRename(s32 port, s32 slot, const char *name, const char *newname);

s32 sceMcInit(void) {
    grx_mc_init();
    sceMcSync(0, NULL, NULL);
    lib_inited = 1;
    return 0;
}

/* Mode 0 waits, mode 1 polls; either way the command has finished */
s32 sceMcSync(s32 mode, s32 *cmd, s32 *result) {
    int no = pend.cmd, r;
    (void)mode;
    if (no == 0) {
        return -1;
    }
    r = lib_run();
    pend.cmd = 0;
    if (cmd) {
        *cmd = no;
    }
    if (result) {
        *result = r;
    }
    return 1;
}

s32 sceMcGetSlotMax(s32 port) {
    int r = lib_begin(0, port, 0);
    (void)port;
    return r ? r : 1;
}

s32 sceMcChangeThreadPriority(s32 prio) {
    int r = lib_begin(CMD_THREADPRIO, 0, 0);
    (void)prio;
    return r ? r : lib_send(CMD_THREADPRIO);
}

s32 sceMcGetInfo(s32 port, s32 slot, s32 *type, s32 *free, s32 *format) {
    int r = lib_begin(CMD_GETINFO, port, slot);
    if (r) {
        return r;
    }
    pend.type = type;
    pend.freep = free;
    pend.format = format;
    return lib_send(CMD_GETINFO);
}

s32 sceMcOpen(s32 port, s32 slot, const char *name, s32 mode) {
    int r = lib_begin(CMD_OPEN, port, slot);
    if (r || (r = lib_name(name))) {
        return r;
    }
    pend.a = mode;
    return lib_send(CMD_OPEN);
}

s32 sceMcMkdir(s32 port, s32 slot, s32 name) {
    int r = lib_begin(CMD_MKDIR, port, slot);
    if (r || (r = lib_name((const char *)(uintptr_t)(u32)name))) {
        return r;
    }
    return lib_send(CMD_MKDIR);
}

static s32 lib_fd_call(int cmd, s32 fd, int a, int b, void *p) {
    int r = lib_begin(cmd, 0, 0);
    if (r) {
        return r;
    }
    pend.fd = fd;
    pend.a = a;
    pend.b = b;
    pend.p = p;
    return lib_send(cmd);
}

s32 sceMcClose(s32 fd) {
    return lib_fd_call(CMD_CLOSE, fd, 0, 0, NULL);
}

s32 sceMcSeek(s32 fd, s32 offset, s32 origin) {
    return lib_fd_call(CMD_SEEK, fd, offset, origin, NULL);
}

s32 sceMcRead(s32 fd, void *buf, s32 size) {
    return lib_fd_call(CMD_READ, fd, size, 0, buf);
}

s32 sceMcWrite(s32 fd, u8 *buf, s32 size) {
    return lib_fd_call(CMD_WRITE, fd, size, 0, buf);
}

s32 sceMcFlush(s32 fd) {
    return lib_fd_call(CMD_FLUSH, fd, 0, 0, NULL);
}

s32 sceMcChdir(s32 port, s32 slot, const char *name, void *curdir) {
    int r = lib_begin(CMD_CHDIR, port, slot);
    if (r || (r = lib_name(name))) {
        return r;
    }
    pend.p = curdir;
    return lib_send(CMD_CHDIR);
}

s32 sceMcGetDir(s32 port, s32 slot, const char *name, s32 mode, s32 maxent, void *table) {
    int r = lib_begin(CMD_GETDIR, port, slot);
    if (r || (r = lib_name(name))) {
        return r;
    }
    pend.a = mode;
    pend.b = maxent;
    pend.p = table;
    return lib_send(CMD_GETDIR);
}

s32 sceMcSetFileInfo(s32 port, s32 slot, const char *name, struct sceMcTblGetDir *info, u32 flags) {
    int r = lib_begin(CMD_SETINFO, port, slot);
    if (r || (r = lib_name(name))) {
        return r;
    }
    /* libmc passes only the created/modified/attribute bits */
    pend.a = (int)(flags & 7);
    pend.info = *info;
    return lib_send(CMD_SETINFO);
}

s32 sceMcRename(s32 port, s32 slot, const char *name, const char *newname) {
    int r = lib_begin(CMD_RENAME, port, slot);
    if (r || (r = lib_name(name))) {
        return r;
    }
    if (!newname) {
        return LIB_BADNAME;
    }
    pend.a = ATTR_FILE;
    strncpy(pend.info.EntryName, newname, sizeof pend.info.EntryName);
    pend.info.EntryName[sizeof pend.info.EntryName - 1] = 0;
    return lib_send(CMD_RENAME);
}

s32 sceMcDelete(s32 port, s32 slot, const char *name) {
    int r = lib_begin(CMD_DELETE, port, slot);
    if (r || (r = lib_name(name))) {
        return r;
    }
    return lib_send(CMD_DELETE);
}

s32 sceMcFormat(s32 port, s32 slot) {
    int r = lib_begin(CMD_FORMAT, port, slot);
    return r ? r : lib_send(CMD_FORMAT);
}

s32 sceMcUnformat(s32 port, s32 slot) {
    int r = lib_begin(CMD_UNFORMAT, port, slot);
    return r ? r : lib_send(CMD_UNFORMAT);
}

s32 sceMcGetEntSpace(s32 port, s32 slot, const char *name) {
    int r = lib_begin(CMD_ENTSPACE, port, slot);
    if (r || (r = lib_name(name))) {
        return r;
    }
    return lib_send(CMD_ENTSPACE);
}

/* ------------------------------------------------------------------------ */
/* Save states (grx_state_parts.h). The cards are host directories and
   stay as they are (as on an emulator, loading a state doesn't undo a
   save on the card); libmc's state, its call in flight (run at its
   sceMcSync) and what the game has seen of each card are kept. Not while
   a file on a card is open. A directory listing kept for its next part is
   this run's: dropped by a load. */
const char *grx_mc_state_blocker(void) {
    int i;
    for (i = 0; i < MAX_FD; i++) {
        if (fds[i].used) {
            return "the memory card is busy";
        }
    }
    return NULL;
}

void grx_mc_state_io(GrxState *s) {
    int i;
    GRX_STATE_VAR(s, lib_inited);
    GRX_STATE_VAR(s, pend);
    for (i = 0; i < 2; i++) {
        GRX_STATE_VAR(s, cards[i].state);
        GRX_STATE_VAR(s, cards[i].probed);
        GRX_STATE_VAR(s, cards[i].cwd);
    }
    if (grx_state_loading(s) && listing.valid) {
        free(listing.v);
        listing.v = NULL;
        listing.valid = 0;
    }
}
