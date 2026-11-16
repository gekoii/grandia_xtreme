"""Skipping cut scenes (port/docs/scene-skip.md, port/src/skip.c).

FldUpdatePlay, the field's frame, tells the skip each frame, right after
the scenario's buttons are read (EvtScnrPadUpdate) and before the
scenario runs (EvtScnrMain): whether a cut scene is running (a scenario,
but not a talk ChrTalkUpdate started: chrTalkWork's first word is the
person talked to), the message state (with 0x10000 while the event's
character select or a mini-game waits for the player), where the script
is, and the scenario's buttons, which a skip sets to pass the messages.
FldUpdate, which runs FldUpdatePlay (or the field menu's frame instead),
asks how many frames to run before drawing.
"""

FIXUPS = {
    # FldUpdate(s32), the field's frame: runs FldUpdatePlay n times, once
    # but with the debug mode's speed-up (R3: fldState.systemSpeed); a skip
    # runs several, drawn once
    "src/field/5B0E60.c": [
        (r'(#include "field/state\.h"\n)', r'\1int grx_skip_updates(int n);\n#line 6\n'),
        (r"\n    pad = padPort0;\n", r"\n    n = grx_skip_updates(n);\n#line 40\n    pad = padPort0;\n"),
    ],
    # FldUpdatePlay(s32)
    "src/field/5B09C8.c": [
        (r'(#include "field/n12\.h"\n)',
         r'\1#include "field/event.h"\n#include "field/n27.h"\n'
         r'extern u8 chrTalkWork[];\n'
         r'void grx_skip_scene(int scene, unsigned msg_flags, unsigned where, u16 *keys);\n#line 7\n'),
        (r"(\n    EvtScnrPadUpdate\(padPort0\);\n)(    EvtScnrMain\(\);)",
         r"\1    grx_skip_scene(EvtScnrIsRunning() != 0 && *(void **)chrTalkWork == NULL,\n"
         r"                   evtMsgCtl->flags | (EvtCharSelIsActive() != 0 || MiniGetState() != 0 ? 0x10000u : 0),\n"
         r"                   (unsigned)((u8 *)evtScnrCur->pc - (u8 *)0)\n"
         r"                       ^ (unsigned)((u8 *)evtMsgCtl->textPc - (u8 *)0) * 31\n"
         r"                       ^ (unsigned)(int)(evtScnrWork->waitTimer * 64.0f), evtScnrInput);\n"
         r"#line 58\n\2"),
    ],
}
