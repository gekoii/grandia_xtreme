#include "common.h"
#include "gph.h"

s32 ChrModelSetPartPrio(GphModel *obj, s32 v) {
    GphPart *e = obj->parts;
    GphPart *end = e + obj->partCount;
    do {
        e->prio = v;
        e++;
    } while (e < end);
    return 0;
}
