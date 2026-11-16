#include "common.h"
#include "snd.h"

void voBufDecCount(VoBuf *arg0) {
    if (arg0->count > 0) {
        arg0->count--;
    }
}
