#include "common.h"
#include "gph.h"

typedef struct {
    u8 c[2][4];
} Cols;

typedef struct {
    u8 o[2];
} Offs;

extern Cols sysDemoCaptionColors;
extern Offs sysDemoCaptionOffsets;

GphBatchQuad *SysDemoCaptionPrims(GphBatchQuad *p, f32 alpha, f32 alpha2) {
    Cols col = sysDemoCaptionColors;
    Offs off = sysDemoCaptionOffsets;
    s32 i;
    u8 r, g, b;

    for (i = 0; i < 2; i++) {
        p->type = 1;
        p->v[0].r = r = col.c[i][0];
        p->v[0].g = g = col.c[i][1];
        p->v[0].b = b = col.c[i][2];
        p->v[0].a = col.c[i][3] * alpha;
        p->v[1].r = r;
        p->v[1].g = g;
        p->v[1].b = b;
        p->v[1].a = col.c[i][3] * alpha;
        p->v[2].r = r;
        p->v[2].g = g;
        p->v[2].b = b;
        p->v[2].a = col.c[i][3] * alpha;
        p->v[3].r = r;
        p->v[3].g = g;
        p->v[3].b = b;
        p->v[3].a = col.c[i][3] * alpha;
        p->v[0].x = (s8)off.o[i];
        p->v[0].y = (s8)off.o[i];
        p->v[1].x = (s8)off.o[i];
        p->v[1].y = (s8)off.o[i] + 0x18;
        p->v[2].x = (s8)off.o[i] + 0xa8;
        p->v[2].y = (s8)off.o[i];
        p->v[3].x = (s8)off.o[i] + 0xa8;
        p->v[3].y = (s8)off.o[i] + 0x18;
        p->z = 0;
        p->v[0].u = 0;
        p->v[0].v = 0x98;
        p->v[1].u = 0;
        p->v[1].v = 0xb0;
        p->v[2].u = 0xa8;
        p->v[2].v = 0x98;
        p->v[3].u = 0xa8;
        p->v[3].v = 0xb0;
        p++;
        p->type = 1;
        p->v[0].r = r;
        p->v[0].g = g;
        p->v[0].b = b;
        p->v[0].a = col.c[i][3] * alpha;
        p->v[1].r = r;
        p->v[1].g = g;
        p->v[1].b = b;
        p->v[1].a = col.c[i][3] * alpha;
        p->v[2].r = r;
        p->v[2].g = g;
        p->v[2].b = b;
        p->v[2].a = col.c[i][3] * alpha;
        p->v[3].r = r;
        p->v[3].g = g;
        p->v[3].b = b;
        p->v[3].a = col.c[i][3] * alpha;
        p->v[0].x = (s8)off.o[i] + 0xb0;
        p->v[0].y = (s8)off.o[i];
        p->v[1].x = (s8)off.o[i] + 0xb0;
        p->v[1].y = (s8)off.o[i] + 0x18;
        p->v[2].x = (s8)off.o[i] + 0x130;
        p->v[2].y = (s8)off.o[i];
        p->v[3].x = (s8)off.o[i] + 0x130;
        p->v[3].y = (s8)off.o[i] + 0x18;
        p->z = 0;
        p->v[0].u = 0;
        p->v[0].v = 0xb0;
        p->v[1].u = 0;
        p->v[1].v = 0xc8;
        p->v[2].u = 0x80;
        p->v[2].v = 0xb0;
        p->v[3].u = 0x80;
        p->v[3].v = 0xc8;
        p++;
        p->type = 1;
        p->v[0].r = r;
        p->v[0].g = g;
        p->v[0].b = b;
        p->v[0].a = col.c[i][3] * alpha2;
        p->v[1].r = r;
        p->v[1].g = g;
        p->v[1].b = b;
        p->v[1].a = col.c[i][3] * alpha2;
        p->v[2].r = r;
        p->v[2].g = g;
        p->v[2].b = b;
        p->v[2].a = col.c[i][3] * alpha2;
        p->v[3].r = r;
        p->v[3].g = g;
        p->v[3].b = b;
        p->v[3].a = col.c[i][3] * alpha2;
        p->v[0].x = (s8)off.o[i];
        p->v[0].y = (s8)off.o[i] + 0x18;
        p->v[1].x = (s8)off.o[i];
        p->v[1].y = (s8)off.o[i] + 0x30;
        p->v[2].x = (s8)off.o[i] + 0xe8;
        p->v[2].y = (s8)off.o[i] + 0x18;
        p->v[3].x = (s8)off.o[i] + 0xe8;
        p->v[3].y = (s8)off.o[i] + 0x30;
        p->z = 0;
        p->v[0].u = 0;
        p->v[0].v = 0xc8;
        p->v[1].u = 0;
        p->v[1].v = 0xe0;
        p->v[2].u = 0xe8;
        p->v[2].v = 0xc8;
        p->v[3].u = 0xe8;
        p->v[3].v = 0xe0;
        p++;
    }
    p->type = 0;
    return p;
}
