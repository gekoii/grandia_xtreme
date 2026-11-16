"""The EE's FPU semantics where the game depends on them.

The EE's FPU has no infinities or NaNs: x/0 is +-FLT_MAX and overflows
clamp to it. The game's C runs on the host's IEEE floats, so a division by
zero the game never noticed on a PS2 can make an infinity, then a NaN that
spreads. Where that happens, the division goes through grx_div_s
(grx_ee.h), which does what div.s does. Find more with GRX_FP_TRAP=field
(port/src/crash.c): it reports each place that makes a NaN or divides by
zero.
"""

FIXUPS = {
    # CamCalcShake: a shake set up with a cycle of 0 on an axis (BATTLE's
    # sword special attacks: x and z) divides by it every frame. On the EE
    # the sine's argument is FLT_MAX and the offset stays finite (0, with
    # the cleared unkD0); on the host it is sinf(inf) = NaN, which went
    # into the view matrix and blacked out the 3D for ~2.5 s until the
    # battle reset the camera
    "src/main/431B10.c": [
        (r"sinf\(t \* 3\.141592f / cam->shakeCycle\[i\] \+",
         r"sinf(grx_div_s(t * 3.141592f, cam->shakeCycle[i]) +"),
    ],
}
