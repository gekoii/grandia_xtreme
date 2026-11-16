#include "common.h"
#include "protos/field.h"
#include "field/n12.h"
#include "field/n20.h"


void MapMiniModelDraw(void) {
    GphDrawListSet(fldDrawListMiniMapBuf);
    GphDrawListClear(NULL);
    if (MapMiniIsHidden(1) == 0) {
        GphModelCalcAndDraw((void *)mapMiniWork->model);
        GphDrawListSort();
        GphDrawListEnvMapCalcUv();
    }
}
