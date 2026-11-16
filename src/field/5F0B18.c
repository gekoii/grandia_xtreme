#include "common.h"
#include "party.h"

s32 ItemGetListCategory(s16 arg0, s16 arg1) {
    s32 r = 0;

    switch (arg0) {
    case 0:
        switch (ptyItemTable[arg1].type) {
        case 5:
            r = 4;
            break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            r = 5;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 11:
        case 12:
        case 13:
        case 14:
            r = 3;
            break;
        }
        break;
    case 1:
        break;
    case 2:
        r = 2;
        break;
    case 3:
        r = 1;
        break;
    }
    return r;
}
