#include "common.h"
#include "gph.h"
#include "protos/main.h"

typedef struct {
    u64 q[16];
} Blk80;

void VuMtxStackPush(u32);
void VuMtxNormalizeAxes(void *, s32);
void VuCopy128(void *, void *);
void GphMeshDrawNoClip(void *, s32);
void GphMeshDrawClip(void *, s32);

extern Blk80 gphDefaultGsRegsPacket;

void GphDrawListRenderNormLight(u64 *list, s32 n) {
    s32 total = 0;
    u32 lastTex = 0xFF;
    s32 sum = 0;
    s32 misses = 0;
    s32 first = 0;
    Blk80 *def;
    u64 mask;
    u64 e;
    GphDrawItem *o;
    u8 flags;
    u8 b0;
    s32 tex;
    s32 v;
    f32 *mtx;

    VuMtxStackPush(0);
    def = &gphDefaultGsRegsPacket;
    *(Blk80 *)0x70003400 = *def;
    mtx = (f32 *)0x70003680;
    GsGifPacketSend((void *)0x70003400);
    __asm__ volatile(
        "vmr32.xyzw $vf5, $vf0\n\t"
        "vmr32.xyzw $vf6, $vf5\n\t"
        "vmr32.xyzw $vf7, $vf6\n\t"
        "sqc2 $vf0, 0x30(%0)\n\t"
        "sqc2 $vf5, 0x20(%0)\n\t"
        "sqc2 $vf6, 0x10(%0)\n\t"
        "sqc2 $vf7, 0x0(%0)"
        : : "r"(mtx) : "memory");
    do {
        e = *list++;
        mask = 0xFFFFFFFF;
        o = (GphDrawItem *)(s32)(e & mask);
        if (lastTex != o->texId) {
            lastTex = o->texId;
            first = 0;
            GphTexBind(lastTex);
            GsGifPacketSend((void *)0x70003400);
        }
        VuCopy128((void *)0x70003600, o->gaObj);
        flags = *(u8 *)0x70003601;
        VuMtxNormalizeAxes((void *)0x70003680, o->mtx);
        *(f32 **)0x7000365C = mtx;
        *(s32 *)0x70003658 = o->mtx;
        b0 = *(u8 *)0x70003600;
        *(u8 *)0x70003602 = o->layer;
        if (flags != 0) {
            if (flags & 1) {
                *(u8 *)0x70003600 = b0 & 0xE7;
            }
            if (flags & 2) {
                if (*(s32 *)0x7000360C != 0) {
                    *(s32 *)0x70003660 = *(s32 *)0x7000360C;
                }
                if (*(s32 *)0x70003624 != 0) {
                    *(s32 *)0x70003664 = *(s32 *)0x70003624;
                }
                if (*(s32 *)0x7000362C != 0) {
                    *(s32 *)0x70003668 = *(s32 *)0x7000362C;
                }
                if (*(s32 *)0x70003634 != 0) {
                    *(s32 *)0x7000366C = *(s32 *)0x70003634;
                }
            }
        }
        tex = *(s32 *)0x70003654;
        if (tex == 0) {
            if (first != 0) {
                first = 1;
                do { tex = (s32)def; } while (0); /* do-while(0): loop weight for tex/def allocation */
            }
        } else {
            first = 1;
        }
        if (*(u8 *)0x70003602 == 0) {
            GphMeshDrawNoClip((void *)0x70003600, tex);
        } else {
            GphMeshDrawClip((void *)0x70003600, tex);
        }
        v = *(volatile s32 *)0x700000E0;
        if (v != 0) {
            total += v;
        } else {
            misses++;
        }
        sum += *(volatile s32 *)0x700000E4;
    } while (--n > 0);
    gphRenderCtx->unk14 += total;
    gphRenderCtx->unk1C += misses;
    gphRenderCtx->unk18 += sum;
}
