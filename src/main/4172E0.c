/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n02.h"

void MenuTextListMeasure(TxtObj **arg0) {
    TxtObj **pp;
    TxtObj *t = arg0[0];
    t->width = 0;
    for (pp = &arg0[1]; *pp != NULL; pp++) {
        t->width = t->width + (f32)(*pp)->width / (*pp)->scale * t->scale;
    }
}
