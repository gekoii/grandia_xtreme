#include "common.h"
#include "protos/field.h"
#include "field/n25.h"


void MiniBallLaunch(MiniBall *cel, MiniSprite *obj, u32 n) {
    cel->state = 1;
    cel->unk2C = 0;
    cel->index = miniBallWork->game.launched;
    cel->color = n;
    cel->pos[0] = miniBallData->params->launchX + (f32)n * miniBallData->params->launchStepX;
    cel->sprite = obj;
    cel->pos[2] = miniBallData->params->launchZ;
    cel->vel[0] = 0.0f;
    cel->vel[2] = miniBallData->params->launchVelZ;
    obj->active = 1;
    obj->rect = n + 0x15;
    obj->drawFlags |= 1;
    SndSePlay3D(0x811, 0, 0, NULL);
}
