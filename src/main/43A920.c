#include "common.h"
#include "main/n05.h"

void GphProjTexClear(void) {
    if (gphProjTexList != NULL) {
        gphProjTexList->count = 0;
    }
}
