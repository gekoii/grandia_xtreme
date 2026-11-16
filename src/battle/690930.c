#include "common.h"
#include "battle/misc.h"
#include "party.h"
#include "protos/battle.h"

void BtlCmdListHelpShow(void) {
    BtlCmdList *b = &btlCmdList;
    f32 x;
    f32 y;
    f32 f;
    PtyItem *p;
    s16 v;
    s32 k;
    PtyMoveLevel *e8;
    if (b->helpSide == 0) {
        f = b->col;
        x = btlCmdListX + f * -8.0f;
        y = btlCmdListY + f * 28.0f;
    } else {
        x = 24.0f;
        y = 352.0f;
    }
    if (b->mode == 1) {
        if (b->rowState[b->row] == 0) {
            p = &ptyItemTable[ptyPartyData->bag[b->cells[b->row * 6 + b->col].id].id];
        } else {
            v = ptyCharData[b->unit->params->id - 0x1001].equip[b->cells[b->row * 6 + b->col].id];
            if (v > 0) {
                p = &ptyItemTable[v];
            } else {
                MenuHelpClose();
                return;
            }
        }
        k = p->type;
        switch (k) {
        case 5:
            MenuHelpOpen(b->helpSide, 4, p, x, y);
            break;
        case 6: case 7: case 8: case 9: case 10:
            MenuHelpOpen(b->helpSide, 5, p, x, y);
            break;
        default:
            MenuHelpOpen(b->helpSide, 3, p, x, y);
            break;
        }
    } else if (b->mode == 2) {
        MenuHelpOpen(b->helpSide, 0, &ptyMoveTable[b->cells[b->row * 6 + b->col].id], x, y);
    } else {
        e8 = &ptyPartyData->moveLevels[(b->unit->params->id - 0x1001) * 5 + b->cells[b->row * 6 + b->col].id];
        if (e8->level == 0) {
            MenuHelpClose();
        } else {
            MenuHelpOpen(b->helpSide, 1, &ptyMoveTable[e8->moveId], x, y);
        }
    }
}
