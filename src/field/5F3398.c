#include "common.h"
#include "protos/field.h"

typedef struct {
    s16 unk0;
    s16 pad2;
    s16 unk4;
    s16 pad6;
    s16 unk8;
    s16 padA;
    u32 unkC;
} Cond;


s32 ItemShopCondCheck(Cond *c) {
    if (ItemIsSold(c->unk0) == 0) {
        return 0;
    }
    if (ItemIsSold(c->unk4) == 0) {
        return 0;
    }
    if (ItemIsSold(c->unk8) == 0) {
        return 0;
    }
    return ItemGetScenarioProgress() >= c->unkC;
}
