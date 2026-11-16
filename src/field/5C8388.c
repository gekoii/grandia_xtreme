#include "common.h"
#include "protos/field.h"
#include "field/state.h"
#include "field/n15.h"
#include "field/n17.h"

typedef struct {
    u16 type;
    u16 id;
    u16 idx;
    u16 slot;
} Ent;


void ChrMotBindFromMap(Ent *e, void *tbl, s32 n) {
    void *o;
    s32 i;
    s32 v;

    if (n != 0) {
        mapMotionTable = FilePacMotionRelocate(tbl);
        for (i = 0; i < n; i++) {
            switch (e->type) {
            case 0:
                o = MapModelGet(e->id);
                v = FilePacGetMotion(mapMotionTable, e->idx);
                /* MapModel motTable.slots[slot], indexed by hand: the field form is 2 words off */
                o = (s32 *)o + e->slot;
                ((s32 *)o)[5] = v;
                break;
            case 1:
                o = ChrTenantGet(e->id);
                if (o != NULL) {
                    ChrMotTableSet(o, FilePacGetMotion(mapMotionTable, e->idx), e->slot);
                }
                break;
            case 2:
                o = ChrGetByNo(e->id);
                if (o != NULL) {
                    ChrMotTableSet(o, FilePacGetMotion(mapMotionTable, e->idx), e->slot);
                }
                break;
            }
            e++;
        }
    }
}
