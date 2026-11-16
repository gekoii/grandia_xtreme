#include "common.h"
#include "ptcl.h"








f32 PtclTrackEval(PtclStarterSlot *, PtclTrack *, PtclGenDef **, PtclGenDef **);

void PtclCtrlTracksUpdate(void) {
    PtclGenDef *from;
    PtclGenDef *to;
    PtclStarterSlot *a;
    PtclCtrl *m;
    s32 i;
    s32 j;
    f32 r;
    s8 *pf;
    s8 *pt;
    s8 *po;

    a = ptclCurSystem->slots;
    for (i = 0; i < ptclCurSystem->slotCount; i++, a++) {
        m = a->ctrl;
        if (m == NULL || a->gen == NULL || a->time > m->length) {
            continue;
        }
        if (m->effectorStep.on != 0) {
            if (a->time <= m->effectorStep.time[0]) {
                if (a->time == m->effectorStep.time[0]) {
                    a->gen->def.effectors[0] = m->keys[1].effectors[0];
                    a->gen->def.effectors[1] = m->keys[1].effectors[1];
                    a->gen->def.effectors[2] = m->keys[1].effectors[2];
                    a->gen->def.effectors[3] = m->keys[1].effectors[3];
                }
            } else if (a->time <= m->effectorStep.time[1]) {
                if (a->time == m->effectorStep.time[1]) {
                    a->gen->def.effectors[0] = m->keys[2].effectors[0];
                    a->gen->def.effectors[1] = m->keys[2].effectors[1];
                    a->gen->def.effectors[2] = m->keys[2].effectors[2];
                    a->gen->def.effectors[3] = m->keys[2].effectors[3];
                }
            } else if (a->time == m->effectorStep.time[2]) {
                a->gen->def.effectors[0] = m->keys[3].effectors[0];
                a->gen->def.effectors[1] = m->keys[3].effectors[1];
                a->gen->def.effectors[2] = m->keys[3].effectors[2];
                a->gen->def.effectors[3] = m->keys[3].effectors[3];
            }
        }
        if (m->posX.lastKey != 0 && m->posX.time[0] < a->time && m->posX.time[m->posX.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->posX, &from, &to);
            a->gen->def.pos[0] = (to->pos[0] - from->pos[0]) * r + from->pos[0];
        }
        if (m->posY.lastKey != 0 && m->posY.time[0] < a->time && m->posY.time[m->posY.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->posY, &from, &to);
            a->gen->def.pos[1] = (to->pos[1] - from->pos[1]) * r + from->pos[1];
        }
        if (m->posZ.lastKey != 0 && m->posZ.time[0] < a->time && m->posZ.time[m->posZ.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->posZ, &from, &to);
            a->gen->def.pos[2] = (to->pos[2] - from->pos[2]) * r + from->pos[2];
        }
        if (m->rotX.lastKey != 0 && m->rotX.time[0] < a->time && m->rotX.time[m->rotX.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->rotX, &from, &to);
            a->gen->def.rot[0] = (f32)(to->rot[0] - from->rot[0]) * r + (f32)from->rot[0];
        }
        if (m->rotY.lastKey != 0 && m->rotY.time[0] < a->time && m->rotY.time[m->rotY.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->rotY, &from, &to);
            a->gen->def.rot[1] = (f32)(to->rot[1] - from->rot[1]) * r + (f32)from->rot[1];
        }
        if (m->rotZ.lastKey != 0 && m->rotZ.time[0] < a->time && m->rotZ.time[m->rotZ.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->rotZ, &from, &to);
            a->gen->def.rot[2] = (f32)(to->rot[2] - from->rot[2]) * r + (f32)from->rot[2];
        }
        if (m->color.lastKey != 0 && m->color.time[0] < a->time && m->color.time[m->color.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->color, &from, &to);
            a->gen->def.color[0] = (to->color[0] - from->color[0]) * r + from->color[0];
            a->gen->def.color[1] = (to->color[1] - from->color[1]) * r + from->color[1];
            a->gen->def.color[2] = (to->color[2] - from->color[2]) * r + from->color[2];
            a->gen->def.color[3] = (to->color[3] - from->color[3]) * r + from->color[3];
        }
        if (m->emitAngX.lastKey != 0 && m->emitAngX.time[0] < a->time && m->emitAngX.time[m->emitAngX.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->emitAngX, &from, &to);
            a->gen->def.emitAngX = (f32)(to->emitAngX - from->emitAngX) * r + (f32)from->emitAngX;
            a->gen->def.emitAngXRange = (f32)(to->emitAngXRange - from->emitAngXRange) * r + (f32)from->emitAngXRange;
        }
        if (m->emitAngY.lastKey != 0 && m->emitAngY.time[0] < a->time && m->emitAngY.time[m->emitAngY.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->emitAngY, &from, &to);
            a->gen->def.emitAngY = (f32)(to->emitAngY - from->emitAngY) * r + (f32)from->emitAngY;
            a->gen->def.emitAngYRange = (f32)(to->emitAngYRange - from->emitAngYRange) * r + (f32)from->emitAngYRange;
        }
        if (m->speed.lastKey != 0 && m->speed.time[0] < a->time && m->speed.time[m->speed.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->speed, &from, &to);
            a->gen->def.speed = (to->speed - from->speed) * r + from->speed;
            a->gen->def.speedRange = (to->speedRange - from->speedRange) * r + from->speedRange;
        }
        if (m->accel.lastKey != 0 && m->accel.time[0] < a->time && m->accel.time[m->accel.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->accel, &from, &to);
            a->gen->def.accel = (to->accel - from->accel) * r + from->accel;
            a->gen->def.accelRange = (to->accelRange - from->accelRange) * r + from->accelRange;
        }
        if (m->life.lastKey != 0 && m->life.time[0] < a->time && m->life.time[m->life.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->life, &from, &to);
            a->gen->def.life = (f32)(to->life - from->life) * r + (f32)from->life;
            a->gen->def.lifeRange = (f32)(to->lifeRange - from->lifeRange) * r + (f32)from->lifeRange;
        }
        if (m->rate.lastKey != 0 && m->rate.time[0] < a->time && m->rate.time[m->rate.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->rate, &from, &to);
            a->gen->def.rate = (to->rate - from->rate) * r + from->rate;
            a->gen->def.rateRange = (to->rateRange - from->rateRange) * r + from->rateRange;
        }
        if (m->angX.lastKey != 0 && m->angX.time[0] < a->time && m->angX.time[m->angX.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->angX, &from, &to);
            a->gen->def.angX = (f32)(to->angX - from->angX) * r + (f32)from->angX;
            a->gen->def.angXRange = (f32)(to->angXRange - from->angXRange) * r + (f32)from->angXRange;
        }
        if (m->angY.lastKey != 0 && m->angY.time[0] < a->time && m->angY.time[m->angY.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->angY, &from, &to);
            a->gen->def.angY = (f32)(to->angY - from->angY) * r + (f32)from->angY;
            a->gen->def.angYRange = (f32)(to->angYRange - from->angYRange) * r + (f32)from->angYRange;
        }
        if (m->radius.lastKey != 0 && m->radius.time[0] < a->time && m->radius.time[m->radius.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->radius, &from, &to);
            a->gen->def.radius = (to->radius - from->radius) * r + from->radius;
            a->gen->def.radiusRange = (to->radiusRange - from->radiusRange) * r + from->radiusRange;
        }
        if (m->onceNum.lastKey != 0 && m->onceNum.time[0] < a->time && m->onceNum.time[m->onceNum.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->onceNum, &from, &to);
            a->gen->def.onceNum = (f32)(to->onceNum - from->onceNum) * r + (f32)from->onceNum;
        }
        if (m->sinX.lastKey != 0 && m->sinX.time[0] < a->time && m->sinX.time[m->sinX.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->sinX, &from, &to);
            a->gen->def.sinXLen = (f32)(to->sinXLen - from->sinXLen) * r + (f32)from->sinXLen;
            a->gen->def.sinXStart = (f32)(to->sinXStart - from->sinXStart) * r + (f32)from->sinXStart;
            a->gen->def.sinXRate = (to->sinXRate - from->sinXRate) * r + from->sinXRate;
        }
        if (m->sxCurveStep.on != 0) {
            if (a->time <= m->sxCurveStep.time[0]) {
                if (a->time == m->sxCurveStep.time[0]) {
                    a->gen->def.sxCurve[0] = m->keys[1].sxCurve[0];
                    a->gen->def.sxCurve[1] = m->keys[1].sxCurve[1];
                    a->gen->def.sxCurve[2] = m->keys[1].sxCurve[2];
                }
            } else if (a->time <= m->sxCurveStep.time[1]) {
                if (a->time == m->sxCurveStep.time[1]) {
                    a->gen->def.sxCurve[0] = m->keys[2].sxCurve[0];
                    a->gen->def.sxCurve[1] = m->keys[2].sxCurve[1];
                    a->gen->def.sxCurve[2] = m->keys[2].sxCurve[2];
                }
            } else if (a->time == m->sxCurveStep.time[2]) {
                a->gen->def.sxCurve[0] = m->keys[3].sxCurve[0];
                a->gen->def.sxCurve[1] = m->keys[3].sxCurve[1];
                a->gen->def.sxCurve[2] = m->keys[3].sxCurve[2];
            }
        }
        if (m->sxTime.lastKey != 0 && m->sxTime.time[0] < a->time && m->sxTime.time[m->sxTime.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->sxTime, &from, &to);
            a->gen->def.sxC1 = (f32)(to->sxC1 - from->sxC1) * r + (f32)from->sxC1;
            a->gen->def.sxC2 = (f32)(to->sxC2 - from->sxC2) * r + (f32)from->sxC2;
        }
        if (m->sx0.lastKey != 0 && m->sx0.time[0] < a->time && m->sx0.time[m->sx0.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->sx0, &from, &to);
            a->gen->def.sx[0] = (to->sx[0] - from->sx[0]) * r + from->sx[0];
            a->gen->def.sx[1] = (to->sx[1] - from->sx[1]) * r + from->sx[1];
        }
        if (m->sxRate.lastKey != 0 && m->sxRate.time[0] < a->time && m->sxRate.time[m->sxRate.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->sxRate, &from, &to);
            a->gen->def.sx[2] = (to->sx[2] - from->sx[2]) * r + from->sx[2];
            a->gen->def.sx[3] = (to->sx[3] - from->sx[3]) * r + from->sx[3];
            a->gen->def.sx[4] = (to->sx[4] - from->sx[4]) * r + from->sx[4];
            a->gen->def.sx[5] = (to->sx[5] - from->sx[5]) * r + from->sx[5];
            a->gen->def.sx[6] = (to->sx[6] - from->sx[6]) * r + from->sx[6];
            a->gen->def.sx[7] = (to->sx[7] - from->sx[7]) * r + from->sx[7];
            a->gen->def.sx[8] = (to->sx[8] - from->sx[8]) * r + from->sx[8];
            a->gen->def.sx[9] = (to->sx[9] - from->sx[9]) * r + from->sx[9];
        }
        if (m->sinY.lastKey != 0 && m->sinY.time[0] < a->time && m->sinY.time[m->sinY.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->sinY, &from, &to);
            a->gen->def.sinYLen = (f32)(to->sinYLen - from->sinYLen) * r + (f32)from->sinYLen;
            a->gen->def.sinYStart = (f32)(to->sinYStart - from->sinYStart) * r + (f32)from->sinYStart;
            a->gen->def.sinYRate = (to->sinYRate - from->sinYRate) * r + from->sinYRate;
        }
        if (m->syCurveStep.on != 0) {
            if (a->time <= m->syCurveStep.time[0]) {
                if (a->time == m->syCurveStep.time[0]) {
                    a->gen->def.syCurve[0] = m->keys[1].syCurve[0];
                    a->gen->def.syCurve[1] = m->keys[1].syCurve[1];
                    a->gen->def.syCurve[2] = m->keys[1].syCurve[2];
                }
            } else if (a->time <= m->syCurveStep.time[1]) {
                if (a->time == m->syCurveStep.time[1]) {
                    a->gen->def.syCurve[0] = m->keys[2].syCurve[0];
                    a->gen->def.syCurve[1] = m->keys[2].syCurve[1];
                    a->gen->def.syCurve[2] = m->keys[2].syCurve[2];
                }
            } else if (a->time == m->syCurveStep.time[2]) {
                a->gen->def.syCurve[0] = m->keys[3].syCurve[0];
                a->gen->def.syCurve[1] = m->keys[3].syCurve[1];
                a->gen->def.syCurve[2] = m->keys[3].syCurve[2];
            }
        }
        if (m->syTime.lastKey != 0 && m->syTime.time[0] < a->time && m->syTime.time[m->syTime.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->syTime, &from, &to);
            a->gen->def.syC1 = (f32)(to->syC1 - from->syC1) * r + (f32)from->syC1;
            a->gen->def.syC2 = (f32)(to->syC2 - from->syC2) * r + (f32)from->syC2;
        }
        if (m->sy0.lastKey != 0 && m->sy0.time[0] < a->time && m->sy0.time[m->sy0.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->sy0, &from, &to);
            a->gen->def.sy[0] = (to->sy[0] - from->sy[0]) * r + from->sy[0];
            a->gen->def.sy[1] = (to->sy[1] - from->sy[1]) * r + from->sy[1];
        }
        if (m->syRate.lastKey != 0 && m->syRate.time[0] < a->time && m->syRate.time[m->syRate.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->syRate, &from, &to);
            a->gen->def.sy[2] = (to->sy[2] - from->sy[2]) * r + from->sy[2];
            a->gen->def.sy[3] = (to->sy[3] - from->sy[3]) * r + from->sy[3];
            a->gen->def.sy[4] = (to->sy[4] - from->sy[4]) * r + from->sy[4];
            a->gen->def.sy[5] = (to->sy[5] - from->sy[5]) * r + from->sy[5];
            a->gen->def.sy[6] = (to->sy[6] - from->sy[6]) * r + from->sy[6];
            a->gen->def.sy[7] = (to->sy[7] - from->sy[7]) * r + from->sy[7];
            a->gen->def.sy[8] = (to->sy[8] - from->sy[8]) * r + from->sy[8];
            a->gen->def.sy[9] = (to->sy[9] - from->sy[9]) * r + from->sy[9];
        }
        if (m->colorCurveStep.on != 0) {
            if (a->time <= m->colorCurveStep.time[0]) {
                if (a->time == m->colorCurveStep.time[0]) {
                    a->gen->def.colorCurve[0] = m->keys[1].colorCurve[0];
                    a->gen->def.colorCurve[1] = m->keys[1].colorCurve[1];
                    a->gen->def.colorCurve[2] = m->keys[1].colorCurve[2];
                }
            } else if (a->time <= m->colorCurveStep.time[1]) {
                if (a->time == m->colorCurveStep.time[1]) {
                    a->gen->def.colorCurve[0] = m->keys[2].colorCurve[0];
                    a->gen->def.colorCurve[1] = m->keys[2].colorCurve[1];
                    a->gen->def.colorCurve[2] = m->keys[2].colorCurve[2];
                }
            } else if (a->time == m->colorCurveStep.time[2]) {
                a->gen->def.colorCurve[0] = m->keys[3].colorCurve[0];
                a->gen->def.colorCurve[1] = m->keys[3].colorCurve[1];
                a->gen->def.colorCurve[2] = m->keys[3].colorCurve[2];
            }
        }
        if (m->colorTime.lastKey != 0 && m->colorTime.time[0] < a->time && m->colorTime.time[m->colorTime.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->colorTime, &from, &to);
            a->gen->def.colorC1 = (f32)(to->colorC1 - from->colorC1) * r + (f32)from->colorC1;
            a->gen->def.colorC2 = (f32)(to->colorC2 - from->colorC2) * r + (f32)from->colorC2;
        }
        if (m->colorKeys.lastKey != 0 && m->colorKeys.time[0] < a->time && m->colorKeys.time[m->colorKeys.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->colorKeys, &from, &to);
            pf = from->colorKeys;
            pt = to->colorKeys;
            po = a->gen->def.colorKeys;
            for (j = 16; j > 0; j--) {
                *po++ = (f32)(*pt++ - *pf) * r + (f32)*pf++;
            }
        }
        if (m->rotXAnim.lastKey != 0 && m->rotXAnim.time[0] < a->time && m->rotXAnim.time[m->rotXAnim.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->rotXAnim, &from, &to);
            a->gen->def.rot0[0] = (f32)(to->rot0[0] - from->rot0[0]) * r + (f32)from->rot0[0];
            a->gen->def.rot0[1] = (f32)(to->rot0[1] - from->rot0[1]) * r + (f32)from->rot0[1];
            a->gen->def.rdx[0] = (f32)(to->rdx[0] - from->rdx[0]) * r + (f32)from->rdx[0];
            a->gen->def.rdx[1] = (f32)(to->rdx[1] - from->rdx[1]) * r + (f32)from->rdx[1];
            a->gen->def.rdx[2] = (f32)(to->rdx[2] - from->rdx[2]) * r + (f32)from->rdx[2];
            a->gen->def.rdx[3] = (f32)(to->rdx[3] - from->rdx[3]) * r + (f32)from->rdx[3];
        }
        if (m->rdxModeStep.on != 0) {
            if (a->time <= m->rdxModeStep.time[0]) {
                if (a->time == m->rdxModeStep.time[0]) {
                    a->gen->def.rdxMode = m->keys[1].rdxMode;
                }
            } else if (a->time <= m->rdxModeStep.time[1]) {
                if (a->time == m->rdxModeStep.time[1]) {
                    a->gen->def.rdxMode = m->keys[2].rdxMode;
                }
            } else if (a->time == m->rdxModeStep.time[2]) {
                a->gen->def.rdxMode = m->keys[3].rdxMode;
            }
        }
        if (m->rotYAnim.lastKey != 0 && m->rotYAnim.time[0] < a->time && m->rotYAnim.time[m->rotYAnim.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->rotYAnim, &from, &to);
            a->gen->def.rot0[2] = (f32)(to->rot0[2] - from->rot0[2]) * r + (f32)from->rot0[2];
            a->gen->def.rot0[3] = (f32)(to->rot0[3] - from->rot0[3]) * r + (f32)from->rot0[3];
            a->gen->def.rdy[0] = (f32)(to->rdy[0] - from->rdy[0]) * r + (f32)from->rdy[0];
            a->gen->def.rdy[1] = (f32)(to->rdy[1] - from->rdy[1]) * r + (f32)from->rdy[1];
            a->gen->def.rdy[2] = (f32)(to->rdy[2] - from->rdy[2]) * r + (f32)from->rdy[2];
            a->gen->def.rdy[3] = (f32)(to->rdy[3] - from->rdy[3]) * r + (f32)from->rdy[3];
        }
        if (m->rdyModeStep.on != 0) {
            if (a->time <= m->rdyModeStep.time[0]) {
                if (a->time == m->rdyModeStep.time[0]) {
                    a->gen->def.rdyMode = m->keys[1].rdyMode;
                }
            } else if (a->time <= m->rdyModeStep.time[1]) {
                if (a->time == m->rdyModeStep.time[1]) {
                    a->gen->def.rdyMode = m->keys[2].rdyMode;
                }
            } else if (a->time == m->rdyModeStep.time[2]) {
                a->gen->def.rdyMode = m->keys[3].rdyMode;
            }
        }
        if (m->rotZAnim.lastKey != 0 && m->rotZAnim.time[0] < a->time && m->rotZAnim.time[m->rotZAnim.lastKey] >= a->time) {
            r = PtclTrackEval(a, &m->rotZAnim, &from, &to);
            a->gen->def.rot0[4] = (f32)(to->rot0[4] - from->rot0[4]) * r + (f32)from->rot0[4];
            a->gen->def.rot0[5] = (f32)(to->rot0[5] - from->rot0[5]) * r + (f32)from->rot0[5];
            a->gen->def.rdz[0] = (f32)(to->rdz[0] - from->rdz[0]) * r + (f32)from->rdz[0];
            a->gen->def.rdz[1] = (f32)(to->rdz[1] - from->rdz[1]) * r + (f32)from->rdz[1];
            a->gen->def.rdz[2] = (f32)(to->rdz[2] - from->rdz[2]) * r + (f32)from->rdz[2];
            a->gen->def.rdz[3] = (f32)(to->rdz[3] - from->rdz[3]) * r + (f32)from->rdz[3];
        }
        if (m->rdzModeStep.on != 0) {
            if (a->time <= m->rdzModeStep.time[0]) {
                if (a->time == m->rdzModeStep.time[0]) {
                    a->gen->def.rdzMode = m->keys[1].rdzMode;
                }
            } else if (a->time <= m->rdzModeStep.time[1]) {
                if (a->time == m->rdzModeStep.time[1]) {
                    a->gen->def.rdzMode = m->keys[2].rdzMode;
                }
            } else if (a->time == m->rdzModeStep.time[2]) {
                a->gen->def.rdzMode = m->keys[3].rdzMode;
            }
        }
        a->time++;
    }
}
