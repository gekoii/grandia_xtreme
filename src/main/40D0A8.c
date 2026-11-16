#include "common.h"
#include "protos/main.h"
#include "gph.h"

extern u8 gphShatterHold;

void GphShatterUpdate(void) {
    if (gphShatter.active != 0 && gphShatter.model != NULL) {
        if (gphShatterHold != 1) {
            switch (gphShatter.mode) {
            case 0:
            case 1:
            case 3:
            case 9:
            case 10:
                /* do-while(0): keeps the call out of tail position (jal, then return) */
                do {
                    GphShatterMovePieces(gphShatter.model);
                } while (0);
                break;
            }
        }
    }
}
