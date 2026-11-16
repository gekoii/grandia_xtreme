/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "main/n02.h"


void MenuHelpDrawStatNames(void) {
    MenuStatLine *e;
    s32 i;
    u32 col;
    f32 s;
    f32 f;

    e = menuHelpStatLines;
    for (i = 4; i > 0; i--, e++) {
        if (e->kind == -1) {
            continue;
        }
        switch (e->kind) {
        case 0:
            col = UtilHsvToRgb(0, 0.9f, 1.0f);
            MenuTextSetAddColor(0x18, 0, 0);
            break;
        case 1:
            col = UtilHsvToRgb(0xAAAA, 0.8f, 1.0f);
            MenuTextSetAddColor(0, 0, 0x18);
            break;
        case 2:
            col = UtilHsvToRgb(0xF333, 0.9f, 1.0f);
            MenuTextSetAddColor(0x14, 0, 0xC);
            break;
        case 3:
            col = UtilHsvToRgb(0xB333, 0.9f, 1.0f);
            MenuTextSetAddColor(0xC, 0, 0x14);
            break;
        case 4:
            col = UtilHsvToRgb(0x2AAA, 0.9f, 1.0f);
            MenuTextSetAddColor(0x10, 0x10, 0);
            break;
        case 5:
            col = UtilHsvToRgb(0x5555, 0.9f, 1.0f);
            MenuTextSetAddColor(0, 0x18, 0);
            break;
        }
        s = e->text.width;
        sprState.col[0].r = col >> 24;
        sprState.col[0].g = col >> 16;
        sprState.col[0].b = col >> 8;
        f = 1.0f;
        if (!(s * sprState.sx <= 20.0f)) {
            f = 20.0f / s;
        }
        SprPushTransform(0, e->text.x * (1.0f - f), 0.0f, f, 1.0f);
        MenuTextDraw(&e->text);
        SprPopTransform();
    }
    sprState.col[0].b = 0x80;
    sprState.col[0].g = 0x80;
    sprState.col[0].r = 0x80;
}
