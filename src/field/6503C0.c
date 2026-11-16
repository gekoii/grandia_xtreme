#include "common.h"
#include "field/n25.h"

void MiniBallGoldDisplayUpdate(void) {
    MiniBallGame *s = &miniBallWork->game;
    MiniTextSlot *g = s->sprites[53].num;
    u32 cur;
    u32 v;

    if (g != NULL) {
        cur = s->gold;
        v = g->value;
        if (v != cur) {
            if (cur < v) v--;
            if (cur + 50 < v) v -= 10;
            if (cur + 500 < v) v -= 100;
            if (cur + 5000 < v) v -= 1000;
            if (cur + 50000 < v) v -= 10000;
            if (cur + 500000 < v) v -= 100000;
            if (cur + 5000000 < v) v -= 1000000;
            g->value = v;
        }
    }
}
