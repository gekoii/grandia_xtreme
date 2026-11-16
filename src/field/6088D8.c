#include "common.h"

typedef struct {
    u8 pad0[6];
    s8 x;
    s8 y;
} Ofs;

extern u8 amapTemplateTable[];
extern u8 amapTemplateShapeTable[];

void AmapTemplateGetDoor(Ofs *p, s32 idx, s32 j, s32 *x, s32 *y) {
    u8 *b;
    u8 *e;

    e = (u8 *)((idx << 7) + (u32)amapTemplateTable);
    b = e[1] * 0x78 + amapTemplateShapeTable;
    p = (Ofs *)(b + j * 4);
    *x = p->x;
    *y = ((Ofs *)(b - -(j * 4)))->y;
}
