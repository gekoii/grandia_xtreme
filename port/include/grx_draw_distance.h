/* Draw distance (port/docs/draw-distance.md): --draw-distance N, or
   GRX_DRAW_DISTANCE. The game's fog distance and far clip, both scaled
   by N (port/tools/fixups/drawdist.py): the scene fades out N times as
   far away. The far clip goes at least 4 times the fog distance out,
   where the fog has faded everything it cuts (some maps set it barely
   past their fog, which a camera turned toward the horizon shows).
   1 is the original. */
#ifndef GRX_DRAW_DISTANCE_H
#define GRX_DRAW_DISTANCE_H

/* The --draw-distance / GRX_DRAW_DISTANCE setting (NULL: GRX_DRAW_DISTANCE,
   else the default). Fatal on a bad value. */
void grx_dd_config(const char *arg);

/* The multiplier (1 = the original) */
float grx_draw_distance(void);

/* VuViewClipSetup's view (x, y, w, h, centerX, centerY, magH, magV,
   projDist, near, far, ...) and its fog distance (as grx_dd_fog gave
   it): c itself, or tmp filled with c and the far clip moved out */
const float *grx_dd_view(const float *c, float *tmp, float fog);

/* GphViewSetFog's fog distance, scaled */
float grx_dd_fog(float dist);

#endif
