/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n14.h"
#include "field/n15.h"


extern u8 camScrSysByte;

static inline f32 wrap_angle(f32 f) {
    if (f >= 360.0f) {
        f -= 360.0f;
    } else if (f < 0.0f) {
        f += 360.0f;
    }
    return f * 0.017453292f;
}

static inline void set_path(CamScrPath *pa) {
    CamScrPathStart(pa, ((s16 *)camScrPc)[2]);
    pa->x = CamScrArgFloat(camScrPc[2]);
    pa->y = CamScrArgFloat(camScrPc[3]);
    pa->z = CamScrArgFloat(camScrPc[4]);
    camScrPc += 5;
}

u32 CamScrExec(u32 arg0, Camera *obj, s32 *arg2) {
    u32 ret;
    s32 v;
    f32 *q;

    ret = 2;
    switch (arg0) {
    case 1:
        camScrPc = arg2;
        CamScrReset(obj);
    case 2:
    default:
        break;
    case 0:
    case 3:
    case 4:
        return arg0;
    }

    if (camScrFlags & 1) {
        camScrWait = 0;
        camScrPc = camScrEndScript;
    }

    while (camScrWait == 0) {
        if (*camScrPc == 0x1FFFF) {
            obj->mode = 0x39;
            ret = 0;
            camScrWait = 0;
            camScrFlags &= ~0xFF;
            break;
        }
        if (*camScrPc == 0x1FFFE) {
            camScrWait = 0;
            ret = 0;
            camScrFlags &= ~0xFF;
            break;
        } else if (*camScrPc == 0x10011) {
            CamScrSetMove(obj, 0, camScrPc[1]);
            CamScrSetMove(obj, 1, camScrPc[2]);
            CamScrSetMove(obj, 2, camScrPc[3]);
            camScrPc += 4;
        } else if (*camScrPc == 0x10012) {
            CamScrSetPos(obj, 0, camScrPc[1]);
            CamScrSetPos(obj, 1, camScrPc[2]);
            CamScrSetPos(obj, 2, camScrPc[3]);
            camScrPc += 4;
        } else if (*camScrPc == 0x10013) {
            CamScrSetTurn(obj, 0, camScrPc[1]);
            CamScrSetTurn(obj, 1, camScrPc[2]);
            CamScrSetTurn(obj, 2, camScrPc[3]);
            camScrPc += 4;
        } else if (*camScrPc == 0x10014) {
            CamScrSetClip(obj, 0, camScrPc[1]);
            CamScrSetClip(obj, 1, camScrPc[2]);
            CamScrSetClip(obj, 2, camScrPc[3]);
            camScrPc += 4;
        } else if (*camScrPc == 0x10015) {
            v = camScrPc[1];
            if (v == 0x7FFFFFFB) {
                obj->zoom = camScrSaveStartZoom;
            } else if (v == 0x7FFFFFFA) {
                obj->zoom = camScrSaveMapZoom;
            } else {
                obj->zoom = CamScrArgFloat(v);
            }
            camScrPc += 2;
        } else if (*camScrPc == 0x10021) {
            camScrWait = camScrPc[1];
            if (camScrWait < 0) {
                camScrWait = 0;
            }
            camScrPc += 2;
        } else if (*camScrPc == 0x10022) {
            EvtFlagSetByType(1, 0xF0, camScrPc[1]);
            camScrPc += 2;
        } else if (*camScrPc == 0x10023) {
            q = (f32 *)&camScrPc[1];
            obj->shakeAmp[0] = q[0];
            obj->shakeAmp[1] = q[1];
            obj->shakeAmp[2] = q[2];
            obj->shakeAmp[3] = q[2];
            obj->shakeRand[0] = VuRandInt(10);
            obj->shakeRand[1] = VuRandInt(10);
            obj->shakeRand[2] = VuRandInt(10);
            obj->shakeRand[3] = VuRandInt(10);
            obj->shakeCycle[0] = VuRandInt(10);
            obj->shakeCycle[1] = VuRandInt(10);
            obj->shakeCycle[2] = VuRandInt(10);
            obj->shakeCycle[3] = VuRandInt(10);
            obj->shakeTime = camScrPc[4];
            if (camScrPc[4] != 0) {
                obj->shakeMode = 0x19;
            } else {
                obj->shakeMode = 9;
            }
            obj->shakeTick = 0;
            obj->shakeFlags |= 1;
            camScrPc += 6;
        } else if (*camScrPc == 0x10025) {
            camScrPc += 6;
        } else if (*camScrPc == 0x10024) {
            if (!(camScrFlags & 2)) {
                ret = 3;
                v = 0; /* dead store: keeps this a branch instead of movz */
            }
            camScrPc += 1;
            break;
        } else if (*camScrPc == 0x10043) {
            CamScrPersInterpStart(&camScrPersInterp, camScrPc, obj);
            camScrPc += 6;
        } else if (*camScrPc == 0x10031) {
            CamScrMoveInterpStart(&camScrMoveInterp[0], 0, camScrPc, obj);
            camScrPc += 6;
        } else if (*camScrPc == 0x10032) {
            CamScrMoveInterpStart(&camScrMoveInterp[1], 1, camScrPc, obj);
            camScrPc += 6;
        } else if (*camScrPc == 0x10033) {
            CamScrMoveInterpStart(&camScrMoveInterp[2], 2, camScrPc, obj);
            camScrPc += 6;
        } else if (*camScrPc == 0x10034) {
            CamScrTurnInterpStart(&camScrTurnInterp[0], 0, camScrPc, obj);
            camScrPc += 7;
        } else if (*camScrPc == 0x10035) {
            CamScrTurnInterpStart(&camScrTurnInterp[1], 1, camScrPc, obj);
            camScrPc += 7;
        } else if (*camScrPc == 0x10036) {
            CamScrTurnInterpStart(&camScrTurnInterp[2], 2, camScrPc, obj);
            camScrPc += 7;
        } else if (*camScrPc == 0x10037) {
            CamScrZoomInterpStart(&camScrZoomInterp, camScrPc, obj);
            camScrPc += 6;
        } else if (*camScrPc == 0x10051) {
            CamScrPosInterpStart(&camScrPosInterp[0], 0, camScrPc, obj);
            camScrPc += 6;
        } else if (*camScrPc == 0x10052) {
            CamScrPosInterpStart(&camScrPosInterp[1], 1, camScrPc, obj);
            camScrPc += 6;
        } else if (*camScrPc == 0x10053) {
            CamScrPosInterpStart(&camScrPosInterp[2], 2, camScrPc, obj);
            camScrPc += 6;
        } else if (*camScrPc == 0x10054) {
            CamScrLengthInterpStart(&camScrLengthInterp, camScrPc, obj);
            camScrPc += 6;
        } else if (*camScrPc == 0x10061) {
            set_path(&camScrMovePath);
        } else if (*camScrPc == 0x10062) {
            set_path(&camScrPosPath);
        } else if (*camScrPc == 0x10046) {
            if (camScrPc[1] != 0) {
                obj->mode = 0x33;
            } else {
                obj->mode = 0x39;
            }
            camScrPc += 2;
        } else if (*camScrPc == 0x10047) {
            camScrPc += 2;
        } else if (*camScrPc == 0x10048) {
            camScrPc += 2;
        } else if (*camScrPc == 0x10001) {
            camScrPc += 1;
        } else if (*camScrPc == 0x10063) {
            camScrPc += 1;
        } else if (*camScrPc == 0x10066) {
            switch ((u32)camScrPc[1]) {
            case 0:
                camScrSysByte = ((u8 *)camScrPc)[8];
                break;
            case 1:
                camScrSysByte = ((u8 *)camScrPc)[8];
                break;
            case 2:
                camScrSysByte = ((u8 *)camScrPc)[8];
                break;
            case 3:
                camScrSysByte = ((u8 *)camScrPc)[8];
                break;
            case 4:
                camScrSysByte = ((u8 *)camScrPc)[8];
                break;
            case 5:
                camScrSysByte = ((u8 *)camScrPc)[8];
                break;
            case 6:
                camScrSysByte = ((u8 *)camScrPc)[8];
                break;
            case 7:
                camScrSysByte = ((u8 *)camScrPc)[8];
                break;
            }
            camScrPc += 4;
        } else {
            camScrWait = 0;
            ret = 4;
            break;
        }
    }

    if (camScrWait > 0) {
        camScrWait--;
        if (camScrMoveInterp[0].active) {
            obj->lookAt[0] = CamScrInterpStep(&camScrMoveInterp[0]);
        }
        if (camScrMoveInterp[1].active) {
            obj->lookAt[1] = CamScrInterpStep(&camScrMoveInterp[1]);
        }
        if (camScrMoveInterp[2].active) {
            obj->lookAt[2] = CamScrInterpStep(&camScrMoveInterp[2]);
        }
        if (camScrTurnInterp[0].active) {
            obj->pitch = wrap_angle(CamScrInterpStep(&camScrTurnInterp[0]));
        }
        if (camScrTurnInterp[1].active) {
            obj->yaw = wrap_angle(CamScrInterpStep(&camScrTurnInterp[1]));
        }
        if (camScrTurnInterp[2].active) {
            obj->roll = wrap_angle(CamScrInterpStep(&camScrTurnInterp[2]));
        }
        if (camScrZoomInterp.active) {
            obj->zoom = CamScrInterpStep(&camScrZoomInterp);
        }
        if (camScrPosInterp[0].active) {
            obj->eye[0] = CamScrInterpStep(&camScrPosInterp[0]);
        }
        if (camScrPosInterp[1].active) {
            obj->eye[1] = CamScrInterpStep(&camScrPosInterp[1]);
        }
        if (camScrPosInterp[2].active) {
            obj->eye[2] = CamScrInterpStep(&camScrPosInterp[2]);
        }
        if (camScrPersInterp.active) {
            obj->fov = CamScrInterpStep(&camScrPersInterp);
        }
        if (camScrLengthInterp.active) {
            obj->dist = CamScrInterpStep(&camScrLengthInterp);
        }
        if (camScrMovePath.active) {
            if (camScrMovePath.type == 0) {
                obj->lookAt[0] = mapPathWork->small[camScrMovePath.idx].x[camScrMovePath.frame] + camScrMovePath.x;
                obj->lookAt[1] = mapPathWork->small[camScrMovePath.idx].y[camScrMovePath.frame] + camScrMovePath.y;
                obj->lookAt[2] = mapPathWork->small[camScrMovePath.idx].z[camScrMovePath.frame] + camScrMovePath.z;
            } else {
                obj->lookAt[0] = mapPathWork->big[camScrMovePath.idx].x[camScrMovePath.frame] + camScrMovePath.x;
                obj->lookAt[1] = mapPathWork->big[camScrMovePath.idx].y[camScrMovePath.frame] + camScrMovePath.y;
                obj->lookAt[2] = mapPathWork->big[camScrMovePath.idx].z[camScrMovePath.frame] + camScrMovePath.z;
            }
            if (++camScrMovePath.frame >= camScrMovePath.count) {
                camScrMovePath.active = 0;
                if (camScrMovePath.type == 0) {
                    mapPathWork->smallUsed[camScrMovePath.idx] = 0;
                } else {
                    mapPathWork->bigUsed[camScrMovePath.idx] = 0;
                }
            }
        }
        if (camScrPosPath.active) {
            if (camScrPosPath.type == 0) {
                obj->eye[0] = mapPathWork->small[camScrPosPath.idx].x[camScrPosPath.frame] + camScrPosPath.x;
                obj->eye[1] = mapPathWork->small[camScrPosPath.idx].y[camScrPosPath.frame] + camScrPosPath.y;
                obj->eye[2] = mapPathWork->small[camScrPosPath.idx].z[camScrPosPath.frame] + camScrPosPath.z;
            } else {
                obj->eye[0] = mapPathWork->big[camScrPosPath.idx].x[camScrPosPath.frame] + camScrPosPath.x;
                obj->eye[1] = mapPathWork->big[camScrPosPath.idx].y[camScrPosPath.frame] + camScrPosPath.y;
                obj->eye[2] = mapPathWork->big[camScrPosPath.idx].z[camScrPosPath.frame] + camScrPosPath.z;
            }
            if (++camScrPosPath.frame >= camScrPosPath.count) {
                camScrPosPath.active = 0;
                if (camScrPosPath.type == 0) {
                    mapPathWork->smallUsed[camScrPosPath.idx] = 0;
                } else {
                    mapPathWork->bigUsed[camScrPosPath.idx] = 0;
                }
            }
        }
    }
    return ret;
}
