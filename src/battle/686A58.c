/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/n28.h"

f32 VuSinAngle(s32);
void VuSinCosAngle(f32 *, f32 *, s32);

void BtlResultMembersUpdate(BtlResult *b) {
    f32 sc[2];
    BtlResultMember *p;
    s32 i;
    s32 j;
    s32 k;
    s32 k1;
    s32 k2;
    s32 k3;
    s32 m;
    s32 t;
    s32 total;
    PtyGrowth *et;
    s32 n;
    f32 y;
    f32 off;
    f32 f;
    BtlResultFx *q;
    BtlResultCast *c;
    BtlResultFxPart *pt2;
    BtlResultFxPart *pt3;
    BtlResultFxPart *pt4;
    BtlResultFxPart *pt5;
    BtlResultGain *sk;
    PtySkillDef *tt;

    p = b->members;
    for (i = 0; i < 4; i++, p++) {
        if (p->active == 0) {
            continue;
        }
        switch (b->state) {
        case 0x10:
            y = i * -22 + 244;
            t = b->timer - i * 3;
            p->x = y;
            if (i & 1) {
                off = 640.0f;
            } else {
                off = -640.0f;
            }
            if (t <= 0) {
                p->x = y + off;
            } else if (t < 24) {
                f = VuSinAngle((24 - t) * 0x2AA);
                p->x += f * f * off;
            }
            break;
        case 0x20:
        case 0x31:
            p->expShown += p->expStep;
            if (p->expShown >= p->expTarget) {
                p->expShown = p->expTarget;
            }
            et = &ptyGrowthTable[(btlUnits + i)->params->id - 0x1001];
            total = p->exp + p->expShown;
            while (p->level < 250 && total >= et->exp[p->level]) {
                BtlResultLevelUpPop(p);
                p->level++;
            }
            p->nextExp = (p->level < 250) ? et->exp[p->level] - total : 0;
            break;
        case 0x51:
        case 0x61:
            for (k = 0; k < 4; k++) {
                p->elemShown[k] += p->elemStep;
                if (p->elemShown[k] >= p->elemTarget[k]) {
                    p->elemShown[k] = p->elemTarget[k];
                }
            }
            break;
        }

        if (p->cast.state >= 0) {
            c = &p->cast;
            switch (p->cast.state) {
            case 0:
                if (c->frame != btlFrameCount) {
                    TxtGlyphBufReset(&p->glyphBuf);
                    TxtObjSetString(&p->skillText, &p->glyphBuf, ptySkillTable[c->id].name);
                    SndSePlay3D(0x7DB, 0, 0, NULL);
                    c->timer = 0;
                    p->cast.state++;
                }
                break;
            case 1:
                if (c->timer >= 0x40) {
                    p->cast.state = -1;
                }
                c->timer++;
                break;
            }
        } else if (p->gainIndex >= 0 && b->state > 0x50 && b->state < 0xF0) {
            sk = &p->gains[p->gainIndex];
            tt = &ptySkillTable[sk->id];
            if (p->elemShown[tt->category - 1] >= sk->value) {
                p->cast.id = sk->id;
                p->cast.state = 0;
                p->cast.frame = btlFrameCount;
                if (sk[1].value <= 0) {
                    p->gainIndex = -1;
                } else {
                    p->gainIndex++;
                }
            }
        }

        if (p->fx[0].state >= 0) {
            q = &p->fx[0];
            switch (q->state) {
            case 0:
                if (q->timer < 0) {
                    break;
                }
                pt4 = p->fx[0].parts;
                for (k1 = 0; k1 < 32; k1++, pt4++) {
                    pt4->rot = 0x2000;
                }
                q->state++;
            case 1:
                if (q->timer < 32) {
                    pt2 = p->fx[0].parts;
                    for (j = 0; j < 32; j++, pt2++) {
                        f = 320 - q->timer * 10;
                        if (j >= 16) {
                            f *= 1.6f;
                            VuSinCosAngle(&sc[0], &sc[1], (j << 12) - q->timer * 0x700);
                        } else {
                            VuSinCosAngle(&sc[0], &sc[1], (j << 12) + q->timer * 0x700);
                        }
                        pt2->x = q->x + sc[0] * f;
                        pt2->y = q->y + sc[1] * f;
                    }
                } else {
                    pt2 = p->fx[0].parts;
                    for (j = 0; j < 32; j++, pt2++) {
                        VuSinCosAngle(&pt2->vx, &pt2->vy, ((rand() >> 15) & 0x7FFF) + 0x4000);
                        pt2->vx *= (rand() >> 15) * (1.0f / 65536.0f) * 12.0f;
                        pt2->vy *= (rand() >> 15) * (1.0f / 65536.0f) * 15.0f - 3.0f;
                        pt2->rot = rand() >> 15;
                        pt2->rotVel = ((rand() >> 15) & 0x3FFF) - 0x2000;
                    }
                    p->unk600 = q->id;
                    p->unk602 = q->x;
                    p->unk604 = q->y;
                    p->expStep = BtlResultExpStep(p->expBonus);
                    p->expTarget += p->expBonus;
                    SndSePlay3D(0x806, 0, 0, NULL);
                    if (b->state == 0x31) {
                        BtlResultCountSe(1);
                    }
                    q->state++;
                }
                break;
            case 2:
                pt3 = p->fx[0].parts;
                n = 32;
                for (m = 0; m < 32; m++, pt3++) {
                    if (pt3->y >= 512.0f) {
                        n--;
                    } else {
                        pt3->rot += pt3->rotVel;
                        pt3->x += pt3->vx;
                        pt3->y += pt3->vy;
                        pt3->vx *= 0.99f;
                        pt3->vy += 0.4f;
                    }
                }
                if (n <= 0) {
                    q->state = -1;
                }
                break;
            }
            q->timer++;
        }

        if (p->fx[1].state >= 0) {
            q = &p->fx[1];
            switch (q->state) {
            case 0:
                if (q->timer < 0) {
                    break;
                }
                pt4 = p->fx[1].parts;
                for (k2 = 0; k2 < 32; k2++, pt4++) {
                    pt4->y = 512.0f;
                }
                q->state++;
            case 1:
                if (q->timer < 32) {
                    pt5 = &q->parts[q->timer];
                    q->x += 8.0f;
                    pt5->x = q->x + ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 8.0f;
                    pt5->y = q->y + ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 8.0f;
                    pt5->vx = ((rand() >> 15) * (1.0f / 65536.0f) - 0.5f) * 3.0f;
                    pt5->vy = (rand() >> 15) * (1.0f / 65536.0f) * -6.0f - 2.0f;
                    pt5->rot = rand() >> 15;
                    pt5->rotVel = ((rand() >> 15) & 0x3FFF) - 0x2000;
                } else if (q->timer == 32) {
                    p->unk600 = q->id;
                    p->unk602 = q->x;
                    p->elemTarget[0] += p->elemBonus[0];
                    p->unk604 = q->y;
                    p->elemTarget[1] += p->elemBonus[1];
                    p->elemTarget[2] += p->elemBonus[2];
                    p->elemTarget[3] += p->elemBonus[3];
                    SndSePlay3D(0x807, 0, 0, NULL);
                    if (b->state == 0x61) {
                        BtlResultCountSe(1);
                    }
                    q->state++;
                }
            case 2:
                pt3 = p->fx[1].parts;
                n = 32;
                for (m = 0; m < 32; m++, pt3++) {
                    if (pt3->y >= 512.0f) {
                        n--;
                    } else {
                        pt3->rot += pt3->rotVel;
                        pt3->x += pt3->vx;
                        pt3->y += pt3->vy;
                        pt3->vx *= 0.99f;
                        pt3->vy += 0.4f;
                    }
                }
                if (n <= 0 && q->state >= 2) {
                    q->state = -1;
                }
                break;
            }
            q->timer++;
        }

        /* the original reuses one pointer variable here; a separate BtlResultPop * changes the copy order in fx2 */
        pt4 = (BtlResultFxPart *)p->pops;
        for (k3 = 0; k3 < 8; k3++) {
            if (((BtlResultPop *)pt4)[k3].t > 0.0f) {
                ((BtlResultPop *)pt4)[k3].t -= 0.03125f;
                p->pops[k3].y -= 1.0f;
            }
        }
    }
}
