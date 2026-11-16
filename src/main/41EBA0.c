/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/main.h"
#include "gph.h"
#include "sys.h"

void GsTexUpload(void *);

void SysDemoDraw(void) {
    GphBatchQuad prim[16];
    GphBatchQuad *p;
    f32 t;

    if (sysDemo.mode == 0) {
        return;
    }
    GphBatchSetOffset(0, 0);
    GphFadeDraw(&sysDemo.fade);
    GphFadeDraw(&sysDemo.fade2);
    if (sysDemo.mode == 0x63 || sysDemo.mode == 0x1C || sysDemo.mode == 0x1D) {
        return;
    }
    if (sysDemo.mode == 0xE) {
        GsTexUpload(sysDemo.texPacket);
        GsTexPacketSend(sysDemo.texPacket);
        p = prim;
        switch (sysDemo.step) {
        case 0:
            if ((s8)(sysDemo.unk53[0] += 4) < 0) {
                sysDemo.step++;
            }
            break;
        case 1:
            if ((s8)(sysDemo.unk53[1] += 4) < 0) {
                sysDemo.step++;
            }
            break;
        case 2:
            if ((s8)(sysDemo.unk53[2] += 2) < 0) {
                sysDemo.step++;
            }
            break;
        case 3:
            if ((s8)(sysDemo.unk53[3] += 2) < 0) {
                sysDemo.step++;
            }
            break;
        case 4:
            if ((sysDemo.unk53[0] -= 8) == 0) {
                sysDemo.step++;
            }
            break;
        case 5:
            if ((sysDemo.unk53[1] -= 8) == 0) {
                sysDemo.step++;
            }
            break;
        case 6:
            if ((sysDemo.unk53[2] -= 4) == 0) {
                sysDemo.step++;
            }
            break;
        case 7:
            if ((sysDemo.unk53[3] -= 4) == 0) {
                sysDemo.step++;
            }
            break;
        }
        GphBatchSetOffset(0xC0, 0xD0);
        p->type = 1;
        p->v[0].r = 0x80;
        p->v[0].g = 0x80;
        p->v[0].b = 0x80;
        p->v[0].a = sysDemo.unk53[0];
        p->v[1].r = 0x80;
        p->v[1].g = 0x80;
        p->v[1].b = 0x80;
        p->v[1].a = sysDemo.unk53[1];
        p->v[2].r = 0x80;
        p->v[2].g = 0x80;
        p->v[2].b = 0x80;
        p->v[2].a = sysDemo.unk53[0];
        p->v[3].r = 0x80;
        p->v[3].g = 0x80;
        p->v[3].b = 0x80;
        p->v[3].a = sysDemo.unk53[1];
        p->v[0].x = 0;
        p->v[0].y = 0;
        p->v[1].x = 0;
        p->v[1].y = 0x20;
        p->v[2].x = 0x100;
        p->v[2].y = 0;
        p->v[3].x = 0x100;
        p->v[3].y = 0x20;
        p->z = 0;
        p->v[0].u = 0;
        p->v[0].v = 0;
        p->v[1].u = 0;
        p->v[1].v = 0x20;
        p[1].type = 0;
        p->v[2].u = 0x100;
        p->v[2].v = 0;
        p->v[3].v = 0x20;
        p->v[3].u = 0x100;
        GphBatchDrawQuads(prim);
        return;
    }
    if (sysDemo.mode >= 2) {
        sysDemo.idleCount++;
        if (sysDemo.mode < 0x19 && sysDemo.idleCount >= 500) {
            GphTexBind(0xE7);
            p = prim;
            if (sysDemo.mode >= 6) {
                sysDemo.idleCount = 2000;
            }
            if (sysDemo.idleCount < 3000) {
                if (sysDemo.unk50 != 0x80) {
                    sysDemo.unk50++;
                }
                if (sysDemo.unk50 >= 0x40 && sysDemo.unk51 != 0x80) {
                    sysDemo.unk51++;
                }
            } else {
                if (sysDemo.unk50 != 0) {
                    sysDemo.unk50--;
                }
                if (sysDemo.unk50 < 0x40 && sysDemo.unk51 != 0) {
                    sysDemo.unk51--;
                }
            }
            GphBatchSetOffset(0x1E0, 0x20);
            p->type = 1;
            p->v[0].r = 0x80;
            p->v[0].g = 0x80;
            p->v[0].b = 0x80;
            p->v[0].a = sysDemo.unk50;
            p->v[1].r = 0x80;
            p->v[1].g = 0x80;
            p->v[1].b = 0x80;
            p->v[1].a = sysDemo.unk50;
            p->v[2].r = 0x80;
            p->v[2].g = 0x80;
            p->v[2].b = 0x80;
            p->v[2].a = sysDemo.unk51;
            p->v[3].r = 0x80;
            p->v[3].g = 0x80;
            p->v[3].b = 0x80;
            p->v[3].a = sysDemo.unk51;
            p->v[0].x = 0;
            p->v[0].y = 0;
            p->v[1].x = 0;
            p->v[1].y = 0x50;
            p->v[2].x = 0x80;
            p->v[2].y = 0;
            p->v[3].x = 0x80;
            p->v[3].y = 0x50;
            p->z = 0;
            p->v[0].u = 0;
            p->v[0].v = 0;
            p->v[1].u = 0;
            p->v[1].v = 0x50;
            p[1].type = 0;
            p->v[2].u = 0x80;
            p->v[2].v = 0;
            p->v[3].v = 0x50;
            p->v[3].u = 0x80;
            GphBatchDrawQuads(prim);
        }
        if (++sysDemo.blinkTimer >= 120) {
            sysDemo.blinkTimer = 0;
        }
        if (sysDemo.unk58 >= 180) {
            t = sysDemo.promptAlpha + 0.01f;
            sysDemo.promptAlpha = t;
            if (t >= 1.0f) {
                sysDemo.promptAlpha = 1.0f;
            }
            if (sysDemo.promptAlpha == 1.0f) {
                sysDemo.captionAlpha0 += 0.01f;
                if (sysDemo.captionAlpha0 >= 1.0f) {
                    sysDemo.captionAlpha0 = 1.0f;
                }
                if (sysDemo.captionAlpha0 == 1.0f) {
                    sysDemo.captionAlpha1 += 0.01f;
                    if (sysDemo.captionAlpha1 >= 1.0f) {
                        sysDemo.captionAlpha1 = 1.0f;
                    }
                }
            }
        } else {
            sysDemo.unk58++;
        }
        if (sysDemo.mode < 0x1E) {
            GsTexUpload(sysDemo.texPacket);
            GsTexPacketSend(sysDemo.texPacket);
            GphBatchSetOffset(0xB0, 0x136);
            if (sysDemo.blinkTimer >= 9) {
                SysDemoPromptPrims(prim, sysDemo.promptAlpha);
                GphBatchDrawQuads(prim);
            }
            GphBatchSetOffset(0xA8, 0x180);
            SysDemoCaptionPrims(prim, sysDemo.captionAlpha0, sysDemo.captionAlpha1)->type = 0;
            GphBatchDrawQuads(prim);
        }
    }
    GphBatchSetOffset(0, 0);
}
