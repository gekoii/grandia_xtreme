#include "common.h"
#include "battle/misc.h"
#include "battle/n30.h"
#include "sys.h"
#include "protos/battle.h"

void PtclBtlSprDrawKeyed(PtclBtlSpr *, s32, PtclSparkKeys *, u8 (*)[4]);

void PtclBurstDrawSpark(PtclBurst *arg0) {
    u8 (*col)[4];

    if (arg0->type >= 0x15) {
        col = ptclSparkColGreen;
    } else if (arg0->type >= 0x11) {
        col = ptclSparkColRainbow;
    } else {
        col = ptclSparkColBlue;
    }
    GphTexBind(arg0->texId);
    SprBegin(0.0f, 0.0f);
    sprState.blend = 2;
    PtclBtlSprDrawKeyed(arg0->parts, arg0->max, &ptclSparkKeys, col);
    SprFlush();
    if (arg0->parts2 != NULL) {
        if (arg0->type >= 0x15) {
            col = ptclSparkGlowColGreen;
        } else if (arg0->type >= 0x11) {
            col = ptclSparkGlowColRainbow;
        } else {
            col = ptclSparkGlowColBlue;
        }
        GphTexBind(0x41);
        SprBegin(0.0f, 0.0f);
        sprState.blend = 2;
        PtclBtlSprDrawKeyed(arg0->parts2, arg0->max2, &ptclSparkGlowKeys, col);
        SprFlush();
    }
}
