#include "common.h"
#include "main/n11.h"

s32 audioDecCreate(MovAudioDec *p, u8 *buf, s32 size, s32 iopBuffSize) {
    p->data = buf;
    p->size = size;
    p->iopBuffSize = iopBuffSize;
    p->state = 0;
    p->hdrCount = 0;
    p->put = 0;
    p->count = 0;
    p->totalBytes = 0;
    p->totalBytesSent = 0;
    p->iopLastPos = 0;
    return 1;
}
