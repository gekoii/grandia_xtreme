#include "common.h"
#include "protos/field.h"
#include "field/state.h"


void ChrTenantUpdateMotions(void) {
    u32 i;
    ChrObj *o;
    u16 f;

    if (chrTenantList.count != 0) {
        for (i = 0; i < chrTenantList.count; i++) {
            o = &chrTenantList.objs[i];
            if (o != NULL) {
                f = o->flags;
                if (f & 0x1000) {
                    if ((f & 2) && o->model != 0) {
                        ChrMotBlendUpdate(o);
                        ChrMotUpdate(chrTenantList.objs[i].model, chrTenantList.objs[i].motSpeed);
                        ChrFaceAnimUpdate(&o->faceAnim, o->model);
                    }
                }
            }
        }
    }
}
