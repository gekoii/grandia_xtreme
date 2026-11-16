/* cc: ee-gcc2.9-991111-01 */
#include "common.h"
#include "camera.h"
#include "protos/main.h"

static inline f32 fsqrt(f32 x) { f32 r; __asm__ volatile ("sqrt.s %0, %1" : "=f"(r) : "f"(x)); return r; }
static inline f32 fmin(f32 a, f32 b) { f32 r; __asm__ volatile ("min.s %0, %1, %2" : "=f"(r) : "f"(a), "f"(b)); return r; }
static inline f32 fmax(f32 a, f32 b) { f32 r; __asm__ ("max.s %0, %1, %2" : "=f"(r) : "f"(a), "f"(b)); return r; }

void CamDirToAngles(Camera *c) {
    f32 x, y, z;
    f32 len, h, a, s, r, t;

    x = c->dir[0];
    y = c->dir[1];
    z = c->dir[2];
    len = fsqrt(x * x + y * y + z * z);
    h = fsqrt(x * x + z * z);
    a = z / h;
    a = fmin(a, 1.0f);
    s = 1.0f;
    a = fmax(a, -1.0f);
    if (x > 0.0f) s = -1.0f;
    r = s * acosf(a);
    s = y / len;
    s = fmin(s, 1.0f);
    t = asinf(fmax(s, -1.0f));
    c->yaw = r;
    c->pitch = -t;
}
