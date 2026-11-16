/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n09.h"


f32 GsGetFrameTimePercent(void) {
    f32 a, b;

    gsTimerLastCount = *(volatile u32 *)0x10000800;
    a = gsTimerLastCount;
    b = gsTimerTicksPerFrame;
    return a / b * 100.0f;
}
