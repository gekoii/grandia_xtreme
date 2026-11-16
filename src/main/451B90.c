#include "common.h"
#include "main/n07.h"

void GphWipeStart(s8 arg0, s8 arg1, s8 arg2) {
    gphWipe.active = 1;
    gphWipe.type = arg0;
    gphWipe.dir = arg1;
    gphWipe.mode = arg2;
    gphWipe.frame = 0;
    gphWipe.state = 0;
}
