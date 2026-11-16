/*
 * SIF, the EE's link to the IOP, without an IOP (port/include/grx_sif.h).
 *
 * Replaces Sony's SIF libraries in src/main (port/tools/fixups/sif.py):
 * sifcmd 48E078-48E5A8, sifrpc 48E7C0-48F6E0, loadfile 492DB8-493A38 and
 * the IOP reset calls 493B18-493CB8, plus the kernel's SIF calls that are
 * syscall stubs in the matching build (sceSifSetDma, sceSifDmaStat,
 * sceSifSetDChain, sceSifSetReg/GetReg, sceSifStopDma,
 * sceSifWriteBackDCache). fileio (48F720-492BF0), libmc and libpad stay
 * the game's code on top of these calls.
 *
 * How the IOP is stood in for:
 *
 *  - Rebooting it (sceSifRebootIop with IOPRP234.IMG) and syncing with it
 *    succeed at once. The reboot forgets the loaded modules.
 *  - Loading a module (sceSifLoadModule "cdrom0:\\NAME.IRX;1") succeeds
 *    without reading anything and makes the module's RPC servers exist.
 *    GS_IOP.IRX's driver starts with the one call to its RPC server
 *    (SysIopRpcBind 45F0B8), as on the PS2: that writes its header to the
 *    IOP block (EE 0x01FF0000 on the PS2, grx_iop_block() here; cdvd.c's
 *    grx_gsiop_boot), which LoadIopModules (45EEB8) then polls for.
 *  - RPC: sceSifBindRpc to a server some loaded module provides fills in
 *    the client at once (serve != 0, which every library waits for).
 *    sceSifCallRpc runs the server's handler (grx_sif_set_rpc_handler) on
 *    the send buffer and has the reply in the receive buffer when it
 *    returns, nowait or not; the end function runs before it returns.
 *    sceSifCheckStatRpc always says done. Answered so far: GS_IOP's own
 *    server (one call at boot) and mcserv, as "no memory card". The
 *    others (fileio, padman, ...) bind, and their first call stops the
 *    game with the server's name; libpad and libcdvd are replaced above
 *    the RPC level, so padman and cdvd are never bound.
 *  - GRX_SIF_TRACE=1 logs module loads, binds and calls.
 *  - SIF DMA (sceSifSetDma) is done before it returns: GS_IOP's command
 *    buffer goes to cdvd.c (grx_gsiop_dma), other IOP addresses to the
 *    targets set with grx_sif_add_dma_target. sceSifDmaStat is always
 *    "finished". IOP-to-EE reads (sceSifGetOtherData) are in cdvd.c.
 *  - Raw SIF commands (sceSifSendCmd) have no receiver: a stop.
 *
 * Game-named functions: no <windows.h> here.
 */
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "grx_disc.h"
#include "grx_hwmem.h"
#include "grx_platform.h"
#include "grx_sif.h"
#include "grx_state_parts.h"

/* EE addresses as the game passes them are host pointers (i686), except
   a PS2 address in the IOP block, should one still arrive untranslated */
static void *ee_ptr(uint32_t addr) {
    return (void *)grx_iop_addr(addr);
}

/* GRX_SIF_TRACE=1 logs module loads, binds and calls */
static int trace_on(void) {
    static int on = -1;

    if (on < 0) {
        const char *e = getenv("GRX_SIF_TRACE");
        on = e != NULL && *e != 0 && *e != '0';
    }
    return on;
}
#define TRACE(...) do { if (trace_on()) grx_log(__VA_ARGS__); } while (0)

/* Logs a message the first n times only */
#define LOG_FEW(counter, n, ...) do { if ((counter) < (n)) { (counter)++; grx_log(__VA_ARGS__); } } while (0)

/* ------------------------------------------------------------------------ */
/* SDK structures (include/sdk/sif.h), with the port's own types           */

/* Their pointer fields are 32-bit EE addresses: the game's own view of
   them is 32 bits on every target (port/docs/win64.md) */

/* sceSifRpcData */
typedef struct {
    uint32_t paddr;        /* the request packet: 0 = no call pending */
    uint32_t pid;
    int32_t tid;
    uint32_t mode;
} SifRpcData;

/* sceSifClientData */
typedef struct {
    SifRpcData rpcd;
    uint32_t command;
    uint32_t buff;         /* the server's receive buffer (IOP address) */
    uint32_t cbuff;
    uint32_t func;         /* end function of the last call */
    uint32_t para;
    uint32_t serve;        /* the server (its sceSifServeData); 0 until bound */
} SifClientData;
_Static_assert(sizeof(SifClientData) == 0x28, "sceSifClientData");

/* sceSifDmaData */
typedef struct {
    uint32_t data;         /* EE source */
    uint32_t addr;         /* IOP destination */
    uint32_t size;
    uint32_t mode;
} SifDmaData;
_Static_assert(sizeof(SifDmaData) == 0x10, "sceSifDmaData");

/* ------------------------------------------------------------------------ */
/* IOP modules and their RPC servers                                        */

static int gsiop_rpc(uint32_t fno, void *send, int32_t ssize, void *recv, int32_t rsize);
static int mcserv_rpc(uint32_t fno, void *send, int32_t ssize, void *recv, int32_t rsize);

/* The servers the game's libraries bind. module NULL: part of the IOP's
   boot image (IOPRP234.IMG), there from the start. */
typedef struct {
    uint32_t sid;
    const char *name;
    const char *module;
    GrxSifRpcHandler fn;
    uint8_t serve[0x44];   /* what client->serve points at (a sceSifServeData's size) */
} Server;

static Server servers[] = {
    { 0x80000001, "fileio", NULL, NULL, {0} },
    { 0x80000003, "iopheap", NULL, NULL, {0} },
    { 0x80000006, "loadfile", NULL, NULL, {0} },
    { 0x80000592, "cdvd (sceCdInit)", NULL, NULL, {0} },
    { 0x80000593, "cdvd (S commands)", NULL, NULL, {0} },
    { 0x80000595, "cdvd (N commands)", NULL, NULL, {0} },
    { 0x80000596, "cdvd (search file)", NULL, NULL, {0} },
    { 0x80000597, "cdvd (disk ready)", NULL, NULL, {0} },
    { 0x80000100, "padman", "PADMAN.IRX", NULL, {0} },
    { 0x80000101, "padman (ext)", "PADMAN.IRX", NULL, {0} },
    /* libmc (46FE20-4714C0); "no card" until there is memory card emulation */
    { 0x80000400, "mcserv", "MCSERV.IRX", mcserv_rpc, {0} },
    /* GS_IOP.IRX, the game's sound/stream driver: SysIopRpcBind 45F0B8 */
    { 0x00415249, "GS_IOP", "GS_IOP.IRX", gsiop_rpc, {0} },
};
#define NSERVERS ((int)(sizeof(servers) / sizeof(servers[0])))

/* Fake IOP address of server i's receive buffer (client->buff): nothing
   on the EE reads it, a DMA there is reported */
#define SERVER_BUFF(i) (0x001E0000u + (uint32_t)(i) * 0x1000u)

#define MAX_MODULES 32
static char modules[MAX_MODULES][32];
static int nmodules;
static int next_module_id = 1;

/* "cdrom0:\\GS_IOP.IRX;1" -> "GS_IOP.IRX", upper case */
static void module_base(const char *path, char *out, size_t n) {
    const char *b = path + strlen(path);
    size_t k = 0;

    while (b > path && b[-1] != '\\' && b[-1] != '/' && b[-1] != ':') {
        b--;
    }
    while (*b != 0 && *b != ';' && k + 1 < n) {
        out[k++] = (char)toupper((unsigned char)*b++);
    }
    out[k] = 0;
}

int grx_sif_module_loaded(const char *name) {
    char want[32];
    int i;

    module_base(name, want, sizeof(want));
    for (i = 0; i < nmodules; i++) {
        if (strcmp(modules[i], want) == 0) {
            return 1;
        }
    }
    return 0;
}

static Server *server_find(uint32_t sid) {
    int i;

    for (i = 0; i < NSERVERS; i++) {
        if (servers[i].sid == sid) {
            return &servers[i];
        }
    }
    return NULL;
}

static Server *server_of(const void *serve) {
    int i;

    for (i = 0; i < NSERVERS; i++) {
        if (serve == servers[i].serve) {
            return &servers[i];
        }
    }
    return NULL;
}

void grx_sif_set_rpc_handler(uint32_t sid, GrxSifRpcHandler fn) {
    Server *s = server_find(sid);

    if (s == NULL) {
        grx_fatal("grx_sif_set_rpc_handler: IOP RPC server %08X isn't in sif.c's table", sid);
    }
    s->fn = fn;
}

/* GS_IOP's RPC server. GS_IOP.IRX's start (create_th) only registers it
   (0x415249, function main2, receive buffer 0x10000-0x7900); main2 is the
   whole driver: it writes the header (magic 0x5A5AA5A5 last) to EE
   0x01FF0000, starts its threads and serves from then on, without
   replying. SysIopRpcBind (45F0B8) calls it once, fno 0, nowait, with 0x20
   zero bytes (sysIopRpcSendData is never written) and a 0x20-byte reply
   nothing reads, and LoadIopModules then polls for the magic. So this call
   is where the driver's emulation starts (cdvd.c's grx_gsiop_boot, on the
   port's IOP block); the reply stays zeros. */
static int gsiop_rpc(uint32_t fno, void *send, int32_t ssize, void *recv, int32_t rsize) {
    (void)send; (void)ssize;
    if (fno != 0) {
        return -1;
    }
    if (recv != NULL && rsize > 0) {
        memset(recv, 0, (size_t)rsize);
    }
    grx_gsiop_boot(grx_iop_block());   /* EE 0x01FF0000 on the PS2 */
    return 0;
}

/* mcserv (MCSERV.IRX) with no memory card in either slot. libmc's calls:
   0xFE init (sceMcInit 46FE20) gets the module versions, which it checks
   against 0x20A and 0x20E; 1 GetInfo (sceMcGetInfo 470958) gets "no card",
   which the game's McUpdate (454BE8) keeps polling for; anything else
   (open, read, write, dir, format, ...) also fails with "no card". The
   reply is the result word at _sceMcRecvBuf+0. A memory card emulation
   replaces this with grx_sif_set_rpc_handler(0x80000400, ...). */
#define MC_RES_NO_CARD (-10)

static int mcserv_rpc(uint32_t fno, void *send, int32_t ssize, void *recv, int32_t rsize) {
    int32_t *r = recv;

    if (r == NULL || rsize < 4) {
        return -1;
    }
    if (fno == 0xFE) {
        if (rsize < 12) {
            return -1;
        }
        r[0] = 0;
        r[1] = 0x20A;          /* mcserv version */
        r[2] = 0x20E;          /* mcman version */
        return 0;
    }
    if (fno == 1 && send != NULL && ssize >= 0x20) {
        /* SceMcDescParam.param (+0x1C): the EE buffer the IOP would DMA
           type (+0), free KB (+4) and format (+0x90) to; the end function
           _sceMcGetInfoApdx copies them out */
        /* a 32-bit EE address on every target (port/docs/win64.md) */
        uint8_t *info = (uint8_t *)(uintptr_t)*(const uint32_t *)((uint8_t *)send + 0x1C);
        if (info != NULL) {
            memset(info, 0, 4);
            memset(info + 4, 0, 4);
            memset(info + 0x90, 0, 4);
        }
    }
    {
        static int logged;
        if (fno != 1) {
            LOG_FEW(logged, 8, "sif: mcserv call %u: no memory card", fno);
        }
    }
    r[0] = MC_RES_NO_CARD;
    return 0;
}

/* ------------------------------------------------------------------------ */
/* Kernel SIF calls (syscall stubs in the matching build)                   */

/* SIF registers: 1 MAINADDR, 2 SUBADDR, 3 MSFLAG, 4 SMFLAG; the kernel's
   software registers 0x80000000-0x8000001F (the libraries' control
   blocks) */
#define SIF_REG_SMFLAG 4
#define SIF_STAT_BOOTED 0x70000u   /* SIFINIT | CMDINIT | BOOTEND */
static uint32_t sif_regs[64] = { [SIF_REG_SMFLAG] = SIF_STAT_BOOTED };

static uint32_t *sif_reg(uint32_t reg) {
    return &sif_regs[(reg & 0x80000000u) ? 32 + (reg & 31) : (reg & 31)];
}

uint32_t sceSifSetReg(uint32_t reg, uint32_t value) {
    uint32_t *r = sif_reg(reg);
    uint32_t old = *r;

    *r = value;
    return old;
}

uint32_t sceSifGetReg(uint32_t reg) {
    return *sif_reg(reg);
}

/* The SIF0 receive chain (IOP-to-EE DMA): there is none */
void sceSifSetDChain(void) {
}

void sceSifStopDma(void) {
}

/* No cache to write back */
void sceSifWriteBackDCache(void *addr, int32_t size) {
    (void)addr; (void)size;
}

#define MAX_DMA_TARGETS 8
static struct {
    uint32_t lo, hi;
    GrxSifDmaHandler fn;
} dma_targets[MAX_DMA_TARGETS];
static int ndma_targets;

void grx_sif_add_dma_target(uint32_t lo, uint32_t hi, GrxSifDmaHandler fn) {
    if (ndma_targets == MAX_DMA_TARGETS) {
        grx_fatal("grx_sif_add_dma_target: too many targets");
    }
    dma_targets[ndma_targets].lo = lo;
    dma_targets[ndma_targets].hi = hi;
    dma_targets[ndma_targets].fn = fn;
    ndma_targets++;
}

static int dma_deliver(uint32_t addr, const void *src, uint32_t size) {
    int i;

    if (grx_gsiop_dma(addr, src, size)) {
        return 1;
    }
    for (i = 0; i < ndma_targets; i++) {
        if (addr >= dma_targets[i].lo && addr < dma_targets[i].hi && dma_targets[i].fn(addr, src, size)) {
            return 1;
        }
    }
    return 0;
}

/* (descriptors, count) - a transfer id, 0 = refused. Done before it
   returns; isceSifSetDma (port/game/main/48C430.c) comes here too. */
static uint32_t sif_dma_id;   /* the last transfer id (save states) */

uint32_t sceSifSetDma(SifDmaData *sdd, int32_t len) {
    static int unhandled;
    int32_t i;

    for (i = 0; i < len; i++) {
        if (!dma_deliver(sdd[i].addr, ee_ptr(sdd[i].data), sdd[i].size)) {
            LOG_FEW(unhandled, 16, "sif: DMA of %u bytes to IOP %08X not emulated, dropped", sdd[i].size, sdd[i].addr);
        }
    }
    if (++sif_dma_id == 0) {
        sif_dma_id = 1;
    }
    return sif_dma_id;
}

/* < 0: finished, which every transfer is */
int32_t sceSifDmaStat(uint32_t id) {
    (void)id;
    return -1;
}

/* ------------------------------------------------------------------------ */
/* sifcmd (48E078-48E5A8)                                                   */

typedef void (*CmdHandler)(void *pkt, void *arg);
static struct {
    CmdHandler fn;
    void *arg;
} cmd_handlers[2][32];     /* [0] system (cid 0x80000000+), [1] user */
static int32_t sif_sregs[32];

/* The command layer's control block (sceSifCmdCtrl): only for
   sceSifGetDataTable; nothing in it is used */
static uint32_t cmd_ctrl[8];

void sceSifInitCmd(void) {
}

void sceSifExitCmd(void) {
}

void *sceSifGetDataTable(void) {
    return cmd_ctrl;
}

int32_t sceSifGetSreg(int32_t i) {
    return sif_sregs[i & 31];
}

int32_t sceSifSetSreg(int32_t i, int32_t value) {
    int32_t old = sif_sregs[i & 31];

    sif_sregs[i & 31] = value;
    return old;
}

/* (handler table, count): the old table. The port keeps its own. */
int32_t sceSifSetCmdBuffer(int32_t table, int32_t n) {
    (void)table; (void)n;
    return 0;
}

int32_t sceSifSetSysCmdBuffer(int32_t table, int32_t n) {
    (void)table; (void)n;
    return 0;
}

/* The IOP never sends commands, so these handlers are only recorded
   (fileio's end-of-call and callback handlers, sceFsInit 48FD88) */
void sceSifAddCmdHandler(int32_t cid, int32_t handler, int32_t arg) {
    int sys = (cid & 0x80000000) != 0;

    cmd_handlers[!sys][cid & 31].fn = (CmdHandler)(uintptr_t)handler;
    cmd_handlers[!sys][cid & 31].arg = (void *)(uintptr_t)arg;
}

void sceSifRemoveCmdHandler(int32_t cid) {
    int sys = (cid & 0x80000000) != 0;

    cmd_handlers[!sys][cid & 31].fn = NULL;
    cmd_handlers[!sys][cid & 31].arg = NULL;
}

/* A command packet to the IOP: nothing there handles one */
uint32_t _sceSifSendCmd(int32_t cid, int32_t mode, void *pkt, int32_t psize, void *src, uint32_t dest, int32_t size) {
    (void)mode; (void)pkt; (void)psize; (void)src; (void)dest; (void)size;
    grx_fatal("sif: SIF command %08X to the IOP not emulated", (uint32_t)cid);
}

uint32_t sceSifSendCmd(int32_t cid, void *pkt, int32_t psize, void *src, uint32_t dest, int32_t size) {
    return _sceSifSendCmd(cid, 0, pkt, psize, src, dest, size);
}

uint32_t isceSifSendCmd(int32_t cid, void *pkt, int32_t psize, void *src, uint32_t dest, int32_t size) {
    return _sceSifSendCmd(cid, 1, pkt, psize, src, dest, size);
}

/* ------------------------------------------------------------------------ */
/* sifrpc (48E7C0-48F6E0; sceSifGetOtherData is in cdvd.c)                  */

void sceSifInitRpc(int32_t mode) {
    (void)mode;
}

void sceSifExitRpc(void) {
}

/* (client, server id, mode) - 0, or < 0 when the server doesn't exist.
   Bound at once, whatever the mode (bit 0 nowait). */
int32_t sceSifBindRpc(SifClientData *c, uint32_t sid, uint32_t mode) {
    Server *s = server_find(sid);
    int i;

    (void)mode;
    c->rpcd.paddr = 0;
    c->rpcd.pid = 0;
    c->rpcd.tid = -1;
    c->command = 0;
    c->func = 0;
    c->para = 0;
    c->serve = 0;
    if (s == NULL) {
        grx_fatal("sif: sceSifBindRpc to IOP RPC server %08X, which sif.c doesn't know", sid);
    }
    if (s->module != NULL && !grx_sif_module_loaded(s->module)) {
        /* on the PS2 the bind never completes and the library spins */
        grx_fatal("sif: sceSifBindRpc to %s (%08X), but %s isn't loaded", s->name, sid, s->module);
    }
    TRACE("sif: bind %s (%08X)", s->name, sid);
    i = (int)(s - servers);
    c->buff = SERVER_BUFF(i);
    c->cbuff = SERVER_BUFF(i) + 0x800;
    c->serve = (uint32_t)(uintptr_t)s->serve;
    if (s->fn == NULL) {
        static int warned;
        LOG_FEW(warned, 8, "sif: %s (IOP RPC %08X) bound, but its calls aren't emulated", s->name, sid);
    }
    return 0;
}

/* (client, fno, mode, send, ssize, recv, rsize, end function, its argument)
   - 0, or < 0. Mode bit 0: don't wait (the same here: the reply is in recv
   when it returns); bit 1: don't write the cache back. */
int32_t sceSifCallRpc(SifClientData *c, uint32_t fno, uint32_t mode, void *send, int32_t ssize, void *recv,
                      int32_t rsize, void (*endfunc)(void *), void *endparam) {
    Server *s = server_of((void *)(uintptr_t)c->serve);

    (void)mode;
    if (s == NULL) {
        grx_fatal("sif: sceSifCallRpc on a client that isn't bound (serve %08X)", c->serve);
    }
    c->rpcd.paddr = 0;         /* nothing pending: sceSifCheckStatRpc says done */
    c->func = (uint32_t)(uintptr_t)endfunc;
    c->para = (uint32_t)(uintptr_t)endparam;
    TRACE("sif: call %s fno %u mode %u send %d reply %d%s", s->name, fno, mode, ssize, rsize, endfunc ? " +end" : "");
    if (s->fn == NULL || s->fn(fno, send, ssize, recv, rsize) < 0) {
        grx_fatal("sif: %s (IOP RPC %08X) call %u (send %d bytes, reply %d) not emulated", s->name, s->sid, fno,
                  ssize, rsize);
    }
    if (endfunc != NULL) {
        endfunc(endparam);
    }
    return 0;
}

/* (client's or receive data's rpcd) - 1 while a call is in flight: never */
int32_t sceSifCheckStatRpc(void *rpcd) {
    (void)rpcd;
    return 0;
}

/* ------------------------------------------------------------------------ */
/* loadfile (492DB8-493A38) and the IOP's reset (493B18-493CB8)             */

int32_t sceSifLoadFileReset(void) {
    return 0;
}

/* (path, argument length, arguments) - the module id (> 0), < 0 on failure */
int32_t sceSifLoadModule(const char *path, int32_t arglen, const char *args) {
    char name[32];
    int i;

    (void)arglen; (void)args;
    module_base(path, name, sizeof(name));
    TRACE("sif: load module %s", path);
    for (i = 0; i < nmodules && strcmp(modules[i], name) != 0; i++) {
    }
    if (i == nmodules) {
        if (nmodules == MAX_MODULES) {
            grx_fatal("sif: too many IOP modules (%s)", path);
        }
        strcpy(modules[nmodules++], name);
    }
    return next_module_id++;
}

/* As sceSifLoadModule, with the module's start result (0: stays resident) */
int64_t sceSifLoadStartModule(const char *path, int32_t arglen, const char *args, int32_t *result) {
    int32_t id = sceSifLoadModule(path, arglen, args);

    if (result != NULL) {
        *result = 0;
    }
    return id;
}

/* (name) - the id of a loaded module, or < 0 */
int32_t sceSifSearchModuleByName(const char *name) {
    return grx_sif_module_loaded(name) ? 1 : -1;
}

/* (IOP image path or NULL, mode) - 1: reset, 0: refused */
int32_t sceSifResetIop(const char *image, int32_t mode) {
    (void)mode;
    TRACE("sif: IOP reset (%s)", image != NULL ? image : "default image");
    nmodules = 0;
    return 1;
}

int32_t sceSifIsAliveIop(void) {
    return 1;
}

/* Nonzero once the IOP has finished booting */
int32_t sceSifSyncIop(void) {
    return 1;
}

/* ("cdrom0:\\IOPRP234.IMG;1") - 1: rebooted with the replacement modules */
int32_t sceSifRebootIop(const char *image) {
    return sceSifResetIop(image, 0);
}

/* ------------------------------------------------------------------------ */
/* Boot-time memory                                                         */

/* SysBoot (42FB00) mounts the disc with SndFsMount(0x01FF1000, 0x01000000,
   "IMAGE"): the directory tree goes to the IOP block (grx_iop_block, in
   cdvd.c), and the directories' sectors are read into scratch at
   0x01000000, which SndFsReadDir (port/game/main/460670.c) aligns with
   & 0x7FFFF80 (its | 0x20000000 is stripped by fixups/memory.py). That is
   in the game heap's range, before MemInit: the fixup in
   port/tools/fixups/sif.py passes this instead, low memory of its own.
   About 70 KB are used (each directory's size + 0x100, 128-aligned). */
#define DIR_SCRATCH_SIZE 0x20000u

void *grx_sif_dir_scratch(void) {
    static void *p;

    if (p == NULL) {
        p = grx_hwmem_alloc_low_at(GRX_FIX_DIR_SCRATCH, DIR_SCRATCH_SIZE);   /* for save states */
        if (p == NULL) {
            grx_hwmem_note_unfixed("the disc's directory scratch");
            p = grx_hwmem_alloc_low(DIR_SCRATCH_SIZE);
        }
        if (p == NULL) {
            grx_fatal("sif: no low memory for the disc's directory scratch");
        }
    }
    return p;
}

/* ------------------------------------------------------------------------ */
/* Save states (grx_state_parts.h): the servers' serve areas (the game's
   clients point at them), the loaded modules, the SIF registers and the
   command handlers (the game's). The handlers in servers[] are this
   file's, the DMA targets the platform's: the same in every run. */
void grx_sif_state_io(GrxState *s) {
    int i;
    for (i = 0; i < NSERVERS; i++) {
        GRX_STATE_VAR(s, servers[i].serve);
    }
    GRX_STATE_VAR(s, modules);
    GRX_STATE_VAR(s, nmodules);
    GRX_STATE_VAR(s, next_module_id);
    GRX_STATE_VAR(s, sif_regs);
    GRX_STATE_VAR(s, cmd_handlers);
    GRX_STATE_VAR(s, sif_sregs);
    GRX_STATE_VAR(s, cmd_ctrl);
    GRX_STATE_VAR(s, sif_dma_id);
}
