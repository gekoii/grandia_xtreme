#include "common.h"
#include "ptcl.h"

s8 *PtclPtbGetString(PtclPtbHeader *obj, s32 idx) {
    s8 *p = (s8 *)obj + obj->stringOffset;
    s32 i;
    if (idx >= obj->stringCount) {
        return NULL;
    }
    for (i = 0; i < idx; i++) {
        while (*p != 0) {
            p++;
        }
        p++;
    }
    return p;
}
