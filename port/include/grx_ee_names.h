/*
 * EE kernel calls named like Windows API functions (port/docs/win64.md).
 *
 * On i686 the Windows ones are stdcall, decorated in the object files
 * (_CreateThread@24), so the game's _CreateThread is a different name. On
 * x86-64 nothing is decorated and the two would be one symbol: the
 * linker stops at the duplicate, or worse, the port's own calls to
 * Windows (crash.c's SuspendThread) would reach the EE kernel's. So on
 * x86-64 the EE's get names of their own, here: included by grx_port.h
 * (every game source) and by the port's files that define or call the
 * EE's (kernel.c, its test). Files that call Windows' don't include it.
 */
#ifndef GRX_EE_NAMES_H
#define GRX_EE_NAMES_H

#ifdef _WIN64
#define CreateThread grx_ee_CreateThread
#define ExitThread grx_ee_ExitThread
#define GetThreadId grx_ee_GetThreadId
#define ResumeThread grx_ee_ResumeThread
#define SuspendThread grx_ee_SuspendThread
#define TerminateThread grx_ee_TerminateThread
#endif

#endif
