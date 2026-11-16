#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void MiniBallReelSetOrder(void) {
    MiniBallGame *s = &miniBallWork->game;
    u8 used[6];
    u32 i;
    u32 c;
    u32 k;
    s32 j;
    s32 n;

    j = 5;
    n = 5;
    for (i = 0; i < 6; i++) {
        used[i] = 0;
    }
    s->reelOrder[0] = s->color;
    used[s->color] = 1;
    MiniBallReelSetTiming(s->color, 6);
    for (i = 1; i < 6; i++) {
        /* do-while(0): loop-depth weight that sets the s registers of k, j and n */
        do {
            k = VuRandInt(j) + 1;
            j = 0;
            for (c = 0; c < k; c++) {
                while (used[j] == 1) j++;
                j++;
            }
            j--;
            n--;
        } while (0);
        s->reelOrder[i] = j;
        used[j] = 1;
        MiniBallReelSetTiming(j, 6 - i);
        j = n;
    }
}
