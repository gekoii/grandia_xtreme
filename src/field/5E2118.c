/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"


ChrObj *ChrTenantFindTalkTarget(u32 *arg0) {
    f32 best;
    u32 i;
    ChrObj *o;
    ChrObj *found;
    u16 f;

    ChrGetByNo(1);
    best = 1000.0f;
    found = NULL;
    for (i = 0; i < chrTenantList.count; i++) {
        o = &chrTenantList.objs[i];
        f = o->flags;
        if (f & 2) {
            if (f & 0x1000) {
                if (!(f & 0x10) && o->id != 0) {
                    if (o->leaderDist < (o->radius + 50.0f) * 2.0f && o->leaderDist < best) {
                        *arg0 = i;
                        best = o->leaderDist;
                        found = o;
                    }
                }
            }
        }
    }
    return found;
}
