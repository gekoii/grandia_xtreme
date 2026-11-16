#include "common.h"
#include "protos/main.h"
#include "main/n02.h"

extern SprRect menuButtonPadSprites[];


void MenuButtonPadDraw(s32 flags, s16 x, s16 y) {
    SprRectListEntry parts[5];
    s32 k;

    k = 4;
    if (!(menuWidgetFrame & 0x10)) k = 8;
    parts[0].rect = &menuButtonPadSprites[(flags & 0x1000) ? k : 0];
    parts[1].rect = &menuButtonPadSprites[(flags & 0x4000) ? k + 1 : 1];
    parts[2].rect = &menuButtonPadSprites[(flags & 0x8000) ? k + 2 : 2];
    parts[3].rect = &menuButtonPadSprites[(flags & 0x2000) ? k + 3 : 3];
    parts[4].rect = NULL;
    parts[0].x = x + 10;
    parts[0].y = y;
    parts[1].x = x + 10;
    parts[1].y = y + 0x11;
    parts[2].x = x;
    parts[2].y = y + 10;
    parts[3].x = x + 0x11;
    parts[3].y = y + 10;
    SprDrawRectList(parts);
}
