#include "common.h"
#include "party.h"



typedef struct {
    s16 a;
    s16 b;
} Pair;



s32 ItemEggCanFuse(s16 x, s16 y) {
    Pair p;
    s32 k;

    p.a = ptyPartyData->eggs[x].itemId;
    p.b = ptyPartyData->eggs[y].itemId;
    k = (p.b - 0x12D) * 0x2C - 0x12D;
    return ptyEggFusionTable->result[p.a + k] != 0 ? 0 : -1;
}
