#include "common.h"
#include "party.h"
#include "protos/field.h"

typedef struct {
    s16 id;
    s16 count;
} Load;

extern Load itemStartItems[];
extern s16 itemStartItemList[];
extern s16 itemStartBooks[];
extern s8 itemStartSkills[];
extern s16 itemStartEggs[];
extern u32 itemStartGold;

void ItemStartSetup(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    PtyCharDef *s;
    s16 r;
    u8 c2;

    for (i = 0; i < 8; i++) {
        s = &ptyCharDefTable[i];
        k = 0;
        /* the three Item*Add calls keep an s32 cast: through their real s16 return the code differs */
        for (j = 0; j < 6; j++) {
            if (s->initSlots[j] != 0) {
                if (j < s->eggSlots) {
                    r = ((s32 (*)(s16))ItemEggAdd)(s->initSlots[j]);
                    ItemCharSetEgg(i, j, r);
                } else {
                    r = ((s32 (*)(s16))ItemBookAdd)(s->initSlots[j]);
                    ItemCharSetBook(i, j, r);
                    for (m = 0; m < 3; m++) {
                        c2 = s->initSkills[k * 3 + m];
                        if (c2 != 0) {
                            r = ((s32 (*)(s16))ItemSkillAddLv1)(c2);
                            ItemCharSetSkill(i, j, m, r);
                        }
                    }
                    k++;
                }
            }
        }
        for (j = 0; j < 6; j++) {
            ptyCharData[i].equip[j] = s->initEquip[j];
        }
    }
    for (i = 0; itemStartItems[i].id != -1; i++) {
        for (j = 0; j < itemStartItems[i].count; j++) {
            ItemAdd(itemStartItems[i].id);
        }
    }
    for (i = 0; itemStartItemList[i] != -1; i++) {
        ItemAdd(itemStartItemList[i]);
    }
    for (i = 0; itemStartBooks[i] != -1; i++) {
        ItemBookAdd(itemStartBooks[i]);
    }
    for (i = 0; itemStartSkills[i] != -1; i++) {
        ItemSkillAddLv1(itemStartSkills[i]);
    }
    for (i = 0; itemStartEggs[i] != -1; i++) {
        ItemEggAdd(itemStartEggs[i]);
    }
    PtySetGold(itemStartGold);
}
