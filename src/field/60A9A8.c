#include "common.h"
#include "protos/field.h"
#include "field/n21.h"


void AmapNodesLinkGroups(s32 arg0, s32 mode) {
    s32 g;
    s32 i;
    s32 j;
    s32 k;
    s32 bj;
    s32 best;
    s32 d;
    s32 og;
    s32 x;
    s32 y;
    s32 tx;
    s32 ty;

    g = 0;
    bj = 0;
    do { } while (0); /* empty do-while(0): scheduling barrier before the loop (15 words differ without it) */
    goto find; /* goto loop: the search loop is entered at find */
    for (;;) {
        for (j = 0; j < amapNodeCount; j++) {
            if (amapNodes[j].group == og) amapNodes[j].group = g;
        }
        g++;
find:
        k = 0xFF;
        if (g >= amapNodeCount) g = 0;
        for (i = 0; i < amapNodeCount; i++) {
            if (amapNodes[i].group >= g) {
                g = amapNodes[i].group;
                k = i;
                break;
            }
        }
        if (k == 0xFF) {
            g++;
            goto find;
        }
        best = 0xFF;
        x = amapNodes[k].x;
        y = amapNodes[k].y;
        for (j = 0; j < amapNodeCount; j++) {
            if (g != amapNodes[j].group) {
                d = AmapGridDist(amapNodes[j].x, amapNodes[j].y, x, y);
                if (d < best) {
                    bj = j;
                    best = d;
                }
            }
        }
        if (best == 0xFF) return;
        AmapLayerClear(3);
        AmapDistFillOpen(x, y, arg0);
        og = amapNodes[bj].group;
        tx = amapNodes[bj].x;
        ty = amapNodes[bj].y;
        if (mode == 4) {
            j = AmapCorridorTraceWide(tx, ty, arg0);
        } else {
            j = AmapCorridorTrace8(tx, ty, arg0);
        }
        if (j == 0) {
            best = 0xFF;
            for (j = 0; j < amapNodeCount; j++) {
                if (g == amapNodes[j].group) {
                    d = AmapGridDist(amapNodes[j].x, amapNodes[j].y, tx, ty);
                    if (d < best) {
                        best = d;
                        k = j;
                    }
                }
            }
            x = amapNodes[k].x;
            y = amapNodes[k].y;
            AmapLayerClear(3);
            AmapDistFillOpen(x, y, arg0);
            if (mode == 4) {
                j = AmapCorridorTraceWide(tx, ty, arg0);
            } else {
                j = AmapCorridorTrace8(tx, ty, arg0);
            }
            if (j == 0) return;
        }
    }
}
