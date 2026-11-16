/* as: ee-gcc2.9-991111 */
#include "common.h"
#include "field/n14.h"
#include "protos/field.h"

typedef struct {
    f32 *x;
    f32 *y;
    f32 *z;
    f32 *e;
} SegIn;

typedef struct {
    f32 *x;
    f32 *y;
    f32 *z;
} SegOut;

extern s32 MapPathLinearResample(SegIn *, SegOut *, s32);

void MapPathTessellate(void) {
    SegIn si;
    SegOut so;
    MapPathWork *w;
    s32 i;
    s32 out;
    s32 n;
    s32 type;
    s32 j;

    i = 0;
    out = 0;
    while (i < mapPathCtrlCount) {
        if (i == mapPathCtrlCount - 1) {
            mapPathWork->px[out] = mapPathWork->x[i];
            mapPathWork->py[out] = mapPathWork->y[i];
            mapPathWork->pz[out] = mapPathWork->z[i];
            out++;
            break;
        }
        w = mapPathWork;
        si.x = &w->x[i];
        si.y = &w->y[i];
        si.z = &w->z[i];
        si.e = &w->e[i];
        so.x = &w->px[out];
        so.y = &w->py[out];
        so.z = &w->pz[out];
        i++;
        type = (s32)w->c[i];
        i++;
        n = 2;
        while (i < mapPathCtrlCount && type == (s32)mapPathWork->c[i]) {
            type = (s32)mapPathWork->c[i];
            i++;
            n++;
        }
        if (type == 0) {
            for (j = 0; j < n; j++) {
                so.x[j] = si.x[j];
                so.y[j] = si.y[j];
                so.z[j] = si.z[j];
            }
            out += n;
        } else {
            if (type == 1 || n < 3) {
                out += MapPathLinearResample(&si, &so, n) / 3;
            } else {
                out += MapPathSplineResample(&si, &so, n) / 3;
            }
        }
        if (i >= mapPathCtrlCount) break;
        i--;
        out--;
    }
    mapPathPointCount = out;
}
