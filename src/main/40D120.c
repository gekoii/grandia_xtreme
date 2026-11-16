/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "sys.h"
#include "gph.h"

extern u8 GphMeshClipTest(GphGaObject *);

/* o is one local for both the GA objects (update_objs) and &gphShatter (cases 1, 2):
 * separate typed locals allocate registers differently */
#define F ((GphShatter *)o)
#define update_objs(e) \
    e = gphShatter.model->parts; \
    e = e - 1; \
    for (i = 0; i < gphShatter.model->partCount; i++) { \
        e = e + 1; \
        o = e->gaObjs; \
        if (o != NULL) { \
            ((GphGaObject *)o)->clip = GphMeshClipTest(o); \
            if (((GphGaObject *)o)->clip != 0xFF) { \
                GphVu1Render(o, 0); \
            } \
        } \
    }

void GphShatterDraw(void) {
    GphBatchQuad *p;
    GphPart *e;
    GphPart *e2;
    s32 i;
    void *o;
    u8 c;

    p = gphShatter.prim;
    if (gphShatter.active == 0) {
        return;
    }
    if (gphShatter.model == NULL) {
        return;
    }
    GphViewSelect(7, 0.0f);
    GphFbSetWorkTexture(0, 0x7F);
    GphBatchSetOffset(0, 0);
    GphFbUploadImage(sysScreenCaptureAddr);
    switch (gphShatter.mode) {
    case 0:
        update_objs(e);
        gphShatter.color[3] -= 0.7f;
        if (gphShatter.color[3] < 0.0f) {
            gphShatter.active = 0;
        }
        break;
    case 1:
        o = &gphShatter;
        if (F->color[3] < 126.0f) {
            p->type = 6;
            p->v[0].r = (s32)F->color[3] * 2;
            p->v[0].g = (s32)F->color[3] * 2;
            p->v[0].b = (s32)F->color[3] * 2;
            p->v[0].a = 0x80;
            p->v[1].r = (s32)F->color[3] * 2;
            p->v[1].g = (s32)F->color[3] * 2;
            p->v[1].b = (s32)F->color[3] * 2;
            p->v[1].a = 0x80;
            p->v[2].r = (s32)F->color[3] * 2;
            p->v[2].g = (s32)F->color[3] * 2;
            p->v[2].b = (s32)F->color[3] * 2;
            p->v[2].a = 0x80;
            p->v[3].r = (s32)F->color[3] * 2;
            p->v[3].g = (s32)F->color[3] * 2;
            p->v[3].b = (s32)F->color[3] * 2;
            p->v[3].a = 0x80;
            p->v[0].x = 0;
            p->v[0].y = 0;
            p->v[1].x = 0;
            p->v[1].y = 0x1C0;
            p->v[2].x = 0x280;
            p->v[2].y = 0;
            p->v[3].x = 0x280;
            p->v[3].y = 0x1C0;
            p->z = 0;
            p[1].type = 0;
            GphBatchDrawQuads(gphShatter.prim);
        }
        GphFbSetWorkTexture(0, 0x7F);
        update_objs(e);
        gphShatter.color[3] -= 0.7f;
        if (gphFbDivParam.blur != 0) {
            gphFbDivParam.blur--;
        }
        if (gphShatter.color[3] < 0.0f) {
            gphFbDivParam.blur = 0;
            gphShatter.active = 0;
        }
        break;
    case 9:
    case 10:
        update_objs(e);
        gphShatter.color[3] -= 1.0f;
        if (gphShatter.color[3] < 0.0f) {
            gphShatter.active = 0;
        }
        break;
    case 2:
        o = &gphShatter;
        c = 0xFF - (s32)F->unkC;
        if (c < 0x10) {
            c = 0x10;
        }
        p->type = 6;
        p->v[0].r = c;
        p->v[0].g = c;
        p->v[0].b = c;
        p->v[0].a = 0x80;
        p->v[1].r = c;
        p->v[1].g = c;
        p->v[1].b = c;
        p->v[1].a = 0x80;
        p->v[2].r = c;
        p->v[2].g = c;
        p->v[2].b = c;
        p->v[2].a = 0x80;
        p->v[3].r = c;
        p->v[3].g = c;
        p->v[3].b = c;
        p->v[3].a = 0x80;
        p->v[0].u = 0;
        p->v[0].v = 0;
        p->v[1].u = 0;
        p->v[1].v = 0x1C0;
        p->v[2].u = 0x280;
        p->v[2].v = 0;
        p->v[3].u = 0x280;
        p->v[3].v = 0x1C0;
        p->v[0].x = 0;
        p->v[0].y = 0;
        p->v[1].x = 0;
        p->v[1].y = 0x1C0;
        p->v[2].x = 0x280;
        p->v[2].y = 0;
        p->v[3].x = 0x280;
        p->v[3].y = 0x1C0;
        p->z = 0;
        p[1].type = 0;
        gphShatter.unk10 += 0.5f;
        gphShatter.unkC += 2.0f;
        GphBatchDrawQuads(gphShatter.prim);
        if (gphShatter.unkC >= 255.0f) {
            gphShatter.active = 0;
            gphFbDivParam.blur = 0;
        }
        break;
    case 3:
        if (gphFbDivParam.system2Mode == 1) {
            p->type = 1;
            p->v[0].r = 0x80;
            p->v[0].g = 0x80;
            p->v[0].b = 0x80;
            p->v[0].a = 0x80;
            p->v[1].r = 0x80;
            p->v[1].g = 0x80;
            p->v[1].b = 0x80;
            p->v[1].a = 0x80;
            p->v[2].r = 0x80;
            p->v[2].g = 0x80;
            p->v[2].b = 0x80;
            p->v[2].a = 0x80;
            p->v[3].r = 0x80;
            p->v[3].g = 0x80;
            p->v[3].b = 0x80;
            p->v[3].a = 0x80;
            p->v[0].u = 0;
            p->v[0].v = 0;
            p->v[1].u = 0;
            p->v[1].v = 0x1C0;
            p->v[2].u = 0x280;
            p->v[2].v = 0;
            p->v[3].u = 0x280;
            p->v[3].v = 0x1C0;
            p->v[0].x = 0;
            p->v[0].y = 0;
            p->v[1].x = 0;
            p->v[1].y = 0x1C0;
            p->v[2].x = 0x280;
            p->v[2].y = 0;
            p->v[3].x = 0x280;
            p->v[3].y = 0x1C0;
            p->z = 0;
            p[1].type = 0;
            GphBatchDrawQuads(gphShatter.prim);
        }
        if (gphShatter.timer != 0) {
            gphShatter.timer--;
        } else {
            if (gphFbDivParam.system2Mode != 0) {
                gphFbDivParam.ripplePhase += 1.2566369f;
                if (gphFbDivParam.ripplePhase > 28.274328f) {
                    gphFbDivParam.ripplePhase = 0.0f;
                    gphFbDivParam.system2Mode = 0;
                }
                GphFbRipple(gphFbDivParam.ripplePhase, 320.0f, 224.0f, 100.0f, 2.0f);
            }
            if (gphFbDivParam.system2Mode != 1) {
                update_objs(e2);
                gphShatter.color[3] -= 1.0f;
                if (gphShatter.color[3] < 0.0f) {
                    gphShatter.color[3] = 0.0f;
                }
            }
        }
        break;
    }
}
