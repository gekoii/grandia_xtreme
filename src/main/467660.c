#include "common.h"
#include "snd.h"
#include "protos/main.h"

VoTag *voBufGetTag(VoBuf *arg0) {
    VoTag *ret;

    if (voBufIsEmpty(arg0) == 0) {
        ret = arg0->tag[(arg0->write - arg0->count + arg0->size) % arg0->size];
    } else {
        ret = NULL;
    }
    return ret;
}
