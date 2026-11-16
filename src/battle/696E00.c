#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"


void BtlCaptionLevelUpStamp(void) {
    BtlCaption *p;
    SndSePlay3D(0x7DB, 0, 0, 0);
    p = &btlCaption;
    if (p->stamp.mode == 0) {
        p->stamp.mode = 1;
    }
}
