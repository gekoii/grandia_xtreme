#include "common.h"
#include "protos/field.h"

typedef struct {
    u8 pad[0x50];
} Elem;

typedef struct {
    s32 count;
    Elem *elems;
} List;

s32 FldCollSphereTriTest(Elem *, s32);

s32 FldCollTriListHitTest(s32 arg0) {
    List *l;
    s32 n;
    s32 i;
    Elem *e;
    Elem *elems;

    l = MapPolyGetList();
    n = l->count;
    if (n == 0) {
        return 0;
    }
    elems = l->elems;
    for (i = 0; i < n; i++) {
        e = &elems[i];
        if (FldCollSphereTriTest(e, arg0) == 1) {
            return 1;
        }
    }
    return 0;
}
