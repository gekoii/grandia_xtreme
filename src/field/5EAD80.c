#include "common.h"
#include "protos/field.h"

extern s16 itemMaxItems;
extern s16 itemEggCapacity;
extern s16 itemBookCapacity;
extern s16 itemSkillCapacity;

void ItemUpdateLimits(void) {
    if (ItemGetScenarioProgress() < 11000) {
        itemMaxItems = 30;
        itemEggCapacity = 12;
        itemBookCapacity = 12;
        itemSkillCapacity = 50;
    } else {
        itemMaxItems = 30;
        itemEggCapacity = 24;
        itemBookCapacity = 24;
        itemSkillCapacity = 100;
    }
}
