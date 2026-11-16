"""The frame loop, video and pads (port/src/video.c, port/src/pad.c).

The game waits for vblank by spinning on state the vblank interrupt
changes. The port's kernel is cooperative and its interrupts come only
from the frame loop, which runs a field when every thread waits, so each
of those loops blocks in grx_frame_wait_vblank() instead.
"""

REPLACES = [
    # libpad (_scePadSendActDirect to _scePadRpcCmd14): port/src/pad.c,
    # over SDL3 gamepads and the keyboard, with padman simulated
    ("main", 0x479818, 0x47AB58),
    # libgraph's _sceWaitVSync and _sceWaitVSyncCsr, which poll INTC_STAT's
    # vblank bit (sceGsSyncV calls them): port/src/video.c
    ("main", 0x48C4E0, 0x48C570),
]

FIXUPS = {
    # GsVsyncFlip spins until GsVblankHandler has counted gsFlipWaitCount
    # down: wait for a field each time round instead (the declaration goes
    # on the include's next line, so line numbers don't move)
    "src/main/45EA88.c": [
        (r'(#include "protos/main.h"\n)', r"\1void grx_frame_wait_vblank(void); void grx_frame_flip(s32); "),
        (r"(while \(gsFlipWaitCount != 0\) \{)(\n    \})", r"\1 grx_frame_wait_vblank();\2"),
        # Count game frames (one flip each) and the fields each asks to
        # wait for, for the frame rate log
        (r"(    sceGsSyncPath\(0, 0\);\n)", r"    grx_frame_flip(arg0); sceGsSyncPath(0, 0);\n"),
    ],
}
