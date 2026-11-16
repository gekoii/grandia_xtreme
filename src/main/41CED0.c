#include "common.h"
#include "sys.h"



void MenuHelpSetElementColor(s8 arg0) {
    switch ((s8)(arg0 - 1)) {
    case 0:
        sprState.col[0].r = 0x80;
        sprState.col[0].g = 0x30;
        sprState.col[0].b = 0x10;
        break;
    case 1:
        sprState.col[0].r = 0x40;
        sprState.col[0].g = 0x80;
        sprState.col[0].b = 0x60;
        break;
    case 2:
        sprState.col[0].r = 0x10;
        sprState.col[0].g = 0x60;
        sprState.col[0].b = 0x80;
        break;
    case 3:
        sprState.col[0].r = 0x60;
        sprState.col[0].g = 0x30;
        sprState.col[0].b = 0;
        break;
    case 4:
        sprState.col[0].r = 0x80;
        sprState.col[0].g = 0x70;
        sprState.col[0].b = 0;
        break;
    case 5:
        sprState.col[0].r = 0x70;
        sprState.col[0].g = 0x40;
        sprState.col[0].b = 0x80;
        break;
    case 6:
        sprState.col[0].r = 0x60;
        sprState.col[0].g = 0x50;
        sprState.col[0].b = 0x80;
        break;
    case 7:
        sprState.col[0].r = 0x30;
        sprState.col[0].g = 0x80;
        sprState.col[0].b = 0;
        break;
    }
}
