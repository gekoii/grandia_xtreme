/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "field/n25.h"

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

extern void VuBezier3(FVECTOR *, FVECTOR *, f32);

void FldPictureUpdate(void) {
    s32 i;

    if (fldPictureWork->shown == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        if (fldPictureWork->slots[i] == 0) {
            continue;
        }
        fldPictureWork->t[i] += 0.02f;
        if (fldPictureWork->t[i] >= 1.0f) {
            fldPictureWork->t[i] = 1.0f;
            if (i != 0) {
                fldPictureWork->slots[i] = 0;
            }
        }
        if (i == 0) {
            fldPictureWork->keys[0][1] = -192.0f;
            fldPictureWork->keys[0][0] = 640.0f;
            fldPictureWork->keys[0][2] = 0.0f;
            fldPictureWork->keys[0][3] = 0.0f;
            fldPictureWork->keys[1][0] = 64.0f;
            fldPictureWork->keys[1][1] = 0.0f;
            fldPictureWork->keys[1][2] = 128.0f;
            fldPictureWork->keys[1][3] = 0.0f;
            copyq(fldPictureWork->keys[2], fldPictureWork->keys[1]);
            copyq(fldPictureWork->keys[3], fldPictureWork->keys[1]);
        } else {
            fldPictureWork->keys[0][0] = 64.0f;
            fldPictureWork->keys[3][0] = -640.0f;
            fldPictureWork->keys[0][1] = 0.0f;
            fldPictureWork->keys[0][2] = 128.0f;
            fldPictureWork->keys[0][3] = 0.0f;
            fldPictureWork->keys[3][1] = -192.0f;
            fldPictureWork->keys[3][2] = 0.0f;
            fldPictureWork->keys[3][3] = 0.0f;
            copyq(fldPictureWork->keys[1], fldPictureWork->keys[0]);
            copyq(fldPictureWork->keys[2], fldPictureWork->keys[0]);
            fldPictureWork->keys[1][2] = 0.0f;
            fldPictureWork->keys[2][2] = 0.0f;
        }
        VuBezier3(&fldPictureWork->out[i], fldPictureWork->keys, fldPictureWork->t[i]);
    }
    if (fldPictureWork->slots[0]) {
        fldPictureWork->dim += 8;
        if (fldPictureWork->dim >= 0x80) {
            fldPictureWork->dim = 0x80;
        }
    } else {
        fldPictureWork->dim -= 4;
        if (fldPictureWork->dim < 0) {
            fldPictureWork->dim = 0;
        }
    }
}
