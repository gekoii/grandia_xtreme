/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n02.h"
#include "protos/main.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void MenuCursorMoveTo(f32 arg0, f32 arg1) {
    f32 dx, dy, d, s, sx, sy;

    dx = arg0 - menuCursor.x;
    dy = arg1 - menuCursor.y;
    menuCursor.targetX = arg0;
    menuCursor.targetY = arg1;
    d = dx * dx + dy * dy;
    if (d < 1.0f) {
        menuCursor.x = arg0;
        menuCursor.mode = 1;
        menuCursor.y = arg1;
        menuCursor.timer = 0;
        return;
    }
    s = 20.0f / fsqrt(d);
    sx = dx * s;
    sy = dy * s;
    if (menuCursor.mode != 2) {
        menuCursor.velX = sx;
        menuCursor.velY = sy;
        menuCursor.mode = 2;
        MenuSparkleSpawn(menuCursor.x, menuCursor.y);
    } else {
        menuCursor.velX = (menuCursor.velX + sx) * 0.5f;
        menuCursor.velY = (menuCursor.velY + sy) * 0.5f;
    }
}
