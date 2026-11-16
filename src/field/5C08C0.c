#include "common.h"
#include "field/n13.h"
#include "protos/field.h"

typedef struct {
    f32 x, y, z;
    f32 c;
    s32 d;
} Src5C08;



void MapWallBuildCylinders(Src5C08 *s, s32 n) {
    s32 i;
    MapWall *d;

    for (i = 0; i < n; i++) {
        d = MapWallAllocCylinder();
        d->radius = s[i].c;
        d->bottom = s[i].y;
        d->cx = s[i].x;
        d->unk14 = 0.0f;
        d->count = 0;
        d->attr.i = s[i].d;
        d->top = d->bottom + d->attr.h;
        d->cz = s[i].z;
        d->unk1C = 0.0f;
        MapWallAdd(d);
    }
}
