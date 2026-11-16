#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n05.h"

extern void GphMeshDrawClip(GphGaObject *, s32);
extern void GphMeshDrawNoClip(GphGaObject *, s32);
extern void FlushCache(s32);

void GphProjTexDraw(void) {
    f32 vec[4];
    GphProjTexEntry *e;
    GphGaObject *m;
    s32 n;
    s32 v;
    s32 t;

    GsFromSprDmaWait();
    GphTexBind(0xFF);
    FlushCache(0);
    GsGifPacketSend((u16 *)&gphProjTexGsPkt);
    GsFromSprDmaWait();
    v = *(s32 *)0x70002704;
    n = gphProjTexList->count;
    e = gphProjTexList->entries;
    vec[0] = 128.0f;
    vec[1] = 128.0f;
    vec[2] = 128.0f;
    vec[3] = 128.0f;
    if (n > 0) {
    do {
        switch (e->type) {
        case 0:
            break;
        case 1:
            m = e->gaObj;
            t = e->mtx;
            m->mtx = t;
            m->mtx2 = (void *)t;
            m->ambient = vec;
            m->lightBlock = v;
            m->uOffset = 0.5f;
            m->vOffset = 0.5f;
            m->unk70 = 1000.0f;
            m->clip = e->layer;
            m->mode = (m->mode & 0xE7) | 3;
            if (m->clip == 0) {
                GphMeshDrawNoClip(m, 0);
            } else {
                GphMeshDrawClip(m, 0);
            }
            if (*(volatile s32 *)0x700000E0 != 0) {
                gphRenderCtx->unk14 += *(volatile s32 *)0x700000E0;
            } else {
                gphRenderCtx->unk1C++;
            }
            gphRenderCtx->unk18 += *(volatile s32 *)0x700000E4;
            break;
        }
        n--;
        GsFromSprDmaWait();
        e++;
    } while (n > 0);
    }
}
