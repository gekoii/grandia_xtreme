/*
 * Tests for the memory cards (port/src/mc.c). A standalone program, run
 * against card directories it makes under a scratch directory (argv[1], or
 * ./mc_test_cards), never the player's own cards.
 *
 * Two parts:
 *
 *  1. libmc calls one at a time: each command's result codes, sceMcSync's
 *     protocol, listings, free space, the closed bit, timestamps, card
 *     changes, unformatted and full cards, attributes kept across a
 *     restart.
 *  2. The game's own card code, linked in unchanged: the driver (main
 *     454630-455748: McDriverInit, McUpdate, McScanPorts, the request
 *     queue), the slot scan and checksums (42C9D0-42D200), and FIELD's
 *     save, load and boot space-check steps (65C648-65FA28), driven frame
 *     by frame as McSaveFlowUpdate / McLoadFlowUpdate / McSpaceCheckUpdate
 *     drive them. The dialogs, sounds and the slot-select menus are stubs
 *     here; everything that reaches libmc is the game's.
 *
 * Build (Git Bash, from the repo root; CC is llvm-mingw's
 * i686-w64-mingw32-clang, OUT a scratch directory):
 *
 *   SDL=port/deps/SDL3-3.4.18/i686-w64-mingw32
 *   GAME="main/454630 main/4546B0 main/454740 main/454780 main/4547A0 main/4547B0
 *     main/454810 main/454870 main/4548A8 main/4548F8 main/454960 main/4549C8
 *     main/454A00 main/454A38 main/454A88 main/454AD8 main/454AE8 main/454B20
 *     main/454B40 main/454BE8 main/454F00 main/455150 main/455450 main/4554D0
 *     main/4555C0 main/455748 main/42C628 main/42C9D0 main/42CAF0 main/42CB00
 *     main/42CBD0 main/42CC20 main/42CCF8 main/42CD38 main/42CDB8 main/42D128
 *     main/42D198 main/42D200 field/65C648 field/65C668 field/65C688
 *     field/65C690 field/65C9D0 field/65CB80 field/65CC00 field/65CCD0
 *     field/65CD20 field/65CF38 field/65D8E8 field/65DA38 field/65DB78
 *     field/65DCB8 field/65DD40 field/65DF18 field/65E270 field/65E318
 *     field/65E3C0 field/65E780 field/65E7D0 field/65F818 field/65F890
 *     field/65F980 field/65FA28"
 *   GFLAGS="-std=gnu89 -fno-builtin -fcommon -fno-strict-aliasing -fwrapv -msse2
 *     -mfpmath=sse -Wno-error=int-conversion -Wno-error=incompatible-pointer-types
 *     -Wno-error=incompatible-function-pointer-types -Wno-error=return-type -w"
 *   for f in $GAME; do $CC -c $GFLAGS -include port/include/grx_port.h \
 *       -Iport/include -Iinclude src/$f.c -o $OUT/g_${f/\//_}.o; done
 *   $CC -std=gnu11 -Wall -msse2 -mfpmath=sse -Iport/include -Iinclude -I$SDL/include \
 *       port/tests/mc_test.c port/src/mc.c $OUT/g_*.o -L$SDL/lib -lSDL3 -o $OUT/mc_test.exe
 *   cp $SDL/bin/SDL3.dll $OUT/
 *
 * Exit status: the failure count.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_time.h>
#undef NULL
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcomment"
#include "field/n26.h"
#pragma clang diagnostic pop
#include "grx_mc.h"

static int failures, checks;
#define CHECK(cond, ...)                                              \
    do {                                                              \
        checks++;                                                     \
        if (!(cond)) {                                                \
            failures++;                                               \
            printf("FAIL %s:%d: %s: ", __FILE__, __LINE__, #cond);    \
            printf(__VA_ARGS__);                                      \
            printf("\n");                                             \
        }                                                             \
    } while (0)
#define CHECK_EQ(a, b) CHECK((a) == (b), "%d, expected %d", (int)(a), (int)(b))

/* The platform layer's log (main.c in the game) */
void grx_log(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("  log: ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}

void grx_fatal(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    exit(100);
}

/* ------------------------------------------------------------------------ */
/* libmc and the game's card code                                           */

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

void McDriverInit(void);
void McUpdate(void);
void McScanPorts(void);
void McSelectPort(s32 port);
void McSetPollInterval(s32 frames);
McPortState *McGetCurPort(void);
void McReset(void);
s32 McIsBusy(void);
McSlotTable *McGetSlotTable(void);
void McSlotTableClear(void);
void McSaveSetChecksum(void *data, s32 size);
s32 McSaveCheckChecksum(void *data);
void McSetSaveData(void *data, s32 size);
void McSpaceCheckReset(void);
s32 McSpaceCheckUpdate(void);
void McSaveSetState(s32 state);
void McSaveCheckCard(void);
void McSaveReadDir(void);
void McSaveCheckGameFiles(void);
void McSaveScanSlots(void);
void McSaveCheckSpace(void);
void McSaveCheckSpaceNew(void);
void McSaveWrite(void);
void McSaveFormat(void);
void McLoadCheckCard(void);
void McLoadReadDir(void);
void McLoadScanSlots(void);
void McLoadRead(void);

/* ---- what the game's code needs from the rest of the game -------------- */

/* The driver (main/n08.h, mc.h) */
McPortState mcPortState[2];
McPortState *mcCurPort;
McPortState *mcCardInfo0, *mcCardInfo1;
McReqPool mcReqPool;
McReqQueue mcReqQueue;
McReq *mcCurReq;
s32 mcPollInterval, mcBusy, mcSyncPort, mcPollFrames, mcBusyFrames, mcScanBusyFrames, mcSyncResult;
McSlotTable mcSlotTable;

/* The save-file table (main/data/49A848.c): one object with its fields'
   names as linker aliases, as in the game */
struct McFiles {
    McFileEntry e[3];
} mcFiles __asm__("_mcIconSysEntry");
__asm__(".globl _mcIconFileEntry\n.set _mcIconFileEntry, _mcIconSysEntry+0x28\n"
        ".globl _mcSaveFileEntry\n.set _mcSaveFileEntry, _mcIconSysEntry+0x50\n"
        ".globl _mcIconSysSize\n.set _mcIconSysSize, _mcIconSysEntry+0x20\n"
        ".globl _mcIconFileSize\n.set _mcIconFileSize, _mcIconSysEntry+0x48\n"
        ".globl _mcSaveDataSize\n.set _mcSaveDataSize, _mcIconSysEntry+0x70\n"
        ".globl _mcSaveDataPtr\n.set _mcSaveDataPtr, _mcIconSysEntry+0x74\n"
        ".globl _mcPortState1\n.set _mcPortState1, _mcPortState+0x30\n"
        ".globl _mcReqQueueHead\n.set _mcReqQueueHead, _mcReqQueue+0x4\n");

/* The card's names (the main executable's and FIELD's copies) */
#define GAME_DIR "BASLUS-20417GrandiaX"
char mcSaveNamePrefix[] = "save";
char mcIconSysName[] = "icon.sys";
char mcIconFileName[] = "icon0.ico";
char mcSaveDirName[] = GAME_DIR;
char mcSaveDirPath[] = "/" GAME_DIR;
char mcSpaceCheckDirPattern[] = "/" GAME_DIR "/*";
char mcSaveDirPattern[] = "/" GAME_DIR "/*";
char mcSaveIconSysName[] = "icon.sys";
char mcSaveIconFileName[] = "icon0.ico";
char mcSaveGameFileName[] = GAME_DIR;
char mcSaveFlowDirPath[] = "/" GAME_DIR;
char mcLoadDirPattern[] = "/" GAME_DIR "/*";
char mcLoadDirPath[] = "/" GAME_DIR;
char mcLoadNotPs2Str[] = "not a PS2 card";

/* Directory listings (mcDirListBuf, mcSpaceCheckDirBuf: 16 entries) */
static union {
    McDirList l;
    u8 b[0x40 + 16 * 0x40];
} dirBuf, spaceDirBuf;
McDirList *mcDirListPtr = &dirBuf.l;
McDirList *mcSpaceCheckDirList = &spaceDirBuf.l;

/* McScanSlots (42CDB8) */
s32 mcScanReqIds[3], mcScanDirIndex, mcScanStep, mcScanSlotNo;
SysSaveData mcSaveReadBuf;
u8 ctypeTable[256];

/* FIELD's flows (field/n26.h) */
s32 mcSpaceCheckState, mcSpaceCheckPrevState, mcSpaceCheckPortPrevState, mcSpaceCheckPortState;
s32 mcSpaceCheckPortStateChanged, mcSpaceCheckResult0, mcSpaceCheckResult1, mcSpaceCheckDirReq;
s32 mcSpaceCheckStateChanged;
Pad *mcSpaceCheckPad;
s32 mcSaveFormatReq, mcSaveFormatStep, mcSaveDirReq, mcSaveIconReqs[3], mcSaveMkdirReq, mcSaveWriteReqs[4];
s8 mcSaveIconStep, mcSaveWriteStep, mcSaveWriteIsNew;
s32 mcSaveState, mcSavePrevState, mcSaveStateChanged;
s32 mcSaveGameFilesKB, mcSaveGameFilesNeedKB, mcSaveFreeKB, mcSavePlayTimeNow;
McSlot *mcSaveSlotRec;
s32 mcLoadDirReq, mcLoadReadReqs[4], mcLoadReadDelay, mcLoadState, mcLoadPrevState, mcLoadStateChanged;
Pad padPort0[2];
s32 sysSavePlayTime;

/* Stubs: dialogs, sounds, the load flow's state calls (in files not linked) */
void FldMenuMcDlgFormatting(s32 m) { (void)m; }
void FldMenuMcDlgReading(s32 m) { (void)m; }
void FldMenuMcDlgSaving(s32 m) { (void)m; }
void FldMenuMcDlgLoading(s32 m) { (void)m; }
void FldMenuMcDlgNoCard(s32 m) { (void)m; }
void FldMenuMcDlgCardFull(s32 m, s32 which) { (void)m, (void)which; }
s32 SndSePlay(void *se, u8 a, u8 b, u8 c, u8 d) { (void)se, (void)a, (void)b, (void)c, (void)d; return 0; }
void McLoadSetState(s32 s) { mcLoadState = s; }
void McLoadSetErrorMsg(char *s) { (void)s; }

/* The game's files: patterns of the right sizes */
static u8 iconSysData[0x3C4];
static u8 iconFileData[0xB638];
static SysSaveData saveData, loadData;

static void game_setup(void) {
    int i;
    memset(ctypeTable, 0, sizeof ctypeTable);
    for (i = '0'; i <= '9'; i++) {
        ctypeTable[i] = 4;
    }
    memset(&mcFiles, 0, sizeof mcFiles);
    strcpy(mcFiles.e[0].name, "icon.sys");
    mcFiles.e[0].size = sizeof iconSysData;
    mcFiles.e[0].data = iconSysData;
    strcpy(mcFiles.e[1].name, "icon0.ico");
    mcFiles.e[1].size = sizeof iconFileData;
    mcFiles.e[1].data = iconFileData;
    strcpy(mcFiles.e[2].name, GAME_DIR);
    for (i = 0; i < (int)sizeof iconSysData; i++) {
        iconSysData[i] = (u8)(i * 7);
    }
    for (i = 0; i < (int)sizeof iconFileData; i++) {
        iconFileData[i] = (u8)(i * 13 + 1);
    }
    McSetSaveData(&saveData, sizeof saveData);
    mcSlotTable.count = 8;
    /* McSaveFlowInit 65CD98 */
    mcSaveGameFilesNeedKB = ((mcFiles.e[0].size + 0x3FF) >> 10) + ((mcFiles.e[1].size + 0x3FF) >> 10) + 2;
}

/* ------------------------------------------------------------------------ */
/* Helpers                                                                  */

static char scratch[1024];

static void rmtree(const char *path) {
    char **list;
    int n, i;
    SDL_PathInfo pi;
    if (!SDL_GetPathInfo(path, &pi)) {
        return;
    }
    if (pi.type == SDL_PATHTYPE_DIRECTORY) {
        list = SDL_GlobDirectory(path, "*", 0, &n);
        /* deepest first */
        for (i = n - 1; list && i >= 0; i--) {
            char sub[2048];
            snprintf(sub, sizeof sub, "%s/%s", path, list[i]);
            rmtree(sub);
        }
        SDL_free(list);
    }
    SDL_RemovePath(path);
}

static void use_root(const char *name) {
    char dir[1100];
    snprintf(dir, sizeof dir, "%s/%s", scratch, name);
    rmtree(dir);
    CHECK(grx_mc_set_root(dir), "set_root %s", dir);
}

/* In part 2 the game's driver may have a command in flight between
   frames (its polling sceMcGetInfo): finish it before calling libmc
   directly */
static void drain(void) {
    sceMcSync(0, NULL, NULL);
}

/* sceMcSync(1): the command number and result of the command in flight */
static int done(int expect_cmd) {
    s32 cmd = -99, res = -99;
    int r = sceMcSync(1, &cmd, &res);
    CHECK(r == 1, "sceMcSync returned %d", r);
    CHECK(cmd == expect_cmd, "command %d, expected %d", cmd, expect_cmd);
    return res;
}

static int host_size(int port, const char *rel) {
    char p[2048];
    SDL_PathInfo pi;
    snprintf(p, sizeof p, "%s%s", grx_mc_card_dir(port), rel);
    if (!SDL_GetPathInfo(p, &pi)) {
        return -1;
    }
    return pi.type == SDL_PATHTYPE_DIRECTORY ? -2 : (int)pi.size;
}

static int mc_getinfo(int port, s32 *type, s32 *freep, s32 *format) {
    drain();
    CHECK_EQ(sceMcGetInfo(port, 0, type, freep, format), 0);
    return done(1);
}

static int mc_open(int port, const char *name, int mode) {
    drain();
    CHECK_EQ(sceMcOpen(port, 0, name, mode), 0);
    return done(2);
}

static int mc_close(int fd) {
    drain();
    CHECK_EQ(sceMcClose(fd), 0);
    return done(3);
}

static int mc_write(int fd, const void *buf, int n) {
    drain();
    CHECK_EQ(sceMcWrite(fd, (u8 *)buf, n), 0);
    return done(6);
}

static int mc_read(int fd, void *buf, int n) {
    drain();
    CHECK_EQ(sceMcRead(fd, buf, n), 0);
    return done(5);
}

static int mc_mkdir(int port, const char *name) {
    drain();
    CHECK_EQ(sceMcMkdir(port, 0, (s32)(uintptr_t)name), 0);
    return done(0xB);
}

static int mc_getdir(int port, const char *name, int mode, int max, sceMcTblGetDir *t) {
    drain();
    CHECK_EQ(sceMcGetDir(port, 0, name, mode, max, t), 0);
    return done(0xD);
}

static int mc_delete(int port, const char *name) {
    drain();
    CHECK_EQ(sceMcDelete(port, 0, name), 0);
    return done(0xF);
}

static const sceMcTblGetDir *find(const sceMcTblGetDir *t, int n, const char *name) {
    int i;
    for (i = 0; i < n; i++) {
        if (!strcmp(t[i].EntryName, name)) {
            return &t[i];
        }
    }
    return NULL;
}

/* PS2 time now (Japan), in minutes since 2000, to compare timestamps with */
static long jst_minutes(const sceMcStDateTime *t) {
    return (((long)(t->Year - 2000) * 372 + (t->Month - 1) * 31 + (t->Day - 1)) * 24 + t->Hour) * 60 + t->Min;
}

static long now_jst_minutes(void) {
    SDL_Time t;
    SDL_DateTime dt;
    sceMcStDateTime m;
    SDL_GetCurrentTime(&t);
    SDL_TimeToDateTime(t + (SDL_Time)9 * 3600 * 1000000000, &dt, false);
    m.Year = (u16)dt.year, m.Month = (u8)dt.month, m.Day = (u8)dt.day;
    m.Hour = (u8)dt.hour, m.Min = (u8)dt.minute;
    return jst_minutes(&m);
}

/* ------------------------------------------------------------------------ */
/* 1. libmc                                                                 */

static void test_protocol(void) {
    s32 type = -9, freep = -9, format = -9, cmd, res;
    printf("libmc protocol\n");
    CHECK_EQ(sceMcGetInfo(0, 0, &type, &freep, &format), -100);
    CHECK_EQ(sceMcInit(), 0);
    CHECK_EQ(sceMcSync(1, &cmd, &res), -1);
    CHECK_EQ(sceMcSync(0, NULL, NULL), -1);
    CHECK_EQ(sceMcGetSlotMax(0), 1);
    use_root("unit");
    grx_mc_set_card(0, GRX_MC_FORMATTED);
    grx_mc_set_card(1, GRX_MC_ABSENT);

    /* Before the first GetInfo mcman doesn't know the card */
    CHECK_EQ(mc_open(0, "/x", 0x200), -13);

    /* A new formatted card, then the same card */
    CHECK_EQ(mc_getinfo(0, &type, &freep, &format), -1);
    CHECK_EQ(type, 2);
    CHECK_EQ(freep, 8000);
    CHECK_EQ(format, 1);
    type = freep = format = -9;
    CHECK_EQ(mc_getinfo(0, &type, &freep, &format), 0);
    CHECK_EQ(type, 2);
    CHECK_EQ(freep, 8000);
    CHECK_EQ(format, 1);
    /* Only the outputs asked for are written */
    freep = -9;
    CHECK_EQ(sceMcGetInfo(0, 0, NULL, &freep, NULL), 0);
    CHECK_EQ(done(1), 0);
    CHECK_EQ(freep, 8000);

    /* No card */
    type = freep = format = -9;
    CHECK_EQ(mc_getinfo(1, &type, &freep, &format), -12);
    CHECK_EQ(type, 0);
    CHECK_EQ(freep, 0);
    CHECK_EQ(format, 0);
    CHECK_EQ(mc_open(1, "/x", 0x200), -12);
    CHECK_EQ(mc_getinfo(5, NULL, NULL, NULL), -12);

    /* One command at a time: libmc's semaphore */
    CHECK_EQ(sceMcGetInfo(0, 0, NULL, NULL, NULL), 0);
    CHECK_EQ(sceMcOpen(0, 0, "/x", 1), -200);
    CHECK_EQ(sceMcClose(0), -200);
    CHECK_EQ(sceMcSync(0, &cmd, &res), 1);
    CHECK_EQ(cmd, 1);
    CHECK_EQ(res, 0);
    CHECK_EQ(sceMcSync(1, &cmd, &res), -1);

    /* Names libmc refuses itself */
    CHECK_EQ(sceMcOpen(0, 0, "", 1), -210);
    CHECK_EQ(sceMcOpen(0, 0, NULL, 1), -210);
    CHECK_EQ(sceMcGetDir(0, 0, "", 0, 1, NULL), -210);
}

static void test_files(void) {
    static u8 buf[9000], back[9000];
    sceMcTblGetDir t[16];
    s32 freep;
    int fd0, fd1, fd2, i, n;
    const sceMcTblGetDir *e;
    long now = now_jst_minutes();
    char cur[0x400];
    printf("libmc files and directories\n");
    for (i = 0; i < (int)sizeof buf; i++) {
        buf[i] = (u8)(i * 31 + 5);
    }

    CHECK_EQ(mc_mkdir(0, "/TESTDIR"), 0);
    CHECK_EQ(mc_mkdir(0, "/TESTDIR"), -4);
    CHECK_EQ(mc_mkdir(0, "/NODIR/SUB"), -4);
    CHECK_EQ(host_size(0, "TESTDIR"), -2);

    fd0 = mc_open(0, "/TESTDIR/a.bin", 0x203);
    fd1 = mc_open(0, "/TESTDIR/b.bin", 0x200);
    fd2 = mc_open(0, "/TESTDIR/c.bin", 0x200);
    CHECK_EQ(fd0, 0);
    CHECK_EQ(fd1, 1);
    CHECK_EQ(fd2, 2);
    CHECK_EQ(mc_open(0, "/TESTDIR/d.bin", 0x200), -7);
    CHECK_EQ(mc_open(0, "/TESTDIR/zzz", 1), -7);
    /* a second writer on an open file */
    CHECK_EQ(mc_close(fd2), 0);
    CHECK_EQ(mc_open(0, "/TESTDIR/a.bin", 2), -5);
    CHECK_EQ(mc_open(0, "/TESTDIR/missing", 1), -4);
    CHECK_EQ(mc_open(0, "/NODIR/x", 0x200), -4);

    CHECK_EQ(mc_write(fd0, buf, 3000), 3000);
    CHECK_EQ(host_size(0, "TESTDIR/a.bin"), 3000);
    CHECK_EQ(sceMcSeek(fd0, 0, 0), 0);
    CHECK_EQ(done(4), 0);
    CHECK_EQ(sceMcSeek(fd0, -10, 2), 0);
    CHECK_EQ(done(4), 2990);
    CHECK_EQ(sceMcSeek(fd0, -5000, 1), 0);
    CHECK_EQ(done(4), 0);
    memset(back, 0, sizeof back);
    CHECK_EQ(mc_read(fd0, back, 4000), 3000);
    CHECK(!memcmp(back, buf, 3000), "read back differs");
    CHECK_EQ(mc_read(fd0, back, 10), 0);
    /* b.bin was opened write-only */
    CHECK_EQ(mc_read(fd1, back, 10), -5);
    CHECK_EQ(mc_close(fd0), 0);
    CHECK_EQ(mc_close(fd1), 0);
    CHECK_EQ(mc_close(fd1), -5);
    CHECK_EQ(mc_close(7), -5);

    /* Listings: "." and ".." first in a subdirectory, files' sizes, the
       closed bit only on what was written and closed */
    n = mc_getdir(0, "/TESTDIR/*", 0, 16, t);
    CHECK_EQ(n, 5);
    CHECK(n >= 2 && !strcmp(t[0].EntryName, ".") && !strcmp(t[1].EntryName, ".."), "%s %s", t[0].EntryName,
          t[1].EntryName);
    CHECK_EQ(t[0].AttrFile, 0x8427);
    CHECK_EQ(t[1].AttrFile, 0x8427);
    CHECK_EQ(t[0].FileSizeByte, 0);
    e = find(t, n, "a.bin");
    CHECK(e && e->AttrFile == 0x8497 && e->FileSizeByte == 3000, "a.bin %04X %u", e ? e->AttrFile : 0,
          e ? e->FileSizeByte : 0);
    e = find(t, n, "b.bin");
    CHECK(e && e->AttrFile == 0x8417 && e->FileSizeByte == 0, "b.bin %04X", e ? e->AttrFile : 0);
    e = find(t, n, "c.bin");
    CHECK(e && e->AttrFile == 0x8417, "c.bin %04X", e ? e->AttrFile : 0);
    CHECK(n == 5 && !strcmp(t[2].EntryName, "a.bin") && !strcmp(t[3].EntryName, "b.bin") &&
          !strcmp(t[4].EntryName, "c.bin"), "entry order");
    e = find(t, n, "a.bin");
    if (e) {
        long m = jst_minutes(&e->_Modify), c = jst_minutes(&e->_Create);
        CHECK(m >= now - 2 && m <= now + 2, "modified %04u-%02u-%02u %02u:%02u, now (JST) %ld", e->_Modify.Year,
              e->_Modify.Month, e->_Modify.Day, e->_Modify.Hour, e->_Modify.Min, now);
        CHECK(c >= now - 2 && c <= now + 2, "created");
        CHECK_EQ(e->_Modify.Resv2, 0);
    }
    /* the root: no "." or ".." */
    n = mc_getdir(0, "/*", 0, 16, t);
    CHECK_EQ(n, 1);
    CHECK(n == 1 && !strcmp(t[0].EntryName, "TESTDIR") && t[0].AttrFile == 0x8427 && t[0].FileSizeByte == 0,
          "root listing");
    /* patterns, and a name without one: the entry itself */
    CHECK_EQ(mc_getdir(0, "/TESTDIR/?.bin", 0, 16, t), 3);
    CHECK_EQ(mc_getdir(0, "/TESTDIR/a*", 0, 16, t), 1);
    CHECK_EQ(mc_getdir(0, "/TESTDIR/*.bi?", 0, 16, t), 3);
    CHECK_EQ(mc_getdir(0, "/TESTDIR/A.BIN", 0, 16, t), 0);
    CHECK_EQ(mc_getdir(0, "/TESTDIR", 0, 16, t), 1);
    CHECK_EQ(mc_getdir(0, "/NODIR/*", 0, 16, t), -4);
    /* continuing a listing (mode != 0) */
    CHECK_EQ(mc_getdir(0, "/TESTDIR/*", 0, 2, t), 2);
    CHECK_EQ(mc_getdir(0, "/TESTDIR/*", 1, 2, t), 2);
    CHECK(!strcmp(t[0].EntryName, "a.bin") && !strcmp(t[1].EntryName, "b.bin"), "%s", t[0].EntryName);
    CHECK_EQ(mc_getdir(0, "/TESTDIR/*", 1, 2, t), 1);
    CHECK_EQ(mc_getdir(0, "/TESTDIR/*", 1, 2, t), 0);
    CHECK_EQ(mc_getdir(0, "/TESTDIR/*", 0, 0, t), 0);

    /* Free space: the root has 3 entries (2 clusters), TESTDIR 5 (3),
       a.bin 3 KB */
    CHECK_EQ(mc_getinfo(0, NULL, &freep, NULL), 0);
    CHECK_EQ(freep, 8001 - 2 - 3 - 3);
    CHECK_EQ(grx_mc_free_clusters(0), 8001 - 2 - 3 - 3);
    CHECK_EQ(sceMcGetEntSpace(0, 0, "/TESTDIR"), 0);
    CHECK_EQ(done(0x12), 1);
    CHECK_EQ(sceMcGetEntSpace(0, 0, "/"), 0);
    CHECK_EQ(done(0x12), 1);

    /* Delete */
    CHECK_EQ(mc_delete(0, "/TESTDIR"), -6);
    CHECK_EQ(mc_delete(0, "/TESTDIR/c.bin"), 0);
    CHECK_EQ(mc_delete(0, "/TESTDIR/c.bin"), -4);
    CHECK_EQ(mc_delete(0, "/"), -4);
    CHECK_EQ(host_size(0, "TESTDIR/c.bin"), -1);

    /* The current directory and relative names */
    memset(cur, 'x', sizeof cur);
    CHECK_EQ(sceMcChdir(0, 0, "/TESTDIR", cur), 0);
    CHECK_EQ(done(0xC), 0);
    CHECK(!strcmp(cur, "/TESTDIR"), "chdir gave %s", cur);
    fd0 = mc_open(0, "rel.dat", 0x200);
    CHECK_EQ(fd0, 0);
    CHECK_EQ(mc_write(fd0, buf, 0), 0);
    CHECK_EQ(mc_close(fd0), 0);
    CHECK_EQ(host_size(0, "TESTDIR/rel.dat"), 0);
    CHECK_EQ(sceMcChdir(0, 0, "..", cur), 0);
    CHECK_EQ(done(0xC), 0);
    CHECK(!strcmp(cur, "/"), "chdir .. gave %s", cur);
    CHECK_EQ(sceMcChdir(0, 0, "NODIR", NULL), 0);
    CHECK_EQ(done(0xC), -4);
    n = mc_getdir(0, "TESTDIR/rel.dat", 0, 16, t);
    /* a zero-byte write closes the file (mcman_write2 ends with close2) */
    CHECK(n == 1 && t[0].AttrFile == 0x8497, "rel.dat %d %04X", n, n == 1 ? t[0].AttrFile : 0);

    /* Rename */
    CHECK_EQ(sceMcRename(0, 0, "/TESTDIR/rel.dat", "renamed.dat"), 0);
    CHECK_EQ(done(0x13), 0);
    CHECK_EQ(host_size(0, "TESTDIR/renamed.dat"), 0);
    CHECK_EQ(sceMcRename(0, 0, "/TESTDIR/renamed.dat", "a.bin"), 0);
    CHECK_EQ(done(0x13), -4);
    CHECK_EQ(sceMcRename(0, 0, "/TESTDIR/none", "x"), 0);
    CHECK_EQ(done(0x13), -4);
    n = mc_getdir(0, "/TESTDIR/renamed.dat", 0, 16, t);
    CHECK(n == 1 && t[0].AttrFile == 0x8497, "renamed.dat keeps its attributes");

    /* SetFileInfo: times and attributes */
    {
        sceMcTblGetDir info;
        memset(&info, 0, sizeof info);
        info._Create.Year = 2001, info._Create.Month = 2, info._Create.Day = 3, info._Create.Hour = 4;
        info._Modify.Year = 2002, info._Modify.Month = 5, info._Modify.Day = 6, info._Modify.Min = 7;
        info.AttrFile = 0x8495;  /* write bit off */
        CHECK_EQ(sceMcSetFileInfo(0, 0, "/TESTDIR/a.bin", &info, 1 | 2 | 4 | 0x10), 0);
        CHECK_EQ(done(0xE), 0);
        n = mc_getdir(0, "/TESTDIR/a.bin", 0, 16, t);
        CHECK(n == 1 && t[0].AttrFile == 0x8495 && t[0]._Create.Year == 2001 && t[0]._Create.Hour == 4 &&
              t[0]._Modify.Year == 2002 && t[0]._Modify.Min == 7 && t[0].FileSizeByte == 3000,
              "setinfo: %04X %u %u", t[0].AttrFile, t[0]._Create.Year, t[0]._Modify.Year);
        CHECK_EQ(mc_open(0, "/TESTDIR/a.bin", 2), -5);
        fd0 = mc_open(0, "/TESTDIR/a.bin", 1);
        CHECK_EQ(fd0, 0);
        CHECK_EQ(mc_write(fd0, buf, 4), -5);
        CHECK_EQ(mc_close(fd0), 0);
        info.AttrFile = 0x8497;
        CHECK_EQ(sceMcSetFileInfo(0, 0, "/TESTDIR/a.bin", &info, 4), 0);
        CHECK_EQ(done(0xE), 0);
        CHECK_EQ(sceMcSetFileInfo(0, 0, "/TESTDIR/zz", &info, 4), 0);
        CHECK_EQ(done(0xE), -4);
    }

    /* Overwriting in place (mode 2) keeps the size and the created time;
       create (0x200) over a file starts it again */
    fd0 = mc_open(0, "/TESTDIR/a.bin", 2);
    CHECK_EQ(mc_write(fd0, buf + 100, 10), 10);
    CHECK_EQ(mc_close(fd0), 0);
    CHECK_EQ(host_size(0, "TESTDIR/a.bin"), 3000);
    n = mc_getdir(0, "/TESTDIR/a.bin", 0, 16, t);
    CHECK(n == 1 && t[0]._Create.Year == 2001 && t[0].AttrFile == 0x8497, "overwrite: %u %04X", t[0]._Create.Year,
          t[0].AttrFile);
    fd0 = mc_open(0, "/TESTDIR/a.bin", 0x200);
    CHECK_EQ(mc_close(fd0), 0);
    CHECK_EQ(host_size(0, "TESTDIR/a.bin"), 0);
    n = mc_getdir(0, "/TESTDIR/a.bin", 0, 16, t);
    CHECK(n == 1 && t[0]._Create.Year != 2001 && t[0].AttrFile == 0x8417, "recreated: %u %04X", t[0]._Create.Year,
          t[0].AttrFile);
    n = mc_getdir(0, "/TESTDIR/*", 0, 16, t);
    CHECK(n == 5 && !strcmp(t[2].EntryName, "a.bin"), "a.bin keeps its place");

    /* A write in progress has no closed bit; a flush sets it */
    fd0 = mc_open(0, "/TESTDIR/b.bin", 2);
    CHECK_EQ(mc_write(fd0, buf, 2000), 2000);
    n = mc_getdir(0, "/TESTDIR/b.bin", 0, 16, t);
    CHECK(n == 1 && t[0].AttrFile == 0x8417 && t[0].FileSizeByte == 2000, "open b.bin %04X", t[0].AttrFile);
    CHECK_EQ(sceMcFlush(fd0), 0);
    CHECK_EQ(done(0xA), 0);
    n = mc_getdir(0, "/TESTDIR/b.bin", 0, 16, t);
    CHECK(n == 1 && t[0].AttrFile == 0x8497, "flushed b.bin %04X", t[0].AttrFile);
    CHECK_EQ(mc_close(fd0), 0);

    /* Names a Windows directory can't hold */
    fd0 = mc_open(0, "/TESTDIR/a:b?c", 0x200);
    CHECK_EQ(fd0, 0);
    CHECK_EQ(mc_close(fd0), 0);
    CHECK_EQ(host_size(0, "TESTDIR/a%3Ab%3Fc"), 0);
    n = mc_getdir(0, "/TESTDIR/a:*", 0, 16, t);
    CHECK(n == 1 && !strcmp(t[0].EntryName, "a:b?c"), "encoded name");
    CHECK_EQ(mc_delete(0, "/TESTDIR/a:b?c"), 0);

    CHECK_EQ(sceMcChangeThreadPriority(0x30), 0);
    CHECK_EQ(done(0x14), 0);
}

static void test_cards(void) {
    static u8 big[64 * 1024];
    sceMcTblGetDir t[16];
    s32 type, freep, format;
    int fd, i, n, written;
    printf("libmc cards: full, unformatted, changed, restarted\n");

    /* A full card */
    fd = mc_open(0, "/big.bin", 0x200);
    CHECK_EQ(fd, 0);
    written = 0;
    for (i = 0; i < 1000; i++) {
        int f = grx_mc_free_clusters(0);
        int r;
        if (f < 64) {
            break;
        }
        r = mc_write(fd, big, sizeof big);
        CHECK_EQ(r, (int)sizeof big);
        written += r;
    }
    CHECK(grx_mc_free_clusters(0) < 64, "card not filled");
    n = grx_mc_free_clusters(0);
    CHECK_EQ(mc_write(fd, big, (n + 1) * 1024), -3);
    /* mcman drops the fd after -3; the entry keeps its length */
    CHECK_EQ(mc_close(fd), -5);
    CHECK_EQ(host_size(0, "big.bin"), written);
    CHECK_EQ(mc_getinfo(0, NULL, &freep, NULL), 0);
    CHECK_EQ(freep, n);
    /* The last clusters: a new entry needs one when its directory's last
       cluster is full (the root has 5 entries with filler, 6 with x) */
    fd = mc_open(0, "/filler", 0x200);
    CHECK_EQ(fd, 0);
    n = grx_mc_free_clusters(0);
    CHECK_EQ(mc_write(fd, big, n * 1024), n * 1024);
    CHECK_EQ(mc_close(fd), 0);
    CHECK_EQ(grx_mc_free_clusters(0), 0);
    fd = mc_open(0, "/x", 0x200);
    CHECK_EQ(fd, 0);
    CHECK_EQ(mc_close(fd), 0);
    CHECK_EQ(grx_mc_free_clusters(0), 0);
    CHECK_EQ(mc_mkdir(0, "/NEWDIR"), -3);
    CHECK_EQ(mc_open(0, "/y", 0x200), -3);
    fd = mc_open(0, "/x", 2);
    CHECK_EQ(fd, 0);
    CHECK_EQ(mc_write(fd, big, 1), -3);
    CHECK_EQ(mc_getinfo(0, NULL, &freep, NULL), 0);
    CHECK_EQ(freep, 0);
    CHECK_EQ(mc_delete(0, "/x"), 0);
    CHECK_EQ(mc_delete(0, "/filler"), 0);
    CHECK_EQ(mc_delete(0, "/big.bin"), 0);
    CHECK_EQ(grx_mc_free_clusters(0), 8001 - 2 - 3 - 0 - 2);

    /* An unformatted card in port 1 */
    grx_mc_set_card(1, GRX_MC_UNFORMATTED);
    CHECK_EQ(mc_open(1, "/x", 0x200), -13);
    type = freep = format = -9;
    CHECK_EQ(mc_getinfo(1, &type, &freep, &format), -2);
    CHECK_EQ(type, 2);
    CHECK_EQ(freep, 0);
    CHECK_EQ(format, 0);
    type = freep = format = -9;
    CHECK_EQ(mc_getinfo(1, &type, &freep, &format), 0);
    CHECK_EQ(type, 2);
    CHECK_EQ(freep, 0);
    CHECK_EQ(format, 0);
    CHECK_EQ(mc_open(1, "/x", 0x200), -2);
    CHECK_EQ(mc_getdir(1, "/*", 0, 16, t), -2);
    CHECK_EQ(sceMcFormat(1, 0), 0);
    CHECK_EQ(done(0x10), 0);
    CHECK_EQ(mc_getinfo(1, &type, &freep, &format), 0);
    CHECK_EQ(freep, 8000);
    CHECK_EQ(format, 1);
    CHECK_EQ(mc_mkdir(1, "/D"), 0);
    CHECK_EQ(host_size(1, "D"), -2);
    CHECK_EQ(sceMcUnformat(1, 0), 0);
    CHECK_EQ(done(0x11), 0);
    CHECK_EQ(mc_getinfo(1, &type, &freep, &format), -2);
    CHECK_EQ(sceMcFormat(1, 0), 0);
    CHECK_EQ(done(0x10), 0);
    CHECK_EQ(host_size(1, "D"), -1);
    CHECK_EQ(mc_getdir(1, "/*", 0, 16, t), 0);

    /* Pulling a card: its fds go, GetInfo sees no card, then a new one */
    fd = mc_open(0, "/TESTDIR/b.bin", 1);
    CHECK_EQ(fd, 0);
    grx_mc_set_card(0, GRX_MC_ABSENT);
    CHECK_EQ(mc_read(fd, big, 10), -5);
    CHECK_EQ(mc_getinfo(0, NULL, NULL, NULL), -12);
    CHECK_EQ(mc_getdir(0, "/*", 0, 16, t), -12);
    grx_mc_set_card(0, GRX_MC_FORMATTED);
    CHECK_EQ(mc_getdir(0, "/*", 0, 16, t), -13);
    CHECK_EQ(mc_getinfo(0, NULL, NULL, NULL), -1);
    CHECK_EQ(mc_getinfo(0, NULL, NULL, NULL), 0);
    CHECK_EQ(mc_getdir(0, "/*", 0, 16, t), 1);
    /* the current directory goes back to the root */
    CHECK_EQ(sceMcChdir(0, 0, "/TESTDIR", NULL), 0);
    CHECK_EQ(done(0xC), 0);
    grx_mc_set_card(0, GRX_MC_FORMATTED);
    CHECK_EQ(mc_getinfo(0, NULL, NULL, NULL), -1);
    CHECK_EQ(mc_getdir(0, "*", 0, 16, t), 1);

    /* Restarting: the attributes and timestamps are kept beside the tree */
    {
        sceMcTblGetDir before[16];
        int nb = mc_getdir(0, "/TESTDIR/*", 0, 16, before);
        char dir[1100];
        snprintf(dir, sizeof dir, "%s/unit", scratch);
        grx_mc_set_root(dir);
        CHECK_EQ(mc_getinfo(0, NULL, NULL, NULL), -1);
        n = mc_getdir(0, "/TESTDIR/*", 0, 16, t);
        CHECK_EQ(n, nb);
        CHECK(n == nb && !memcmp(t, before, (size_t)n * sizeof *t), "listing changed across a restart");
        /* a file put in by hand: a closed file, the host's times */
        {
            char p[2048];
            SDL_IOStream *io;
            snprintf(p, sizeof p, "%sTESTDIR/handmade.bin", grx_mc_card_dir(0));
            io = SDL_IOFromFile(p, "wb");
            SDL_WriteIO(io, big, 1500);
            SDL_CloseIO(io);
        }
        n = mc_getdir(0, "/TESTDIR/handmade.bin", 0, 16, t);
        CHECK(n == 1 && t[0].AttrFile == 0x8497 && t[0].FileSizeByte == 1500, "handmade %d", n);
    }
}

/* ------------------------------------------------------------------------ */
/* 2. The game's card code                                                  */

/* Frames of McUpdate with port's driver state */
static int run_until_state(int port, int state, int max) {
    int i;
    McSelectPort(port);
    for (i = 0; i < max && mcPortState[port].state != state; i++) {
        McUpdate();
    }
    return mcPortState[port].state == state;
}

/* McSaveFlowUpdate's steps from the card check on (state 0x100, after the
   port select); the slot select picks <slot>, the format question is
   answered yes. Returns the state it ends in. */
static int save_flow_run(int port, int slot, int max_frames, int init, int stop_at) {
    int i;
    McSelectPort(port);
    if (init) {
        mcSaveState = 0x100;
        mcSavePrevState = 0xFFFF;
    }
    for (i = 0; i < max_frames; i++) {
        int state, cur;
        if (mcSaveState == stop_at) {
            return stop_at;
        }
        mcSaveStateChanged = mcSavePrevState != mcSaveState;
        state = mcSaveState;
        switch (state) {
        case 0x100: McSaveCheckCard(); break;
        case 0x101: McSaveReadDir(); break;
        case 0x102: McSaveCheckGameFiles(); break;
        case 0x104: McSaveScanSlots(); break;
        case 0x200: McSaveCheckSpaceNew(); break;
        case 0x201: McSaveCheckSpace(); break;
        case 2:
            /* McSaveSelectSlot 65D590 */
            if (McIsBusy() == 0) {
                sprintf(mcFiles.e[2].name, "save%02d.dat", slot);
                mcSaveSlotRec = &McGetSlotTable()->slots[slot];
                McSaveSetState(3);
            }
            break;
        case 3: McSaveWrite(); break;
        case 4: McSaveFormat(); break;
        case 0x305: McSaveSetState(4); break;
        default:
            return state;
        }
        mcSavePrevState = state;
        McUpdate();
        cur = mcSaveState;
        if (cur == 0 || cur == 0x309 || cur == 3 || cur == 0x303 || cur == 4 || cur == 0x307 || cur == 0x30B) {
            continue;
        }
        if (cur != 0x800 && McGetCurPort()->state < 2) {
            McSaveSetState(0x309);
        }
    }
    return mcSaveState;
}

static int save_flow(int port, int slot, int max_frames) {
    return save_flow_run(port, slot, max_frames, 1, -1);
}

static int load_flow(int port, int slot, int max_frames) {
    int i;
    McSelectPort(port);
    mcLoadState = 0x100;
    mcLoadPrevState = 0xFFFF;
    for (i = 0; i < max_frames; i++) {
        int cur;
        mcLoadStateChanged = mcLoadPrevState != mcLoadState;
        cur = mcLoadState;
        switch (cur) {
        case 0x100: McLoadCheckCard(); break;
        case 0x101: McLoadReadDir(); break;
        case 0x104: McLoadScanSlots(); break;
        case 2:
            if (McIsBusy() == 0) {
                sprintf(mcFiles.e[2].name, "save%02d.dat", slot);
                McLoadSetState(3);
            }
            break;
        case 3: McLoadRead(); break;
        default:
            return cur;
        }
        mcLoadPrevState = cur;
        McUpdate();
        if (mcLoadState != 0 && mcLoadState != 3 && mcLoadState != 0x30A && mcLoadState != 0x304 &&
            mcLoadState != 0x801 && mcLoadState != 0x800 && McGetCurPort()->state < 2) {
            McLoadSetState(0x309);
        }
    }
    return -1;
}

/* The boot check (McSpaceCheckUpdate 65C690): 4 when a card has room,
   2 card full, 3 no card */
static int space_check(int max_frames) {
    int i, last = -1;
    McSpaceCheckReset();
    for (i = 0; i < max_frames; i++) {
        last = mcSpaceCheckState;
        if (McSpaceCheckUpdate() == 1) {
            return 4;
        }
        if (mcSpaceCheckState == 2 || mcSpaceCheckState == 3) {
            return mcSpaceCheckState;
        }
    }
    (void)last;
    return -1;
}

/* Frames until the driver has port's card with <kb> free */
static int run_until_free(int port, int kb, int max) {
    int i;
    McSelectPort(port);
    for (i = 0; i < max; i++) {
        if (mcPortState[port].state == 2 && mcPortState[port].freeKB == kb) {
            return 1;
        }
        McUpdate();
    }
    return 0;
}

/* Fills port's card up to <leave> free clusters behind the driver's back,
   then puts it in again so the driver reads its free space anew, as it
   would after a card change */
static void fill_card(int port, int leave) {
    static u8 chunk[64 * 1024];
    int fd, f;
    mc_getinfo(port, NULL, NULL, NULL);
    fd = mc_open(port, "/fill.bin", 0x200);
    CHECK(fd >= 0, "fill.bin: %d", fd);
    while ((f = grx_mc_free_clusters(port) - leave) > 0) {
        f = f > 64 ? 64 : f;
        CHECK_EQ(mc_write(fd, chunk, f * 1024), f * 1024);
    }
    mc_close(fd);
    CHECK_EQ(grx_mc_free_clusters(port), leave);
    grx_mc_set_card(port, GRX_MC_FORMATTED);
    CHECK(run_until_free(port, leave, 600), "port %d: state %d, %d free", port, mcPortState[port].state,
          mcPortState[port].freeKB);
}

static void fill_save(int seed) {
    u8 *p = (u8 *)&saveData;
    int i;
    for (i = 0; i < (int)sizeof saveData; i++) {
        p[i] = (u8)(i * 3 + seed);
    }
    saveData.playTime = 1000 + seed;
}

static void test_game(void) {
    sceMcTblGetDir t[16];
    McSlotTable *st;
    int r, n;
    const sceMcTblGetDir *e;
    printf("game: driver, space check, save, load\n");
    use_root("game");
    grx_mc_set_card(0, GRX_MC_FORMATTED);
    grx_mc_set_card(1, GRX_MC_ABSENT);
    game_setup();
    McDriverInit();
    McSetPollInterval(0x1E);

    /* The driver finds the card in port 0 and none in port 1 */
    CHECK(run_until_state(0, 2, 200), "port 0 state %d", mcPortState[0].state);
    CHECK_EQ(mcPortState[0].type, 2);
    CHECK_EQ(mcPortState[0].formatted, 1);
    CHECK_EQ(mcPortState[0].freeKB, 8000);
    CHECK(!run_until_state(1, 2, 200), "port 1 found a card");
    CHECK_EQ(mcPortState[1].state, 0);
    CHECK(mcPortState[1].result < -10, "port 1 result %d", mcPortState[1].result);
    /* the port select screen's scan (McScanPorts) */
    {
        int i;
        for (i = 0; i < 300; i++) {
            McScanPorts();
        }
        CHECK_EQ(mcPortState[0].state, 2);
        CHECK_EQ(mcPortState[1].state, 0);
    }

    /* The boot check: an empty card has room */
    CHECK_EQ(space_check(2000), 4);

    /* A first save: directory, icon files, the game file, save00.dat */
    fill_save(1);
    r = save_flow(0, 0, 3000);
    CHECK(r == 0x304, "first save ended in %X", r);
    CHECK_EQ(host_size(0, GAME_DIR "/icon.sys"), 0x3C4);
    CHECK_EQ(host_size(0, GAME_DIR "/icon0.ico"), 0xB638);
    CHECK_EQ(host_size(0, GAME_DIR "/" GAME_DIR), 0);
    CHECK_EQ(host_size(0, GAME_DIR "/save00.dat"), 0x3300);
    n = mc_getdir(0, "/" GAME_DIR "/*", 0, 16, t);
    CHECK_EQ(n, 6);
    e = find(t, n, "icon.sys");
    CHECK(e && e->AttrFile == 0x8497, "icon.sys %04X", e ? e->AttrFile : 0);
    e = find(t, n, "save00.dat");
    CHECK(e && e->AttrFile == 0x8497 && e->FileSizeByte == 0x3300, "save00.dat");
    /* the root: 3 entries, 2 clusters; the directory: 6, 3; files 1+46+13 */
    CHECK_EQ(grx_mc_free_clusters(0), 8001 - 2 - 3 - 1 - 46 - 13);
    CHECK_EQ(mcPortState[0].freeKB, 8001 - 2 - 3 - 1 - 46 - 13);

    /* A second save: the directory is there; the game file was created
       and closed without a write, so it isn't "closed" and is rewritten */
    fill_save(2);
    r = save_flow(0, 1, 3000);
    CHECK(r == 0x304, "second save ended in %X", r);
    st = McGetSlotTable();
    CHECK_EQ(st->fileMask, 7);
    CHECK_EQ(st->slots[0].used, 1);
    CHECK_EQ(st->slots[0].status, 0);
    CHECK_EQ(st->slots[0].playTime, 1001);
    CHECK_EQ(host_size(0, GAME_DIR "/save01.dat"), 0x3300);

    /* Overwriting slot 0 (opened with mode 2) */
    fill_save(3);
    r = save_flow(0, 0, 3000);
    CHECK(r == 0x304, "overwrite ended in %X", r);
    CHECK_EQ(st->slots[1].used, 1);
    CHECK_EQ(st->slots[1].status, 0);
    CHECK_EQ(st->slots[1].playTime, 1002);

    /* Loading slot 1 back */
    McSetSaveData(&loadData, sizeof loadData);
    memset(&loadData, 0, sizeof loadData);
    r = load_flow(0, 1, 3000);
    CHECK(r == 0x304, "load ended in %X", r);
    CHECK_EQ(loadData.playTime, 1002);
    CHECK(McSaveCheckChecksum(&loadData), "loaded data's checksum");
    r = load_flow(0, 0, 3000);
    CHECK(r == 0x304, "load slot 0 ended in %X", r);
    CHECK_EQ(loadData.playTime, 1003);
    /* a slot with nothing in it */
    r = load_flow(0, 5, 3000);
    CHECK(r == 0x30A, "load of an empty slot ended in %X", r);
    McSetSaveData(&saveData, sizeof saveData);

    /* An unformatted card in port 1: the save flow asks, formats, saves */
    grx_mc_set_card(1, GRX_MC_UNFORMATTED);
    CHECK(run_until_state(1, 2, 300), "port 1 state %d", mcPortState[1].state);
    CHECK_EQ(mcPortState[1].type, 2);
    CHECK_EQ(mcPortState[1].formatted, 0);
    fill_save(4);
    r = save_flow(1, 3, 3000);
    CHECK(r == 0x304, "save to the unformatted card ended in %X", r);
    CHECK_EQ(grx_mc_card_state(1), GRX_MC_FORMATTED);
    CHECK_EQ(host_size(1, GAME_DIR "/save03.dat"), 0x3300);

    /* The card pulled in the middle of a save */
    fill_save(5);
    r = save_flow_run(1, 4, 3000, 1, 3);
    CHECK(r == 3, "save to slot 4 didn't get to writing: %X", r);
    save_flow_run(1, 4, 3, 0, -1);
    grx_mc_set_card(1, GRX_MC_ABSENT);
    r = save_flow_run(1, 4, 3000, 0, -1);
    CHECK(r == 0x309 || r == 0x303, "save with the card pulled ended in %X", r);
    CHECK_EQ(mcPortState[1].state, 0);
    r = save_flow(1, 4, 3000);
    CHECK(r == 0x309, "save without a card ended in %X", r);

    /* The boot check on a full card with the game's saves on it: they
       count as room (McSpaceCheckFreeKBWithSaves) */
    fill_card(0, 40);
    McSelectPort(0);
    CHECK_EQ(space_check(3000), 4);

    /* A full card without saves: "card full"; then no card at all */
    use_root("game2");
    fill_card(0, 40);
    CHECK_EQ(space_check(3000), 2);
    CHECK_EQ(mcSpaceCheckResult0, -2);
    CHECK_EQ(mcSpaceCheckResult1, -1);
    grx_mc_set_card(0, GRX_MC_ABSENT);
    CHECK(run_until_state(0, 0, 300), "port 0 state %d", mcPortState[0].state);
    CHECK_EQ(space_check(3000), 3);
    CHECK_EQ(mcSpaceCheckResult0, -1);
    CHECK_EQ(mcSpaceCheckResult1, -1);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        snprintf(scratch, sizeof scratch, "%s", argv[1]);
    } else {
        char *cwd = SDL_GetCurrentDirectory();
        snprintf(scratch, sizeof scratch, "%smc_test_cards", cwd ? cwd : "./");
        SDL_free(cwd);
    }
    SDL_CreateDirectory(scratch);
    /* never the player's cards */
    SDL_setenv_unsafe("GRX_MC_DIR", scratch, 1);
    test_protocol();
    test_files();
    test_cards();
    test_game();
    grx_mc_shutdown();
    if (failures) {
        printf("%d of %d checks FAILED\n", failures, checks);
    } else {
        printf("all %d checks passed\n", checks);
    }
    return failures;
}
