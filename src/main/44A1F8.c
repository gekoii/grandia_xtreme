#include "common.h"
#include "main/n07.h"

typedef struct {
    s16 a[8];
} V8;

extern void GsTexSetupDraw(s32, s32, void *, s32, s32, s32);
extern u8 ptclLightBlock[];
extern V8 ptclStripIndices[];
extern u8 ptclRsAlphaFog[];
extern u8 ptclRsAddFog[];
extern u8 ptclRsSubFog[];
extern u8 ptclRsAlpha[];
extern u8 ptclRsAdd[];
extern u8 ptclRsSub[];

void PtclInit(void) {
    s32 i;
    s32 j;
    s32 k;
    s16 *p;
    u32 *q1;
    s16 *p2;
    u32 *q2;
    u8 *c;
    f32 f;
    GphGaObject *cx;

    GsTexSetupDraw(0, 0, ptclRsAlphaFog, 0, 1, 0);
    GsTexSetupDraw(0, 0, ptclRsAddFog, 1, 1, 0);
    GsTexSetupDraw(0, 0, ptclRsSubFog, 2, 1, 0);
    GsTexSetupDraw(0, 0, ptclRsAlpha, 0, 0, 0);
    GsTexSetupDraw(0, 0, ptclRsAdd, 1, 0, 0);
    GsTexSetupDraw(0, 0, ptclRsSub, 2, 0, 0);

    p = (s16 *)ptclStripIndices;
    for (i = 30; i >= 0; i--) {
        *p++ = i * 64;
        *p++ = i * 64 + 16;
        *p++ = i * 64 + 32;
        *p++ = i * 32;
        *p++ = i * 64;
        *p++ = i * 64 + 32;
        *p++ = i * 64 + 48;
        *p++ = i * 32 + 16;
    }
    *p++ = 0;
    *p++ = 0;
    *p++ = 0;
    *p++ = 0;

    q1 = ptclStripUvs;
    *q1++ = 0x20000000;
    *q1++ = 0x10101010;
    *q1++ = 0;
    *q1++ = 0x757C0080;
    p2 = (s16 *)q1;
    for (j = 0; j < 31; j++) {
        *p2++ = 0;
        *p2++ = 0x400;
        *p2++ = 0;
        *p2++ = 0;
        *p2++ = 0x400;
        *p2++ = 0;
        *p2++ = 0x400;
        *p2++ = 0x400;
    }

    q2 = ptclStripColors;
    *q2++ = 0;
    *q2++ = 0;
    *q2++ = 0;
    *q2++ = 0x6E7C0100;
    c = (u8 *)q2;
    for (k = 0; k < 31; k++) {
        *c++ = 0; *c++ = 0; *c++ = 0; *c++ = 0;
        *c++ = 0; *c++ = 0; *c++ = 0; *c++ = 0;
        *c++ = 0; *c++ = 0; *c++ = 0; *c++ = 0;
        *c++ = 0; *c++ = 0; *c++ = 0; *c++ = 0;
    }

    cx = &ptclDrawCtx;
    f = 1.0f;
    cx->flags = 0;
    cx->clip = 0;
    cx->unk3 = 0;
    cx->center[0] = 0.0f;
    cx->center[1] = 0.0f;
    cx->center[2] = 0.0f;
    cx->radius = 2000.0f;
    cx->deformBuf0 = NULL;
    cx->deformBuf1 = NULL;
    cx->unk30 = 0.0f;
    cx->envUv = NULL;
    cx->unk40 = 0;
    cx->skin = NULL;
    cx->mtx2 = NULL;
    cx->mtx = 0;
    cx->verts = NULL;
    cx->normals = NULL;
    cx->unk70 = 0;
    cx->unk74 = 0;
    cx->uOffset = 0.0f;
    cx->mode = 0x22;
    cx->center[3] = f;
    cx->unk28 = 2000.0f;
    cx->ambient = ptclDefaultColor;
    cx->lightBlock = (s32)ptclLightBlock;
    cx->triCount = 0x3E;
    cx->vtxCount = 0x7C;
    cx->indices = ptclStripIndices;
    cx->uvs = ptclStripUvs;
    cx->colors = ptclStripColors;
    cx->vOffset = 0.0f;
}
