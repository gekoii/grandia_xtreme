/* The port's platform layer: what the game's C calls instead of the PS2
   hardware and kernel. Only for the port's own sources (port/src/). */
#ifndef GRX_PLATFORM_H
#define GRX_PLATFORM_H

void grx_fatal(const char *fmt, ...) __attribute__((noreturn, format(printf, 1, 2)));
void grx_log(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

/* Report access violations (crash.c) */
void grx_crash_init(void);

/* Report a hang: no field for `seconds` (crash.c; 0 = off) */
void grx_watchdog_start(unsigned seconds);

/* Sample the game thread when GRX_PROFILE is set (crash.c) */
void grx_profile_start(void);

/* Each field, on the game's thread: from field GRX_FP_TRAP on, stop at the
   first NaN made or division by zero (crash.c) */
void grx_fp_trap_field(unsigned long long field);

/* Called by the generated stubs for code the port doesn't have yet */
void grx_unimplemented(const char *name);

/* Code the port doesn't link (programs.c): a MIPS program from the disc,
   called at its EE address with its first argument, and a program
   sysProgramTable names that isn't on the disc */
int grx_mips_call(unsigned int addr, unsigned int a0);
int grx_program_absent(int a0, int a1);

#endif
