#include "common.h"
#include "protos/field.h"
#include "field/state.h"


s32 ChrInstAlloc(s32 arg0) {
    u16 g;
    s32 i;
    s32 r;
    struct GphModel *o;

    g = SysCommonModelGroup(arg0);
    for (i = 0; i < 32; i++) {
        if (chrInstPool->slots[g][i].used == 0) {
            if (chrInstPool->slots[g][i].model == 0) {
                r = SysCommonModelGet(arg0);
                if (r == 0) {
                    return -1;
                }
                o = ChrModelCreate(r);
                chrInstPool->slots[g][i].model = o;
                if (o == 0) {
                    return -1;
                }
            }
            chrInstPool->slots[g][i].used = 1;
            chrInstPool->slots[g][i].visible = 1;
            chrInstPool->slots[g][i].blink = 0;
            chrInstPool->slots[g][i].spin = 0;
            chrInstPool->count++;
            return i;
        }
    }
    return -1;
}
