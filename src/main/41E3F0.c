#include "common.h"
#include "gph.h"

typedef struct {
    u8 c[3][4];
} Cols;

typedef struct {
    u8 o[3];
} Offs;

extern Cols sysDemoPromptColors;
extern Offs sysDemoPromptOffsets;

GphBatchQuad *SysDemoPromptPrims(GphBatchQuad *p, f32 alpha) {
    GphBatchQuad *q;
    Cols col = sysDemoPromptColors;
    Offs off = sysDemoPromptOffsets;
    s32 i;
    u8 r, g, b;

    for (i = 0; i < 3; i++) {
        q = p + 1;
        p[0].type = 1;
        p[0].v[0].r = r = col.c[i][0];
        p[0].v[0].g = g = col.c[i][1];
        p[0].v[0].b = b = col.c[i][2];
        p[0].v[0].a = col.c[i][3] * alpha;
        p[0].v[1].r = r;
        p[0].v[1].g = g;
        p[0].v[1].b = b;
        p[0].v[1].a = col.c[i][3] * alpha;
        p[0].v[2].r = r;
        p[0].v[2].g = g;
        p[0].v[2].b = b;
        p[0].v[2].a = col.c[i][3] * alpha;
        p[0].v[3].r = r;
        p[0].v[3].g = g;
        p[0].v[3].b = b;
        p[0].v[3].a = col.c[i][3] * alpha;
        p[0].v[0].x = (s8)off.o[i];
        p[0].v[0].y = 0;
        p[0].v[1].x = (s8)off.o[i];
        p[0].v[1].y = 24;
        p[0].v[2].x = (s8)off.o[i] + 0xAC;
        p[0].v[2].y = 0;
        p[0].v[3].x = (s8)off.o[i] + 0xAC;
        p[0].v[3].y = 24;
        p[0].z = 0;
        p[0].v[0].u = 0;
        p[0].v[0].v = 0x68;
        p[0].v[1].u = 0;
        p[0].v[1].v = 0x80;
        p[0].v[2].u = 0xac;
        p[0].v[2].v = 0x68;
        p[0].v[3].u = 0xac;
        p[0].v[3].v = 0x80;
        q->type = 1;
        q->v[0].r = r;
        q->v[0].g = g;
        q->v[0].b = b;
        q->v[0].a = col.c[i][3] * alpha;
        q->v[1].r = r;
        q->v[1].g = g;
        q->v[1].b = b;
        q->v[1].a = col.c[i][3] * alpha;
        q->v[2].r = r;
        q->v[2].g = g;
        q->v[2].b = b;
        q->v[2].a = col.c[i][3] * alpha;
        q->v[3].r = r;
        q->v[3].g = g;
        q->v[3].b = b;
        q->v[3].a = col.c[i][3] * alpha;
        q->v[0].x = (s8)off.o[i] + 0xB2;
        q->v[0].y = 0;
        q->v[1].x = (s8)off.o[i] + 0xB2;
        q->v[1].y = 24;
        q->v[2].x = (s8)off.o[i] + 0x11A;
        q->v[2].y = 0;
        q->v[3].x = (s8)off.o[i] + 0x11A;
        q->v[3].y = 24;
        q->z = 0;
        q->v[0].u = 0;
        q->v[0].v = 0x80;
        q->v[1].u = 0;
        q->v[1].v = 0x98;
        q->v[2].u = 0x68;
        q->v[2].v = 0x80;
        q->v[3].u = 0x68;
        q->v[3].v = 0x98;
        p += 2;
    }
    p->type = 0;
    return p;
}
