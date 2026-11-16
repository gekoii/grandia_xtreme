#include "common.h"
#include "protos/field.h"
#include "field/n14.h"

typedef struct {
    s16 unk0;
    s16 start;
    s16 count;
} Req;

s32 MapPathBuild(Req *arg0) {
    s32 i;
    s32 r, r2, ret;
    MapPathNode *p;

    i = 0;
    mapPathCtrlCount = arg0->count;
    if (arg0->count > 0) {
        do {
            p = &fldPathNodeData[arg0->start + i];
            mapPathWork->x[i] = p->x;
            mapPathWork->y[i] = p->y;
            mapPathWork->z[i] = p->z;
            mapPathWork->e[i] = p->event;
            mapPathWork->c[i] = p->type;
            i++;
        } while (i < mapPathCtrlCount);
    }
    MapPathTessellate();
    mapPathCtrlCount = 0;
    if ((u32)(mapPathPointCount - 1) >= 0x220) return -1;
    if (mapPathPointCount < 0x41) {
        r = MapPathStoreSmall();
        if (r < 0) {
            r = MapPathStoreBig();
            if (r < 0) return -1;
            r |= 0x10000;
        }
    } else {
        r = MapPathStoreBig();
        if (r < 0) return -1;
        r |= 0x10000;
    }
    return r;
}
