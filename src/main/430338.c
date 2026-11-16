#include "common.h"
#include "gph.h"

/* Turns the texture header's 15 offsets into pointers, as
 * GphTexDataRelocate 43F360 does. */
void GsHeaderRelocate(GphTex *arg0) {
    if (arg0->data[0] != 0) {
        arg0->data[0] += (s32)arg0;
    }
    if (arg0->data[1] != 0) {
        arg0->data[1] += (s32)arg0;
    }
    if (arg0->data[2] != 0) {
        arg0->data[2] += (s32)arg0;
    }
    if (arg0->data[3] != 0) {
        arg0->data[3] += (s32)arg0;
    }
    if (arg0->data[4] != 0) {
        arg0->data[4] += (s32)arg0;
    }
    if (arg0->data[5] != 0) {
        arg0->data[5] += (s32)arg0;
    }
    if (arg0->data[6] != 0) {
        arg0->data[6] += (s32)arg0;
    }
    if (arg0->unk40[0] != 0) {
        arg0->unk40[0] += (s32)arg0;
    }
    if (arg0->unk40[1] != 0) {
        arg0->unk40[1] += (s32)arg0;
    }
    if (arg0->unk40[2] != 0) {
        arg0->unk40[2] += (s32)arg0;
    }
    if (arg0->unk40[3] != 0) {
        arg0->unk40[3] += (s32)arg0;
    }
    if (arg0->unk40[4] != 0) {
        arg0->unk40[4] += (s32)arg0;
    }
    if (arg0->unk40[5] != 0) {
        arg0->unk40[5] += (s32)arg0;
    }
    if (arg0->unk40[6] != 0) {
        arg0->unk40[6] += (s32)arg0;
    }
    if (arg0->unk40[7] != 0) {
        arg0->unk40[7] += (s32)arg0;
    }
}
