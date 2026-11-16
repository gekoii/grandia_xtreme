#include "common.h"
#include "protos/battle.h"
#include "battle/n29.h"
#include "party.h"


void BtlCmdListComboPartnersShow(void) {
    BtlCmdList *p;
    BtlCmdListCellAt *c;
    s16 v;

    p = &btlCmdList;
    IpComboPreviewSet(0);
    c = BTL_CMD_LIST_CELL_AT(p, p->row * 6 + p->col);
    if (!(c->cell.flags & 2)) {
        v = ptyPartyData->moveLevels[(p->unit->params->id - 0x1001) * 5 + c->cell.id].moveId;
        if (v >= 0xDD) {
            IpComboPreviewSet(&ptyMoveTable[v]);
        }
    }
}
