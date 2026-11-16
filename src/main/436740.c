#include "common.h"
#include "gph.h"
#include "protos/main.h"

typedef struct {
    u64 q[16];
} Blk80;


void VuMtxStackPush(u32);
void VuCopy128(void *, void *);
void GphMeshDrawNoClip(void *, s32);
void GphMeshDrawClip(void *, s32);

extern Blk80 gphDefaultGsRegsPacket;

void GphDrawListRender(u64 *list, s32 n) {
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

    VuMtxStackPush(0);
    def = &gphDefaultGsRegsPacket;
    *(Blk80 *)0x70003400 = *def;
    GsGifPacketSend((void *)0x70003400);
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
        VuCopy128((void *)0x70003800, o->gaObj);
        flags = *(u8 *)0x70003801;
        *(s32 *)0x70003858 = o->mtx;
        b0 = *(u8 *)0x70003800;
        *(s32 *)0x7000385C = o->mtx;
        *(u8 *)0x70003802 = o->layer;
        if (flags != 0) {
            if (flags & 1) {
                *(u8 *)0x70003800 = b0 & 0xE7;
            }
            if (flags & 2) {
                if (*(s32 *)0x7000380C != 0) {
                    *(s32 *)0x70003860 = *(s32 *)0x7000380C;
                }
                if (*(s32 *)0x70003824 != 0) {
                    *(s32 *)0x70003864 = *(s32 *)0x70003824;
                }
                if (*(s32 *)0x7000382C != 0) {
                    *(s32 *)0x70003868 = *(s32 *)0x7000382C;
                }
                if (*(s32 *)0x70003834 != 0) {
                    *(s32 *)0x7000386C = *(s32 *)0x70003834;
                }
            }
        }
        tex = *(s32 *)0x70003854;
        if (tex == 0) {
            if (first != 0) {
                first = 1;
                do { tex = (s32)def; } while (0); /* do-while(0): loop weight for tex/def allocation */
            }
        } else {
            first = 1;
        }
        if (*(u8 *)0x70003802 == 0) {
            GphMeshDrawNoClip((void *)0x70003800, tex);
        } else {
            GphMeshDrawClip((void *)0x70003800, tex);
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
