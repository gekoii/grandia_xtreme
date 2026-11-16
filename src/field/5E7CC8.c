#include "common.h"
#include "field/n17.h"

MapModel *MapModelGet(u16 arg0) {
    if (arg0 >= mapModelCount) {
        return 0;
    }
    return &mapModelTable[arg0];
}
