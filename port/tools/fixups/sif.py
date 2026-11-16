"""SIF and the IOP (port/src/sif.c, port/include/grx_sif.h).

Sony's SIF libraries are replaced at the sceSif* level: the port has no
IOP, so modules load by name, RPC servers are C handlers answering at once
and SIF DMA is synchronous. fileio (48F720-492BF0), libmc and libpad stay:
they run on top of the replaced calls.

The EE/IOP shared block (PS2 0x01FF0000-0x02000000, cached and through the
uncached alias 0x21FF0000) is an ordinary object in the port,
grx_iop_block() (cdvd.c): the game's literal uses of it are rewritten here.
Those are all of them (C and asm; the crt0's D_1FF0000 is only the stack
address SetupThread gets, port/game/main/400008.c doesn't use it).
Declarations go on an existing line, so line numbers don't move.
"""

REPLACES = [
    # sifcmd: _sceSifCmdSetSreg .. isceSifSendCmd
    ("main", 0x48E078, 0x48E5A8),
    # sifrpc: sceSifInitRpc .. sceSifRpcLoop (sceSifGetOtherData, 48EBD8,
    # is already replaced by cdvd.c in build.py's own list)
    ("main", 0x48E7C0, 0x48F6E0),
    # loadfile: _sceSifLoadFileInit .. sceSifIopSetVal
    ("main", 0x492DB8, 0x493A38),
    # the IOP's reset: sceSifResetIop, sceSifIsAliveIop, sceSifSyncIop, sceSifRebootIop
    ("main", 0x493B18, 0x493CB8),
]

IOP_BLOCK = "void *grx_iop_block(void); "

FIXUPS = {
    # SndFsMount's directory tree goes to the IOP block (+0x1000), and the
    # directories' sectors to scratch in place of EE 0x01000000 (in the
    # heap's range, before MemInit; sif.c)
    "src/main/42FB00.c": [
        (r"\nvoid SysBoot\(", "\n" + IOP_BLOCK + r"void *grx_sif_dir_scratch(void); void SysBoot("),
        (r"SndFsMount\(0x01FF1000, 0x01000000, ",
         "SndFsMount((s32)grx_iop_block() + 0x1000, (s32)grx_sif_dir_scratch(), "),
    ],
    # LoadIopModules: GS_IOP's header (+0) and the status copy (+0x100),
    # both through the uncached alias
    "src/main/45EEB8.c": [
        (r"\nvoid LoadIopModules\(", "\n" + IOP_BLOCK + "void LoadIopModules("),
        (r"sndIopHeader = \(IopHdr \*\)0x21FF0000;", "sndIopHeader = (IopHdr *)grx_iop_block();"),
        (r"\*\(volatile s32 \*\)0x21FF0000 != ", "*(volatile s32 *)grx_iop_block() != "),
        (r"sndIopStatus = \(SndIopStatus \*\)0x21FF0100;",
         "sndIopStatus = (SndIopStatus *)((u8 *)grx_iop_block() + 0x100);"),
    ],
    # SndIopStatusRecv copies the IOP status block to the status copy
    "src/main/45F578.c": [
        (r"\ns32 SndIopStatusRecv\(", "\n" + IOP_BLOCK + "s32 SndIopStatusRecv("),
        (r"sndIopStatusAddr, 0x01FF0100, ", "sndIopStatusAddr, (s32)grx_iop_block() + 0x100, "),
    ],
}
