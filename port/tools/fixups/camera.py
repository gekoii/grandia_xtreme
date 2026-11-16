"""The field camera on the right stick (port/docs/camera.md, port/src/camera.c).

The field turns its camera by the L1 and R1 pressures (CamPadTurnInput)
and has no control for its pitch. CamPadTurnInput's turn gets the right
stick's X added, and CamFollowUpdate, the walking camera, has its pitch and
rest pitch moved by the stick's Y, each frame. Both read the pad the game
already reads (padPort0), so the field's input lock, which centres the
sticks, holds the camera still in scenes. CamFollowClearTurn, the walking
camera turning when something is in the way, keeps the turn's speed, and
CamFollowClear moves the eye for a tilt as it does for a turn. With
--camera buttons nothing changes.

CamFollowUpdate also puts a NaN'd walking camera back behind the party
(grx_cam_unnan), with either setting: the EE makes no NaNs, so the game
never recovers from one.
"""

FIXUPS = {
    # CamPadTurnInput(Camera *, Pad *pad, s32 digital, f32 scale)
    "src/field/5CAA40.c": [
        (r'(#include "field/n15\.h"\n)', r'\1float grx_cam_turn(float turn, int right_x, int reverse);\n#line 7\n'),
        (r"\n    return ret;\n}", r"\n    return grx_cam_turn(ret, pad->rightX, sysOptCamReverse);\n}"),
    ],
    # CamAiTown(Camera *o, ChrObj *s, void *pad): pitch fixed at 20 degrees
    # (the Pad's rightY is at +0x1F; pad is a void * here)
    "src/field/5CBA30.c": [
        (r'(#include "field/state\.h"\n)', r'\1float grx_cam_town_pitch(float pitch, int right_y);\n#line 7\n'),
        (r"o->pitch = 0\.34906584f;",
         r"o->pitch = grx_cam_town_pitch(0.34906584f, ((const u8 *)pad)[0x1F]);"),
    ],
    # CamFollowClearTurn(out, eye, lookAt): the eye turning when its sweep
    # hits something (a rock, a pillar) goes to where the turn puts it, the
    # distance pulled in to the hit, rather than slowly towards it (flag 2:
    # at most about 20 units a frame, from 0, so a quarter of the turn)
    "src/field/5CD718.c": [
        (r'(#include "field/n15\.h"\n)', r'\1int grx_cam_turn_free(void);\n#line 5\n'),
        (r"if \(l < da\) \{", r"if (l < da || grx_cam_turn_free()) {"),
    ],
    # CamFollowClear(out, eye, lookAt): the eye, the party in sight, goes
    # to where a turn puts it (CamFollowClearTurn) and otherwise chases it,
    # speeding up slowly (CamFollowClearChase); a tilt goes as a turn does
    "src/field/5CD680.c": [
        (r'(#include "field/n15\.h"\n)', r'\1int grx_cam_tilting(void);\n#line 4\n'),
        (r"if \(camFollowCur->turn != 0\.0f\) \{", r"if (camFollowCur->turn != 0.0f || grx_cam_tilting()) {"),
    ],
    # CamFollowUpdate(Camera *o, u32 mode)
    "src/field/5CB060.c": [
        (r'(#include "field/n15\.h"\n)', r'\1void grx_cam_pitch(float *pitch, float *rest, int right_y, float rate);'
         r' void grx_cam_unnan(void *cam, void *work);\n#line 8\n'),
        (r"(\n    pad = padPort0;)\n",
         r"\1 grx_cam_unnan(o, camFollow); grx_cam_pitch(&camFollow->pitch, &camFollow->unk30, pad->rightY,"
         r" camFollow->rate[0]);\n"),
    ],
}
