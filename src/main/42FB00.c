/* rodata: 0x004A6790-0x004A67C0 */
#include "common.h"
#include "sys.h"
#include "main/n03.h"
#include "protos/main.h"

void MemSet(void *, s32, s32);
char *strcat(char *, char *);

void SysBoot(s32 arg0, char *path) {
    char buf[256];
    s32 i;

    MemSet(sysBootPath, 0, 0x40);
    sysIsNtsc = arg0;
    sysFrameCount = 0;
    LoadIopModules(path);
    SndFsMount(0x01FF1000, 0x01000000, "IMAGE");
    PadInit();
    for (i = 0; i < 0x40 && path[i] != 0; i++) {
        sysBootPath[i] = path[i];
    }
    MemSet(sysLibVersion, 0, 0x100);
    MemSet(buf, 0, 0x100);
    sprintf(buf, "%s", "Jul 30 2002");
    strcat(sysLibVersion, "GRX Library ");
    strcat(sysLibVersion, buf);
}
