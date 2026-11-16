#include "common.h"
#include "gph.h"

void GphTexTableSelect(GphTexTable *arg0) {
    gphTexTable = arg0;
}

GphTexTable *GphTexTableGetCurrent(void) {
    return gphTexTable;
}
