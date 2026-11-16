/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/n30.h"


void FxWallUpdate(BtlUnit *arg0) {
    FxWall *p = &fxWall[arg0->id];

    switch (p->state) {
    case 1:
        if (p->stamp != btlFrameCount) {
            if (p->pool != 0) {
                MemFree(p->pool);
            }
            p->model = BtlModelCreateWithPool(fxWallModel, &p->pool);
            if (p->model == NULL) {
                FxWallFree(p);
                return;
            }
            p->state++;
        }
        break;
    case 2:
        if (p->model != NULL) {
            ChrModelUvScrollUpdate(p->model, 1.0f);
            if (p->flash > 0.0f) {
                p->flash -= 0.01171875f;
                if (p->flash <= 0.0f) {
                    p->flash -= 0.0f;
                }
            }
        }
        if (arg0->wall <= 0) {
            p->state = 3;
            p->stamp = btlFrameCount;
            p->model = NULL;
        }
        break;
    case 3:
        if (p->stamp != btlFrameCount) {
            FxWallFree(p);
        }
        break;
    }
}
