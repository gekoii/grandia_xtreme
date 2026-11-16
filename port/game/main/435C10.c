/* Port-only C for GphMorphNormalsLerp (0x00435C10), which the decompilation still has in assembly. */
#include "common.h"
#include "gph.h"
#include "protos/main.h"
#include "vu0_port.h"

void GphMorphNormalsLerp(GphPart *part, s64 *a, s64 *b, f32 t);

/*
 * For each GA object of the part: a fresh packet (its 16-byte header copied
 * from the original normals) of normals lerped between two keys of 4.12
 * s16 x 4 normals (a * (1 - t) + b * t) and renormalised (w from a). a and
 * b run on across the objects; the count is rounded up to even.
 *
 * VU0: the asm keeps 1 - t and t in $vf7/$vf8 across GsPacketAlloc64
 * calls (C, which doesn't touch VU0); here they are locals.
 */
void GphMorphNormalsLerp(GphPart *part, s64 *a, s64 *b, f32 t) {
    GphGaObject *obj = part->gaObjs;
    s32 count = part->gaObjCount;
    s16 *pa = (s16 *)a;
    s16 *pb = (s16 *)b;
    f32 wa = 1.0f - t;
    f32 wb = t;
    s32 k;

    for (k = 0; k < count; k++, obj++) {
        s32 n = (obj->vtxCount + 1) & 0xFFFE;
        u8 *buf = (u8 *)GsPacketAlloc64(n * 8 + 0x10);
        s16 *dst;
        s32 i;

        /* 0 when the packet buffer is full: the asm writes to address 0
         * (low RAM on the PS2); here this object and the rest get no
         * morphed normals, as GphPoly3DDraw skips its draw */
        if (buf == NULL) {
            return;
        }
        obj->deformBuf0 = buf;
        obj->flags |= 2;
        VuCopy4(buf, obj->normals);
        dst = (s16 *)(buf + 0x10);
        for (i = 0; i < n; i++) {
            f32 v[4];
            f32 aw;
            f32 q;
            s32 w[4];
            s32 c;

            for (c = 0; c < 4; c++) {
                v[c] = VuItof(pa[c], 12) * wa + VuItof(pb[c], 12) * wb;
            }
            aw = VuItof(pa[3], 12);
            q = VuRsqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            w[0] = VuFtoi(v[0] * q, 12);
            w[1] = VuFtoi(v[1] * q, 12);
            w[2] = VuFtoi(v[2] * q, 12);
            w[3] = VuFtoi(aw, 12);
            VuPackH4(dst, w);
            dst += 4;
            pa += 4;
            pb += 4;
        }
    }
}
