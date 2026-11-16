/* Pass this file to the linker with the `-T config/linker_script_extra.ld` flag */
ENTRY(_start);

/* Placeholder names some C files still use for the soft-float helpers */
func_00481250 = dpadd;
func_004812A8 = dpsub;
func_00481310 = dpmul;
func_00481838 = dpcmp;
func_00481888 = litodp;
func_00481940 = dptoli;
func_004815B8 = dpdiv;
func_004826D8 = fptodp;
func_00481AE0 = dptofp;
func_0047BFB8 = sqrtf;
func_0048281C = memset;

/* Placeholder names some C files may still use for the libgcc helpers */
func_00480DE0 = __muldi3;
func_00480D48 = __floatdidf;
func_004946F8 = __fixdfdi;
func_00494758 = __fixunsdfdi;
func_00494008 = __divdi3;
func_00494848 = __moddi3;
func_00494EB0 = __udivdi3;
func_00495480 = __umoddi3;
