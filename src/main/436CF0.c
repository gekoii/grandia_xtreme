#include "common.h"
#include "gph.h"

void ChrPartTreeUvScroll(GphPart *n) {
    for (;;) {
        if (n->attr & 0x20) {
            n->uvScroll[0] = (n->uvScroll[0] + n->uvScroll[1]) & 0xFFF;
            n->uvScroll[2] = (n->uvScroll[2] + n->uvScroll[3]) & 0xFFF;
        }
        /* do-while(0): the alignment nop before the child test */
        do {
            if (n->child != 0) {
                ChrPartTreeUvScroll(&n[n->child]);
            }
        } while (0);
        if (n->next == 0) {
            break;
        }
        n = &n[n->next];
    }
}
