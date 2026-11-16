/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "ptcl.h"
#include "protos/main.h"

void VuVecCopy(void *, void *);
void VuVecNormalize(f32 *, f32 *);

#define RANDF() (((f32)rand() - (f32)rand()) * 2.3283064e-10f)

void PtclEmitterSpawn(PtclEmitter *em, s32 n) {
    f32 v[4];
    u8 i;
    PtclEmitterParticle *parts;
    u8 k;
    PtclEmitterVert *q;
    s32 j;
    FVECTOR *pp;

    i = n;
    pp = (FVECTOR *)em->verts;
    parts = em->parts;
    parts[i].active = 1;
    parts[i].life = em->life + em->life * RANDF() * 0.5f;
    parts[i].delay = em->spawnTime + (em->interval + em->interval * RANDF() * 0.5f);
    parts[i].lifeMax = parts[i].life;
    switch (em->alphaMode) {
    case 0:
    default:
        parts[i].alpha = 127.0f;
        parts[i].alphaStep = 127.0f / parts[i].life;
        break;
    case 1:
        parts[i].alpha = 0.0f;
        parts[i].alphaStep = 255.0f / parts[i].life;
        break;
    case 2:
        parts[i].alpha = 127.0f;
        parts[i].alphaStep = 127.0f / em->fadeFrames;
        break;
    case 3:
        parts[i].alpha = 127.0f;
        parts[i].life = 999.0f;
        parts[i].alphaStep = 0.0f;
        parts[i].delay = 0.0f;
        parts[i].lifeMax = 0.0f;
        break;
    }
    k = n;
    VuVecCopy(&pp[k * 4 + 1], em->sizeStart);
    switch (em->sizeRandMode) {
    case 1:
        v[0] = em->sizeRandX * RANDF();
        v[1] = em->sizeRandY * RANDF();
        if (v[0] < 0.0f) {
            v[0] = -v[0];
        }
        if (v[1] < 0.0f) {
            v[1] = -v[1];
        }
        q = (PtclEmitterVert *)((k << 6) + (s32)pp);
        q->size[0] += v[0];
        q->size[1] += v[1];
        break;
    case 2:
        v[0] = em->sizeRandX * RANDF();
        if (v[0] < 0.0f) {
            v[0] = -v[0];
        }
        q = (PtclEmitterVert *)((k << 6) + (s32)pp);
        q->size[0] += v[0];
        q->size[1] += v[0];
        break;
    }
    parts[k].sizeStepX = (em->sizeEnd[0] - em->sizeStart[0]) / parts[k].life;
    parts[k].sizeStepY = (em->sizeEnd[1] - em->sizeStart[1]) / parts[k].life;
    v[0] = RANDF();
    v[1] = RANDF();
    v[2] = RANDF();
    v[3] = 1.0f;
    VuVecNormalize(v, v);
    v[0] *= em->spawnRadius;
    if (em->spawnFlags & 1) {
        v[1] = 0.0f;
    } else {
        v[1] *= em->spawnRadius;
    }
    v[2] *= em->spawnRadius;
    VuVecCopy(&pp[k * 4], v);
    v[0] = em->dir[0];
    v[1] = em->dir[1];
    v[2] = em->dir[2];
    j = 0;
    do {
        /* do-while(0) around the switch: loop weight (allocation) */
        do {
            switch (em->dirMode) {
            case 1:
                if (em->dir[j] < 0.0f) {
                    if (v[j] > 0.0f) {
                        v[j] = -v[j];
                    }
                } else if (v[j] < 0.0f) {
                    v[j] = -v[j];
                }
                break;
            case 2:
                if (em->dirAxis[j] != 0) {
                    if (em->dir[j] < 0.0f) {
                        if (v[j] > 0.0f) {
                            v[j] = -v[j];
                        }
                    } else if (v[j] < 0.0f) {
                        v[j] = -v[j];
                    }
                    break;
                }
            case 0:
                if (RANDF() < 0.0f) {
                    v[j] = -v[j];
                }
                break;
            }
        } while (0);
        if (em->spread[j] != 0.0f) {
            parts[k].pos[j] = 2.0f * ((v[j] / (1.0f / em->spread[j])) * RANDF());
        }
        parts[k].pos[j] = parts[k].pos[j] + v[j];
    } while (++j < 3);
    switch (em->motionMode) {
    default:
        parts[k].vel[0] = 0.0f;
        parts[k].vel[1] = 0.0f;
        parts[k].vel[2] = 0.0f;
        break;
    case 1:
        parts[k].vel[0] = -(parts[k].pos[0] / parts[k].life);
        parts[k].vel[1] = -(parts[k].pos[1] / parts[k].life);
        parts[k].vel[2] = -(parts[k].pos[2] / parts[k].life);
        break;
    case 2:
        parts[k].vel[0] = parts[k].pos[0] / parts[k].life;
        parts[k].vel[1] = parts[k].pos[1] / parts[k].life;
        parts[k].vel[2] = parts[k].pos[2] / parts[k].life;
        parts[k].pos[0] = 0.0f;
        parts[k].pos[1] = 0.0f;
        parts[k].pos[2] = 0.0f;
        break;
    case 3:
        parts[k].vel[0] = -(parts[k].pos[0] / parts[k].life);
        parts[k].vel[1] = -(parts[k].pos[1] / (parts[k].life * em->spread[3]));
        parts[k].vel[2] = -(parts[k].pos[2] / parts[k].life);
        break;
    case 4:
        parts[k].vel[0] = -(parts[k].pos[0] / (parts[k].life * em->spread[3]));
        parts[k].vel[1] = -(parts[k].pos[1] / (parts[k].life * em->spread[3]));
        parts[k].vel[2] = -(parts[k].pos[2] / (parts[k].life * em->spread[3]));
        break;
    case 5:
        parts[k].pos[0] = pp[k * 4][0];
        parts[k].pos[1] = pp[k * 4][1];
        parts[k].pos[2] = pp[k * 4][2];
        parts[k].rot = RANDF() * 16.0f;
        parts[k].vel[0] = 0.0f;
        parts[k].vel[1] = 0.0f;
        parts[k].vel[2] = 0.0f;
        parts[k].rotVel = RANDF() * 16.0f;
        break;
    }
}
