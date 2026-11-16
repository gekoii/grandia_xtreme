/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/field.h"
#include "pad.h"
#include "field/state.h"
#include "field/n12.h"

typedef struct {
    f32 x, y, z, w;
} Vec;

typedef struct {
    u8 pad0[0x10];
    Vec pos;
    u8 pad20[0x24 - 0x20];
    s32 rot;
    u8 pad28[0x84 - 0x28];
    f32 x84;
} Mdl;

typedef struct {
    u8 pad0[0x6];
    s16 x6;
    s32 flags;
    u16 flags2;
    u8 padE[0x15 - 0xE];
    u8 x15;
    u8 pad16[0x1B - 0x16];
    u8 x1B;
    u8 pad1C[0x2C - 0x1C];
    s32 x2C;
    s32 x30;
    u8 pad34[0x4C - 0x34];
    s32 x4C;
    u8 pad50[0x54 - 0x50];
    s32 x54;
    u8 pad58[0x5F - 0x58];
    u8 x5F;
    Mdl *mdl;
    s32 x64;
    u8 pad68[0x6C - 0x68];
    s32 x6C;
    u8 pad70[0x180 - 0x70];
    Vec vel;
} Chara;

typedef struct {
    Vec v0;
    Vec org;
    Vec dir;
    Vec out;
    u8 pad40[0x40];
} Ray;


void ChrTrailPush(s32, void *, s32);
void ChrObjMove(Chara *);
s32 ItemCanAdd(s16);
void ChrBreathPuffSpawn(Chara *, s32);
void MemSet(void *, s32, s32);
void VuVecCopy(void *, void *);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

#define F64(c) (*(u64 *)&(c)->flags) /* a 64-bit load of flags (actFlags) for the 0xC0/0xA0 tests: a 32-bit one changes the code */
#define VZERO(p) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(p) : "memory")

void ChrPlayerUpdate(Pad *pad) {
    Chara *c;
    s32 dir;
    s32 got;
    s32 id;
    s16 n;
    s32 h;
    s32 t;
    s32 v;
    s32 y;
    s32 fall;
    s32 f;
    Mdl *m;
    Ray r;

    c = ChrGetByNo(1);
    if (c == NULL) {
        return;
    }
    if (c->mdl == NULL) {
        return;
    }
    if (c->x2C != 0 && --c->x2C == 0 && (c->flags & 0x100)) {
        c->flags ^= 0x100;
    }
    ChrFindHeadPart(c);
    if ((pad->trigger & 1) && ChrPlayerIsBusy() == 0) {
        if (camCtl.aiType == 2) {
            CamSetEyeMode(1);
        } else {
            CamSetEyeMode(0);
        }
    }
    dir = MapCollGetObjFlags();
    if (SysIsDebugMode() != 0 && (pad->held & 0x80)) {
        dir = 0;
        if (c->flags & 0x80) {
            c->flags ^= 0x80;
        }
    }
    c->x6 = dir;
    if (pad->held & 0x40) {
        if (camCtl.unk5 >= 2 && ChrPlayerIsBusy() == 0 && c->x15 == 0 && FldMapIsNoKamae() == 0 && c->x1B == 0 && !(F64(c) & 0xC0) && camCtl.aiType != 2) {
        f = c->flags;
        if (!(f & 0x10)) {
            if (!(f & 8)) {
                c->flags = f | 0x18;
                FldSndSePlay(3, 0xFA6, 0);
            }
        }
        } else {
        if (c->flags & 8) {
            c->flags = (c->flags ^ 8) | 0x10;
        }
        }
    } else if (!(c->flags & 0x10)) {
        if (c->flags & 8) {
            c->flags = (c->flags ^ 8) | 0x10;
        }
    }
    got = 0;
    if (padPort0Trigger & 0x20) {
        id = FldTreasureFindNearest(&c->mdl->pos);
        if (id != -1) {
            FldTreasureOpen(id);
            if (FldTreasureIsIdle(id) == 1) {
                n = FldTreasureGetItem(id);
                if (ItemCanAdd(n) != -1) {
                    got = 1;
                    FldSndSePlay(0, 0x7D7, 0);
                    ItemAdd(n);
                    FldTreasureRemove(id);
                } else {
                    FldSndSePlay(0, 0x7D4, 0);
                }
            }
        }
    }
    ChrExtraPartsUpdate(c);
    ChrBreathPuffSpawn(c, 0);
    if ((pad->trigger & 0x20) && ChrPlayerIsBusy() == 0 && got == 0 && FldSpotIsIdle() != 0 && !(c->flags & 0x40)) {
        h = FldSpotFindNear(&c->mdl->pos);
        if (h != -1) {
            t = FldSpotGetModel(h);
            c->flags |= 0x40;
            if (t != 0) {
                v = FldCalcYaw(&c->mdl->pos, (void *)(t + 0x10)) + 0x8000;
                c->x54 = v;
                c->mdl->rot = (v + 0x8000) & 0xFFFF;
            }
            FldSpotOpen(h);
            PtclDustSpawn(&c->mdl->pos, 0);
        }
    }
    switch (c->x15) {
    case 0:
    case 3:
    case 4:
        if (ChrPlayerIsBusy() != 1) {
            ChrPlayerPadMove(c, pad);
        } else {
            VZERO(&c->vel);
            c->x30 = 0;
        }
        break;
    case 2:
        c->x6 = 0;
        ChrLadderUpdate(c, 0);
        break;
    }
    if (c->flags & 0x40) {
        VZERO(&c->vel);
        c->x5F = 0x10;
        ChrMotSetOnce(c, 0x10);
        if (ChrMotIsEnd(c->mdl) != 1) {
            return;
        }
        c->flags ^= 0x40;
    }
    if (F64(c) & 0xA0) {
        VZERO(&c->vel);
    }
    if (camCtl.aiType == 2) {
        VZERO(&c->vel);
    }
    y = c->mdl->pos.y;
    ChrTurnUpdate(c);
    ChrObjMove(c);
    if (fsqrt(c->vel.x * c->vel.x + c->vel.z * c->vel.z) > 0.01f || MapLoadGetState() != -1) {
        ChrTrailPush(c->x6C, &c->mdl->pos, 0);
    }
    if (c->x15 != 2 && WmapIsActive() == 0 && c->x64 == 0) {
        MemSet(&r, 0, 0x80);
        MapCollGatherQuery(&c->mdl->pos, 200.0f);
        copyq(&r.org, &c->mdl->pos);
        VZERO(&r.dir);
        r.v0.w = 0;
        r.dir.y = -200.0f;
        r.org.y += 1.0f;
        VuVecCopy(&r.out, &r.dir);
        if (FldCollTriListHitTest(&r) != 0) {
            fall = 1;
            if (r.out.y > -150.0f) {
                if (c->flags & 0x80) {
                    m = c->mdl;
                    if (m->x84 >= 40.0f && m->x84 < 41.0f) {
                        PtclDustSpawn(&m->pos, 0);
                        SndSePlay3D(FldSndGetStepSe(evtAreaStepSound), 3, 0, &c->mdl->pos);
                    }
                    fall = 0;
                    if (ChrMotIsEnd(c->mdl) == 1) {
                        c->flags ^= 0x80;
                        c->flags2 |= 8;
                    }
                } else {
                    ChrStepSoundUpdate(c);
                }
                fall = 0;
            }
        } else {
            fall = 1;
        }
        if (c->x4C != 0) {
            if (c->flags & 0x80) {
                fall = 0;
                if (ChrMotIsEnd(c->mdl) == 1) {
                    c->flags ^= 0x80;
                    c->flags2 |= 8;
                }
            } else {
                ChrStepSoundUpdate(c);
            }
            fall = 0;
        }
        if (fall != 0 && y - (s32)c->mdl->pos.y >= 0x10) {
            c->x5F = 0xF;
            c->flags |= 0x80;
            ChrMotSetOnce(c, 0xF);
            ChrMotSetFrame(c->mdl, 20.0f);
            if (c->flags2 & 8) {
                c->flags2 ^= 8;
            }
        }
    }
    if (c->flags & 0x20) {
        if (c->flags & 0x80) {
            c->flags ^= 0x80;
            c->flags2 |= 8;
        }
        c->x5F = 0x1E;
        ChrMotSetOnce(c, 0x1E);
        if (ChrMotIsEnd(c->mdl) == 1) {
            c->flags ^= 0x20;
        }
    }
    if (c->flags2 & 0x80) {
        if (fsqrt(c->vel.x * c->vel.x + c->vel.z * c->vel.z) > 0.01f) {
            FldDirToRot(&c->vel.x, &r.v0.x);
            c->x54 = (s32)(r.v0.y * 10430.378f) & 0xFFFF;
        }
    }
    m = c->mdl;
    m->rot = c->x54 + 0x8000;
    if ((c->flags & 0x10) && ChrMotIsEnd(m) >= 0) {
        c->flags ^= 0x10;
    }
    ChrFollowersUpdate();
    ChrStunUpdate(c);
    ChrPartyFadeOutStep();
}
