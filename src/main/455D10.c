/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "vu.h"

typedef struct {
    u8 pad[0x80];
    FVECTOR v;
    f32 sx, sy, sz;
} Obj;

typedef struct {
    FVECTOR v;
    u8 pad[0x40];
} Local;

void GphSphereClipTest(s32, Local *);

void GphObjSphereClipTest(s32 arg0, Obj *arg1) {
    Local l;
    f32 m;

    m = arg1->sx;
    if (m < arg1->sy) {
        m = arg1->sy;
    }
    if (m < arg1->sz) {
        m = arg1->sz;
    }
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(l.v), "r"(arg1->v) : "$2", "memory");
    l.v[3] = m;
    GphSphereClipTest(arg0, &l);
}
