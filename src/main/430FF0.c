#include "common.h"
#include "main/n03.h"

void GsPacketBufRewind(void *arg0) {
    GsPacketBuf *p = arg0;

    p->cur = p->start;
}
