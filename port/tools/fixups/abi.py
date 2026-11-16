"""Argument and result mismatches (port/README.md, "Argument order").

The matching C has declarations and casts that give a function another
parameter order, count or return type than its definition. On the EE that
made no difference: the EABI passes ints in $a0-$t3 and floats in $f12-$f19
independently, a parameter the caller doesn't pass still holds whatever was
in its register, and a result is read from $v0 or $f0 whatever the
declaration says. On i686 (cdecl) every argument goes on the stack in
order, and float results come back in st(0), so those calls pass or read
the wrong values. port/tools/proto_audit.py finds them
(build/port/proto_audit.txt); these fixups make each one say what the EE
did:

- floats in the wrong position: the declaration or cast gets the
  definition's order (the declared types kept, so each argument is still
  converted as the EE converted it), and every call through it the same
  permutation of its arguments;
- arguments the caller never passed: the value that was in the register,
  read from the caller's code (build/src/*.s of the matching build); a
  parameter the callee never reads gets 0;
- results read from a function that returns nothing, or returned in
  another class: the definition fixed when it was what's wrong, the call
  otherwise.

The same changes can be made in src/ (on the EE they change no code, since
each register class keeps its order); the list is in the module's report.
Patterns are literal text (re.escape), declarations anchored at the start
of their line, so each matches only what it was written for.
"""

import re


def lit(old, new):
    """Literal text -> literal text"""
    return (re.escape(old), new.replace("\\", "\\\\"))


def decl(old, new):
    """A declaration on a line of its own: 'old;' -> 'new;'"""
    return ("(?m)^" + re.escape(old + ";"), (new + ";").replace("\\", "\\\\"))


FIXUPS = {
    # ------------------------------------------------------------------
    # battle

    # BtlCamQuakeSetY is (s32, f32, f32)
    "src/battle/6823C8.c": [
        lit("((void (*)(f32, f32, s32))BtlCamQuakeSetY)(2.0f, 2.0f, 0xA)",
            "((void (*)(s32, f32, f32))BtlCamQuakeSetY)(0xA, 2.0f, 2.0f)"),
        lit("((void (*)(f32, f32, s32))BtlCamQuakeSetY)(100.0f, 50.0f, 0)",
            "((void (*)(s32, f32, f32))BtlCamQuakeSetY)(0, 100.0f, 50.0f)"),
        lit("((void (*)(f32, f32, s32))BtlCamQuakeSetY)(8.0f, 5.0f, 0x1E)",
            "((void (*)(s32, f32, f32))BtlCamQuakeSetY)(0x1E, 8.0f, 5.0f)"),
    ],
    "src/battle/6D5398.c": [
        # BtlHudDamageNumSpawn's third parameter, flags: $a2 held info.flags
        # (lw $6,20($sp) on both paths to the calls)
        lit("((void (*)(s32, VuVec4 *))BtlHudDamageNumSpawn)(info.damage, &tmp)",
            "((void (*)(s32, VuVec4 *, s32))BtlHudDamageNumSpawn)(info.damage, &tmp, info.flags)"),
        lit("((void (*)(s32, VuVec4 *))BtlHudDamageNumSpawn)(info.damage + btlHitDamageAccum[unit->id], &tmp)",
            "((void (*)(s32, VuVec4 *, s32))BtlHudDamageNumSpawn)(info.damage + btlHitDamageAccum[unit->id], &tmp, info.flags)"),
        # BtlUnitHitReact is (BtlUnit *, s32, s32, f32); two identical calls
        lit("((void (*)(BtlUnit *, s32, f32, s32))BtlUnitHitReact)(unit, hit->angle, info.knockback, info.reaction)",
            "((void (*)(BtlUnit *, s32, s32, f32))BtlUnitHitReact)(unit, hit->angle, info.reaction, info.knockback)"),
    ],
    "src/battle/6C7EA8.c": [
        lit("((s32 (*)(BtlUnit *, f32 *, f32, s32))BtlPathAvoidHeading)(u, dst, r, flags)",
            "((s32 (*)(BtlUnit *, f32 *, s32, f32))BtlPathAvoidHeading)(u, dst, flags, r)"),
    ],
    "src/battle/6D0908.c": [
        lit("((void (*)(BtlUnit *, s32, f32, s32))BtlUnitFadeTo)(NULL, 7, 1.0f, 0x20)",
            "((void (*)(BtlUnit *, s32, s32, f32))BtlUnitFadeTo)(NULL, 7, 0x20, 1.0f)"),
    ],
    # BtlUnitApproachTargetBy returns nothing, but it ends calling
    # BtlUnitMoveToward, so on the EE $v0 held its result, 1 once the unit
    # has reached its target: the skill programs that run up to the target
    # (61 of them, X Slash and Great Divide among them) loop on it. Here it
    # returns that (protos/battle.h declares it void, so the definition's
    # file keeps that declaration out of its way; the MIPS bridge reads the
    # result, port/tools/gen_mips_bridge.py)
    "src/battle/6C82F8.c": [
        lit('#include "protos/battle.h"\n',
            '#define BtlUnitApproachTargetBy BtlUnitApproachTargetBy_declared_void\n'
            '#include "protos/battle.h"\n#undef BtlUnitApproachTargetBy\n#line 3\n'),
        lit("void BtlUnitApproachTargetBy(BtlUnit *arg0, s32 mode, f32 r) {",
            "s32 BtlUnitApproachTargetBy(BtlUnit *arg0, s32 mode, f32 r) {"),
        lit("((s32 (*)(BtlUnit *, VuVec3 *, f32, f32, s32))BtlUnitMoveToward)(arg0, &t->pos, BtlUnitGetMoveSpeed(u), r, mode)",
            "return ((s32 (*)(BtlUnit *, VuVec3 *, s32, f32, f32))BtlUnitMoveToward)(arg0, &t->pos, mode, BtlUnitGetMoveSpeed(u), r)"),
    ],
    # BtlUnitCollectAreaTargetsAtTarget returns nothing, but on the EE $v0
    # still held BtlUnitCollectAreaTargets's result, the area's target mask,
    # which it also stores in btlTargetMask: read that (the definition
    # can't return it here, protos/battle.h declares it void)
    "src/battle/6A7558.c": [
        lit("((s32 (*)(BtlUnit *))BtlUnitCollectAreaTargetsAtTarget)(&btlUnits[t])",
            "(BtlUnitCollectAreaTargetsAtTarget(&btlUnits[t]), btlTargetMask)"),
        lit("((s32 (*)(BtlUnit *))BtlUnitCollectAreaTargetsAtTarget)(&btlUnits[ipGauge.actor])",
            "(BtlUnitCollectAreaTargetsAtTarget(&btlUnits[ipGauge.actor]), btlTargetMask)"),
    ],
    "src/battle/6C37D8.c": [
        lit("m = ((u32 (*)(BtlUnit *))BtlUnitCollectAreaTargetsAtTarget)(u);",
            "m = (BtlUnitCollectAreaTargetsAtTarget(u), btlTargetMask);"),
    ],
    # FxAtkRingStart is (BtlUnit *, s8, BtlUnit *, s32, f32, s32)
    "src/battle/6CC2A8.c": [
        lit("((void (*)(BtlUnit *, s32, BtlUnit *, f32, s32, s32))FxAtkRingStart)(u, 1, tg3, u->scopeRadius, u->scopeType,",
            "((void (*)(BtlUnit *, s32, BtlUnit *, s32, f32, s32))FxAtkRingStart)(u, 1, tg3, u->scopeType, u->scopeRadius,"),
        lit("((void (*)(BtlUnit *, s32, BtlUnit *, f32, s32, s32))FxAtkRingStart)(u, 0, tg3, u->scopeRadius, u->scopeType,",
            "((void (*)(BtlUnit *, s32, BtlUnit *, s32, f32, s32))FxAtkRingStart)(u, 0, tg3, u->scopeType, u->scopeRadius,"),
    ],
    "src/battle/6CF0E8.c": [
        lit("((void (*)(BtlUnit *, s8, BtlUnit *, f32, s32, s32))FxAtkRingStart)(arg0, 1, t, u->scopeRadius, u->scopeType, x)",
            "((void (*)(BtlUnit *, s8, BtlUnit *, s32, f32, s32))FxAtkRingStart)(arg0, 1, t, u->scopeType, u->scopeRadius, x)"),
        lit("((void (*)(BtlUnit *, s8, BtlUnit *, f32, s32, s32))FxAtkRingStart)(arg0, 0, t, u->scopeRadius, u->scopeType, x)",
            "((void (*)(BtlUnit *, s8, BtlUnit *, s32, f32, s32))FxAtkRingStart)(arg0, 0, t, u->scopeType, u->scopeRadius, x)"),
    ],
    # PtclGenCalcSize's second parameter only goes on to
    # ChrModelCalcInstanceSize, which ignores it: 0
    "src/battle/6B1770.c": [
        lit("((s32 (*)(PtclGenRequest *))PtclGenCalcSize)(&req)",
            "((s32 (*)(PtclGenRequest *, s32))PtclGenCalcSize)(&req, 0)"),
    ],

    # ------------------------------------------------------------------
    # field

    # CamAiFollowStub is a tail jump that hands its own a0/a1 (camera,
    # look-at) on to CamAiFollowUpdate (Camera *, f32 *)
    "src/field/5CB578.c": [
        decl("void CamAiFollowUpdate()", "void CamAiFollowUpdate(void *, f32 *)"),
        lit("void CamAiFollowStub(void) {\n    CamAiFollowUpdate();",
            "void CamAiFollowStub(void *obj, f32 *pos) {\n    CamAiFollowUpdate(obj, pos);"),
    ],
    "src/field/63AB88.c": [
        # FldMenuNumber2Build is (GphBatchQuad *, s32, s32, s32, s32, u8, f32, f32)
        decl("FldMenuQuad *FldMenuNumber2Build(FldMenuQuad *, s32, s32, s32, s32, f32, f32, u8)",
             "FldMenuQuad *FldMenuNumber2Build(FldMenuQuad *, s32, s32, s32, s32, u8, f32, f32)"),
        lit("FldMenuNumber2Build(s2, MN.val[i] < 0 ? -MN.val[i] : MN.val[i], 0, MN.slide[i] + 0x5E, i * 40 + 0x20, 1.0f, 1.0f, eq6 * 2)",
            "FldMenuNumber2Build(s2, MN.val[i] < 0 ? -MN.val[i] : MN.val[i], 0, MN.slide[i] + 0x5E, i * 40 + 0x20, eq6 * 2, 1.0f, 1.0f)"),
        lit("FldMenuNumber2Build(s2, MN.val[i] < 0 ? -MN.val[i] : MN.val[i], 0, MN.slide[i] + 0x5E, i * 40 + 0x20, 1.0f, 1.0f, eq6 * 2 + 1)",
            "FldMenuNumber2Build(s2, MN.val[i] < 0 ? -MN.val[i] : MN.val[i], 0, MN.slide[i] + 0x5E, i * 40 + 0x20, eq6 * 2 + 1, 1.0f, 1.0f)"),
        # FldMenuTaggedNumberBuild is (Spr *, s32, s32, s32, s32, u8, u8, f32, f32)
        decl("FldMenuQuad *FldMenuTaggedNumberBuild(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32, s32)",
             "FldMenuQuad *FldMenuTaggedNumberBuild(FldMenuQuad *, s32, s32, s32, s32, s32, s32, f32, f32)"),
        lit("FldMenuTaggedNumberBuild(s5, W->shop[k].count, 0, 0xE6, 8, 1.0f, 1.0f, 0, 0)",
            "FldMenuTaggedNumberBuild(s5, W->shop[k].count, 0, 0xE6, 8, 0, 0, 1.0f, 1.0f)"),
        lit("FldMenuTaggedNumberBuild(s2, price, 0, MN.ofs[k] + 0x12A, i * 24 + 8, 1.0f, 1.0f, 0, 0)",
            "FldMenuTaggedNumberBuild(s2, price, 0, MN.ofs[k] + 0x12A, i * 24 + 8, 0, 0, 1.0f, 1.0f)"),
        # FldMenuWinTextAddIndexed has ten parameters; the tenth (a stack
        # word on the EE) is never read: 0
        decl("void FldMenuWinTextAddIndexed(u8, u8, s32, s32, s32, s32, s32, s32, s32)",
             "void FldMenuWinTextAddIndexed(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32)"),
        (r"(FldMenuWinTextAddIndexed\((?:0xD, b|MN\.buf \+ 8, MN\.buf), FONT->tbl\[10\], [^;]*?, 3)\);",
         r"\1, 0);"),
    ],
    "src/field/627320.c": [
        decl("FldMenuQuad *FldMenuTaggedNumberBuild(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32, s32)",
             "FldMenuQuad *FldMenuTaggedNumberBuild(FldMenuQuad *, s32, s32, s32, s32, s32, s32, f32, f32)"),
        lit("FldMenuTaggedNumberBuild(s2, itemWork->bag[idx].count, 0, fldMenuItem.marks[idx] + 0x17C, i * 0x18 + 8, 1.0f, 1.0f, 0, 0)",
            "FldMenuTaggedNumberBuild(s2, itemWork->bag[idx].count, 0, fldMenuItem.marks[idx] + 0x17C, i * 0x18 + 8, 0, 0, 1.0f, 1.0f)"),
        lit("((void (*)(u8, u8, s32, s32, s32, s32, s32, s32, s32))FldMenuWinTextAddIndexed)(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[10], fldMenuItem.marks[idx] + 0xFE, i * 0x18 + 8, w, w, itemWork->books2[idx].type, 3)",
            "((void (*)(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32))FldMenuWinTextAddIndexed)(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[10], fldMenuItem.marks[idx] + 0xFE, i * 0x18 + 8, w, w, itemWork->books2[idx].type, 3, 0)"),
        lit("((void (*)(u8, u8, s32, s32, s32, s32, s32, s32, s32))FldMenuWinTextAddIndexed)(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[10], fldMenuItem.marks[idx] + 0x116, i * 0x18 + 8, w, w, itemWork->books[idx].type, 3)",
            "((void (*)(u8, u8, s32, s32, s32, s32, s32, s32, s32, s32))FldMenuWinTextAddIndexed)(id, fldMenuItem.page, sysGrxsysPac->msg->tbl[10], fldMenuItem.marks[idx] + 0x116, i * 0x18 + 8, w, w, itemWork->books[idx].type, 3, 0)"),
    ],
    # FldMenuNumberBuild is (GphBatchQuad *, s32, u8, s32, s32, f32, f32, s32)
    "src/field/626310.c": [
        lit("((void *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(s, itemWork->chars[i].level, 0, 0x7C, 0x1A, 0, 1.0f, 0.8f)",
            "((void *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberBuild)(s, itemWork->chars[i].level, 0, 0x7C, 0x1A, 1.0f, 0.8f, 0)"),
    ],
    "src/field/626A80.c": [
        lit("((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(s, itemWork->gold, 1, 0xB0, 4, 0x80, 1.0f, 0.9f)",
            "((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberBuild)(s, itemWork->gold, 1, 0xB0, 4, 1.0f, 0.9f, 0x80)"),
    ],
    "src/field/62C7B0.c": [
        lit("((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(s3, PtyCharCalcLevel(i + 1), 0, fldMenuPartyOrder.rowX[i] + 0x68, i * 0x38 + 0x12, 0, 1.0f, 0.8f)",
            "((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberBuild)(s3, PtyCharCalcLevel(i + 1), 0, fldMenuPartyOrder.rowX[i] + 0x68, i * 0x38 + 0x12, 1.0f, 0.8f, 0)"),
    ],
    "src/field/62DE20.c": [
        lit("((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, f32, f32, s32, s32))FldMenuNumberBuild)(s3, v, 0, 0x48, 1.0f, 1.0f, i * 0x18 + 0x45, 0)",
            "((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberBuild)(s3, v, 0, 0x48, i * 0x18 + 0x45, 1.0f, 1.0f, 0)"),
        lit("((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, f32, f32, s32, s32))FldMenuNumberBuild)(s3, itemWork->chars[fldMenuEquip.chr].level, 0, 0x64, 1.0f, 0.9f, 8, 0)",
            "((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberBuild)(s3, itemWork->chars[fldMenuEquip.chr].level, 0, 0x64, 8, 1.0f, 0.9f, 0)"),
        # FldMenuNumberTintBuild is (GphBatchQuad *, s32, s32, s32, s32, u8, u32, f32, f32)
        lit("((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, f32, f32, s32, s32, s32))FldMenuNumberTintBuild)(s3, nv, 0, 0x88, 1.0f, 1.0f, i * 0x18 + 0x45, 0, v)",
            "((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberTintBuild)(s3, nv, 0, 0x88, i * 0x18 + 0x45, 0, v, 1.0f, 1.0f)"),
    ],
    "src/field/634C90.c": [
        lit("((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(fldMenuSaveSprNumbers, rec->level[i], 0, arg1 + 0x60 + i * 0x78, arg2 + 0x2E, 0, 1.1f, 1.2f)",
            "((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberBuild)(fldMenuSaveSprNumbers, rec->level[i], 0, arg1 + 0x60 + i * 0x78, arg2 + 0x2E, 1.1f, 1.2f, 0)"),
        # FldMenuPlayTimeBuild is (GphBatchQuad *, s32, s32, s32, s32, u8, f32, f32)
        lit("((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuPlayTimeBuild)(fldMenuSaveSprNumbers, rec->playTime, 0, arg1 + 0x1AE, arg2 + 2, 0.8f, 0.8f, 0x80)",
            "((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuPlayTimeBuild)(fldMenuSaveSprNumbers, rec->playTime, 0, arg1 + 0x1AE, arg2 + 2, 0x80, 0.8f, 0.8f)"),
    ],
    "src/field/64EF00.c": [
        lit("((Spr *(*)(Spr *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberBuild)(buf, o->num->value, o->num->align, x, y, a, sx, sy)",
            "((Spr *(*)(Spr *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberBuild)(buf, o->num->value, o->num->align, x, y, sx, sy, a)"),
    ],
    # FldMenuNumberColorBuild is (GphBatchQuad *, s32, u8, s32, s32, u8, f32, f32)
    "src/field/637C80.c": [
        lit("((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuNumberColorBuild)(t, (s16)itemWork->u76F8.junc.ents[idx].unk4, 0, 0xE0, 0x30, 1.0f, 0.8f, 1)",
            "((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuNumberColorBuild)(t, (s16)itemWork->u76F8.junc.ents[idx].unk4, 0, 0xE0, 0x30, 1, 1.0f, 0.8f)"),
    ],
    # FldMenuPercentBuild is (GphBatchQuad *, s32, s32, s32, s32, u8, f32, f32)
    "src/field/631230.c": [
        lit("((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, f32, f32, s32))FldMenuPercentBuild)(q, (s32)fldMenuStatus.gauge[i], 0, (s32)(cs * g * 1.2f) + r, (s32)(sn * g) + 0x7A, 1.0f, 1.0f, 0)",
            "((FldMenuQuad *(*)(FldMenuQuad *, s32, s32, s32, s32, s32, f32, f32))FldMenuPercentBuild)(q, (s32)fldMenuStatus.gauge[i], 0, (s32)(cs * g * 1.2f) + r, (s32)(sn * g) + 0x7A, 0, 1.0f, 1.0f)"),
    ],
    # FldObjMoveVelocity is (s32, f32 *, s32, f32)
    "src/field/5BBCF0.c": [
        lit("((void (*)(ChrObj *, f32 *, f32, s32))FldObjMoveVelocity)(c, l.v, spd, k)",
            "((void (*)(ChrObj *, f32 *, s32, f32))FldObjMoveVelocity)(c, l.v, k, spd)"),
    ],
    # FldSndEmitterEnableById returns nothing; k is a dead store (it is
    # set again before it's read)
    "src/field/5FD000.c": [
        lit("k = ((s32 (*)(u16, u8))FldSndEmitterEnableById)(e->arg[1], e->arg[2]);",
            "((void (*)(u16, u8))FldSndEmitterEnableById)(e->arg[1], e->arg[2]);"),
    ],
    # GphScreenFxSet is (GphScreenFx *, u8, s32, u32, f32)
    "src/field/5BF360.c": [
        lit("((void (*)(void *, f32, u8, s32, u32))GphScreenFxSet)(fldScreenFx, 0.0f, 7, a->noHold ^ 1, a->time)",
            "((void (*)(void *, u8, s32, u32, f32))GphScreenFxSet)(fldScreenFx, 7, a->noHold ^ 1, a->time, 0.0f)"),
        lit("((void (*)(void *, f32, u8, s32, u32))GphScreenFxSet)(fldScreenFx, 0.0f, t, a->noHold ^ 1, a->time)",
            "((void (*)(void *, u8, s32, u32, f32))GphScreenFxSet)(fldScreenFx, t, a->noHold ^ 1, a->time, 0.0f)"),
    ],
    "src/field/5C0BB8.c": [
        lit("((void (*)(void *, f32, s32, s32, s32))GphScreenFxSet)(fldScreenFx, 0.0f, 4, 1, it->effectTime)",
            "((void (*)(void *, s32, s32, s32, f32))GphScreenFxSet)(fldScreenFx, 4, 1, it->effectTime, 0.0f)"),
    ],
    "src/field/619828.c": [
        lit("((void (*)(void *, u8, f32, s32, u32))GphScreenFxSet)(fldScreenFx, 6, 0.0f, 1, 0)",
            "((void (*)(void *, u8, s32, u32, f32))GphScreenFxSet)(fldScreenFx, 6, 1, 0, 0.0f)"),
        lit("((void (*)(void *, u8, f32, s32, u32))GphScreenFxSet)(fldScreenFx, 4, 0.0f, 1, 0x14)",
            "((void (*)(void *, u8, s32, u32, f32))GphScreenFxSet)(fldScreenFx, 4, 1, 0x14, 0.0f)"),
        lit("((void (*)(void *, u8, f32, s32, u32))GphScreenFxSet)(fldScreenFx, 6, 0.0f, 0, 0)",
            "((void (*)(void *, u8, s32, u32, f32))GphScreenFxSet)(fldScreenFx, 6, 0, 0, 0.0f)"),
    ],
    # MapBorderAddSegments is (Seg *, f32 *, s32, f32)
    "src/field/5E48C0.c": [
        decl("void MapBorderAddSegments(void *, void *, f32, s32)",
             "void MapBorderAddSegments(void *, void *, s32, f32)"),
        lit("MapBorderAddSegments(arg1, arg0, arg3, arg2)", "MapBorderAddSegments(arg1, arg0, arg2, arg3)"),
    ],
    # MapCollMove is (void *, Obj *, f32 *, void *, s32, s32, f32)
    "src/field/5C2D90.c": [
        decl("void MapCollMove(s32, Mdl *, f32, f32 *, void *, u16, u16)",
             "void MapCollMove(s32, Mdl *, f32 *, void *, u16, u16, f32)"),
        lit("MapCollMove(MapCollModelGet(0), a->x60, 40.0f, a->x180, a->x228, a->x224, a->x6)",
            "MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, a->x224, a->x6, 40.0f)"),
    ],
    "src/field/5C3270.c": [
        decl("void MapCollMove(s32, Obj *, f32 *, void *, f32, s32, s32)",
             "void MapCollMove(s32, Obj *, f32 *, void *, s32, s32, f32)"),
        lit("MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, 40.0f, a->x224, a->x6)",
            "MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, a->x224, a->x6, 40.0f)"),
    ],
    "src/field/5C3A78.c": [
        decl("void MapCollMove(s32, Obj *, f32 *, void *, f32, s32, s32)",
             "void MapCollMove(s32, Obj *, f32 *, void *, s32, s32, f32)"),
        lit("MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, 40.0f, a->x224, a->x6)",
            "MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, a->x224, a->x6, 40.0f)"),
    ],
    "src/field/5C3ED8.c": [
        decl("void MapCollMove(s32, Mdl *, FVEC, void *, f32, u16, u16)",
             "void MapCollMove(s32, Mdl *, FVEC, void *, u16, u16, f32)"),
        lit("MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, 40.0f, a->x224, a->x6)",
            "MapCollMove(MapCollModelGet(0), a->x60, a->x180, a->x228, a->x224, a->x6, 40.0f)"),
    ],
    "src/field/5E2CA8.c": [
        decl("Model *MapCollMove(s32, Node *, f32 *, void *, f32, s32, s32)",
             "Model *MapCollMove(s32, Node *, f32 *, void *, s32, s32, f32)"),
        lit("MapCollMove(MapCollModelGet(0), n, o->vel, o->unk228, o->unk34, o->unk224, o->unk6)",
            "MapCollMove(MapCollModelGet(0), n, o->vel, o->unk228, o->unk224, o->unk6, o->unk34)"),
    ],
    # MapHtaHitBox, MapHtaHitCapsule and MapHtaHitCylinder take a fourth,
    # float parameter: MapHtaHitArea's own r, still in $f12
    "src/field/5E50E0.c": [
        decl("s32 MapHtaHitBox(s32, s32, EvtArea *)", "s32 MapHtaHitBox(s32, s32, EvtArea *, f32)"),
        decl("s32 MapHtaHitCapsule(s32, s32, EvtArea *)", "s32 MapHtaHitCapsule(s32, s32, EvtArea *, f32)"),
        decl("s32 MapHtaHitCylinder(s32, s32, EvtArea *)", "s32 MapHtaHitCylinder(s32, s32, EvtArea *, f32)"),
        (r"(return MapHtaHit(?:Box|Capsule|Cylinder)\(arg0, arg1, arg2)\);", r"\1, r);"),
    ],
    # MapPolyHitTest is (f32 *, f32 *, Item *, f32)
    "src/field/5E6CB0.c": [
        decl("s32 MapPolyHitTest(f32 *, f32 *, f32, MapPolyTri *)", "s32 MapPolyHitTest(f32 *, f32 *, MapPolyTri *, f32)"),
        lit("MapPolyHitTest(pos, vel, dy, it)", "MapPolyHitTest(pos, vel, it, dy)"),
        lit("MapPolyHitTest(pos, vel, 1.0f, it)", "MapPolyHitTest(pos, vel, it, 1.0f)"),
    ],
    # MiniConcertBurstSpawn is (u8, f32, f32)
    "src/field/647BB8.c": [
        lit("((void (*)(f32, f32, u8))MiniConcertBurstSpawn)(e->unk60, e->unk64, e->unk79)",
            "((void (*)(u8, f32, f32))MiniConcertBurstSpawn)(e->unk79, e->unk60, e->unk64)"),
    ],
    "src/field/5E12F0.c": [
        lit("((s32 (*)(PtclGenRequest *))PtclGenCalcSize)(ptclFldWork->links[i][k])",
            "((s32 (*)(PtclGenRequest *, s32))PtclGenCalcSize)(ptclFldWork->links[i][k], 0)"),
    ],
    # VuPointInTriangle is (f32 *, FVECTOR *, f32), VuTriPlaneNear (f32 *, Tri *, f32)
    "src/field/5E6250.c": [
        decl("s32 VuTriPlaneNear(f32 *, f32, FVECTOR *)", "s32 VuTriPlaneNear(f32 *, FVECTOR *, f32)"),
        decl("s32 VuPointInTriangle(f32 *, f32, FVECTOR *)", "s32 VuPointInTriangle(f32 *, FVECTOR *, f32)"),
        lit("VuTriPlaneNear(v[0], lim, tri)", "VuTriPlaneNear(v[0], tri, lim)"),
        lit("VuPointInTriangle(v[0], lim, tri)", "VuPointInTriangle(v[0], tri, lim)"),
    ],
    "src/field/61D810.c": [
        decl("s32 VuTriPlaneNear(f32 *, f32, f32 *)", "s32 VuTriPlaneNear(f32 *, f32 *, f32)"),
        decl("s32 VuPointInTriangle(f32 *, f32, f32 *)", "s32 VuPointInTriangle(f32 *, f32 *, f32)"),
        lit("VuTriPlaneNear(SPR(2), arg2, SPR(4))", "VuTriPlaneNear(SPR(2), SPR(4), arg2)"),
        lit("VuPointInTriangle(SPR(2), arg2, SPR(4))", "VuPointInTriangle(SPR(2), SPR(4), arg2)"),
    ],

    # ------------------------------------------------------------------
    # main

    "src/main/431300.c": [
        # CamCalcMatrix (Camera *): CamApply's arg0, still in $a0
        decl("void CamCalcMatrix()", "void CamCalcMatrix(Camera *)"),
        lit("    CamCalcMatrix();", "    CamCalcMatrix(arg0);"),
        # GphViewSetFog is (s32, s32, s32, s32, f32, f32, f32)
        lit("((void (*)(s32, f32, f32, f32, s32, s32, s32))GphViewSetFog)(arg0->viewNo, arg0->fogDist, arg0->fogMax, arg0->fogMin, arg0->fogColor[0], arg0->fogColor[1], arg0->fogColor[2])",
            "((void (*)(s32, s32, s32, s32, f32, f32, f32))GphViewSetFog)(arg0->viewNo, arg0->fogColor[0], arg0->fogColor[1], arg0->fogColor[2], arg0->fogDist, arg0->fogMax, arg0->fogMin)"),
    ],
    "src/main/431110.c": [
        decl("void GphViewSetFog(s32, f32, f32, f32, s32, s32, s32)", "void GphViewSetFog(s32, s32, s32, s32, f32, f32, f32)"),
        lit("GphViewSetFog(p->viewNo, p->fogDist, p->fogMax, p->fogMin, p->fogColor[0], p->fogColor[1], p->fogColor[2])",
            "GphViewSetFog(p->viewNo, p->fogColor[0], p->fogColor[1], p->fogColor[2], p->fogDist, p->fogMax, p->fogMin)"),
    ],
    # The VU1 render stages hand their own a0/a1 (mesh, texture) on to the
    # mesh drivers (GphGaObject *, s32), untouched
    "src/main/430DF0.c": [
        decl("extern void GphMeshDrawClip(void)", "extern void GphMeshDrawClip(GphGaObject *, s32)"),
        lit("    GphMeshDrawClip();", "    GphMeshDrawClip(arg0, (s32)arg1);"),
    ],
    "src/main/430E70.c": [
        decl("void GphMeshDrawVu0(void)", "void GphMeshDrawVu0(GphGaObject *, s32)"),
        lit("void GphVu1RenderStage2(void) {\n    GphMeshDrawVu0();",
            "void GphVu1RenderStage2(GphGaObject *mesh, s32 tex) {\n    GphMeshDrawVu0(mesh, tex);"),
    ],
    "src/main/430EF0.c": [
        decl("void GphMeshDrawNoClip(void)", "void GphMeshDrawNoClip(GphGaObject *, s32)"),
        lit("void GphVu1RenderStage3(void) {\n    GphMeshDrawNoClip();",
            "void GphVu1RenderStage3(GphGaObject *mesh, s32 tex) {\n    GphMeshDrawNoClip(mesh, tex);"),
    ],
    # GphMotKeyLerpVec is (GphMotKeys *, f32, void *)
    "src/main/434FE8.c": [
        decl("void GphMotKeyLerpVec(s32, s128 *, f32)", "void GphMotKeyLerpVec(s32, f32, s128 *)"),
        lit("GphMotKeyLerpVec(*q, arg0, f)", "GphMotKeyLerpVec(*q, f, arg0)"),
    ],
    "src/main/435028.c": [
        decl("void GphMotKeyLerpVec(s32, void *, f32)", "void GphMotKeyLerpVec(s32, f32, void *)"),
        lit("GphMotKeyLerpVec(*q, arg0, f)", "GphMotKeyLerpVec(*q, f, arg0)"),
        lit("GphMotKeyLerpVec(*q, p2, f)", "GphMotKeyLerpVec(*q, f, p2)"),
    ],
    # GphTexPrim2DBuild's second parameter is a u64 (one register on the
    # EE: GphTexPrim2DDraw's s32 arg0, sign-extended, as the conversion does)
    "src/main/43DD40.c": [
        decl("void GphTexPrim2DBuild(s32, s32, s32, s32)", "void GphTexPrim2DBuild(s32, u64, s32, s32)"),
    ],
    # fabsf was implicit (ee-gcc's builtin: abs.s)
    "src/main/43E8E0.c": [
        decl("f32 VuVecDot(f32 *, f32 *)", "f32 VuVecDot(f32 *, f32 *); float fabsf(float)"),
    ],
    "src/main/4023A8.c": [
        # both calls
        lit("((s32 (*)(PtclGenRequest *))PtclGenCalcSize)(&req)",
            "((s32 (*)(PtclGenRequest *, s32))PtclGenCalcSize)(&req, 0)"),
    ],
    # The definition: SndPlayAtPos returns SndPlay's result (its last
    # call; SndSeUpdate reads it from $v0)
    "src/main/461460.c": [
        lit("void SndPlayAtPos(Obj *arg0) {", "s32 SndPlayAtPos(Obj *arg0) {"),
        lit("    SndPlay(&m);\n}", "    return SndPlay(&m);\n}"),
    ],
    # The definition: VuSinAngle returns a float (port/game/main/45D6B0.c),
    # and so does VuCosf, which passes it on in $f0
    "src/main/45D648.c": [
        decl("s32 VuSinAngle(s32)", "f32 VuSinAngle(s32)"),
        lit("s32 VuCosf(f32 a) {", "f32 VuCosf(f32 a) {"),
    ],
    # VuMtxSetRot[XYZ] (f32 *, s32): the angle is VuMtxRot[XYZ]'s own arg1,
    # still in $a1
    "src/main/45DF50.c": [
        decl("void VuMtxSetRotX(s32 *)", "void VuMtxSetRotX(s32 *, s32)"),
        lit("    VuMtxSetRotX(buf);", "    VuMtxSetRotX(buf, arg1);"),
    ],
    "src/main/45DF98.c": [
        decl("void VuMtxSetRotY(void *)", "void VuMtxSetRotY(void *, s32)"),
        lit("    VuMtxSetRotY(buf);", "    VuMtxSetRotY(buf, arg1);"),
    ],
    "src/main/45DFE0.c": [
        decl("void VuMtxSetRotZ(void *)", "void VuMtxSetRotZ(void *, s32)"),
        lit("    VuMtxSetRotZ(buf);", "    VuMtxSetRotZ(buf, arg1);"),
    ],
    # VuVecCopy (dst, src): the source is DbgLightAngleSet's arg1, still in $a1
    "src/main/420730.c": [
        decl("void VuVecCopy(DbgLightAngle *)", "void VuVecCopy(DbgLightAngle *, void *)"),
        lit("    VuVecCopy(&dbgLightAngles[arg0]);", "    VuVecCopy(&dbgLightAngles[arg0], arg1);"),
    ],

    # libgcc's soft float (linked: VIEWER's acosf/asinf wrappers call
    # fptodp/dptofp): __pack_d and __pack_f return a double / float, which
    # on i686 comes back in st(0) and must be read as that (a call through
    # a void declaration leaves it on the x87 stack)
    "src/main/481888.c": [
        # litodp returns the double's bits (s64), as it did on the EE
        decl("s64 __pack_d(fp_number_type *)", "double __pack_d(fp_number_type *)"),
        lit("    return __pack_d(&in);",
            "    { union { double d; s64 q; } u_; u_.d = __pack_d(&in); return u_.q; }"),
    ],
    "src/main/481AB0.c": [
        decl("void __pack_d(S *)", "double __pack_d(S *)"),
    ],
    "src/main/4826A8.c": [
        decl("void __pack_f(s32 *)", "f32 __pack_f(s32 *)"),
    ],
    # dptofp and fptodp are ee-gcc's names for libgcc's __truncdfsf2 and
    # __extendsfdf2. Their C here (481AE0.c, 4826D8.c) returns nothing (on
    # the EE the result was left in a register), so the wrappers convert
    # natively instead: the same IEEE conversions
    "src/viewer/7027D8.c": [
        (r"(?m)^f32 dptofp\(double\);$", "#define dptofp(d) ((f32)(d))"),
        (r"(?m)^double fptodp\(f32\) __attribute__\(\(const\)\);$", "#define fptodp(f) ((double)(f))"),
    ],
    "src/viewer/7028D8.c": [
        (r"(?m)^f32 dptofp\(double\);$", "#define dptofp(d) ((f32)(d))"),
        (r"(?m)^double fptodp\(f32\) __attribute__\(\(const\)\);$", "#define fptodp(f) ((double)(f))"),
    ],

    # Sony's SDK
    "src/main/472D60.c": [
        # _decodeFramePicture gets _decodePicture's own three arguments
        decl("s32 _decodeFramePicture(void)", "s32 _decodeFramePicture(s32, s32, s32)"),
        lit("    return _decodeFramePicture();", "    return _decodeFramePicture(arg0, arg1, arg2);"),
    ],
    # sceMpegDemuxPssRing returns the bytes it took, an s32; declared s16
    # here, the movie player read 0x3F00E (its first call, 252 KB) as
    # -4082, gave the read buffer bytes back instead of freeing them, and
    # the movie stopped once the buffer had wrapped
    "src/main/463F00.c": [
        decl("s16 sceMpegDemuxPssRing(VideoDec *, void *, s32, MovReadBuf *, s32)",
             "s32 sceMpegDemuxPssRing(VideoDec *, void *, s32, MovReadBuf *, s32)"),
    ],
    "src/main/464560.c": [
        decl("s16 sceMpegDemuxPssRing(void *, void *, s32, void *, s32)",
             "s32 sceMpegDemuxPssRing(void *, void *, s32, void *, s32)"),
    ],
    "src/main/471B50.c": [
        decl("s16 sceMpegDemuxPssRing(s32, s32, s32, s32, s32)", "s32 sceMpegDemuxPssRing(s32, s32, s32, s32, s32)"),
    ],
    "src/main/4790E8.c": [
        # _sceMpegBsShowBits (bs, n): _sceMpegBsGetBits's own arguments
        lit("((s32 (*)(void))_sceMpegBsShowBits)()", "((s32 (*)(s32, s32))_sceMpegBsShowBits)(arg0, arg1)"),
    ],
    "src/main/491A30.c": [
        # _sceFsGetFd (fd): sceDread's first parameter is the descriptor
        lit("((SceFsFd *(*)(void))_sceFsGetFd)()", "((SceFsFd *(*)(u32))_sceFsGetFd)(unused)"),
    ],
    "src/main/48D8B0.c": [
        # On the EE, __fixunsdfdi's u64 (one register) went on to
        # _scePrintfDoubleToInt (double, the same register): it reads the
        # integer's bits as a double, so the same bits are passed here
        decl("extern s32 _scePrintfDoubleToInt(s32)", "extern s32 _scePrintfDoubleToInt(double)"),
        decl("extern s32 func_00494758(double)", "extern u64 func_00494758(double)"),
        lit("    n = _scePrintfDoubleToInt(func_00494758(x * 1000000.0));",
            "    { union { u64 q; double d; } u_; u_.q = func_00494758(x * 1000000.0); n = _scePrintfDoubleToInt(u_.d); }"),
    ],
    # sceSifLoadStartModuleBuffer passes its four arguments on to
    # _sceSifLoadModuleBuffer untouched. protos/main.h declares it (void),
    # which its definition can't then contradict: renamed around the include
    "src/main/493570.c": [
        lit('#include "protos/main.h"\n\n\n',
            '#define sceSifLoadStartModuleBuffer sceSifLoadStartModuleBuffer_void\n#include "protos/main.h"\n'
            "#undef sceSifLoadStartModuleBuffer\n"),
        lit("s64 sceSifLoadStartModuleBuffer(void) {\n    return ((s32 (*)(void))_sceSifLoadModuleBuffer)();",
            "s64 sceSifLoadStartModuleBuffer(s32 a0, s32 len, void *src, s32 *out) {\n"
            "    return _sceSifLoadModuleBuffer(a0, len, src, out);"),
    ],
    # The interrupt-mask calls: libkernel's iEnableIntc(cause) etc. hand
    # their argument to the syscall and return its result
    "src/main/48CCC0.c": [
        decl("void _iEnableIntc(void)", "s32 _iEnableIntc(s32)"),
        (r"void iEnableIntc\(void\) \{\n    _iEnableIntc\(\);\n(.*)\n\}",
         r"s32 iEnableIntc(s32 cause) {\n    s32 r = _iEnableIntc(cause);\n\1 return r;\n}"),
    ],
    "src/main/48CCE0.c": [
        decl("void _iDisableIntc(void)", "s32 _iDisableIntc(s32)"),
        (r"void iDisableIntc\(void\) \{\n    _iDisableIntc\(\);\n(.*)\n\}",
         r"s32 iDisableIntc(s32 cause) {\n    s32 r = _iDisableIntc(cause);\n\1 return r;\n}"),
    ],
    "src/main/48CD00.c": [
        decl("void _iEnableDmac(void)", "s32 _iEnableDmac(s32)"),
        (r"void iEnableDmac\(void\) \{\n    _iEnableDmac\(\);\n(.*)\n\}",
         r"s32 iEnableDmac(s32 channel) {\n    s32 r = _iEnableDmac(channel);\n\1 return r;\n}"),
    ],
    "src/main/48CD20.c": [
        decl("void _iDisableDmac(void)", "s32 _iDisableDmac(s32)"),
        (r"void iDisableDmac\(void\) \{\n    _iDisableDmac\(\);\n(.*)\n\}",
         r"s32 iDisableDmac(s32 channel) {\n    s32 r = _iDisableDmac(channel);\n\1 return r;\n}"),
    ],
}
