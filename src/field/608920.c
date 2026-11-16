#include "common.h"
#include "protos/field.h"

typedef struct {
    u8 a;
    u8 b;
    u8 c;
    u8 d;
    s16 id;
} Ent;

typedef struct {
    s16 idx;
    s8 x;
    s8 y;
    u8 f190;
    u8 f191;
    u8 pad[0x2A - 6];
} Rec;

typedef struct {
    u8 pad0[0xF];
    u8 type;
    u8 pad10[0x1E - 0x10];
    u8 count;
    u8 pad1F[0xEC - 0x1F];
    u16 n3;
    u16 n2;
    u16 n1;
    u8 padF2[0x108 - 0xF2];
    u8 f108;
    u8 f109;
    u8 f10A;
    u8 f10B;
    u8 f10C;
    u8 f10D;
    u8 pad10E[0x12C - 0x10E];
    Ent *e3;
    Ent *e2;
    Ent *e1;
    u8 pad138[0x18C - 0x138];
    Rec rooms[1];
} Obj;

typedef struct {
    u8 a;
    u8 b;
    s8 x;
    s8 y;
} Pt;

typedef struct {
    u8 w;
    u8 h;
    u8 n;
    u8 pad3;
    Pt pts[4];
    u8 tiles[0x64];
} Shape;

typedef struct {
    u8 type;
    u8 flags;
    u8 x;
    u8 y;
} Tmpl;

typedef struct {
    Tmpl e[32];
} Layout;

extern u8 amapTemplateTable[];
extern Shape amapTemplateShapeTable[];
extern s32 amapBoundY0;
extern s32 fldMapLayerSize;
extern s32 amapBoundY1;
extern s32 amapBoundX1;
extern s32 amapBoundX0;
extern u8 *fldMapCells;
extern s32 fldMapWidth;


/* Room layouts are 128 bytes: entry 0 holds the shape index in .flags.
 * ROOM(i) adds the scaled index before the base, as the original (&rooms[i] gives base-first). */
#define ROOM(i) ((Layout *)(((i) << 7) + (u32)rooms))
#define MAP(l, x, y) fldMapCells[fldMapLayerSize * (l) + (x) + (y) * fldMapWidth]

void AmapTemplateStamp(Obj *obj, s32 idx, s32 x, s32 y, s32 rx, s32 ry) {
    s32 w;
    s32 h;
    s32 s;
    s32 n;
    s32 cx;
    s32 cy;
    s32 hit;
    s32 i;
    s32 j;
    s32 k;
    s32 id;
    s32 f;
    Ent *e1;
    Ent *e2;
    Ent *e3;
    u8 *rooms;
    Shape *shapes;

    rooms = amapTemplateTable;
    shapes = amapTemplateShapeTable;
    s = ROOM(idx)->e[0].flags;
    obj->rooms[obj->count].f191 = 0;
    obj->rooms[obj->count].f190 = 0;
    obj->rooms[obj->count].idx = (u8)idx;
    n = 0;
    w = amapBoundX1 - shapes[s].w - amapBoundX0;
    h = amapBoundY1 - shapes[s].h - amapBoundY0;
    do {
        if (x == -1) {
            cx = VuRandInt(w) + amapBoundX0;
            cy = VuRandInt(h) + amapBoundY0;
        } else if (x <= 0) {
            cx = x;
            cy = y;
        } else {
            cx = x + (VuRandInt(rx * 2 + 1) - rx);
            cy = y + (VuRandInt(ry * 2 + 1) - ry);
        }
        hit = 0;
        for (j = -3; j < shapes[s].h + 3; j++) {
            for (i = -3; i < shapes[s].w + 3; i++) {
                if (MAP(1, cx + i, cy + j)) {
                    hit = 1;
                    break;
                }
            }
        }
        if (!hit) break;
    } while (++n < 0x401);
    obj->rooms[obj->count].x = cx;
    obj->rooms[obj->count].y = cy;
    for (j = -1; j < shapes[s].h + 1; j++) {
        for (i = -1; i < shapes[s].w + 1; i++) {
            AmapCellReserve(cx + i, cy + j);
        }
    }
    for (j = 0; j < shapes[s].h; j++) {
        for (i = 0; i < shapes[s].w; i++) {
            AmapCellSetRoom(cx + i, cy + j, shapes[s].tiles[j * shapes[s].w + i]);
            if (shapes[s].tiles[j * shapes[s].w + i] == 2) {
                MAP(6, cx + i, cy + j) = 3;
            }
        }
    }
    for (k = 0; k < shapes[s].n; k++) {
        i = shapes[s].pts[k].x;
        j = shapes[s].pts[k].y;
        /* Both arms are the same; jump2 merges them and leaves the load. */
        if (shapes[s].pts[k].b) {
            fldMapCells[fldMapLayerSize * 2 + ((cy + j) * fldMapWidth + (cx + i))] = 0;
        } else {
            fldMapCells[fldMapLayerSize * 2 + ((cy + j) * fldMapWidth + (cx + i))] = 0;
        }
    }
    e2 = obj->e2;
    e1 = obj->e1;
    e3 = obj->e3;
    for (k = 1; k < 32; k++) {
        id = 0;
        i = ROOM(idx)->e[k].x + cx;
        j = ROOM(idx)->e[k].y + cy;
        f = ROOM(idx)->e[k].flags & 0x1F;
        if (f) {
            id = obj->count * 0x32 + f;
        }
        switch (ROOM(idx)->e[k].type) {
        case 1: {
            u8 *fl;

            fl = &ROOM(idx)->e[0].flags;
            fl += k * 4;
            e1[obj->n1].a = *fl & 0xDF;
            e1[obj->n1].b = i;
            e1[obj->n1].c = 0;
            if (!(*fl & 0x20)) {
                e1[obj->n1].c = 2;
            }
            e1[obj->n1].d = j;
            e1[obj->n1].id = id;
            MAP(4, i, j) = 1;
            MAP(5, i, j) = 1;
            obj->n1++;
            break;
        }
        case 2:
            e2[obj->n2].b = ROOM(idx)->e[k].flags & 0xDF;
            e2[obj->n2].c = i;
            e2[obj->n2].d = j;
            e2[obj->n2].id = id;
            MAP(4, i, j) = 1;
            MAP(5, i, j) = 1;
            obj->n2++;
            break;
        case 3:
            e3[obj->n3].a = (ROOM(idx)->e[k].flags & 0xC0) | 3;
            e3[obj->n3].b = i;
            e3[obj->n3].d = j;
            e3[obj->n3].id = id;
            MAP(4, i, j) = 1;
            MAP(4, i, j - 1) = 1;
            MAP(4, 1 + i, j) = 1;
            MAP(4, i, j + 1) = 1;
            MAP(4, -1 + i, j) = 1;
            obj->n3++;
            break;
        case 4: {
            u8 *fl;

            fl = &ROOM(idx)->e[0].flags;
            fl += k * 4;
            e3[obj->n3].a = (*fl & 0xC0) | 0x18;
            e3[obj->n3].b = i;
            e3[obj->n3].c = 0;
            if (!(*fl & 0x20)) {
                e3[obj->n3].c = 1;
            }
            e3[obj->n3].d = j;
            e3[obj->n3].id = id;
            MAP(4, i, j) = 1;
            MAP(4, i, j - 1) = 1;
            MAP(4, 1 + i, j) = 1;
            MAP(4, i, j + 1) = 1;
            MAP(4, -1 + i, j) = 1;
            obj->n3++;
            break;
        }
        case 5:
            e3[obj->n3].a = (ROOM(idx)->e[k].flags & 0xC0) | 4;
            e3[obj->n3].b = i;
            e3[obj->n3].d = j;
            e3[obj->n3].id = id;
            MAP(4, i, j) = 1;
            MAP(4, i, j - 1) = 1;
            MAP(4, 1 + i, j) = 1;
            MAP(4, i, j + 1) = 1;
            MAP(4, -1 + i, j) = 1;
            obj->n3++;
            break;
        case 30:
            obj->f109 = i;
            obj->f108 = 1;
            obj->f10A = j;
            MAP(4, i, j) = 1;
            MAP(5, i, j) = 1;
            break;
        case 31:
            obj->f10C = i;
            obj->f10B = 1;
            obj->f10D = j;
            MAP(4, i, j) = 1;
            MAP(5, i, j) = 1;
            break;
        case 6: {
            s32 t;

            t = ROOM(idx)->e[k].flags;
            if ((t & 0x1F) != 0x12 || (u32)(obj->type - 5) < 2) {
                e3[obj->n3].a = t;
                e3[obj->n3].b = i;
                e3[obj->n3].d = j;
                e3[obj->n3].id = 0;
                MAP(4, i, j) = 1;
                MAP(5, i, j) = 1;
                obj->n3++;
            }
            break;
        }
        }
    }
    obj->count++;
}
