#include "common.h"
#include "camera.h"

typedef f32 FVEC[4] __attribute__((aligned(16)));

/* s32 view of camFollow (CamFollowWork, turn at +0xC) and camPadTurn: with the
   shared f32 types the three closing stores come out in another order. */
typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    u8 pad14[0x50 - 0x14];
    FVEC unk50;
    FVEC unk60;
    FVEC unk70;
    FVEC unk80;
    FVEC unk90;
    FVEC unkA0;
} Cam;

extern Cam *camFollow;
extern s32 camPadTurn[];

static inline void copyq(void *dst, void *src) {
    __asm__ volatile("lq $2, 0(%1)\n\tsq $2, 0(%0)" : : "r"(dst), "r"(src) : "$2", "memory");
}

void CamFollowSyncToCamera(Camera *cam) {
    copyq(camFollow->unk50, cam->lookAt);
    copyq(camFollow->unk60, cam->lookAt);
    copyq(camFollow->unk70, cam->lookAt);
    copyq(camFollow->unk80, cam->eye);
    __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(camFollow->unkA0) : "memory");
    camFollow->unkC = 0;
    camFollow->unk10 = 0;
    camPadTurn[0] = 0;
    camPadTurn[1] = 0;
}
