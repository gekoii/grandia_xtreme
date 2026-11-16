/* Port-only C for FldMenuJunctionRecipeCellBuild (0x00637978), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "field/menu.h"
#include "field/n24.h"

/* One recipe grid cell: a background sprite on layer 1, then two frame
   corner sprites on layer 2 (scaled down when the cell is 128 or less).
   Each sprite list is ended by a type 0 after the last one written. The
   float to s16 stores truncate, like cvt.w.s. */
void FldMenuJunctionRecipeCellBuild(FldMenuRecipeCell *cell) {
    FldMenuQuad *q;
    f32 bx;
    f32 bw;
    f32 by;
    f32 bh;
    f32 k;

    q = cell->spr;
    q->type = 1;
    q->v[0].r = cell->alpha;
    q->v[0].g = cell->alpha;
    q->v[0].b = cell->alpha;
    q->v[0].a = 0;
    q[1].type = 0;
    cell->spr = q + 1;
    q->v[0].x = cell->x;
    q->z = 0;
    q->v[0].y = cell->y;
    q->v[1].x = cell->x + cell->w;
    q->v[1].y = cell->y + cell->h;
    q->v[0].u = 0;
    q->v[0].v = 0;
    q->v[1].u = 0x80;
    q->v[1].v = 0x80;

    if (cell->w > 128) {
        bx = 12.0f;
        bw = 64.0f;
    } else {
        k = (f32)cell->w * 0.0078125f;
        bx = k * 12.0f;
        bw = k * 64.0f;
    }
    if (cell->h > 128) {
        bh = 48.0f;
        by = 12.0f;
    } else {
        k = (f32)cell->h * 0.0078125f;
        by = k * 12.0f;
        bh = k * 48.0f;
    }

    q = cell->spr2;
    q->type = 1;
    q->v[0].r = cell->alpha;
    q->v[0].g = cell->alpha;
    q->v[0].b = cell->alpha;
    q->v[0].a = 0;
    cell->spr2 = q + 1;
    q->v[0].x = (s32)((f32)cell->x + bx);
    q->v[0].y = (s32)((f32)cell->y + by);
    q->v[1].x = (s32)((f32)cell->x + bx + bw);
    q[1].type = 1;
    q->v[1].v = 0x30;
    q->z = 0;
    q->v[0].u = 0;
    q->v[0].v = 0;
    q->v[1].u = 0x40;
    q->v[1].y = (s32)((f32)cell->y + by + bh);

    q = cell->spr2;
    q->v[0].r = cell->alpha;
    q->v[0].g = cell->alpha;
    q->v[0].b = cell->alpha;
    q->v[0].a = 0;
    cell->spr2 = q + 1;
    q->v[0].x = (s32)((f32)(cell->x + cell->w) - bw - bx);
    q->v[0].y = (s32)((f32)(cell->y + cell->h) - bh - by);
    q->v[1].x = (s32)((f32)(cell->x + cell->w) - bw - bx + bw);
    q->v[0].v = 0x34;
    q->v[1].u = 0x40;
    q->v[1].v = 0x64;
    q[1].type = 0;
    q->z = 0;
    q->v[0].u = 0;
    q->v[1].y = (s32)((f32)(cell->y + cell->h) - bh - by + bh);
}
