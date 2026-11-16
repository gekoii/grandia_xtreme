#include "common.h"
#include "field/n25.h"

void WmapNodesMarkLinked(void) {
    s32 i;
    s32 k;
    s16 *l;

    for (i = 0; i < 6; i++) {
        if (wmapWork->nodes[i].state != 0) {
            wmapWork->nodes[i].state = 2;
        }
    }
    { WmapNode *n = wmapWork->nodes; n = &n[wmapWork->cur]; l = n->links; }
    for (i = 0; i < 8; i++) {
        k = l[i];
        if (k != 0xFF) {
            if (wmapWork->nodes[k].state != 0) {
                wmapWork->nodes[k].state = 1;
            }
        }
    }
}
