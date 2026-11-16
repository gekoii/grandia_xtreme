#include "common.h"
#include "protos/main.h"


void GphTexRegisterOwnId(u8 *arg0) {
    GphTexRegisterAt(arg0, arg0[0x60]);
}
