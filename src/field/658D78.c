#include "common.h"
#include "protos/field.h"
#include "ptcl.h"
#include "field/state.h"
#include "field/n25.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} Col;

typedef struct {
    u16 u;
    u16 v;
} UV;

typedef struct {
    s32 unk0;
    u8 pad4[0x4C];
    Col col[4];
    UV uv[4];
} Pkt;

typedef struct {
    s16 v[8];
} Entry;

void GphQuad3DSetCorners(Pkt *, void *, FVECTOR *, f32, f32);
void GsTexUpload(void *);

#define SWAP(a, b) if ((a) != (b)) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

void FldSpriteDraw(void) {
    FVECTOR m[4];
    s32 kind;
    s32 i;
    PtclFldSprite *s;
    Pkt *pk;
    Entry *e;

    GphBatchSetOffset(0, 0);
    VuMtxSetRotXY(m, camField[0].pitch, camField[0].yaw, camField[0].roll);
    for (kind = 0; kind < 9; kind++) {
        if (fldSpriteWork->counts[kind] == 0) {
            continue;
        }
        pk = (Pkt *)fldSpriteWork->quads;
        for (i = 0; i < 0x40; i++) {
            s = &ptclFldSlots[i];
            if (s->used == 0 || s->kind != kind) {
                continue;
            }
            FldSpriteSelectAnim(kind);
            e = GphAnimGetCellUv(s->tex);
            pk->unk0 = 0xE;
            if (kind == 7) {
                pk->col[0].r = 0x40;
                pk->col[0].g = 0xF0;
                pk->col[0].b = 0x40;
                pk->col[0].a = s->alpha;
                pk->col[1].r = 0x40;
                pk->col[1].g = 0xF0;
                pk->col[1].b = 0x40;
                pk->col[1].a = s->alpha;
                pk->col[2].r = 0x40;
                pk->col[2].g = 0xF0;
                pk->col[2].b = 0x40;
                pk->col[2].a = s->alpha;
                pk->col[3].r = 0x40;
                pk->col[3].g = 0xF0;
                pk->col[3].b = 0x40;
                pk->col[3].a = s->alpha;
            } else if (kind == 8) {
                pk->col[0].r = 0x40;
                pk->col[0].g = 0x20;
                pk->col[0].b = 0;
                pk->col[0].a = s->alpha;
                pk->col[1].r = 0x40;
                pk->col[1].g = 0x20;
                pk->col[1].b = 0;
                pk->col[1].a = s->alpha;
                pk->col[2].r = 0x40;
                pk->col[2].g = 0x20;
                pk->col[2].b = 0;
                pk->col[2].a = s->alpha;
                pk->col[3].r = 0x40;
                pk->col[3].g = 0x20;
                pk->col[3].b = 0;
                pk->col[3].a = s->alpha;
            } else if (kind == 2) {
                pk->col[0].r = 0x80;
                pk->col[0].g = 0xA0;
                pk->col[0].b = 0xF0;
                pk->col[0].a = s->alpha;
                pk->col[1].r = 0x80;
                pk->col[1].g = 0xA0;
                pk->col[1].b = 0xF0;
                pk->col[1].a = s->alpha;
                pk->col[2].r = 0x80;
                pk->col[2].g = 0xA0;
                pk->col[2].b = 0xF0;
                pk->col[2].a = s->alpha;
                pk->col[3].r = 0x80;
                pk->col[3].g = 0xA0;
                pk->col[3].b = 0xF0;
                pk->col[3].a = s->alpha;
            } else {
                pk->col[0].r = 0x80;
                pk->col[0].g = 0x80;
                pk->col[0].b = 0x80;
                pk->col[0].a = s->alpha;
                pk->col[1].r = 0x80;
                pk->col[1].g = 0x80;
                pk->col[1].b = 0x80;
                pk->col[1].a = s->alpha;
                pk->col[2].r = 0x80;
                pk->col[2].g = 0x80;
                pk->col[2].b = 0x80;
                pk->col[2].a = s->alpha;
                pk->col[3].r = 0x80;
                pk->col[3].g = 0x80;
                pk->col[3].b = 0x80;
                pk->col[3].a = s->alpha;
            }
            GphQuad3DSetCorners(pk, m, &s->pos, s->size, s->size);
            pk->uv[0].u = e->v[2] / 8;
            pk->uv[0].v = e->v[3] / 8;
            pk->uv[1].u = e->v[0] / 8;
            pk->uv[1].v = e->v[1] / 8;
            pk->uv[2].u = e->v[4] / 8;
            pk->uv[2].v = e->v[5] / 8;
            pk->uv[3].u = e->v[6] / 8;
            pk->uv[3].v = e->v[7] / 8;
            if (s->flags & 1) {
                SWAP(pk->uv[0].v, pk->uv[1].v);
                SWAP(pk->uv[2].v, pk->uv[3].v);
            }
            if (s->flags & 2) {
                SWAP(pk->uv[0].u, pk->uv[2].u);
                SWAP(pk->uv[1].u, pk->uv[3].u);
            }
            pk++;
        }
        GsTexUpload(fldSpriteWork->texPackets[kind]);
        GsTexPacketSend(fldSpriteWork->texPackets[kind]);
        pk->unk0 = 0;
        GphBatchDrawQuads3D((Pkt *)fldSpriteWork->quads);
    }
}
