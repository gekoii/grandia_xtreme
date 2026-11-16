#include "common.h"
#include "main/n11.h"

void RotateThreadReadyQueue(s32);

void switchThread(void) {
    RotateThreadReadyQueue(movMainPrio);
}
