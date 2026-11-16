#include "common.h"
#include "protos/field.h"
#include "gph.h"
#include "field/n13.h"


GphModel *ChrModelCreateFromPack(s32 arg0, s32 arg1, s32 arg2) {
    GphModel *o;
    s32 r;

    if (arg0 != 0) {
        r = FilePacGetModel(arg0, arg2);
        if (r == 0) {
            return 0;
        }
        o = ChrModelCreate(r);
        o->flags = 0;
        o->objId = fldModelTable.count + 200;
        if (o != 0) {
            goto ok; /* goto: puts the success path after the return 0 block */
        }
    }
    return 0;
ok:
    ChrModelSetPartPrio(o, arg1);
    return o;
}
