"""Draw distance (port/docs/draw-distance.md, port/src/draw_distance.c).

How far the game shows is set by its fog more than by its far clip. The
VU1 fogs a vertex at distance r by
    F = fogMax * d^2 / (d^2 + r^2)
(255 is no fog; d is the view's fog distance, GphViewSetFog), so the scene
is half faded at d, and the far clip each map sets is usually far enough
into the fog that what it cuts is already nearly gone (the attract mode's
cave: d 2000, far clip 7200, 93% faded there).

Both are scaled by grx_draw_distance(): the fog distance where the fog
packet is built (the view keeps the game's value, in case it is read
back), and the far clip in the per-view clip block that every 3D cull
reads (VuViewClipSetup, as in widescreen.py; the view keeps its far clip).
The far clip also goes at least 4 fog distances out (the fog packet's,
b->f40, already scaled), where 6% of what it cuts still shows: some maps
set it barely past their fog (Locca: d 5750, far clip 7200, 61% faded),
which a camera tilted toward the horizon shows as a hard edge. The depth
the GS gets is near / depth, which a farther far clip doesn't change. Fog
distance 0 (a view faded out entirely) stays 0.

The field's sky (the painted mountains and the dome, about 8000 across) is
kept centred on the camera and drawn first, with depth. Past the original
far clip that hid everything behind it: what the longer view draws there
showed only when the camera came within reach, popping in and out as it
moved. Above 1 the depth is cleared after the sky, as the mini map does
after its own list, so the sky stays behind everything.
"""

FIXUPS = {
    # GphViewSetFog
    "src/main/42FA40.c": [
        (r'(#include "gph\.h"\n)', r'\1float grx_dd_fog(float dist);\n#line 3\n'),
        (r"e->fogParam\[0\], e->fogParam\[1\]", r"grx_dd_fog(e->fogParam[0]), e->fogParam[1]"),
    ],
    # VuViewClipSetup(Out *o, In1 *a, In2 *b, In3 *c, f32 z): c is the view's
    # area (..., projDist, near, far, ...); after widescreen.py's widening
    "src/main/45E0A8.c": [
        (r"\n    o->f10 = 0;\n",
         r"\n    const float *grx_dd_view(const float *c, float *tmp, float fog); In3 grx_dv;"
         r" c = (In3 *)grx_dd_view((const float *)c, (float *)&grx_dv, b->f40); o->f10 = 0;\n"),
    ],
    # FldDraw: the sky's list, then everything else
    "src/field/5B06A0.c": [
        (r'(#include "common\.h"\n)', r'\1float grx_draw_distance(void); void GsClearZBuffer(void);\n#line 2\n'),
        (r"(        GphDrawListSet\(fldSkyDrawList\);\n        GphDrawListDrawAll\(\);\n)",
         r"\1        if (grx_draw_distance() != 1.0f) GsClearZBuffer();\n#line 17\n"),
    ],
}
