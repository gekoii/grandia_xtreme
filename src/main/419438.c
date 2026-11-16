#include "common.h"
#include "main/n02.h"
#include "protos/main.h"

typedef struct {
    s8 b0;
    u8 pad[0xF];
} Ctx;

void MenuHelpBuildBasic(void) {
    Ctx c;
    s8 i;
    MenuHelp *m = &menuHelp;

    c.b0 = 0;
    if (m->style == 1) {
        i = 1;
        MenuHelpAddDesc(&c, 6, 6);
        m->lines[0] = 2;
    } else {
        i = 3;
        MenuHelpAddName(&c, 6, 6);
        m->lines[0] = 1;
        MenuHelpAddDesc(&c, 0x2E, 0x1E);
        m->lines[1] = 2;
        m->lines[2] = 0;
        if (m->kind != 2) {
            if (MenuHelpAddTargetRange(&c, 0x2E, 0x4E) != 0) {
                m->lines[3] = i;
                i = 4;
            }
        }
    }
    for (; i < 4; i++) {
        (m->lines)[i] = 0;
    }
}
