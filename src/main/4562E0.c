#include "common.h"

void GsVif0Wait(void) {
    while (*(volatile s32 *)0x10003800 & 8) {
    }
}
