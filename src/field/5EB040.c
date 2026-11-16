#include "common.h"
#include "field/menu.h"
#include "protos/field.h"

typedef struct {
    u8 n[5];
} Counts;

extern Counts itemFlagGroupSizes;

s16 ItemFlagGroupListBuild(s32 arg0) {
    Counts c;
    s32 i;
    s32 j;
    ItemRow *r;
    ItemRow *base;
    u8 *pa;
    u8 *pb;
    s16 m;
    u8 *ta;
    u8 *tb;

    base = itemWork->rows;
    c = itemFlagGroupSizes;
    for (i = 0; i < 5; i++) {
        r = &base[i];
        r->count = c.n[i];
        r->flag = 0;
        if (EvtGlobalFlagGet(i + 0x4D0) != 0) {
            r->flag = 1;
        }
        for (j = 0; j < base[i].count; j++) {
            ta = base[i].a;
            tb = base[i].b;
            pa = ta + j;
            pb = tb + j;
            *pa = 0;
            *pb = 0;
            if (EvtGlobalFlagGet(i * 8 + 0x440 + j) != 0) {
                *pa = 1;
            }
            if (EvtGlobalFlagGet(i * 8 + 0x480 + j) != 0) {
                *pb = 1;
            }
        }
    }
    m = 4;
    if (ItemGetScenarioProgress() >= 15570) {
        m = 5;
    }
    itemWork->counts[5] = m;
    return m;
}
