#include "common.h"
#include "main/n05.h"
#include "protos/main.h"

#define UNIT_MATRIX(m) \
    __asm__ volatile( \
        "vmr32.xyzw $vf5, $vf0\n\t" \
        "vmr32.xyzw $vf6, $vf5\n\t" \
        "vmr32.xyzw $vf7, $vf6\n\t" \
        "sqc2 $vf0, 0x30(%0)\n\t" \
        "sqc2 $vf5, 0x20(%0)\n\t" \
        "sqc2 $vf6, 0x10(%0)\n\t" \
        "sqc2 $vf7, 0x0(%0)" \
        : : "r"(m) : "memory")

#define copyq(dst, src) \
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory")

typedef struct {
    u8 pad0;
    u8 f1;
    u8 pad2[2];
    s32 f4;
    s32 f8;
    s32 padC;
} Item;

typedef struct {
    s32 count;
    u8 pad4[0x48];
    Item *items;
    s16 *list;
} Mgr;

typedef struct {
    u8 f0;
    u8 f1;
    u8 f2;
    u8 pad3[9];
    s32 fC;
    u8 pad10[0x14];
    s32 f24;
    u8 pad28[4];
    s32 f2C;
    u8 pad30[4];
    s32 f34;
    f32 *f38;
    void *f3C;
    u8 pad40[0x18];
    s32 f58;
    s32 f5C;
    s32 f60;
    s32 f64;
    s32 f68;
    s32 f6C;
    f32 f70;
} Spr;

typedef struct {
    u8 pad0[0x14];
    s32 f14;
    s32 f18;
    s32 f1C;
} Stats;

#define MGR (*(Mgr **)0x70002708)

void VuCopy128(Spr *, s32);
void GphMeshDrawClip(Spr *, s32);
void GphMeshDrawNoClip(Spr *, s32);

void GphEnvMapDrawLayer(s32 arg0, u8 arg1) {
    FVECTOR v;
    FMATRIX m;
    Mgr *mgr;
    Item *base;
    Item *e;
    s16 *list;
    s32 n;
    Spr *spr;
    u8 *pkt;

    if (gphEnvMapTable[arg1].tex == 0) {
        return;
    }
    mgr = MGR;
    n = mgr->count;
    if (n == 0) {
        return;
    }
    list = mgr->list;
    n = (u16)*list;
    if (n == 0) {
        return;
    }
    base = mgr->items;
    spr = (Spr *)0x70003000;
    UNIT_MATRIX(m);
    pkt = (u8 *)&gphEnvMapGsPkt;
    GphEnvMapSendTex(arg1);
    copyq(v, gphEnvMapTable[arg1].color);
    GsGifPacketSend(pkt);
    GsFromSprDmaWait();
    list++;
    do {
        e = &base[*list];
        list++;
        if (e->f1 == arg0) {
            VuCopy128(spr, e->f4);
            spr->f0 &= 3;
            spr->f70 += 100.0f;
            spr->f58 = e->f8;
            spr->f3C = (void *)0x70002704;
            spr->f5C = e->f8;
            spr->f38 = v;
            spr->f68 = spr->f34;
            if (spr->f1 & 2) {
                if (spr->fC != 0) {
                    spr->f60 = spr->fC;
                }
                if (spr->f24 != 0) {
                    spr->f64 = spr->f24;
                }
                if (spr->f2C != 0) {
                    spr->f68 = spr->f2C;
                }
                if (spr->f34 != 0) {
                    spr->f6C = spr->f34;
                }
            }
            if (spr->f2 == 0) {
                GphMeshDrawNoClip(spr, 0);
            } else {
                GphMeshDrawClip(spr, 0);
            }
            if (*(volatile s32 *)0x700000E0 != 0) {
                ((Stats *)0x70002700)->f14 += *(s32 *)0x700000E0;
            } else {
                ((Stats *)0x70002700)->f1C++;
            }
            ((Stats *)0x70002700)->f18 += *(volatile s32 *)0x700000E4;
        }
        do { n--; } while (0); /* do-while(0): loop weight of n (allocation) */
    } while (n != 0);
}
