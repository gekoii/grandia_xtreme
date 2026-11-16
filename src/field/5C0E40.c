#include "common.h"
#include "gph.h"
#include "field/state.h"
#include "field/n14.h"

void ChrTenantSetArea(ChrObj *a, s32 idx) {
    ChrTenantArea *m;
    GphModel *p;

    if (idx == 0xFF) {
        a->flags &= 0xEFFF;
        return;
    }
    if ((u32)idx >= 8) {
        a->flags &= 0xEFFF;
        return;
    }
    a->area = idx;
    p = a->model;
    m = a->areas[idx];
    p->pos[0] = m->centre.x;
    a->flags |= 0x1000;
    p->pos[1] = m->centre.y;
    p->pos[2] = m->centre.z;
}
