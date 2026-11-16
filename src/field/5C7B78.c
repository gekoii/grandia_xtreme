#include "common.h"
#include "field/n14.h"

s32 ChrMotTableTake(ChrMotTable *p, s32 idx) {
    s32 v;

    if (idx >= 64 || p->count == 0) {
        return 0;
    }
    p->count--;
    v = p->slots[idx];
    p->slots[idx] = -1;
    return v;
}
