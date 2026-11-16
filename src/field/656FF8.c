/* rodata: 0x00675958-0x00675974 */
#include "common.h"
#include "protos/field.h"
#include "field/n25.h"

void GsTexUpload(GsTexPacket *);

void FldTitleDraw(void) {
    u8 ofs[5][2] = { { 2, 2 }, { 0xFE, 0xFE }, { 2, 0xFE }, { 0xFE, 2 }, { 0, 0 } };
    s32 xs[3] = { 0, 8, -20 };
    GphBatchQuad *q;
    s32 i;
    s32 j;

    if (SysDemoGetMode() < 0x1E) {
        return;
    }
    if (SysDemoGetMode() == 0x63) {
        return;
    }
    fldTitleWork->blink++;
    if (fldTitleWork->blink >= 0x78) {
        fldTitleWork->blink = 0;
    }
    GsTexUpload(&fldTitleWork->logoTex);
    GsTexPacketSend(&fldTitleWork->logoTex);
    if (SysDemoGetMode() == 0x1E) {
        q = fldTitleWork->quads;
        GphBatchSetOffset(0xB0, 0x136);
        if (fldTitleWork->blink >= 9) {
            SysDemoPromptPrims(q, 1.0f);
            GphBatchDrawQuads(fldTitleWork->quads);
        }
    }
    GphBatchSetOffset(0xA8, 0x180);
    SysDemoCaptionPrims(fldTitleWork->quads, 1.0f, 1.0f)->type = 0;
    GphBatchDrawQuads(fldTitleWork->quads);
    if (SysDemoGetMode() == 0x20) {
        GphBatchSetOffset(0x100, 0x122);
        q = fldTitleWork->quads;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 5; j++) {
                q->type = 1;
                if (fldTitleWork->cursor == i) {
                    if (j == 4) {
                        q->v[0].r = 0xFF;
                        q->v[0].g = 0xFF;
                        q->v[0].b = 0xFF;
                        q->v[0].a = 0x80;
                        q->v[1].r = 0xFF;
                        q->v[1].g = 0xFF;
                        q->v[1].b = 0xFF;
                        q->v[1].a = 0x80;
                        q->v[2].r = 0xFF;
                        q->v[2].g = 0xFF;
                        q->v[2].b = 0xFF;
                        q->v[2].a = 0x80;
                        q->v[3].r = 0xFF;
                        q->v[3].g = 0xFF;
                        q->v[3].b = 0xFF;
                        q->v[3].a = 0x80;
                    } else {
                        q->v[0].r = 0;
                        q->v[0].g = 0x6F;
                        q->v[0].b = 0xB4;
                        q->v[0].a = 0x40;
                        q->v[1].r = 0;
                        q->v[1].g = 0x6F;
                        q->v[1].b = 0xB4;
                        q->v[1].a = 0x40;
                        q->v[2].r = 0;
                        q->v[2].g = 0x6F;
                        q->v[2].b = 0xB4;
                        q->v[2].a = 0x40;
                        q->v[3].r = 0;
                        q->v[3].g = 0x6F;
                        q->v[3].b = 0xB4;
                        q->v[3].a = 0x40;
                    }
                } else {
                    if (j == 4) {
                        q->v[0].r = 0x40;
                        q->v[0].g = 0x40;
                        q->v[0].b = 0x40;
                        q->v[0].a = 0x80;
                        q->v[1].r = 0x40;
                        q->v[1].g = 0x40;
                        q->v[1].b = 0x40;
                        q->v[1].a = 0x80;
                        q->v[2].r = 0x40;
                        q->v[2].g = 0x40;
                        q->v[2].b = 0x40;
                        q->v[2].a = 0x80;
                        q->v[3].r = 0x40;
                        q->v[3].g = 0x40;
                        q->v[3].b = 0x40;
                        q->v[3].a = 0x80;
                    } else {
                        q->v[0].r = 0;
                        q->v[0].g = 0;
                        q->v[0].b = 0;
                        q->v[0].a = 0x40;
                        q->v[1].r = 0;
                        q->v[1].g = 0;
                        q->v[1].b = 0;
                        q->v[1].a = 0x40;
                        q->v[2].r = 0;
                        q->v[2].g = 0;
                        q->v[2].b = 0;
                        q->v[2].a = 0x40;
                        q->v[3].r = 0;
                        q->v[3].g = 0;
                        q->v[3].b = 0;
                        q->v[3].a = 0x40;
                    }
                }
                q->v[0].x = xs[i] + (s8)ofs[j][0];
                q->v[0].y = (s8)ofs[j][1] + i * 32;
                q->v[1].x = xs[i] + (s8)ofs[j][0];
                q->v[1].y = (s8)ofs[j][1] + i * 32 + 0x18;
                q->v[2].x = xs[i] + (s8)ofs[j][0] + 0x80;
                q->v[2].y = (s8)ofs[j][1] + i * 32;
                q->v[3].x = xs[i] + (s8)ofs[j][0] + 0x80;
                q->v[3].y = (s8)ofs[j][1] + i * 32 + 0x18;
                q->z = 0;
                q->v[0].u = 0;
                q->v[0].v = i * 24 + 0x20;
                q->v[1].u = 0;
                q->v[1].v = i * 24 + 0x38;
                q->v[2].u = 0xB9;
                q->v[2].v = i * 24 + 0x20;
                q->v[3].u = 0xB9;
                q->v[3].v = i * 24 + 0x38;
                q++;
            }
        }
        q->type = 0;
        GphBatchDrawQuads(fldTitleWork->quads);
    }
}
