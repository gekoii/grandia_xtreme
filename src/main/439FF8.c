#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "main/n05.h"

void GphPartDrawSimple(GphPart *obj, s32 arg1) {
    GphGaObject *e;
    s32 n;
    s32 a;
    s32 b;
    s32 v;

    n = obj->gaObjCount;
    if (n == 0) {
        return;
    }
    if (obj->drawFlags & 1) {
        e = obj->gaObjsCopy;
    } else {
        e = obj->gaObjs;
    }
    a = 0;
    b = 0;
    do {
        v = 7;
        if (*(s32 *)0x70002804 != 0) {
            v = 0;
        }
        e->clip = v;
        b++;
        GphDrawListAddGaObj((s32)obj, (u8 *)e, arg1);
        a++;
        n--;
        e++;
    } while (n > 0);
    gphRenderCtx->gaObjDrawn += a;
    gphRenderCtx->gaObjCount += b;
}
