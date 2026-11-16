#include "common.h"
#include "protos/field.h"
#include "pad.h"


extern s32 dbgFldMonsterInfoPage;
extern s32 dbgFldMonsterInfoPages;
extern char dbgMonInfoStrTitle[];


void DbgFldMonsterInfoMenu(void) {
    void *w;
    Pad *pad;
    u16 btn;

    pad = &padPort1;
    w = AmapGetWork();
    dbgFldMonsterInfoPages = AmapDbgItemLogPage(w, pad, 0) + 2;
    if (!(padPort1.held & 2)) {
        btn = pad->trigger;
        if ((btn & 4) && dbgFldMonsterInfoPage != 0) {
            dbgFldMonsterInfoPage--;
        }
        if ((btn & 8) && dbgFldMonsterInfoPage < dbgFldMonsterInfoPages - 1) {
            dbgFldMonsterInfoPage++;
        }
    }
    DbgPrintf(2, 3, dbgMonInfoStrTitle, dbgFldMonsterInfoPage + 1, dbgFldMonsterInfoPages);
    if (dbgFldMonsterInfoPage < dbgFldMonsterInfoPages - 2) {
        AmapDbgItemLogPage(w, &padPort1, dbgFldMonsterInfoPage + 1);
    }
    if (dbgFldMonsterInfoPage == dbgFldMonsterInfoPages - 2) {
        DbgFldMonsterTablePage();
    }
    if (dbgFldMonsterInfoPage == dbgFldMonsterInfoPages - 1) {
        DbgFldSetFilePage();
    }
}
