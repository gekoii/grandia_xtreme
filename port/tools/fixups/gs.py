"""GS fixups (port/tools/fixups/README.md).

sceGsExecStoreImage (src/main/46D9A8.c, libgraph) reads GS local memory
back over the VIF1 FIFO, which the port's synchronous DMA doesn't do; the
platform layer has its own version (port/src/gs_store_image.c).
"""

REPLACES = [
    ("main", 0x46D9A8, 0x46D9A8),
]
