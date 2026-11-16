#include "common.h"
#include "main/n07.h"

void GphWipeStop(void) {
    gphWipe.active = 0;
    gphWipe.type = 0;
    gphWipe.dir = 0;
    gphWipe.mode = 0;
    gphWipe.frame = 0;
    gphWipe.state = 0;
}
