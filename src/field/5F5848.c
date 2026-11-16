#include "common.h"
#include "protos/field.h"
#include "m2c_macros.h"


s32 ItemCharEquipBook(s16 arg0, s16 arg1, s16 arg2) {
    ItemCharUnequipBook((s16) (arg0 + 0x3E8), arg1);
    ItemCharSetBookRefresh(arg0, arg1, arg2);
    return 0;
}

s32 ItemCharEquipSkill(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    ItemCharUnequipSkill((s16) (arg0 + 0x3E8), arg1, arg2);
    ItemCharSetSkill(arg0, arg1, arg2, arg3);
    return 0;
}

s32 ItemCharEquipEgg(s16 arg0, s16 arg1, s16 arg2) {
    ItemCharUnequipEgg((s16) (arg0 + 0x3E8), arg1);
    ItemCharSetEggRefresh(arg0, arg1, arg2);
    return 0;
}
