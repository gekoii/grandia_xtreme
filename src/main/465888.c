#include "common.h"
#include "protos/main.h"

void defMain(void) {
    for (;;) {
        switchThread();
    }
}

void MovErrMessage(void *arg0) {

}
