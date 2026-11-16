/* cflags: -ffast-math */
/* -ffast-math: sqrtf is a bare sqrt.s (no NaN check and sqrtf fallback call) */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n13.h"
#include "protos/field.h"

typedef struct {
    u16 first;
    u16 last;
    s32 unk4;
} Ent5C03;



void MapWallBuildChains(Ent5C03 *ents, VuVec3 *vtx, s32 cnt) {
    s32 i, j, n;
    u16 first;
    Ent5C03 *e;
    MapWall *d;
    f32 maxX, minX, maxY, minY, maxZ, minZ;
    f32 x2, y2, z2, x, y, z, cx, cz, dx, dz, dist, maxd;

    for (i = 0; i < cnt; i++) {
        e = (Ent5C03 *)((i << 3) + (u32)ents);
        maxX = -9999999.0f;
        minX = 9999999.0f;
        first = e->first;
        maxY = maxX;
        minY = minX;
        maxZ = maxY;
        minZ = minY;
        n = e->last - first + 1;
        d = MapWallAllocChain(n);
        d->attr.i = e->unk4;
        if (d == NULL) {
            return;
        }
        d->count = n;
        for (j = 0; j < n; j++) {
            x = vtx[first + j].x;
            z = vtx[first + j].z;
            ((MapWall *)((u32)d + (j << 4)))->points[0][0] = x;
            y = vtx[first + j].y;
            ((MapWall *)((u32)d + (j << 4)))->points[0][1] = y;
            ((MapWall *)((u32)d + (j << 4)))->points[0][2] = vtx[first + j].z;
            ((MapWall *)((u32)d + (j << 4)))->points[0][3] = 0.0f;
            x2 = x;
            y2 = y;
            z2 = z;
            maxY = (y < maxY) ? maxY : y2;
            minY = (minY < y) ? minY : y2;
            maxX = (x > maxX) ? x2 : maxX;
            maxZ = (z > maxZ) ? z2 : maxZ;
            minX = (x < minX) ? x2 : minX;
            minZ = (z < minZ) ? z2 : minZ;
        }
        cx = (maxX + minX) * 0.5f;
        cz = (maxZ + minZ) * 0.5f;
        maxd = 0.0f;
        for (j = 0; j < n; j++) {
            dx = vtx[first + j].x;
            dz = vtx[first + j].z;
            x = dx + cx;
            z = dz + cz;
            dist = x * x + z * z;
            if (maxd < dist) {
                maxd = dist;
            }
        }
        d->cx = cx;
        d->cz = cz;
        d->unk14 = 0.0f;
        d->unk1C = 0.0f;
        d->top = (s16)maxY + d->attr.h;
        d->bottom = (s32)minY;
        do {
        } while (0); /* scheduling barrier: nothing moves across it to the sqrtf (64 words off without) */
        d->radius = sqrtf(maxd);
        MapWallAdd(d);
    }
}
