/* The field camera on the right stick (grx_camera.h, port/docs/camera.md).

   Turning: CamPadTurnInput turns the camera by the L1 and R1 pressures,
   eased (up to about 4 degrees a frame held fully). The stick's X adds a
   turn of its own, proportional and eased the same way, so either works.

   Tilting: the follow camera, the one the field uses while walking, keeps
   a pitch it rests at (8 degrees above the target) and eases its pitch
   back to it when nothing is in the way. The stick's Y moves both: the
   pitch at once, so it answers the stick, and the rest pitch, so it stays
   there. Once moved, the rest pitch is kept across areas (the game sets 8
   degrees again when it sets the camera up). The game's own wall and floor
   checks still push the camera when it would go through them. The tilt
   goes as fast as the turn, eased the same way: in each camera, the stick
   fully over tilts by as many degrees a frame as it turns. While it
   tilts, the walking camera's eye goes where the tilt puts it, as it does
   for a turn, rather than chasing it.

   Turning past what's in the way: the walking camera turns at full speed
   when its sweep hits a rock or a pillar, pulled in to clear it, where the
   game slows it to a quarter of the turn or less.

   A NaN in the walking camera: the EE's FPU has no NaNs, so the game never
   recovers from one. CamFollowClearTurn feeds the camera's approach speed
   back into itself each frame, and the eye it works out from that is NaN
   from then on: the scene goes black (the 2D still draws) and stays so,
   in the save states made after. grx_cam_unnan puts the camera back where
   it would rest. */
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "grx_camera.h"
#include "grx_platform.h"
#include "grx_video.h"

#define DEAD 24            /* of 128: the stick's rest jitter and drift */
#define TURN_MAX 4.5f      /* degrees a frame, the stick fully over */
#define TURN_EASE 0.3f
#define DEG 0.017453292f   /* radians */
#define PITCH_MIN -0.26f   /* -15 degrees: a little below the target */
#define PITCH_MAX 1.13f    /* 65 degrees: nearly overhead */

static int on = 1;
static float turn_now;
static float follow_tilt;  /* the walking camera's tilt this frame */
static int have_rest;
static float rest_pitch;

void grx_cam_config(const char *arg) {
    if (arg == NULL || !*arg) {
        arg = getenv("GRX_CAMERA");
    }
    if (arg == NULL || !*arg || strcmp(arg, "stick") == 0) {
        on = 1;
    } else if (strcmp(arg, "buttons") == 0) {
        on = 0;
    } else {
        grx_fatal("--camera / GRX_CAMERA: stick or buttons, not \"%s\"", arg);
    }
}

/* A stick byte as -1..1 past the dead zone, squared for fine control near
   the centre; positive towards 0x00 (right, down) */
static float axis(int v) {
    float d = (float)(0x80 - v), m = fabsf(d);
    if (m <= DEAD) {
        return 0.0f;
    }
    m = (m - DEAD) / (127.0f - DEAD);
    if (m > 1.0f) {
        m = 1.0f;
    }
    return d < 0 ? -m * m : m * m;
}

float grx_cam_turn(float turn, int right_x, int reverse) {
    float want;
    if (!on) {
        return turn;
    }
    /* Pushed right, the camera goes round as R1 turns it (negative): the
       view turns left, the scene swings right. The Camera Rotation option
       reverses it, as it does L1 and R1. */
    want = -axis(right_x) * TURN_MAX;
    if (reverse == 1) {
        want = -want;
    }
    turn_now += (want - turn_now) * TURN_EASE;
    if (fabsf(turn_now) < 0.001f) {
        turn_now = 0.0f;
    }
    return turn + turn_now;
}

/* The tilt this frame (radians, positive looks down), eased as the turn is:
   up to `max_deg` degrees a frame, the turn's top speed in that camera */
static float tilt(float *now, int right_y, float max_deg) {
    /* Pushed up (negative), the pitch drops: the camera goes down and
       looks up */
    float want = axis(right_y) * max_deg * DEG;
    *now += (want - *now) * TURN_EASE;
    if (fabsf(*now) < 0.001f * DEG) {
        *now = 0.0f;
    }
    return *now;
}

float grx_cam_town_pitch(float pitch, int right_y) {
    static int have;
    static float now, speed;
    float step;
    if (!on) {
        return pitch;
    }
    /* The town camera turns by CamPadTurnInput's degrees as they are */
    step = tilt(&speed, right_y, TURN_MAX);
    if (step != 0.0f) {
        if (!have) {
            now = pitch;
            have = 1;
        }
        now += step;
        if (now < PITCH_MIN) now = PITCH_MIN;
        if (now > PITCH_MAX) now = PITCH_MAX;
    }
    return have ? now : pitch;
}

void grx_cam_pitch(float *pitch, float *rest, int right_y, float rate) {
    float step;
    if (!on) {
        return;
    }
    /* The walking camera turns by CamPadTurnInput's degrees times its rate
       (0.6) */
    step = tilt(&follow_tilt, right_y, TURN_MAX * rate);
    if (step != 0.0f) {
        if (!have_rest) {
            rest_pitch = *rest;
            have_rest = 1;
        }
        rest_pitch += step;
        if (rest_pitch < PITCH_MIN) rest_pitch = PITCH_MIN;
        if (rest_pitch > PITCH_MAX) rest_pitch = PITCH_MAX;
        *pitch += step;
        if (*pitch < PITCH_MIN) *pitch = PITCH_MIN;
        if (*pitch > PITCH_MAX) *pitch = PITCH_MAX;
    }
    if (have_rest) {
        *rest = rest_pitch;
    }
}

int grx_cam_tilting(void) {
    return follow_tilt != 0.0f;
}

int grx_cam_turn_free(void) {
    return on;
}

float grx_cam_lift(float pitch) {
    if (on) {
        return sinf(pitch);
    }
    return asinf(pitch > 1.0f ? 1.0f : pitch < -1.0f ? -1.0f : pitch);
}

/* The walking camera's state, by offset (include/camera.h Camera,
   include/field/n15.h CamFollowWork and CamState); all floats */
#define CAM_LOOKAT 0x50   /* Camera: lookAt, eye, dir (vectors) */
#define CAM_EYE 0x60
#define CAM_DIR 0x70
#define CAM_PITCH 0x90    /* pitch, yaw, roll, dist */
#define CAM_DIST 0x9C
#define FW_SPEED 0x08     /* CamFollowWork: the approach speed */
#define FW_PITCH 0x24     /* the pitch it rests at */
#define FW_DIST 0x34      /* dist[0], the walking distance */
#define FW_UNK90 0x90     /* the eye CamCalcEyePos works out */
#define FW_UNKA0 0xA0
#define FW_CUR 0xD0       /* CamState cur, save: lookAt, eye, dir, dist, */
#define FW_SAVE 0x110     /* pitch, yaw, unk3C */
#define ST_SIZE 0x40

static float *at(void *base, int off) {
    return (float *)((uint8_t *)base + off);
}

static int finite_all(const float *f, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!isfinite(f[i])) {
            return 0;
        }
    }
    return 1;
}

/* CamFollowUpdate, each frame, before it works on them: when the camera
   (the field's, `cam`) or the follow camera's work (`work`) holds a NaN or
   an infinity. With the camera whole, only the work is filled again, from
   the camera. Otherwise both go back behind the target where the camera
   rests: the camera's look-at (else the one the follow camera aims at),
   the eye the follow camera keeps if it is near (else straight behind at
   the pitch it rests at and the walking distance), its speed 0. */
void grx_cam_unnan(void *cam, void *work) {
    static unsigned reported;
    float *save = at(work, FW_SAVE), look[3], eye[3], dir[3], pitch, yaw, dist, len;
    int i, cam_ok = finite_all(at(cam, CAM_LOOKAT), (CAM_DIST + 4 - CAM_LOOKAT) / 4);
    if (cam_ok && finite_all(at(work, FW_SPEED), 1) && finite_all(at(work, FW_UNK90), 8) &&
        finite_all(at(work, FW_CUR), 2 * ST_SIZE / 4)) {
        return;
    }
    if (cam_ok) {
        memcpy(look, at(cam, CAM_LOOKAT), sizeof(look));
        memcpy(eye, at(cam, CAM_EYE), sizeof(eye));
        memcpy(dir, at(cam, CAM_DIR), sizeof(dir));
        pitch = at(cam, CAM_PITCH)[0];
        yaw = at(cam, CAM_PITCH)[1];
        len = *at(cam, CAM_DIST);
    } else {
        if (finite_all(at(cam, CAM_LOOKAT), 3)) {
            memcpy(look, at(cam, CAM_LOOKAT), sizeof(look));
        } else if (finite_all(save, 3)) {
            memcpy(look, save, sizeof(look));
        } else {
            if (reported++ < 4) {
                grx_log("camera: the field camera is NaN and has no target to put it back on");
            }
            return;
        }
        pitch = *at(work, FW_PITCH);
        if (!isfinite(pitch) || pitch < PITCH_MIN || pitch > PITCH_MAX) {
            pitch = 0.1396f; /* the game's 8 degrees */
        }
        yaw = isfinite(save[14]) ? save[14] : 0.0f;
        dist = *at(work, FW_DIST);
        if (!isfinite(dist) || dist < 100.0f || dist > 5000.0f) {
            dist = 800.0f;
        }
        for (i = 0, len = 0.0f; i < 3; i++) {
            dir[i] = look[i] - save[4 + i];
            len += dir[i] * dir[i];
        }
        if (finite_all(save + 4, 3) && len > 1.0f && len < 16.0f * dist * dist) {
            memcpy(eye, save + 4, sizeof(eye));
        } else {
            eye[0] = look[0];
            eye[1] = look[1] + dist * sinf(pitch);
            eye[2] = look[2] - dist * cosf(pitch);
        }
        for (i = 0, len = 0.0f; i < 3; i++) {
            dir[i] = look[i] - eye[i];
            len += dir[i] * dir[i];
        }
        len = sqrtf(len);
        for (i = 0; i < 3; i++) {
            dir[i] /= len;
        }
        /* The camera; its matrix is built again from these this frame */
        memcpy(at(cam, CAM_LOOKAT), look, sizeof(look));
        memcpy(at(cam, CAM_EYE), eye, sizeof(eye));
        memcpy(at(cam, CAM_DIR), dir, sizeof(dir));
        at(cam, CAM_PITCH)[0] = pitch;
        at(cam, CAM_PITCH)[1] = yaw;
        at(cam, CAM_PITCH)[2] = 0.0f;
        *at(cam, CAM_DIST) = len;
    }
    if (reported++ < 4) {
        grx_log("camera: %s went NaN (field %llu): %s", cam_ok ? "the walking camera's state" : "the field camera",
                (unsigned long long)grx_frame_count(),
                cam_ok ? "set again from the camera" : "put back behind the target");
    }
    /* The follow camera: still, its eye there, and both states the same */
    *at(work, FW_SPEED) = 0.0f;
    memcpy(at(work, FW_UNK90), eye, sizeof(eye));
    memset(at(work, FW_UNKA0), 0, 3 * sizeof(float));
    for (i = 0; i < 2; i++) {
        float *st = at(work, i == 0 ? FW_CUR : FW_SAVE);
        memcpy(st, look, sizeof(look));
        memcpy(st + 4, eye, sizeof(eye));
        memcpy(st + 8, dir, sizeof(dir));
        st[12] = len;
        st[13] = pitch;
        st[14] = yaw;
    }
}
