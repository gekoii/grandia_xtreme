#include "common.h"
#include "protos/field.h"
#include "field/n21.h"


void AmapGenLayoutOpenDense(AmapWork *arg0) {
    arg0->nodeCount = (arg0->width * arg0->height) / 20;
    AmapBoundsSet(2, 2, arg0->width - 2, arg0->height - 2);
    AmapStartPlace(arg0);
    AmapExitPlace(arg0);
    AmapNodesScatter(arg0->nodeCount);
    AmapBoundsSet(-1, -1, -1, -1);
    AmapLayerClear(3);
    AmapNodesLinkNearest(arg0, 4);
    AmapNodesLinkGroups(arg0, 4);
    AmapNodesMark();
}
