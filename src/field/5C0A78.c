#include "common.h"
#include "protos/field.h"
#include "field/n13.h"


MapEntrance *MapEntranceGetCurrent(void) {
    /* s32 cast: the id is passed unmasked (the definition takes u16) */
    return ((MapEntrance *(*)(s32))MapEntranceFindById)(mapEntranceCur);
}
