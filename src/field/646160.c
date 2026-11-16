#include "common.h"
#include "field/n24.h"
#include "protos/field.h"

void MiniDraw(void) {
    MiniCtl *w = miniCtl;

    if (w->dataFile != 0 && w->state != 0 && w->state != 1) {
        if (w->kind == 2) {
            ((s64 (*)(MiniCtl *))MiniBallDraw)(w);
        } else {
            ((s64 (*)(MiniCtl *))MiniConcertDraw)(w);
        }
    }
}
