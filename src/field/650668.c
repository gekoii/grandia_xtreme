#include "common.h"
#include "field/n25.h"

void MiniBallPlayBegin(void) {
    MiniBallWork *p = miniBallWork;
    p->subState = 2;
    p->frame = 0;
}
