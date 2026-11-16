#include "common.h"
#include "protos/main.h"
#include "snd.h"

/* Defined in 4727B0.c (sceMpeg *, s32 ni, s32 np, s32 nb) with a local type. */
void sceMpegSetDecodeMode();

void videoDecSetDecodeMode(VideoDec *vd, s32 ni, s32 np, s32 nb) {
    sceMpegSetDecodeMode(&vd->mpeg, ni, np, nb);
}

s32 videoDecSetStream(VideoDec *vd, u32 strType, u64 ch, void *cb, void *data) {
    sceMpegAddStrCallback(&vd->mpeg, strType, ch, cb, data);
    return 1;
}
