#include "common.h"
#include "gph.h"

s32 GphDrawListInit(s32 arg0, GphDrawList *arg1) {
    u32 a;
    u32 b;
    u32 c;

    a = ((u32)arg1 + 0x73) & 0xFFFFFFE0;
    b = a + arg0 * 8;
    b = (b + 0x1F) & 0xFFFFFFE0;
    c = b + arg0 * 16;
    c = (c + 0xF) & 0xFFFFFFF0;
    arg1->capacity = arg0;
    arg1->keys = (u64 *)a;
    arg1->items = (GphDrawItem *)b;
    arg1->count = 0;
    arg1->envMapList = (u16 *)c;
    return 0;
}
