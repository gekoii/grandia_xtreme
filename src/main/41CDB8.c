#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n02.h"

extern SprRect menuHelpMinusSprite[];
extern SprRect menuHelpPlusSprite[];
extern MenuNumField menuHelpStatNumField;


void MenuHelpDrawStatValues(void) {
    MenuStatLine *e;
    s32 i;

    e = menuHelpStatLines;
    for (i = 4; i > 0; i--, e++) {
        if (e->kind != -1) {
            sprState.offX = e->text.x + 0x22;
            sprState.offY = e->text.y - 3;
            if (e->value < 0) {
                sprState.col[0].b = 0x10;
                sprState.col[0].g = 0x10;
                SprDrawRect(menuHelpMinusSprite, -0xC, 0);
            } else {
                SprDrawRect(menuHelpPlusSprite, -0xE, 0);
            }
            MenuNumberDraw(&menuHelpStatNumField, e->value);
            sprState.col[0].b = 0x80;
            sprState.col[0].g = 0x80;
            sprState.col[0].r = 0x80;
        }
    }
    sprState.offY = 0;
    sprState.offX = 0;
}
