#include "common.h"
#include "gph.h"

void BtlChrPartSetUvOffset(void *arg0, s16 arg1, s16 arg2) {
    ((GphPart *)arg0)->uvScroll[2] = (s16) (arg2 & 0xFFF);
    ((GphPart *)arg0)->uvScroll[0] = (s16) (-arg1 & 0xFFF);
}
