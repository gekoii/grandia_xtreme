#include "common.h"

typedef struct {
    f32 x;
    f32 y;
} __attribute__((aligned(8))) Vec2_00457778;

Vec2_00457778 GsPackST(f32 a, f32 c) {
    Vec2_00457778 v;
    v.x = a;
    v.y = c;
    return v;
}
