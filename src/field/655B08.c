#include "common.h"
#include "save.h"
#include "field/n25.h"

void WmapUnlockByProgress(void) {
    u32 x;

    x = evtMainFlag;
    wmapWork->nodes[0].unlocked = 1;
    wmapWork->nodes[1].unlocked = 0;
    wmapWork->nodes[2].unlocked = 0;
    wmapWork->nodes[3].unlocked = 0;
    wmapWork->nodes[4].unlocked = 0;
    wmapWork->nodes[5].unlocked = 0;
    if (x >= 10100 && x < 11000) {
        wmapWork->nodes[0].unlocked = 1;
        wmapWork->nodes[1].unlocked = 1;
        wmapWork->nodes[2].unlocked = 0;
        wmapWork->nodes[3].unlocked = 0;
        wmapWork->nodes[4].unlocked = 0;
        wmapWork->nodes[5].unlocked = 0;
        wmapWork->nodes[0].links[2] = 1;
        wmapWork->nodes[1].links[4] = 0;
        wmapWork->nodes[1].links[6] = 0;
    } else if (x >= 11000 && x < 11800) {
        wmapWork->nodes[0].unlocked = 1;
        wmapWork->nodes[1].unlocked = 1;
        wmapWork->nodes[2].unlocked = 1;
        wmapWork->nodes[3].unlocked = 0;
        wmapWork->nodes[4].unlocked = 0;
        wmapWork->nodes[5].unlocked = 0;
        wmapWork->nodes[0].links[2] = 1;
        wmapWork->nodes[1].links[4] = 0;
        wmapWork->nodes[1].links[6] = 0;
        wmapWork->nodes[2].links[2] = 0;
        wmapWork->nodes[2].links[3] = 0;
    } else if (x >= 11800 && x < 12170) {
        wmapWork->nodes[0].unlocked = 1;
        wmapWork->nodes[1].unlocked = 1;
        wmapWork->nodes[2].unlocked = 1;
        wmapWork->nodes[3].unlocked = 1;
        wmapWork->nodes[4].unlocked = 0;
        wmapWork->nodes[5].unlocked = 0;
        wmapWork->nodes[0].links[2] = 1;
        wmapWork->nodes[1].links[4] = 0;
        wmapWork->nodes[2].links[2] = 0;
        wmapWork->nodes[2].links[3] = 0;
    } else if (x >= 12170 && x < 14750) {
        wmapWork->nodes[0].unlocked = 1;
        wmapWork->nodes[1].unlocked = 1;
        wmapWork->nodes[2].unlocked = 1;
        wmapWork->nodes[3].unlocked = 1;
        wmapWork->nodes[4].unlocked = 1;
        wmapWork->nodes[5].unlocked = 0;
    } else if (x >= 14750 && x < 15356) {
        wmapWork->nodes[0].unlocked = 1;
        wmapWork->nodes[1].unlocked = 1;
        wmapWork->nodes[2].unlocked = 1;
        wmapWork->nodes[3].unlocked = 1;
        wmapWork->nodes[4].unlocked = 1;
        wmapWork->nodes[5].unlocked = 1;
        wmapWork->nodes[0].links[0] = 5;
        wmapWork->nodes[0].links[7] = 5;
        wmapWork->nodes[1].links[4] = 5;
        wmapWork->nodes[1].links[5] = 5;
        wmapWork->nodes[2].links[1] = 5;
        wmapWork->nodes[3].links[3] = 5;
    } else if (x >= 15356 && x < 15600) {
        wmapWork->nodes[0].unlocked = 1;
        wmapWork->nodes[1].unlocked = 1;
        wmapWork->nodes[2].unlocked = 1;
        wmapWork->nodes[3].unlocked = 1;
        wmapWork->nodes[4].unlocked = 1;
        wmapWork->nodes[5].unlocked = 0;
    } else if (x >= 15600) {
        wmapWork->nodes[0].unlocked = 1;
        wmapWork->nodes[1].unlocked = 1;
        wmapWork->nodes[2].unlocked = 1;
        wmapWork->nodes[3].unlocked = 1;
        wmapWork->nodes[4].unlocked = 1;
        wmapWork->nodes[5].unlocked = 0;
    }
}
