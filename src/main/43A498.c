#include "common.h"
#include "gph.h"
#include "main/n04.h"

void GphDrawListAddGaObj(s32 arg0, u8 *arg1, s32 arg2) {
    u32 i = gphDrawListCur->count;
    GphDrawItem *e;

    if (i < gphDrawListCur->capacity) {
        e = &gphDrawListCur->items[i];
        e->texId = 0;
        e->gaObj = (GphGaObject *)arg1;
        e->mtx = arg2;
        *(s16 *)&e->layer = arg1[2]; /* layer = gaObj->clip, unk3 = 0 */
        {
            s32 off = i * sizeof(u64);
            *(u64 *)((u8 *)gphDrawListCur->keys + off) = (u32)e;
        }
        gphDrawListCur->count = i + 1;
    }
}
