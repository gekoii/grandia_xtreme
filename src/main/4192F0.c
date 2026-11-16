#include "common.h"
#include "main/n02.h"

void MenuHelpClose(void) {
    if ((u32)(menuHelp.state & 0xF0) < 0x11U) {
        menuHelp.state = 0;
        return;
    }
    menuHelp.state = 0x30;
    menuHelp.nextStyle = 0x7F;
    menuHelp.nextData = NULL;
}
