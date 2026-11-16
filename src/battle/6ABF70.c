/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "protos/battle.h"

void PtclBurstRegisterDebris(PtclBurst *arg0) {
    s32 i;
    PtclDebris *e;

    i = 7;
    e = arg0->parts;
    do {
        i--;
        if (!(e->life < 0.0f)) {
            BtlChrModelCalcDraw(e->model);
        }
        e++;
    } while (i >= 0);
}
