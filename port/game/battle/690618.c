/* Port-only C for BtlCmdListOpen (0x00690618), which the decompilation still has in assembly. */
#include "common.h"
#include "protos/main.h"
#include "protos/battle.h"
#include "battle/misc.h"
#include "battle/unit.h"

/* Opens the Items (1) / Magic (2) / Moves (3) list for u: resets the list,
 * drops the item reservations of party members no longer casting an item,
 * builds the cells and puts the cursor on the first filled one from the
 * last position; with no cell filled the list closes at once (state 0xF0). */
void BtlCmdListOpen(BtlUnit *u, s32 arg1)
{
    BtlCmdList *l = &btlCmdList;
    s8 mode = arg1;
    BtlUnit *o;
    s32 state;
    s32 i;
    s8 j;

    u->combo = 1;
    l->unit = u;
    l->alive = -1;
    l->helpSide = 1;
    l->titleSlide = 200.0f;
    l->rightSlide = -160.0f;
    l->leftSlide = -80.0f;
    l->state = 0;
    l->unk1C = 1;
    l->side = 0;
    l->pageShown[1] = 0;
    l->pageShown[0] = 0;
    l->unk22 = 0;
    l->unk23 = 0;
    for (i = 0; i < 12; i++) { /* both page sides */
        l->entries[i / 6][i % 6].type = 0;
    }

    o = btlUnits;
    for (i = 0; i < 4; i++, o++) {
        if (btlCmdListReservedItem[i] >= 0) {
            state = o->state < 0x80 ? o->retState : o->state;
            if (state != BTL_STATE_CAST || o->action != BTL_ACT_ITEM) {
                btlCmdListReservedItem[i] = -1;
            }
        }
    }

    if (mode != l->mode) {
        l->col = 0;
        l->row = 0;
        if (mode == 1) {
            l->row = mode;
        }
    }
    btlCmdListComboNg = 0;
    l->mode = mode;
    BtlCmdListBuild();

    for (j = 35; j >= 0; j--) {
        if (l->cells[j].flags != 0) {
            break;
        }
    }
    if (j < 0) {
        l->state = 0xF0;
        return;
    }
    j = l->col + l->row * 6;
    while (l->cells[j].flags == 0) {
        j++;
        if (j >= 36) {
            j = 0;
        }
    }
    l->row = j / 6;
    l->col = j % 6;

    if (mode == 1) {
        btlCmdListX = 148.0f;
    } else {
        btlCmdListX = 169.0f;
    }
    btlCmdListY = 180.0f;
    MenuCursorShow(btlCmdListX + l->col * -8.0f - 16.0f, btlCmdListY + l->col * 28.0f - 12.0f);
    MenuLrArrowsSet(320.0f, 244.0f, 280.0f);
    BtlCmdListHelpShow();
    btlCmdListSwapArrowAnim = -1;
    if (l->mode == 3) {
        BtlCmdListComboPartnersShow();
    }
}
