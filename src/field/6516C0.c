#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniBallShufflePrizes(void) {
    MiniBallGame *s = &miniBallWork->game;
    u8 used[6];
    u32 i;
    u32 c;
    u32 k;
    s32 j;
    s32 n;

    n = 6;
    for (i = 0; i < 6; i++) {
        used[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        j = 0;
        k = VuRandInt(n) + 1;
        for (c = 0; c < k; c++) {
            while (used[j] == 1) j++;
            j++;
        }
        j--;
        s->prizes[i].reel = j;
        used[j] = 1;
        n--;
    }
}
