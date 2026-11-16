#include "common.h"
#include "protos/field.h"
#include "field/n21.h"


void AmapNodesLinkNearest(s32 arg0, s32 mode) {
    s32 i;
    s32 j;
    s32 best;
    s32 bj;
    s32 d;
    s32 g;
    s32 x;
    s32 y;
    s32 tx;
    s32 ty;

    for (i = 0; i < amapNodeCount; i++) {
        amapNodes[i].group = 0xFF;
        amapNodes[i].unk3 = 0xFF;
    }
    g = 0;
    bj = 0;
    for (i = 0; i < amapNodeCount; i++) {
        if (amapNodes[i].group == 0xFF) {
            amapNodes[i].group = g;
            best = 0xFF;
            x = amapNodes[i].x;
            y = amapNodes[i].y;
            for (j = 0; j < amapNodeCount; j++) {
                if (i != j) {
                    d = AmapGridDist(amapNodes[j].x, amapNodes[j].y, x, y);
                    if (d < best) {
                        best = d;
                        bj = j;
                    }
                }
            }
            if (amapNodes[bj].group == 0xFF) {
                amapNodes[bj].group = g;
                g++;
            } else {
                amapNodes[i].group = amapNodes[bj].group;
            }
            tx = amapNodes[bj].x;
            ty = amapNodes[bj].y;
            AmapLayerClear(3);
            AmapDistFillOpen(x, y, arg0);
            if (mode == 4) {
                AmapCorridorTraceWide(tx, ty, arg0);
            } else {
                AmapCorridorTrace8(tx, ty, arg0);
            }
        }
    }
}
