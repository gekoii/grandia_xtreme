/* main executable .rodata 0x004A6A98-0x004A6B70: IOP module paths and the
 * sound driver's ini formats. Data-only C (plan step 6b, docs/shifting.md);
 * the users are LoadIopModules (45EEB8.c), SysLoadIopModule (45F2C8.c) and
 * SndDriverInit (460B60.c).
 */
/* rodata: 0x004A6A98-0x004A6B70 */

/* LoadIopModules: the IOP replacement image, then the modules */
const char sysIoprpImagePath[] = "cdrom0:\\IOPRP234.IMG;1";
const char sysSio2manPath[] = "cdrom0:\\SIO2MAN.IRX;1";
const char sysPadmanPath[] = "cdrom0:\\PADMAN.IRX;1";
const char sysMcmanPath[] = "cdrom0:\\MCMAN.IRX;1";
const char sysMcservPath[] = "cdrom0:\\MCSERV.IRX;1";
const char sysLibsdPath[] = "cdrom0:\\LIBSD.IRX;1";
const char sysGsIopPath[] = "cdrom0:\\GS_IOP.IRX;1";

/* SysLoadIopModule: the (empty) argument string of sceSifLoadModule(path,
 * 0, args); 16 bytes in the original */
const char sysIopModuleArgs[16] = "";

/* SndDriverInit: sscanf formats for the sound ini lines */
const char sndIniFmtDec[] = "%d";
const char sndIniFmtHex[] = "%X";
const char sndIniFmtHexPair[] = "%X,0x%X";
