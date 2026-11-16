/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n07.h"
#include "protos/main.h"

typedef struct {
    f32 x, y, z, w;
} Vec4;

typedef struct {
    u8 pad[0xE40];
    Vec4 out[4];
} ScratchPad;

extern s32 MemMove128(void *, void *, s32);
extern void VuProjectVerts(void *, void *, void *, s32);

#define RGBAQ(c) ((u64)(u32)((((c).a << 24) & 0xFF000000) | (((c).b << 16) & 0xFF0000) | (((c).g << 8) & 0xFF00) | ((c).r & 0xFF)) | ((u64)0x3F800000 << 32))
#define UV(t) (((u64)(((t)[1] << 4) & 0xFFF0) << 16) | (((t)[0] << 4) & 0xFFF0))
#define XYZ(q) ((((s32)(q).x + xo) << 4) | (((s32)(q).y + yo) << 20) | ((u64)(s32)(q).z << 32))

void GphBatchDrawQuads3D(GphQuad3D *p) {
    GphBatchScratch *spr;
    GphBatchQuadPkt *pkt;
    ScratchPad *t;
    Vec4 *out;
    s32 cur;
    s32 buf;
    s64 xo;
    s64 yo;
    void *mtx;
    s32 clip;
    s32 i;

    if (p->type == 0) {
        return;
    }
    MemMove128((void *)0x70003180, &gphBatchPackets, 0xE8);
    spr = (GphBatchScratch *)0x70003180;
    buf = 0;
    cur = -1;
    xo = gphBatchOffsetX + 0x800;
    yo = gphBatchOffsetY + 0x800;
    mtx = VuMtxStackGetTop();
    spr->pkts.line[1].v[0].rgbaq = 0;
    spr->pkts.sprite[0].rgbaq = 0;
    out = (Vec4 *)spr->projOut;
    for (;;) {
        VuProjectVerts(out, mtx, p->pos, 4);
        clip = 0;
        for (i = 0; i < 4; i++) {
            t = (ScratchPad *)((u8 *)spr + i * 16);
            if (t->out[0].x > 2047.9f || t->out[0].x < -2047.9f) {
                clip = 1;
                break;
            }
            if (t->out[0].y > 2047.9f || t->out[0].y < -2047.9f || t->out[0].z < 0.0f) {
                clip = 1;
                break;
            }
            t->out[0].z *= 0.0625f;
        }
        if (clip == 1) {
            p++;
            if (p->type == 0) {
                break;
            }
            continue;
        }
        if (cur != p->type) {
            GsRenderStatePacketSend(&spr->states[p->type - 1]);
            cur = p->type;
        }
        pkt = &spr->pkts.quad[buf];
        pkt->v[0].rgbaq = RGBAQ(p->col[0]);
        pkt->v[0].uv = UV(p->uv[0]);
        pkt->v[0].xyz = XYZ(out[0]);
        pkt->v[1].rgbaq = RGBAQ(p->col[1]);
        pkt->v[1].uv = UV(p->uv[1]);
        pkt->v[1].xyz = XYZ(out[1]);
        pkt->v[2].rgbaq = RGBAQ(p->col[2]);
        pkt->v[2].uv = UV(p->uv[2]);
        pkt->v[2].xyz = XYZ(out[2]);
        pkt->v[3].rgbaq = RGBAQ(p->col[3]);
        pkt->v[3].uv = UV(p->uv[3]);
        pkt->v[3].xyz = XYZ(out[3]);
        p++;
        GsFromSprDmaWait();
        GsGifPacketSend(pkt);
        if (p->type == 0) {
            break;
        }
        buf ^= 1;
    }
    GsFromSprDmaWait();
}
