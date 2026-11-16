/* rodata: 0x00673E70-0x00673ED0 */
#include "common.h"
#include "party.h"
#include "save.h"
#include "protos/field.h"

s32 ItemAdd(s16 arg0) {
    u8 tbl[26] = "@ABHIJKPQRSTXYZ[\\]`abcdefg";
    s32 r = -1;
    s32 k;

    switch (ptyItemTable[arg0].type) {
    case 15:
        break;
    case 0:
        return -1;
    case 11:
        if (ItemEggAdd(arg0) == 0) {
            r = 2;
        }
        break;
    case 12:
        if (ItemBookAdd(arg0) == 0) {
            r = 2;
        }
        break;
    case 14:
        if ((u16)(arg0 - 0x33) >= 30) {
            r = ItemKeyAdd(arg0);
        } else {
            k = tbl[arg0 - 0x33];
            /* subGameFlags[k / 8], indexed from globalFlags as the match needs */
            sysSaveData.globalFlags[k / 8 + 0x80] |= 1 << (k % 8);
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 13:
        r = ItemBagAdd(arg0, 10);
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        r = ItemBagAdd(arg0, 1);
        break;
    }
    return r;
}
