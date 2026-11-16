#include "common.h"
#include "field/event.h"
#include "field/n13.h"

void OccSwitchForwardStart(OccEntry *obj) {
    OccSwitch *s = OCC_PARAMS(obj, OccSwitch);

    switch (s->mode) {
    case 0:
    case 1:
        obj->timer = 1;
        break;
    case 2:
        switch (s->ramp) {
        case 0:
            s->cur = s->interval;
            break;
        case 1:
            s->cur = s->interval;
            break;
        case 2:
            s->cur = 0;
            break;
        }
        obj->timer = s->time;
        s->wait = 0;
        break;
    }
}
