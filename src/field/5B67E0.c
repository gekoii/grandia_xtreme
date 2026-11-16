#include "common.h"
#include "field/event.h"

void OccForwardStartDefault(s16 *arg0) {
    ((OccEntry *)arg0)->timer = 1;
}
