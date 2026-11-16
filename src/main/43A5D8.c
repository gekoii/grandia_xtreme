/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n04.h"
#include "main/n05.h"
#include "main/n07.h"

void GphMeshDrawClip(void *, s32);
void GphMeshDrawNoClip(void *, s32);
void VuCopy128(void *, s32);

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void GphShadowDrawLayer(u32 arg0) {
    FVECTOR a;
    FVECTOR b;
    s32 n;
    s32 idx;
    u64 *p;
    u64 e;
    u64 mask;
    GphDrawItem *o;
    s32 *q;
    GphDrawList *h;
    s32 *tbl;
    GsAdPkt *pk;

    n = gphDrawListCur->count;
    if (n == 0) {
        return;
    }
    GsGifPacketSend(&gphShadowGsPkt);
    pk = &gphShadowZbufPkt;
    pk->reg[0].addr = 0x4E;
    pk->reg[0].data = (u64)(gsZBufAddr >> 11) | 0x100000000;
    GsGifPacketSend(pk);
    copyq(a, gphShadowColor);
    copyq(b, gphShadowColor);
    h = gphDrawListCur;
    b[3] -= 128.0f;
    p = h->keys;
    tbl = (s32 *)h->layerStart;
    q = (s32 *)((arg0 << 2) + (u32)tbl);
    idx = *q;
    p += idx;
    if (arg0 < 15) {
        n = q[1] - idx;
    } else {
        n -= idx;
    }
    if (n == 0) {
        return;
    }
    do {
        e = *p++;
        mask = 0xFFFFFFFF;
        o = (GphDrawItem *)(s32)(e & mask);
        VuCopy128((void *)0x70003000, (s32)o->gaObj);
        ((GphGaObject *)0x70003000)->clip = o->layer;
        *(f32 **)0x70003038 = (((GphGaObject *)0x70003000)->unk30 != 0.0f) ? b : a;
        *(u8 *)0x70003000 &= 0xE7;
        *(s32 *)0x70003058 = o->mtx;
        if (*(u8 *)0x70003001 & 2) {
            if (*(s32 *)0x7000300C != 0) {
                *(s32 *)0x70003060 = *(s32 *)0x7000300C;
            }
            if (*(s32 *)0x70003024 != 0) {
                *(s32 *)0x70003064 = *(s32 *)0x70003024;
            }
            if (*(s32 *)0x7000302C != 0) {
                *(s32 *)0x70003068 = *(s32 *)0x7000302C;
            }
            if (*(s32 *)0x70003034 != 0) {
                *(s32 *)0x7000306C = *(s32 *)0x70003034;
            }
        }
        if (*(u8 *)0x70003002 == 0) {
            GphMeshDrawNoClip((void *)0x70003000, 0);
        } else {
            GphMeshDrawClip((void *)0x70003000, 0);
        }
        if (*(volatile s32 *)0x700000E0 != 0) {
            gphRenderCtx->unk14 += *(s32 *)0x700000E0;
        } else {
            gphRenderCtx->unk1C++;
        }
        gphRenderCtx->unk18 += *(volatile s32 *)0x700000E4;
    } while (--n != 0);
    pk = &gphShadowZbufPkt;
    pk->reg[0].addr = 0x4E;
    pk->reg[0].data = (u64)(gsZBufAddr >> 11);
    GsGifPacketSend(pk);
}
