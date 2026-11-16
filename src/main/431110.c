#include "common.h"
#include "camera.h"


/* Local prototypes: GphViewSetFog is declared (s32, f32 x3, s32 x3) for the
 * EABI argument-load order, and a cast call would not be a tail call. */
void GphViewSetClip(s32, f32, f32);
void GphViewSetFovScaled(s32, f32, f32);
void GphViewSetFog(s32, f32, f32, f32, s32, s32, s32);

#define VCLEAR(v) __asm__ volatile("sqc2 $vf0, 0(%0)" : : "r"(v) : "memory")

#define VUNIT(m) \
    __asm__ volatile( \
        "vmr32.xyzw $vf5, $vf0\n\t" \
        "vmr32.xyzw $vf6, $vf5\n\t" \
        "vmr32.xyzw $vf7, $vf6\n\t" \
        "sqc2 $vf0, 0x30(%0)\n\t" \
        "sqc2 $vf5, 0x20(%0)\n\t" \
        "sqc2 $vf6, 0x10(%0)\n\t" \
        "sqc2 $vf7, 0x0(%0)" \
        : : "r"(m) : "memory")

void CamInit(Camera *p, s32 id)
{
    if (p != NULL) {
        VUNIT(p->viewMtx);
        p->eye[2] = -1000.0f;
        p->eye[3] = -1000.0f;
        p->dir[2] = 1.0f;
        p->dir[3] = 1.0f;
        p->lookAt[0] = 0.0f;
        p->lookAt[1] = 0.0f;
        p->lookAt[2] = 0.0f;
        p->lookAt[3] = 0.0f;
        p->eye[0] = 0.0f;
        p->eye[1] = 0.0f;
        p->dir[0] = 0.0f;
        p->dir[1] = 0.0f;
        VCLEAR(p->shakeAmp);
        VCLEAR(p->shakeRand);
        VCLEAR(p->shakeOfs);
        VCLEAR(p->unkD0);
        VCLEAR(p->unk120);
        p->unk134 = 0;
        p->unk138 = 0;
        p->fogMin = 0.0f;
        p->shakeFlags = 0;
        p->shakeMode = 0;
        p->mode = 0x23;
        p->screenW = 0x280;
        p->screenH = 0x1C0;
        p->fov = 55.0f;
        p->fogDist = 10000.0f;
        p->fogMax = 255.0f;
        p->viewNo = id;
        p->zoom = 1.0f;
        p->clipFar = 8000.0f;
        p->clipNear = 1.0f;
        p->fogColor[0] = 0xFF;
        p->fogColor[1] = 0xFF;
        p->fogColor[2] = 0xFF;
        GphViewSetClip(p->viewNo, p->clipNear, p->clipFar);
        GphViewSetFovScaled(p->viewNo, p->fov, p->zoom);
        GphViewSetFog(p->viewNo, p->fogDist, p->fogMax, p->fogMin, p->fogColor[0], p->fogColor[1], p->fogColor[2]);
    }
}
