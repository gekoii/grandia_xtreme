#include "common.h"
#include "field/n20.h"

void MapMiniResetDelay(void) {
    if (mapMiniWork != NULL) {
        mapMiniWork->showDelay = 1;
    }
}
