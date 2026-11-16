#include "common.h"
#include "main/n05.h"

typedef int s128 __attribute__((mode(TI)));

void GphEnvMapSetColor(s32 arg0, s128 *arg1) {
    s128 *d = (s128 *)gphEnvMapTable[arg0].color;
    s128 t;
    __asm__ volatile("lq %0, 0(%1)" : "=&r"(t) : "r"(arg1));
    *d = t;
}
