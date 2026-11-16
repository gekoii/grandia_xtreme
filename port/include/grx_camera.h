/* The field camera on the right stick (port/docs/camera.md): --camera
   stick|buttons, or GRX_CAMERA. With "stick" (the default) the right
   stick turns the camera left and right, as L1 and R1 do (they still
   work), and tilts it up and down, which the game has no control for. With
   "buttons" the camera is the original's. */
#ifndef GRX_CAMERA_H
#define GRX_CAMERA_H

/* The --camera / GRX_CAMERA setting (NULL: GRX_CAMERA, else stick).
   Fatal on a bad value. */
void grx_cam_config(const char *arg);

/* For the game (port/tools/fixups/camera.py). The stick bytes are the
   game's Pad's: 0x80 centre, inverted (0xFF left, up).
   CamPadTurnInput's turn (degrees this frame, positive as L1 turns) with
   the stick's added; `reverse` is the game's Camera Rotation option. */
float grx_cam_turn(float turn, int right_x, int reverse);

/* CamFollowUpdate, each frame: the follow camera's pitch and the pitch it
   rests at, moved by the stick (radians, positive looks down); `rate`
   scales its turn (camFollow->rate[0]), and so the tilt's speed */
void grx_cam_pitch(float *pitch, float *rest, int right_y, float rate);

/* CamFollowClear: whether the stick is tilting the walking camera this
   frame, so its eye goes where the tilt puts it, as for a turn, rather
   than chasing it */
int grx_cam_tilting(void);

/* CamAiTown, each frame: the town camera's pitch, which the game sets to
   `pitch` (20 degrees) every frame, as the stick has moved it */
float grx_cam_town_pitch(float pitch, int right_y);

/* CamFollowClearTurn: whether the walking camera turns at full speed past
   what its sweep hits, pulled in to clear it (1 with "stick"), rather
   than easing towards it at a quarter of the turn or less */
int grx_cam_turn_free(void);

/* CamFollowClearChase: the walking camera's eye lifted to its pitch
   (radians) is the target's height plus the distance times this. The game
   has asinf(pitch), NaN past 1 radian, which the stick's tilt (up to 65
   degrees) reaches: with "stick" it is sinf(pitch), what it means; with
   "buttons" the game's asinf, of at most 1 */
float grx_cam_lift(float pitch);

/* CamFollowUpdate, each frame: when the field camera or the follow
   camera's work (CamFollowWork) holds a NaN, which the EE never makes and
   the game never recovers from, both are put back behind the target */
void grx_cam_unnan(void *cam, void *work);

#endif
