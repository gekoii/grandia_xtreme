#include "common.h"
#include "protos/field.h"

s32 ItemCharSlotListBuild(s16);

s32 ItemCharSetBookRefresh(s16 arg0, s16 arg1, s16 arg2) {
    ItemCharSetBook(arg0, arg1, arg2);
    ItemCharSlotListBuild(arg0);
    return 0;
}
