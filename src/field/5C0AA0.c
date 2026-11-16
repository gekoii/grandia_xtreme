#include "common.h"
#include "protos/field.h"
#include "field/n13.h"
#include "field/n22.h"

void VuSinCosDeg(f32 *, f32 *, f32);

void MapEntranceLoad(AmapEntranceRec *src, s32 n) {
    s32 i;
    MapEntrance *d;
    f32 sc[2];
    for (i = 0; i < n; i++) {
        AmapEntranceRec *e = &src[i];
        d = MapEntranceGet(i);
        d->id = e->id;
        d->pos.x = e->pos[0];
        d->pos.y = e->pos[1];
        d->keepYaw = e->unk1A & 1;
        d->pos.z = e->pos[2];
        d->effect = e->unk18;
        d->yaw = e->rot2;
        d->effectTime = e->unk1E;
        d->unk24 = e->rot;
        d->walkTime = e->unk1C;
        VuSinCosDeg(&sc[0], &sc[1], e->rot + 180.0f);
        d->dir[0] = sc[0] * 10.0f;
        d->dir[1] = 0.0f;
        d->dir[2] = -sc[1] * 10.0f;
    }
}
