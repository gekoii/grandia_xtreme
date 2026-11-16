#include "common.h"
#include "protos/battle.h"
#include "battle/env.h"
#include "battle/n31.h"

f32 VuSinAngle(s32);

void EnvStageZoneDebugDraw(void) {
    GphQuad3D poly[2];
    GphQuad3D *p;
    EnvStageZone *z;
    s32 i;

    z = envStageZones;
    p = poly;
    poly[0].type = 0x11;
    poly[1].type = 0;
    for (i = 0; i < envStageZoneCount; z++) {
        if (EnvStageZoneIsVisible(z)) {
            switch (i & 3) {
            case 0:
                p->col[0].r = 0;
                p->col[0].g = 0;
                p->col[0].b = 0xF0;
                p->col[0].a = 0x20;
                p->col[1].r = 0;
                p->col[1].g = 0;
                p->col[1].b = 0xF0;
                p->col[1].a = 0x20;
                p->col[2].r = 0;
                p->col[2].g = 0;
                p->col[2].b = 0xF0;
                p->col[2].a = 0x20;
                p->col[3].r = 0;
                p->col[3].g = 0;
                p->col[3].b = 0xF0;
                p->col[3].a = 0x20;
                break;
            case 1:
                p->col[0].r = 0xF0;
                p->col[0].g = 0xF0;
                p->col[0].b = 0xF0;
                p->col[0].a = 0x20;
                p->col[1].r = 0xF0;
                p->col[1].g = 0xF0;
                p->col[1].b = 0xF0;
                p->col[1].a = 0x20;
                p->col[2].r = 0xF0;
                p->col[2].g = 0xF0;
                p->col[2].b = 0xF0;
                p->col[2].a = 0x20;
                p->col[3].r = 0xF0;
                p->col[3].g = 0xF0;
                p->col[3].b = 0xF0;
                p->col[3].a = 0x20;
                break;
            case 2:
                p->col[0].r = 0;
                p->col[0].g = 0xF0;
                p->col[0].b = 0;
                p->col[0].a = 0x20;
                p->col[1].r = 0;
                p->col[1].g = 0xF0;
                p->col[1].b = 0;
                p->col[1].a = 0x20;
                p->col[2].r = 0;
                p->col[2].g = 0xF0;
                p->col[2].b = 0;
                p->col[2].a = 0x20;
                p->col[3].r = 0;
                p->col[3].g = 0xF0;
                p->col[3].b = 0;
                p->col[3].a = 0x20;
                break;
            case 3:
                p->col[0].r = 0;
                p->col[0].g = 0;
                p->col[0].b = 0;
                p->col[0].a = 0x20;
                p->col[1].r = 0;
                p->col[1].g = 0;
                p->col[1].b = 0;
                p->col[1].a = 0x20;
                p->col[2].r = 0;
                p->col[2].g = 0;
                p->col[2].b = 0;
                p->col[2].a = 0x20;
                p->col[3].r = 0;
                p->col[3].g = 0;
                p->col[3].b = 0;
                p->col[3].a = 0x20;
                break;
            }
        } else {
            p->col[0].r = 0xF0;
            p->col[0].g = 0;
            p->col[0].b = 0;
            p->col[0].a = 0x40;
            p->col[1].r = 0xF0;
            p->col[1].g = 0;
            p->col[1].b = 0;
            p->col[1].a = 0x40;
            p->col[2].r = 0xF0;
            p->col[2].g = 0;
            p->col[2].b = 0;
            p->col[2].a = 0x40;
            p->col[3].r = 0xF0;
            p->col[3].g = 0;
            p->col[3].b = 0;
            p->col[3].a = 0x40;
        }
        i++;
        p->pos[0].y = p->pos[1].y = p->pos[3].y = 0.0f;
        p->pos[2].y = VuCosAngle(z->pitch) * 2500.0f;
        p->pos[0].x = p->pos[1].x = p->pos[0].z = p->pos[1].z = 0.0f;
        p->pos[2].x = p->pos[3].x = VuSinAngle(z->angle) * 2500.0f;
        p->pos[2].z = p->pos[3].z = VuCosAngle(z->angle) * 2500.0f;
        GphBatchDrawQuads3D(poly);
        p->pos[2].x = p->pos[3].x = VuSinAngle(z->angle + z->width) * 2500.0f;
        p->pos[2].z = p->pos[3].z = VuCosAngle(z->angle + z->width) * 2500.0f;
        GphBatchDrawQuads3D(poly);
    }
}
