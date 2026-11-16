/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "main/n02.h"
#include "protos/main.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }

void MenuCursorUpdate(void) {
    MenuCursor *p = &menuCursor;
    f32 dx, dy, dist, vx, vy, lim;

    if (p->mode == 2) {
        dx = p->targetX - p->x;
        dy = p->targetY - p->y;
        dist = fsqrt(dx * dx + dy * dy);
        if (dist > 1.0f) {
            lim = 10.0f / dist;
            vx = p->velX + dx * lim;
            vy = p->velY + dy * lim;
            p->velY = vy;
            p->velX = vx;
            lim = fsqrt(vx * vx + vy * vy);
            if (lim > 80.0f) {
                lim = 80.0f / lim;
                p->velX = vx * lim;
                p->velY = vy * lim;
            }
            if (dist < 200.0f) {
                lim = dist * 0.001f + 0.6f;
                p->velX = p->velX * lim;
                p->velY = p->velY * lim;
            }
        }
        MenuSparkleSpawn(p->x, p->y);
        if ((rand() >> 15) < 0x4000) {
            MenuSparkleSpawn(p->x, p->y);
        }
        if (dist > 1.0f) {
            lim = fsqrt(p->velX * p->velX + p->velY * p->velY);
        } else {
            lim = 1.0f;
        }
        if (dist < lim) {
            p->mode = 1;
            p->x = p->targetX;
            p->y = p->targetY;
            p->timer = 0;
        } else {
            p->x = p->x + p->velX;
            p->y = p->y + p->velY;
        }
    }
    p->timer++;
}
