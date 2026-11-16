#include "common.h"
#include "sys.h"



void MenuHelpSetColorCyan(void) {
    sprState.col[0].r = 0;
    sprState.col[0].g = 0x80;
    sprState.col[0].b = 0x80;
}
