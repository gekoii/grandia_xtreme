#include "common.h"
#include "protos/main.h"

typedef struct {
    s32 a;
    s32 b;
    s32 c;
} Sub;

typedef struct {
    u32 x0;
    u32 x4;
    u8 pad8[0x30 - 8];
    Sub sub;
    u8 pad3C[0x8C - 0x3C];
} Ent;

/* Local view of sndBgmChannels (SndBgmChannel, snd.h) from +0x10: the
 * original's loop pointer starts at channel + 0x10 (index) and stores the
 * driver arguments at +0x30 from it, and the -1s are built with lui/ori
 * (u32 fields); the header type gives base-relative offsets and li -1. */
typedef struct {
    u8 pad0[0x10];
    Ent ent[2];
} Mgr;

void MemSet(void *, s32, s32);
extern s32 sndBgmCurChannel;
extern s32 sndBgmEnabled;
extern Mgr sndBgmChannels;

void SndInit(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    Ent *e;

    MemSet(&sndBgmChannels, 0, 0x118);
    sndBgmEnabled = 1;
    sndBgmCurChannel = 0;
    e = sndBgmChannels.ent;
    for (i = 0; i < 2; i++, e++) {
        e->sub.a = arg0;
        e->sub.b = arg1;
        e->sub.c = arg2;
        e->x4 = 0xFFFFFFFF;
        e->x0 = 0xFFFFFFFF;
    }
    SndDriverInit(&sndBgmChannels.ent[0].sub);
}
