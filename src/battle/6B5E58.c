/* cc: ee-gcc2.9-991111-01 */
/* as: ee-gcc2.9-991111-01 */
#include "common.h"
#include "protos/battle.h"
#include "gph.h"
#include "battle/env.h"
#include "battle/n31.h"
#include "pad.h"
#include "sys.h"

extern char dbgEnvLightTitleFmt[];
extern char dbgEnvLightNoFmt[];
extern char dbgEnvLightOnStr[];
extern char dbgEnvLightOffStr[];
extern char dbgEnvLightAmbStr[];
extern char dbgEnvLightCharStr[];
extern char dbgEnvLightRFmt[];
extern char dbgEnvLightGFmt[];
extern char dbgEnvLightBFmt[];
extern char dbgEnvLightAFmt[];
extern char dbgEnvLightGroundStr[];
extern char dbgEnvLightDirNoFmt[];
extern char dbgEnvLightIntensityFmt[];
extern char dbgEnvLightAngleXFmt[];
extern char dbgEnvLightAngleYFmt[];
extern char dbgEnvLightVecFmt[];
extern char dbgEnvLightModeFmt[];
extern char dbgEnvLightDirStr[];
extern char dbgEnvLightPointStr[];
extern char dbgEnvLightCharToggleStr[];
extern char dbgEnvLightGroundToggleStr[];
extern char dbgEnvLightPosXFmt[];
extern char dbgEnvLightPosYFmt[];
extern char dbgEnvLightPosZFmt[];
extern char dbgEnvLightRangeFmt[];

void GphShadowCalcMtx(void *, f32 *);
void BtlDirFromPitchYaw(s32, s32, f32 *);

void DbgEnvLightPage(void) {
    GphLight *o1;
    GphLight *o2;
    GphLight *o3;
    s32 flags;
    Pad *pad;
    s32 t1, t2, t3;

    if (!(sysState.dbgMenuFlags & 0x20000000) && sysState.dbgCursor == 0) {
        dbgBtlEnvLightNo = (dbgBtlEnvLightNo + DbgEditValue(0, 0, 1, 1.0f) < 0.0f) ? 15
                   : (dbgBtlEnvLightNo + DbgEditValue(0, 0, 1, 1.0f) > 15.0f) ? 0
                   : dbgBtlEnvLightNo + DbgEditValue(0, 0, 1, 1.0f);
    }
    GphLightListSet(envLight.lists[0]);
    o1 = GphLightGet(dbgBtlEnvLightNo);
    GphLightListSet(envLight.lists[1]);
    o2 = GphLightGet(dbgBtlEnvLightNo);
    GphLightListSet(envLight.lists[2]);
    o3 = GphLightGet(dbgBtlEnvLightNo);

    if (!(sysState.dbgMenuFlags & 0x20000000)) {
        pad = &padPort1;
        switch (dbgBtlEnvLightNo) {
        case 0:
            DbgBtlLineSelect(pad, 8);
            switch (sysState.dbgCursor) {
            case 1: envLight.amb[0][0] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 2: envLight.amb[0][1] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 3: envLight.amb[0][2] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 4: envLight.amb[0][3] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 5: envLight.amb[1][0] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 6: envLight.amb[1][1] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 7: envLight.amb[1][2] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 8: envLight.amb[1][3] += DbgEditValue(2, 0, 3, 1.0f); break;
            }
            o1->color[0] = envLight.amb[0][0];
            o1->color[1] = envLight.amb[0][1];
            o1->color[2] = envLight.amb[0][2];
            o1->color[3] = envLight.amb[0][3];
            break;
        case 1:
        case 2:
            DbgBtlLineSelect(pad, 0xE);
            switch (sysState.dbgCursor) {
            case 1: envLight.intensity[0][dbgBtlEnvLightNo - 1] += DbgEditValue(2, -2, 1, 0.1f); break;
            case 2: envLight.color[0][dbgBtlEnvLightNo - 1][0] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 3: envLight.color[0][dbgBtlEnvLightNo - 1][1] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 4: envLight.color[0][dbgBtlEnvLightNo - 1][2] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 5: envLight.color[0][dbgBtlEnvLightNo - 1][3] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 6: envLight.angles[0].x[dbgBtlEnvLightNo - 1] += DbgEditValue(1, 0, 4, 256.0f); break;
            case 7: envLight.angles[0].y[dbgBtlEnvLightNo - 1] += DbgEditValue(1, 0, 4, 256.0f); break;
            case 8: envLight.intensity[1][dbgBtlEnvLightNo - 1] += DbgEditValue(2, -2, 1, 0.1f); break;
            case 9: envLight.color[1][dbgBtlEnvLightNo - 1][0] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 10: envLight.color[1][dbgBtlEnvLightNo - 1][1] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 11: envLight.color[1][dbgBtlEnvLightNo - 1][2] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 12: envLight.color[1][dbgBtlEnvLightNo - 1][3] += DbgEditValue(2, 0, 3, 1.0f); break;
            case 13: envLight.angles[1].x[dbgBtlEnvLightNo - 1] += DbgEditValue(1, 0, 4, 256.0f); break;
            case 14: envLight.angles[1].y[dbgBtlEnvLightNo - 1] += DbgEditValue(1, 0, 4, 256.0f); break;
            }
            BtlDirFromPitchYaw(envLight.angles[0].x[dbgBtlEnvLightNo - 1], envLight.angles[0].y[dbgBtlEnvLightNo - 1], envLight.dir[0][dbgBtlEnvLightNo - 1]);
            BtlDirFromPitchYaw(envLight.angles[1].x[dbgBtlEnvLightNo - 1], envLight.angles[1].y[dbgBtlEnvLightNo - 1], envLight.dir[1][dbgBtlEnvLightNo - 1]);
            if (dbgBtlEnvLightNo == 1) {
                GphShadowCalcMtx(envShadowMtx, envLight.dir[0][0]);
                envShadowMtx[1][0] = envLight.dir[0][0][0] / ((envLight.dir[0][0][1] < 0.0f) ? -envLight.dir[0][0][1] : envLight.dir[0][0][1]);
                envShadowMtx[1][2] = envLight.dir[0][0][2] / ((envLight.dir[0][0][1] < 0.0f) ? -envLight.dir[0][0][1] : envLight.dir[0][0][1]);
                envShadowMtx[3][1] = 0.9f;
            }
            o1->color[0] = envLight.color[0][dbgBtlEnvLightNo - 1][0];
            o1->color[1] = envLight.color[0][dbgBtlEnvLightNo - 1][1];
            o1->color[2] = envLight.color[0][dbgBtlEnvLightNo - 1][2];
            o1->color[3] = envLight.color[0][dbgBtlEnvLightNo - 1][3];
            o1->intensity = envLight.intensity[0][dbgBtlEnvLightNo - 1];
            o1->dir[0] = envLight.dir[0][dbgBtlEnvLightNo - 1][0];
            o1->dir[1] = envLight.dir[0][dbgBtlEnvLightNo - 1][1];
            o1->dir[2] = envLight.dir[0][dbgBtlEnvLightNo - 1][2];
            o1->dir[3] = 0.0f;
            o2->color[0] = envLight.color[0][dbgBtlEnvLightNo - 1][0];
            o2->color[1] = envLight.color[0][dbgBtlEnvLightNo - 1][1];
            o2->color[2] = envLight.color[0][dbgBtlEnvLightNo - 1][2];
            o2->color[3] = envLight.color[0][dbgBtlEnvLightNo - 1][3];
            o2->intensity = envLight.intensity[0][dbgBtlEnvLightNo - 1];
            o2->dir[0] = envLight.dir[0][dbgBtlEnvLightNo - 1][0];
            o2->dir[1] = envLight.dir[0][dbgBtlEnvLightNo - 1][1];
            o2->dir[2] = envLight.dir[0][dbgBtlEnvLightNo - 1][2];
            o2->dir[3] = 0.0f;
            o3->color[0] = envLight.color[1][dbgBtlEnvLightNo - 1][0];
            o3->color[1] = envLight.color[1][dbgBtlEnvLightNo - 1][1];
            o3->color[2] = envLight.color[1][dbgBtlEnvLightNo - 1][2];
            o3->color[3] = envLight.color[1][dbgBtlEnvLightNo - 1][3];
            o3->intensity = envLight.intensity[1][dbgBtlEnvLightNo - 1];
            o3->dir[0] = envLight.dir[1][dbgBtlEnvLightNo - 1][0];
            o3->dir[1] = envLight.dir[1][dbgBtlEnvLightNo - 1][1];
            o3->dir[2] = envLight.dir[1][dbgBtlEnvLightNo - 1][2];
            o3->dir[3] = 0.0f;
            break;
        default:
            if (sysState.dbgCursor == 0) {
                if (pad->trigger & 0x20) {
                    envLight.lightFlags[dbgBtlEnvLightNo] ^= 0x800000;
                }
                t1 = 0;
                if (envLight.lightFlags[dbgBtlEnvLightNo] & 0x800000) {
                    if (envLight.lightFlags[dbgBtlEnvLightNo] & 0x10000) t1 = 1;
                }
                o1->enable = t1;
                t2 = 0;
                if (envLight.lightFlags[dbgBtlEnvLightNo] & 0x800000) {
                    if (envLight.lightFlags[dbgBtlEnvLightNo] & 0x10000) t2 = 1;
                }
                o2->enable = t2;
                t3 = 0;
                if (envLight.lightFlags[dbgBtlEnvLightNo] & 0x800000) {
                    if (envLight.lightFlags[dbgBtlEnvLightNo] & 0x20000) t3 = 1;
                }
                o3->enable = t3;
            }
            if (envLight.lightFlags[dbgBtlEnvLightNo] & 0x800000) {
                DbgBtlLineSelect(pad, 0xC);
                switch (sysState.dbgCursor) {
                case 1:
                    if (pad->repeat & 0xA000) {
                        envLight.lightFlags[dbgBtlEnvLightNo] ^= 0x400000;
                    }
                    o1->type = ((envLight.lightFlags[dbgBtlEnvLightNo] >> 22) ^ 1) & 1;
                    o2->type = ((envLight.lightFlags[dbgBtlEnvLightNo] >> 22) ^ 1) & 1;
                    o3->type = ((envLight.lightFlags[dbgBtlEnvLightNo] >> 22) ^ 1) & 1;
                    break;
                case 2:
                    if (pad->repeat & 0xA000) {
                        envLight.lightFlags[dbgBtlEnvLightNo] ^= 0x10000;
                    }
                    o1->enable = (envLight.lightFlags[dbgBtlEnvLightNo] >> 16) & 1;
                    o2->enable = (envLight.lightFlags[dbgBtlEnvLightNo] >> 16) & 1;
                    break;
                case 3:
                    if (pad->repeat & 0xA000) {
                        envLight.lightFlags[dbgBtlEnvLightNo] ^= 0x20000;
                    }
                    o3->enable = (envLight.lightFlags[dbgBtlEnvLightNo] >> 17) & 1;
                    break;
                case 4:
                    o2->intensity = o3->intensity = o1->intensity += DbgEditValue(2, -2, 1, 0.1f);
                    break;
                case 5:
                    o2->color[0] = o3->color[0] = o1->color[0] += DbgEditValue(2, 0, 3, 1.0f);
                    break;
                case 6:
                    o2->color[1] = o3->color[1] = o1->color[1] += DbgEditValue(2, 0, 3, 1.0f);
                    break;
                case 7:
                    o2->color[2] = o3->color[2] = o1->color[2] += DbgEditValue(2, 0, 3, 1.0f);
                    break;
                case 8:
                    o2->color[3] = o3->color[3] = o1->color[3] += DbgEditValue(2, 0, 3, 1.0f);
                    break;
                case 9:
                    o2->dir[0] = o3->dir[0] = o1->dir[0] += DbgEditValue(2, -1, 3, 1.0f);
                    break;
                case 10:
                    o2->dir[1] = o3->dir[1] = o1->dir[1] += DbgEditValue(2, -1, 3, 1.0f);
                    break;
                case 11:
                    o2->dir[2] = o3->dir[2] = o1->dir[2] += DbgEditValue(2, -1, 3, 1.0f);
                    break;
                case 12:
                    o2->range = o3->range = o1->range += DbgEditValue(2, -1, 3, 1.0f);
                    break;
                }
            }
            break;
        }
    }

    switch (dbgBtlEnvLightNo) {
    case 0:
        if (sysState.dbgCursor == 0) {
            DbgDrawCursor(1, 3);
        } else if (sysState.dbgCursor < 5) {
            DbgDrawCursor(1, sysState.dbgCursor + 7);
        } else {
            DbgDrawCursor(0x15, sysState.dbgCursor + 3);
        }
        break;
    case 1:
    case 2:
        if (sysState.dbgCursor == 0) {
            DbgDrawCursor(1, 3);
        } else if (sysState.dbgCursor < 8) {
            DbgDrawCursor(1, sysState.dbgCursor + 6);
        } else {
            DbgDrawCursor(0x15, sysState.dbgCursor - 1);
        }
        break;
    default:
        DbgDrawCursor(1, sysState.dbgCursor + 3);
        break;
    }

    flags = envLight.lightFlags[dbgBtlEnvLightNo];
    DbgPrintf(2, 2, dbgEnvLightTitleFmt, dbgEnvTitleColorStr, dbgEnvColor0Str);
    DbgPrintf(2, 3, dbgEnvLightNoFmt, dbgBtlEnvLightNo, (flags & 0x800000) ? dbgEnvLightOnStr : dbgEnvLightOffStr);
    if (!(flags & 0x800000)) {
        return;
    }
    switch (dbgBtlEnvLightNo) {
    case 1:
    case 2:
        DbgPrintf(2, 5, dbgEnvLightDirNoFmt, dbgBtlEnvLightNo - 1);
        DbgPrintf(2, 6, dbgEnvLightCharStr);
        DbgPrintf(2, 7, dbgEnvLightIntensityFmt, envLight.intensity[0][dbgBtlEnvLightNo - 1]);
        DbgPrintf(2, 8, dbgEnvLightRFmt, envLight.color[0][dbgBtlEnvLightNo - 1][0]);
        DbgPrintf(2, 9, dbgEnvLightGFmt, envLight.color[0][dbgBtlEnvLightNo - 1][1]);
        DbgPrintf(2, 10, dbgEnvLightBFmt, envLight.color[0][dbgBtlEnvLightNo - 1][2]);
        DbgPrintf(2, 11, dbgEnvLightAFmt, envLight.color[0][dbgBtlEnvLightNo - 1][3]);
        DbgPrintf(2, 12, dbgEnvLightAngleXFmt, (u16)envLight.angles[0].x[dbgBtlEnvLightNo - 1]);
        DbgPrintf(2, 13, dbgEnvLightAngleYFmt, (u16)envLight.angles[0].y[dbgBtlEnvLightNo - 1]);
        DbgPrintf(2, 14, dbgEnvLightVecFmt, envLight.dir[0][dbgBtlEnvLightNo - 1][0], envLight.dir[0][dbgBtlEnvLightNo - 1][1],
                      envLight.dir[0][dbgBtlEnvLightNo - 1][2]);
        DbgPrintf(0x16, 6, dbgEnvLightGroundStr);
        DbgPrintf(0x16, 7, dbgEnvLightIntensityFmt, envLight.intensity[1][dbgBtlEnvLightNo - 1]);
        DbgPrintf(0x16, 8, dbgEnvLightRFmt, envLight.color[1][dbgBtlEnvLightNo - 1][0]);
        DbgPrintf(0x16, 9, dbgEnvLightGFmt, envLight.color[1][dbgBtlEnvLightNo - 1][1]);
        DbgPrintf(0x16, 10, dbgEnvLightBFmt, envLight.color[1][dbgBtlEnvLightNo - 1][2]);
        DbgPrintf(0x16, 11, dbgEnvLightAFmt, envLight.color[1][dbgBtlEnvLightNo - 1][3]);
        DbgPrintf(0x16, 12, dbgEnvLightAngleXFmt, (u16)envLight.angles[1].x[dbgBtlEnvLightNo - 1]);
        DbgPrintf(0x16, 13, dbgEnvLightAngleYFmt, (u16)envLight.angles[1].y[dbgBtlEnvLightNo - 1]);
        DbgPrintf(0x16, 14, dbgEnvLightVecFmt, envLight.dir[1][dbgBtlEnvLightNo - 1][0], envLight.dir[1][dbgBtlEnvLightNo - 1][1],
                      envLight.dir[1][dbgBtlEnvLightNo - 1][2]);
        return;
    case 0:
        DbgPrintf(2, 5, dbgEnvLightAmbStr);
        DbgPrintf(2, 7, dbgEnvLightCharStr);
        DbgPrintf(2, 8, dbgEnvLightRFmt, envLight.amb[0][0]);
        DbgPrintf(2, 9, dbgEnvLightGFmt, envLight.amb[0][1]);
        DbgPrintf(2, 10, dbgEnvLightBFmt, envLight.amb[0][2]);
        DbgPrintf(2, 11, dbgEnvLightAFmt, envLight.amb[0][3]);
        DbgPrintf(0x16, 7, dbgEnvLightGroundStr);
        DbgPrintf(0x16, 8, dbgEnvLightRFmt, envLight.amb[1][0]);
        DbgPrintf(0x16, 9, dbgEnvLightGFmt, envLight.amb[1][1]);
        DbgPrintf(0x16, 10, dbgEnvLightBFmt, envLight.amb[1][2]);
        DbgPrintf(0x16, 11, dbgEnvLightAFmt, envLight.amb[1][3]);
        return;
    default:
        DbgPrintf(2, 4, dbgEnvLightModeFmt, (flags & 0x400000) ? dbgEnvLightDirStr : dbgEnvLightPointStr);
        DbgBtlPrintOnOff(2, 5, dbgEnvLightCharToggleStr, flags & 0x10000);
        DbgBtlPrintOnOff(2, 6, dbgEnvLightGroundToggleStr, flags & 0x20000);
        DbgPrintf(2, 7, dbgEnvLightIntensityFmt, o1->intensity);
        DbgPrintf(2, 8, dbgEnvLightRFmt, o1->color[0]);
        DbgPrintf(2, 9, dbgEnvLightGFmt, o1->color[1]);
        DbgPrintf(2, 10, dbgEnvLightBFmt, o1->color[2]);
        DbgPrintf(2, 11, dbgEnvLightAFmt, o1->color[3]);
        DbgPrintf(2, 12, dbgEnvLightPosXFmt, o1->dir[0]);
        DbgPrintf(2, 13, dbgEnvLightPosYFmt, o1->dir[1]);
        DbgPrintf(2, 14, dbgEnvLightPosZFmt, o1->dir[2]);
        DbgPrintf(2, 15, dbgEnvLightRangeFmt, o1->range);
        return;
    }
}
