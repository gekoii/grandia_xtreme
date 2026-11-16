/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n14.h"


s32 ChrTenantCheckLeaveArea(ChrObj *a, ChrTenantTrack *t) {
    ChrTenantRect r;
    ChrTenantArea *p;
    GphModel *o;
    f32 x;
    f32 z;
    s8 st;

    if (a->aiMode == 0) {
        return 0;
    }
    if (a->vel[0] == 0.0f && a->vel[2] == 0.0f) {
        return 0;
    }
    p = a->areas[a->area];
    if (p->x0 <= p->x1) {
        r.x0 = p->x0;
        r.x1 = p->x1;
    } else {
        r.x0 = p->x1;
        r.x1 = p->x0;
    }
    p = a->areas[a->area];
    if (p->z0 <= p->z1) {
        r.z0 = p->z0;
        r.z1 = p->z1;
    } else {
        r.z0 = p->z1;
        r.z1 = p->z0;
    }
    o = a->model;
    x = o->pos[0] + a->vel[0];
    if (r.x0 <= x - 10.0f && x + 10.0f <= r.x1) {
        z = o->pos[2] + a->vel[2];
        if (r.z0 <= z - 10.0f && z + 10.0f <= r.z1) {
            return 0;
        }
    }
    st = a->opState;
    if (st != 4 && st != 8 && st != 0x14 && st != 0x15 && st != 0xA) {
        ChrTenantAiSetMode(a, 3);
        if (a->sense != NULL) {
            CHR_SENSE(a)->seTimer = CHR_SENSE(a)->charDef->seInterval + VuRandInt(CHR_SENSE(a)->charDef->seIntervalRand);
        }
        a->vel[0] = a->vel[1] = a->vel[2] = 0.0f;
        a->moveDir = a->headingDeg * 182.04445f;
        return 1;
    }
    return 0;
}
