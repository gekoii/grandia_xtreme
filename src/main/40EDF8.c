#include "common.h"
#include "main/n01.h"

s32 ChrPortraitGetTex(s32 index) {
    return (s32)chrPortraitSlots.slots[index]->texPacket;
}
