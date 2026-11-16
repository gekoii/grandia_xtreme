#include "common.h"
#include "protos/field.h"
#include "field/n14.h"

/* Map data form of a wander area: +2 is the point count - 1, +3 the kind. */
typedef struct {
    u16 id;
    u8 unk2;
    u8 unk3;
    f32 x0, z0, x1, z1;
    VuVec3 c;
    VuVec3 pts[6];
} Ent;


void ChrTenantAreaLoad(Ent *src, s32 n) {
    Ent *s;
    ChrTenantArea *e;
    s32 i;
    s32 j;

    ChrTenantAreaSetCount(n);
    for (i = 0; i < n; i++) {
        s = &src[i];
        e = ChrTenantAreaGet(i);
        if ((s8)s->unk2 == 0) {
            e->pointCount = 0;
        } else {
            e->pointCount = s->unk2 + 1;
        }
        e->kind = s->unk3;
        e->id = s->id;
        e->x0 = s->x0;
        e->z0 = s->z0;
        e->x1 = s->x1;
        e->z1 = s->z1;
        e->centre.x = s->c.x;
        e->centre.y = s->c.y;
        e->centre.z = s->c.z;
        for (j = 0; j < 6; j++) {
            e->points[j].x = s->pts[j].x;
            e->points[j].y = s->pts[j].y;
            e->points[j].z = s->pts[j].z;
        }
    }
}
