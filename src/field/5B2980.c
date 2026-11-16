#include "common.h"
#include "field/event.h"

void EvtAreaClearAllMoves(void) {
    s32 i;
    s32 n = evtAreaTable->count;

    for (i = 0; i < n; i++) {
        __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(evtAreaTable->areas[i].move) : "memory");
    }
}
