#include "common.h"
#include "protos/main.h"
#include "main/n11.h"


s32 MovGifEndHandler(void) {
    if (movDispActive != 0) {
        if (movFrameSent != 0) {
            voBufDecCount(&movVoBuf);
            movFrameSent = 0;
        }
    }
    return 0;
}
