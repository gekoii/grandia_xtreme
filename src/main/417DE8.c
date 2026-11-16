#include "common.h"
#include "main/n02.h"

void MenuTextSetAddColor(u8 arg0, u8 arg1, u8 arg2) {
    menuTextAddR = arg0;
    menuTextAddG = arg1;
    menuTextAddB = arg2;
}

void MenuTextSetGradColor(u8 arg0, u8 arg1, u8 arg2) {
    menuTextGradR = arg0;
    menuTextGradGValue = arg1;
    menuTextGradB = arg2;
}

void MenuTextSetOffset(s8 arg0, s8 arg1) {
    menuTextOffsetX = arg0;
    menuTextOffsetY = arg1;
}
