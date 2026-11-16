/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/n28.h"


void BtlDropCoinsUpdate(void *p) {
    BtlDropCoin *q;
    GphModel *m;
    s32 n;
    s32 a;
    s32 t;

    n = ((BtlDropObj *)p)->coinCount;
    q = ((BtlDropObj *)p)->coins;
    for (; n > 0; n--, q++) {
        m = p = q->model;
        if (m->pos[1] == 0.9f && q->vy == 0.0f) {
            m->rot[0] = (s32)((0x8000 - m->rot[0]) * 0.9f + 32768.0f) & 0xFFFF;
            t = (u16)q->rotVelY;
            q->rotVelY = t;
            if (t <= 0x7FFF) {
                q->rotVelY = t * 0.9f;
            } else {
                q->rotVelY = 65536.0f - (0x10000 - t) * 0.9f;
            }
            ((GphModel *)p)->rot[1] += q->rotVelY;
            continue;
        }
        q->vy -= 1.0f;
        ((GphModel *)p)->rot[0] += q->rotVelX;
        ((GphModel *)p)->pos[0] += q->vx;
        ((GphModel *)p)->rot[1] += q->rotVelY;
        ((GphModel *)p)->pos[1] += q->vy;
        ((GphModel *)p)->pos[2] += q->vz;
        if (((GphModel *)p)->pos[1] <= 0.9f) {
            p = NULL;
            if (q->bounced == 0) {
                SndSePlay3D(0x7EC, 0, 8, m->pos);
                q->bounced++;
                m = q->model;
            }
            q->vy *= -0.5f;
            q->vx *= 0.5f;
            q->vz *= 0.5f;
            if (q->vy <= 1.0f) {
                q->vy = 0.0f;
                m->pos[1] = 0.9f;
                a = (u16)m->rot[0];
                m->rot[0] = a;
                if (a < 0x4000) {
                    m->rot[0] = a + 0x8000;
                } else if (a > 0xBFFF) {
                    m->rot[0] = a - 0x8000;
                }
            } else {
                m->pos[1] = 1.8f - m->pos[1];
                a = (u16)m->rot[0];
                m->rot[0] = a;
                if (a < 0x4000) {
                    q->rotVelX = q->vy * -1024.0f;
                } else if (a > 0x7FFF) {
                    if (a <= 0xBFFF) {
                        q->rotVelX = q->vy * -1024.0f;
                    } else {
                        q->rotVelX = q->vy * 1024.0f;
                    }
                } else {
                    q->rotVelX = q->vy * 1024.0f;
                }
                t = (u16)q->rotVelY;
                q->rotVelY = t;
                if (t <= 0x7FFF) {
                    q->rotVelY = t * 0.9f;
                } else {
                    q->rotVelY = 65536.0f - (0x10000 - t) * 0.9f;
                }
            }
        }
    }
}
