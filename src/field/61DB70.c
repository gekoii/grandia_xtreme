#include "common.h"
#include "protos/field.h"

typedef f32 FVECTOR[4] __attribute__((aligned(16)));

typedef struct {
    u8 pad[0x50];
} Elem;

typedef struct {
    s32 count;
    Elem *elems;
} List;

typedef struct {
    FVECTOR unk0;
    FVECTOR pos;
    FVECTOR dir;
    u8 pad30[0x50];
} Ray;

s32 FldCollSphereTriTest(Elem *, Ray *);
void VuVecCopy(f32 *, f32 *);

s32 FldCollTriListHitAt(f32 *arg0, f32 arg1) {
    Ray r;
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
    VuVecCopy(r.pos, arg0);
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(r.dir) : "memory");
    r.dir[1] = -arg1;
    elems = l->elems;
    for (i = 0; i < n; i++) {
        e = &elems[i];
        if (FldCollSphereTriTest(e, &r) == 1) {
            return 1;
        }
    }
    return 0;
}
