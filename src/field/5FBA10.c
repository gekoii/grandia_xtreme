#include "common.h"
#include "field/state.h"

void ChrStunUpdate(ChrObj *p) {
    u8 *q;

    if (p->stunState == 0) {
        return;
    }
    q = &p->stunSpin;
    if (++*q > 0xB4) {
        *q -= 0xB4;
    }
    switch (p->stunState) {
    case 1:
        if (++p->stunAnim > 0x20) {
            p->stunAnim = 0;
            p->stunState++;
        }
        break;
    case 2:
        if (++p->stunAnim > 0x2D) {
            p->stunAnim = 0;
        }
        if (p->stunTimer == 0 || --p->stunTimer == 0) {
            p->stunAnim = 0;
            p->stunState++;
        }
        break;
    case 3:
        if (++p->stunAnim > 0x20) {
            p->stunAnim = 0;
            p->stunState = 0;
        }
        break;
    }
}
