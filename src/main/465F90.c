#include "common.h"
#include "protos/main.h"
#include "snd.h"


void videoDecMain(VideoDec *arg0) {
    viBufReset(&arg0->vibuf);
    voBufReset(&movVoBuf);
    decBs0(arg0);
    while (movVoBuf.count != 0 && arg0->state != 1) {
    }
    videoDecSetState(arg0, 3);
}
