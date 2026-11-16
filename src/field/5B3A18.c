#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/state.h"
#include "field/event.h"

typedef struct FieldSrc {
    /* 0x00 */ s8 type;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ s8 unk3;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ s8 unkC;
    /* 0x0D */ u8 link;
    /* 0x0E */ s16 linkId;
    /* 0x10 */ VuVec3 p0;
    /* 0x1C */ VuVec3 p1;
    /* 0x28 */ VuVec3 base;
    /* 0x34 */ VuVec3 rot;
    /* 0x40 */ f32 r;
    /* 0x44 */ f32 h;
} FieldSrc;

extern s32 evtAreaShapeTable[];
extern s32 evtAreaTargetMaskTable[];
extern s32 evtAreaAttrTable[];
extern s32 evtAreaFireCondTable[];

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void EvtAreaLoad(FieldSrc *src, s32 n) {
    f32 cx = 0.0f;
    f32 cy = cx;
    f32 cz = cx;
    s32 i;
    FieldSrc *s;
    EvtArea *o;
    f32 one;
    f32 dx, dy, dz;
    f32 *b;
    f32 h, r, x0;
    u32 fl;

    EvtAreaSetCount(n);
    i = 0;
    if (n > 0) {
    do {
        s = &src[i];
        o = EvtAreaGet(i);
        o->id = i;
        o->unk0 = 0x10;
        o->state = 0;
        o->target = s->unk1;
        o->flags = s->unk8;
        o->result = 0;
        o->link = NULL;
        if (o->target != 3) {
            switch ((s8)s->link) {
            case 0:
                o->link = NULL;
                break;
            case 1:
                o->link = FldObjGet(s->linkId);
                break;
            case 2:
                o->link = ChrTenantGet(s->linkId)->model;
                break;
            }
        }
        o->shape = evtAreaShapeTable[s->type];
        if (o->target == 2 || o->target == 6) {
            if (s->type >= 2) {
                o->target = 7;
            }
        }
        if (o->shape != 0) {
            o->flags &= ~4;
        }
        o->chk = evtAreaTargetMaskTable[s->unk3];
        o->unkA = evtAreaAttrTable[s->unkC];
        o->mode = evtAreaFireCondTable[s->unk2];
        one = 1.0f;
        o->scale[0] = one;
        o->scale[1] = one;
        o->scale[2] = one;
        o->unk5C = 0.0f;
        o->ofs[0] = 0.0f;
        o->ofs[1] = 0.0f;
        o->ofs[2] = 0.0f;
        o->ofs[3] = 0.0f;
        o->result = 0;
        switch (s->type) {
        case 0:
            b = o->shapeData.box;
            h = s->p0.x;
            cx = (s->p1.x + h) * 0.5f;
            cy = (s->p1.y + s->p0.y) * 0.5f;
            cz = (s->p1.z + s->p0.z) * 0.5f;
            dx = h - cx;
            dy = s->p0.y - cy;
            dz = s->p0.z - cz;
            o->radius = fsqrt(dx * dx + dy * dy + dz * dz);
            o->pos[0] = cx;
            o->pos[1] = cy;
            o->pos[2] = cz;
            o->shapeData.box[0] = fabsf(s->p0.x - s->p1.x);
            b[1] = fabsf(s->p0.y - s->p1.y);
            b[2] = fabsf(s->p0.z - s->p1.z);
            b[3] = one;
            break;
        case 1:
            h = s->h;
            cx = (s->p0.x + s->p1.x) * 0.5f;
            cy = (s->p0.y + s->p1.y) * 0.5f + h * 0.5f;
            cz = (s->p0.z + s->p1.z) * 0.5f;
            o->pos[2] = cz;
            o->pos[0] = cx;
            o->pos[1] = cy;
            b = o->shapeData.box;
            o->shapeData.box[0] = s->p0.x - cx;
            b[1] = s->p0.y - cy;
            b[2] = s->p0.z - cz;
            b[4] = s->p1.x - cx;
            b[5] = s->p1.y - cy;
            b[6] = s->p1.z - cz;
            b[8] = s->h;
            o->radius = fsqrt(o->shapeData.box[0] * o->shapeData.box[0] + b[1] * b[1] + b[2] * b[2]);
            break;
        case 2:
            b = o->shapeData.box;
            h = s->h;
            r = s->r;
            cx = s->p0.x;
            cy = s->p0.y + h * 0.5f;
            cz = s->p0.z;
            o->pos[0] = cx;
            o->pos[2] = cz;
            o->pos[1] = cy;
            o->radius = fsqrt(r * r + h * 0.25f * h);
            o->shapeData.box[0] = 0.0f;
            b[4] = r;
            b[3] = one;
            b[5] = h;
            b[2] = 0.0f;
            b[1] = -h * 0.5f;
            break;
        case 3:
            cx = s->p0.x;
            cy = s->p0.y;
            cz = s->p0.z;
            o->pos[0] = cx;
            o->pos[1] = cy;
            o->pos[2] = cz;
            o->radius = s->r;
            break;
        }
        fl = o->flags;
        if (fl & 4) {
            o->pos[0] = s->base.x;
            o->pos[1] = s->base.y;
            o->pos[2] = s->base.z;
            o->ofs[0] = cx - o->pos[0];
            o->ofs[1] = cy - o->pos[1];
            o->ofs[2] = cz - o->pos[2];
        }
        o->rot[0] = s->rot.x * 182.04445f;
        o->rot[1] = s->rot.y * 182.04445f;
        o->rot[2] = s->rot.z * 182.04445f;
        if (o->link != NULL) {
            o->pos[0] -= o->link->pos[0];
            o->pos[1] -= o->link->pos[1];
            o->pos[2] -= o->link->pos[2];
        }
        o->pos[3] = 1.0f;
        o->ofs[3] = 1.0f;
        switch (o->target) {
        case 0:
            o->start = s->unk4;
            o->count = s->unk6;
            break;
        case 3:
            o->start = s->linkId;
            o->count = (s8)s->link;
            break;
        default:
            o->start = (s8)s->link;
            o->count = s->linkId;
            break;
        }
        i++;
    } while (i < n);
    }
    EvtAreaSetCount(n);
}
