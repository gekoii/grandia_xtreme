/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/n30.h"

f32 IpIconGetPos(s32 arg0) {
    return (50.0f - ipGauge.icons[arg0].pos) / 50.0f;
}
