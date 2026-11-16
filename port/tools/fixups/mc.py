"""Memory cards (port/src/mc.c, port/include/grx_mc.h).

Sony's libmc is replaced at the sceMc* level: the cards are host
directories and each call runs at the next sceMcSync, with mcman's
results. Its sources (all "Sony PS2 SDK library code") are left out:
sceMcInit 46FE20, _sceMcGetRpcClient 46FFD8, sceMcChangeThreadPriority,
sceMcGetSlotMax, sceMcOpen, sceMcMkdir, sceMcClose, sceMcSeek,
_sceMcReadFixAlign, sceMcRead, sceMcWrite, _sceMcSleep, sceMcSync,
_sceMcGetInfoApdx, sceMcGetInfo, sceMcGetDir, _sceMcChdirApdx, sceMcChdir,
sceMcFormat, sceMcDelete, sceMcFlush, sceMcSetFileInfo, sceMcRename,
sceMcUnformat and sceMcGetEntSpace 4714C0 (libmpeg starts at 4715D8).
libmc's data (_sceMcCmdNo, _sceMcSema, the RPC blocks in bss_4A9680.c)
stays, unused; so does the asm-only _sceMcSleepAlarmHandler 4707B0, which
only _sceMcSleep referred to.
"""

REPLACES = [
    ("main", 0x46FE20, 0x4714C0),
]
