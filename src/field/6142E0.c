#include "common.h"
#include "protos/field.h"
#include "field/n22.h"


void AmapPlaceSpots(AmapWork *o) {
    s32 tries;
    s32 n;
    FldSpotPlaceTable *list;
    s32 x;
    s32 z;
    u8 c;
    u8 *p;

    tries = 0;
    n = 0;
    list = o->spots;
    while (n < o->spotCount) {
        {
            x = VuRandInt(o->width - 2) + 1;
            z = VuRandInt(o->height - 2) + 1;
            c = fldMapCells[x + z * fldMapWidth];
            if (c == 1 && fldMapCells[fldMapLayerSize + x + z * fldMapWidth] != c) {
                p = &fldMapCells[fldMapLayerSize * 4 + x + z * fldMapWidth];
                if (*p == 0) {
                    *p = c;
                    x = x * 500 - 250;
                    z = z * 500 - 250;
                    list->spots[n].pos[0] = x;
                    list->spots[n].pos[1] = 0.0f;
                    list->spots[n].pos[2] = -z;
                    list->spots[n].pos[3] = 0.0f;
                    list->spots[n].scale[0] = 1.0f;
                    list->spots[n].rot[0] = 0;
                    list->spots[n].scale[1] = 1.0f;
                    list->spots[n].rot[1] = 0;
                    list->spots[n].scale[2] = 1.0f;
                    list->spots[n].rot[2] = 0;
                    list->spots[n].scale[3] = 1.0f;
                    list->spots[n].rot[3] = 0;
                    n++;
                }
            }
            tries++;
            if (tries >= 256) {
                break;
            }
        }
    }
    list->count = o->spotCount;
}
