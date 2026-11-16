/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.96-hazard */
#include "common.h"
#include "protos/battle.h"
#include "party.h"
#include "battle/unit.h"

extern s32 btlPhase;
extern f32 btlInvPartyAvgSpeed;
extern s32 btlTickCount;
extern s8 btlPartyAliveCount;
extern s32 btlActingUnit;
extern s32 btlDbgIpStopMask;
extern FVECTOR btlVecOrigin;
extern FVECTOR btlCamParamCmrPos;


static inline void ApplyMatrix(f32 *out, f32 *m, f32 *in)
{
    __asm__ volatile(
        "lqc2 $vf1, 0(%2)\n\t"
        "lqc2 $vf4, 0x0(%1)\n\t"
        "lqc2 $vf5, 0x10(%1)\n\t"
        "lqc2 $vf6, 0x20(%1)\n\t"
        "lqc2 $vf7, 0x30(%1)\n\t"
        "vmulax.xyzw ACC, $vf4, $vf1x\n\t"
        "vmadday.xyzw ACC, $vf5, $vf1y\n\t"
        "vmaddaz.xyzw ACC, $vf6, $vf1z\n\t"
        "vmaddw.xyzw $vf1, $vf7, $vf1w\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(out), "r"(m), "r"(in) : "memory");
}

static inline void SubVector(f32 *out, f32 *a, f32 *b)
{
    __asm__ volatile(
        "lqc2 $vf1, 0(%1)\n\t"
        "lqc2 $vf2, 0(%2)\n\t"
        "vsub.xyzw $vf1, $vf1, $vf2\n\t"
        "sqc2 $vf1, 0(%0)"
        : : "r"(out), "r"(a), "r"(b) : "memory");
}

#define ABS(x) ((x) < 0 ? -(x) : (x))

void BtlUnitUpdate(BtlUnit *u)
{
    FVECTOR v;
    FVECTOR d;
    s32 pitch;
    s32 yaw;
    PtyUnit *st;
    f32 sc;
    f32 r;
    s32 ret;
    BtlChrData *p;
    s32 i;
    GphPart *n;
    GphPart *m;
    BtlUnit *o;

    if (btlPhase == 0x80 && btlActingUnit == 0xFF) {
        st = u->params;
        if ((st->status & 0x20E0) || ((btlDbgIpStopMask >> u->id) & 1)) {
            u->ipStop = 1;
        }
        if (u->ipStop > 0) {
            u->ipStop--;
        } else if (u->ipDamage > 0.0f) {
            u->ipDamage -= (f32)u->params->params[4] * btlInvPartyAvgSpeed / ((f32)btlPartyAliveCount * 60.0f + 240.0f);
            if (u->ipDamage < 0.0f) {
                u->ipDamage = 0.0f;
            }
        } else {
            if (u->ipMode == 2) {
                if (u->flags & 0x80) {
                    u->ipPos = 1.0f;
                } else {
                    u->ipPos -= u->ipSpeed;
                }
            } else {
                sc = 1.0f;
                if (st->status & 0x100) {
                    sc = 1.5f;
                }
                if (u->powUp & 8) {
                    sc -= 0.5f;
                } else if (u->powUp & 0x800) {
                    sc += 0.5f;
                }
                u->ipPos -= u->ipSpeed * sc;
            }
            if (u->ipPos <= 0.0f) {
                u->ipPos = 0.0f;
                u->ipRate = 1.0f;
            }
        }
        if (st->status & 0x4000) {
            if (--u->vanish <= 0) {
                st->status &= 0xFFFFBFFF;
                if (u->id < 4) {
                    PtyUnitCalcParams(st, NULL, NULL);
                } else {
                    PtyUnitCalcParams(st, u->def->data, &ptyBattleParams[u->id - 4]);
                }
                BtlHitQueueAddStatus(u, st->status2, 0, 0, 0x5DC);
            }
        } else if (st->status & 0x2000) {
            if (!(u->flags & 0x400)) {
                if (--u->exeBad <= 0) {
                    u->exeBad = st->exeBad / 2;
                    BtlHitQueueAddDamage(u, (f32)st->maxHp * 0.05f, 0.0f, u->rot[1], 0x6800, 0);
                }
            }
            if (--u->bind <= 0) {
                st->status &= 0xFFFFDFFF;
            }
        } else {
            if (st->status & 0xE0) {
                if (--u->sleep <= 0) {
                    st->status &= 0xFFFFFF1F;
                }
            }
            if (!(st->status & 0x80)) {
                if (st->status & 1) {
                    if (--u->transp <= 0) {
                        st->status &= 0xFFFFFFFE;
                    }
                }
                if ((st->status & 2) && !(st->status2 & 2)) {
                    if (--u->antiMagic <= 0) {
                        st->status &= 0xFFFFFFFD;
                    }
                }
                if (st->status & 8) {
                    if (--u->exeGood <= 0) {
                        u->exeGood = st->exeGood;
                        BtlHitQueueAddDamage(u, (f32)st->maxHp * 0.05f, 0.0f, u->rot[1], 0x100, 0);
                    }
                    if (!(st->status2 & 8)) {
                        if (--u->regene <= 0) {
                            st->status &= 0xFFFFFFF7;
                        }
                    }
                }
                if ((st->status & 0x810) && (u->state >= 0x80 || (st->status & 0x60))) {
                    if (--u->exeBad <= 0) {
                        u->exeBad = st->exeBad;
                        if (st->status & 0x800) {
                            r = (f32)(rand() >> 15) * (1.0f / 65536.0f);
                            ret = 0;
                            if (r < 0.05f) {
                                ret = BtlHitQueueAddStatus(u, 0, 1, 0, 0x5DC);
                            } else if (r < 0.1f) {
                                ret = BtlHitQueueAddStatus(u, 0, 2, 0, 0x5DC);
                            } else if (r < 0.15f) {
                                ret = BtlHitQueueAddStatus(u, 0, 4, 0, 0x5DC);
                            } else if (r < 0.2f) {
                                ret = BtlHitQueueAddStatus(u, 0, 8, 0, 0x5DC);
                            } else if (r < 0.25f) {
                                ret = BtlHitQueueAddStatus(u, 0, 0x10, 0, 0x5DC);
                            } else if (r < 0.3f) {
                                ret = BtlHitQueueAddStatus(u, 0x20, 0, 0, 0x5DC);
                            } else if (r < 0.35f) {
                                ret = BtlHitQueueAddStatus(u, 0x40, 0, 0, 0x5DC);
                            } else if (r < 0.4f) {
                                ret = BtlHitQueueAddStatus(u, 0x80, 0, 0, 0x5DC);
                            } else if (r < 0.45f) {
                                ret = BtlHitQueueAddStatus(u, 0x100, 0, 0, 0x5DC);
                            } else if (r < 0.5f) {
                                ret = BtlHitQueueAddStatus(u, 0x200, 0, 0, 0x5DC);
                            } else if (r < 0.55f) {
                                ret = BtlHitQueueAddStatus(u, 0x400, 0, 0, 0x5DC);
                            } else if (r < 0.6f) {
                                ret = BtlHitQueueAddStatus(u, 0x1000, 0, 0, 0x5DC);
                            }
                            if (ret == 0) {
                                BtlHitQueueAddDamage(u, (f32)st->maxHp * 0.1f, 0.0f, u->rot[1],
                                              (st->status & 0xE0) ? 0xE800 : 0x6800, 0);
                            } else {
                                BtlHitQueueAddDamage(u, 0, 0.0f, u->rot[1],
                                              (st->status & 0xE0) ? 0xF800 : 0x7800, 0);
                            }
                            PtclBurstSpawnAtUnit(0xC, u, u->rot[1]);
                        } else {
                            BtlHitQueueAddDamage(u, (f32)st->maxHp * 0.05f, 0.0f, u->rot[1],
                                          (st->status & 0xE0) ? 0xE800 : 0x6800, 0);
                            PtclBurstSpawnAtUnit(0xB, u, u->rot[1]);
                        }
                    }
                }
                if ((st->status & 0x100) && !(st->status2 & 0x100)) {
                    if (--u->confu <= 0) {
                        st->status &= 0xFFFFFEFF;
                    }
                }
                if ((st->status & 0x200) && !(st->status2 & 0x200)) {
                    if (--u->sealS <= 0) {
                        st->status &= 0xFFFFFDFF;
                    }
                }
                if ((st->status & 0x400) && !(st->status2 & 0x400)) {
                    if (--u->sealM <= 0) {
                        st->status &= 0xFFFFFBFF;
                    }
                }
                if (u->id < 4 && !(st->status & 0x860) && st->exeSp > 0) {
                    if (--u->exeSp <= 0) {
                        u->exeSp = st->exeSp;
                        BtlHitQueueAddDamage(u, 1, 0.0f, u->rot[1], 0x7980, 0);
                    }
                }
                if (u->powUp & 0x101) {
                    if (--u->statMod[0] <= 0) {
                        u->powUp &= 0xFEFE;
                    }
                }
                if (u->powUp & 0x202) {
                    if (--u->statMod[1] <= 0) {
                        u->powUp &= 0xFDFD;
                    }
                }
                if (u->powUp & 0x404) {
                    if (--u->statMod[2] <= 0) {
                        u->powUp &= 0xFBFB;
                    }
                }
                if (u->powUp & 0x808) {
                    if (--u->statMod[3] <= 0) {
                        u->powUp &= 0xF7F7;
                    }
                }
                if (u->powUp & 0x1010) {
                    if (--u->statMod[4] <= 0) {
                        u->powUp &= 0xEFEF;
                    }
                }
                if (u->id >= 4) {
                    if (--u->exp <= 0) {
                        u->exp = st->exeSp;
                        p = u->def->data;
                        for (i = 0; i < 2; i++) {
                            if (p->expSklKind[i] != 0) {
                                st->expSkl[i] -= (s8)p->gaugeDrain;
                                if (st->expSkl[i] < p->gaugeMin[i]) {
                                    st->expSkl[i] = p->gaugeMin[i];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    IpIconUpdate(u);
    BtlChrModelInvalidate(u->id);
    if (btlActingUnit == 0xFF || (u->flags & 8)) {
        if (u->hookPre != NULL) {
            u->hookPre(u);
        }
        if (u->parts[0] >= 0) {
            BtlUnitPartSyncAction(u);
        }
        if (u->motion != 0) {
            u->frame += u->frameSpeed;
            if (u->frame > u->loopEnd) {
                u->frame = u->loopStart;
                u->evtCur = u->evtLoop;
                if (u->loopCount < 0) {
                    u->evtLoop = NULL;
                    u->loopStart = u->loopEnd = u->motionLast;
                }
            }
            BtlUnitMotionEvents(u, 0);
        }
        u->stateHandler(u);
        u->stateCount++;
        if (u->hookPost != NULL) {
            u->hookPost(u);
        }
        if (u->model != NULL && !(u->params->status & 0x80)) {
            n = ChrModelFindPart(u->model, 0x280);
            if (n != NULL) {
                o = &btlUnits[u->target];
                if (u->headLook & 4) {
                    ApplyMatrix(v, n->mtx, btlVecOrigin);
                    SubVector(d, btlCamParamCmrPos, v);
                    BtlVecGetPitchYaw(&pitch, &yaw, d, 0);
                    if (u->params->id == 0x1007 && u->state == 0x83) {
                        pitch = (pitch + 0x2000) & 0xFFFF;
                    } else {
                        pitch = (pitch + 0x4000) & 0xFFFF;
                    }
                    yaw = (u->rot[1] - yaw) & 0xFFFF;
                    if (pitch > 0x8000) {
                        pitch -= 0x10000;
                    }
                    if (yaw > 0x8000) {
                        yaw -= 0x10000;
                    }
                } else if (u->target == u->id || o->body == u->id
                           || (*(u64 *)&u->loopCount & 0x3000000) != 0x1000000) { /* headLook & 3 read with a doubleword load (ld), as the original does */
                    yaw = 0;
                    pitch = 0;
                } else {
                    ApplyMatrix(v, n->mtx, btlVecOrigin);
                    if (o->model != NULL) {
                        m = ChrModelFindPart(o->model, 0x280);
                        if (m != NULL) {
                            ApplyMatrix(d, m->mtx, btlVecOrigin);
                        } else {
                            BtlChrGetPartPos(d, o, 1);
                            d[1] = (d[1] + o->params->colH + o->pos[1]) * 0.5f;
                        }
                    } else {
                        BtlChrGetPartPos(d, o, 1);
                    }
                    SubVector(d, d, v);
                    BtlVecGetPitchYaw(&pitch, &yaw, d, 0);
                    pitch = (pitch + 0x4000) & 0xFFFF;
                    yaw = (u->rot[1] - yaw) & 0xFFFF;
                    if (pitch > 0x8000) {
                        pitch -= 0x10000;
                    }
                    if (yaw > 0x8000) {
                        yaw -= 0x10000;
                    }
                }
                n->rot[0] &= 0xFFFF;
                n->rot[1] &= 0xFFFF;
                if (n->rot[0] > 0x8000) {
                    n->rot[0] -= 0x10000;
                }
                if (n->rot[1] > 0x8000) {
                    n->rot[1] -= 0x10000;
                }
                if (ABS(n->rot[0] - pitch) <= 0x180) {
                    n->rot[0] = pitch;
                } else if (n->rot[0] < pitch) {
                    n->rot[0] += 0x180;
                } else {
                    n->rot[0] -= 0x180;
                }
                if (ABS(n->rot[1] - yaw) <= 0x380) {
                    n->rot[1] = yaw;
                } else if (n->rot[1] < yaw) {
                    n->rot[1] += 0x380;
                } else {
                    n->rot[1] -= 0x380;
                }
                if (u->params->id == 0x1007 && u->state == 0x83) {
                    if (n->rot[0] > 0) {
                        n->rot[0] = 0;
                    } else if (n->rot[0] < -0x3000) {
                        n->rot[0] = -0x3000;
                    }
                } else {
                    if (n->rot[0] > 0x2000) {
                        n->rot[0] = 0x2000;
                    } else if (n->rot[0] < -0x2000) {
                        n->rot[0] = -0x2000;
                    }
                }
                if (u->params->id == 0x1007 && u->motionNo == 0) {
                    if (n->rot[1] > 0x800) {
                        n->rot[1] = 0x800;
                    } else if (n->rot[1] < -0x4800) {
                        n->rot[1] = -0x4800;
                    }
                } else {
                    if (n->rot[1] > 0x2800) {
                        n->rot[1] = 0x2800;
                    } else if (n->rot[1] < -0x2800) {
                        n->rot[1] = -0x2800;
                    }
                }
            }
        }
    }
    if (u->hookDraw != NULL) {
        u->hookDraw(u);
    }
    if (u->alphaStep != 0.0f) {
        u->alpha += u->alphaStep;
        if (u->alpha >= 1.0f) {
            u->alpha = 1.0f;
            u->alphaStep = 0.0f;
        } else if (u->alpha <= 0.0f) {
            u->alpha = 0.0f;
            u->alphaStep = 0.0f;
        }
    }
    if (u->model != NULL) {
        ChrFaceAnimUpdate(&u->face, u->model);
        ChrModelUvScrollUpdate(u->model, 1.0f);
        BtlChrModelUpdate(u);
    }
    if (u->alpha > 0.0f && u->params->angryProb >= 3.0f && !(btlTickCount & 7)
        && !(u->flags & 2) && u->pos[1] <= 0.0f) {
        BtlChrGetPartPos(v, u, 1);
        PtclBtlDustSpawn(0x81, NULL, v[0], v[2]);
    }
}
