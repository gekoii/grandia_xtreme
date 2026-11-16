#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"
#include "battle/n28.h"
#include "sys.h"

typedef struct {
    s16 x0;
    s16 y;
} Quad;

extern Quad menuTacticsTagQuad;
f32 VuSinAngle(s32);

void MenuTacticsDrawTags(void) {
    s32 i;
    MenuTacticsTag *e;
    f32 half;
    s16 y;

    if (btlPhase == 0x80 || btlPhase == 1) {
        if (menuTacticsState > 0) {
            e = menuTacticsTags;
            half = 0.5f;
            SprBegin(80.0f, 0.0f);
            for (i = 0; i < 4; i++, e++) {
                if (btlUnits[i].state >= 2) {
                    y = e->y + 26.0f;
                    menuTacticsTagQuad.y = y;
                    if (menuTacticsState == 2 && i == menuTacticsMember) {
                        SprDrawColorQuadTinted(&menuTacticsTagQuad, 0xA0A0FF00, e->alpha,
                                      (VuSinAngle(btlTickCount * 0x300) * half + half) * 0.3f);
                    } else {
                        SprDrawColorQuad(&menuTacticsTagQuad, e->alpha * half);
                    }
                }
                sprState.ox += 160.0f;
            }
            SprFlush();
        }
    }
}
