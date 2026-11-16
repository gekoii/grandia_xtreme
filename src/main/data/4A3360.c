/* main executable: the start of a file's .rodata - the file names of the
 * system data (SysDataInit 41D1B8, SysPartyPacLoad 41E020). Data-only C
 * (plan step 6b, docs/shifting.md). The next file's .rodata (4A3400.c)
 * starts 16-aligned, which recreates the zeros after sysPathPartyPac.
 * Don't include the project headers here: declare what the data refers to
 * locally.
 */
/* rodata: 0x004A3360-0x004A3400 */

/* A header's unused static string: the first thing in each file's .rodata */
static const char save[] = "save";

/* SysDataInit */
const char sysPathGbxPac[] = "SYS/GBX.PAC";
const char sysPathGrxsysPac[] = "SYS/GRXSYS.PAC";
const char sysPathSndIdx[] = "MUSIC/GRX_SND.IDX";
const char sysPathVmsInit[] = "MUSIC/VMSINIT.INI";
const char sysPathSysSe[] = "MUSIC/SYS_F.SE";
const char sysPath6500Se[] = "MUSIC/6500.SE";
const char sysPathFaceDat[] = "SYS/FACE.DAT";

/* SysPartyPacLoad */
const char sysPathPartyPac[] = "SYS/PARTY.PAC";
