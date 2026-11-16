#include "common.h"
#include "field/n22.h"

void AmapRollItemsByRank(s32, s32, FldMapSet *, u16 *);

void AmapRollItems(s32 arg0, s32 arg1, FldMapSet *arg2, u16 *arg3) {
    s32 v;

    switch (arg1) {
    case 1:
        v = arg2->dropArea1;
        break;
    case 0:
    default:
        v = arg2->dropArea0;
        break;
    case 2:
        v = arg2->dropArea2;
        break;
    }
    AmapRollItemsByRank(arg0, v, arg2, arg3);
}
