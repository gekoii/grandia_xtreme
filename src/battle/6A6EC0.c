#include "common.h"
#include "protos/battle.h"
#include "battle/n30.h"


void IpComboMarkSet(s8 arg0) {
    IpGauge *p = &ipGauge;

    if (arg0 == 0) {
        p->comboMark = (p->comboMark != 0) * 2;
    } else {
        p->comboMark = 1;
    }
    if (arg0 != 0) {
        SndSePlay3D(0x7D6, 0, 0, NULL);
    }
}
