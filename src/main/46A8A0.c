#include "common.h"
#include "main/n11.h"

void sceGsPutDispEnv(MovDisp *);

void MovDispSetBuffer(MovDisp *obj, s32 sel) {
    s32 v;
    if (sel != 0) {
        v = obj->fbp1;
    } else {
        v = obj->fbp0;
    }
    obj->fbp = v;
    sceGsPutDispEnv(obj);
}
