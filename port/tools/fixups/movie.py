"""Movies (port/src/movie/, port/include/grx_movie.h).

1. libmpeg's decoder and libipu are replaced by port/src/movie/libmpeg.c
   (sceMpeg* over the software MPEG-2 decoder in mpeg2.c, reading the
   toIPU DMA chain the game's viBuf code builds). Left out: 4723B0-478FF7
   (sceMpegInit, sceMpegCreate and the other calls, the MSSG header
   parsing, the macroblock layer, the IPU command wrappers, the CSC) and
   libipu (479208-4795E0). Kept: the demuxer (sceMpegDemuxPssRing and the rest of
   4715D8-471E28) and its bit reader _sceMpegBs* (478FF8-4791D8), plain C
   that reads the stream callbacks sceMpegCreate leaves in sceMpeg.sys.
   The port-only C written for three of the decoder's functions still in
   assembly (_formPrediction, _addBlock, the put twin) and the extension
   dispatch table in the asm-only data would pull the left-out sources back
   into the link: they are compiled out / emptied.

2. The movie player's own GIF DMA starts (MovDispInit, MovClearScreen,
   MovGifSendChain store D2_CHCR directly) go through the platform's DMA
   (grx_dma_write_chcr, port/src/dma.c), which runs the transfer through
   grx_gs_gif and calls the GIF end handler (MovGifEndHandler).

3. The player's busy waits. The kernel is cooperative, and its fields
   (vblank: MovVblankHandler shows the frames, MovGifEndHandler frees
   them) come from the frame loop when every thread waits. The player's
   threads never wait: they spin, yielding to each other with
   switchThread (RotateThreadReadyQueue). So switchThread also runs a
   field when one is due in real time (grx_frame_poll), and
   videoDecMain's wait for the display to empty the frame queue, which
   spins without yielding, yields.

4. The read loops' empty counting loops (MovReadLoop, MovListReadLoop:
   for (i = 0; i < 100000; i++) {} each pass, and before giving up or
   while flushing) compile to nothing here. On the EE a pass took about a
   millisecond, so their "no progress" limits, 40000 and 100000 passes,
   were tens of seconds; here they ran out in a fraction of a field,
   before the vblank had shown the first frame, and every movie stopped
   after decoding five frames. grx_ee_spin gives the loops the EE's time,
   running fields meanwhile.
"""

REPLACES = [
    # libmpeg's decoder: sceMpegInit (4723B0) .. _sceMpegCscFromHandler's
    # file and the CSC (478DA0); the bit reader from 478FF8 on stays
    ("main", 0x4723B0, 0x478FF7),
    # libipu: _sceIpuSetFromChcr .. sceIpuInit
    ("main", 0x479208, 0x4795E0),
]

_OFF_START = (r"\A", "#if 0 /* libmpeg's decoder is replaced: port/tools/fixups/movie.py */\n")
_OFF_END = (r"\Z", "\n#endif\n")

FIXUPS = {
    # port-only C of the decoder's asm functions: not linked
    "port/game/main/474558.c": [_OFF_START, _OFF_END],
    "port/game/main/475818.c": [_OFF_START, _OFF_END],
    "port/game/main/475878.c": [_OFF_START, _OFF_END],
    # MSSG's extension dispatch (_extensionAndUserData, left out): no
    # references to the left-out parsers
    "port/game/main/data_49DD70.c": [(
        r"(ExtFunc _sceMpegExtensionTable\[11\] = \{\n)"
        r"    _unknownExtension, _sequenceExtension, _sequenceDisplayExtension, _quantMatrixExtension,\n"
        r"    _copyrightExtension, _sequenceScalableExtension, _unknownExtension, _pictureDisplayExtension,\n"
        r"    _pictureCodingExtension, _pictureSpatialScalableExtension, _pictureTemporalScalableExtension,\n",
        r"\1    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* port: libmpeg's decoder is replaced (movie.py) */\n",
    )],

    # GIF DMA starts: the platform's DMA runs them
    "src/main/467FD8.c": [
        (r"\nvoid MovDispInit\(", r"\nvoid grx_dma_write_chcr(int ch, unsigned int chcr); void MovDispInit("),
        (r"\*\(volatile u32 \*\)0x1000A000 = 0x101;", "grx_dma_write_chcr(2, 0x101);"),
    ],
    "src/main/46A8D8.c": [
        (r"\nvoid MovClearScreen\(", r"\nvoid grx_dma_write_chcr(int ch, unsigned int chcr); void MovClearScreen("),
        (r"\*\(\(volatile u32 \*\) 0x1000A000\) = 0x101;", "grx_dma_write_chcr(2, 0x101);"),
    ],
    "src/main/46B390.c": [
        (r"\nvoid MovGifSendChain\(", r"\nvoid grx_dma_write_chcr(int ch, unsigned int chcr); void MovGifSendChain("),
        (r"\*\(volatile u32 \*\)0x1000A000 = 0x105;", "grx_dma_write_chcr(2, 0x105);"),
    ],

    # The busy waits
    "src/main/464E40.c": [
        (r"\nvoid switchThread\(void\) \{\n    RotateThreadReadyQueue\(movMainPrio\);\n",
         r"\nvoid grx_frame_poll(void); void switchThread(void) {\n    RotateThreadReadyQueue(movMainPrio); grx_frame_poll();\n"),
    ],
    "src/main/465F90.c": [
        (r"(while \(movVoBuf\.count != 0 && arg0->state != 1\) \{)(\n    \})", r"\1 switchThread();\2"),
    ],

    # The counting loops: the EE's time (the counter ends as it did)
    "src/main/463F00.c": [
        (r"(s32 MovReadLoop\()", r"void grx_ee_spin(unsigned int loops);\n#line 13\n\1"),
        (r"for \(i = 0; i < 100000; i\+\+\) \{", r"i = 100000; grx_ee_spin(i); {"),
        (r"while \(--i != 0\) \{", r"grx_ee_spin(i); i = 0; {"),
        (r"while \(--j != 0\) \{", r"grx_ee_spin(j); j = 0; {"),
    ],
    "src/main/464560.c": [
        (r"(s32 MovListReadLoop\()", r"void grx_ee_spin(unsigned int loops);\n#line 13\n\1"),
        (r"for \(k = 0; k < (100000|0x80000|0x20000); k\+\+\) \{\}", r"k = \1; grx_ee_spin(k);"),
        (r"for \(w = 0; w < 0x1999A; w\+\+\) \{\}", r"w = 0x1999A; grx_ee_spin(w);"),
    ],
}
