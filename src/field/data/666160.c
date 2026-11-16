/* FIELD .data: the OCC (object control) handler tables and the tenant AI
 * op table, with the variables around them. Data-only C (plan step 6b,
 * docs/shifting.md). Don't include the project headers here: declare what
 * the data refers to locally.
 */
/* data: 0x00666160-0x00666788 */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;
typedef void (*Func)();

/* A loaded table: {count, entries} */
typedef struct {
    u32 count;
    void *entries;
} Table;

extern void ChrTenantOpAppear();
extern void ChrTenantOpChaseLeader();
extern void ChrTenantOpEnd();
extern void ChrTenantOpFleeLeader();
extern void ChrTenantOpFlyRandomDir();
extern void ChrTenantOpFlyToLeader();
extern void ChrTenantOpGoToLeaderPos();
extern void ChrTenantOpGroundOff();
extern void ChrTenantOpGroundOn();
extern void ChrTenantOpHide();
extern void ChrTenantOpHopRandomDir();
extern void ChrTenantOpHopToLeader();
extern void ChrTenantOpJumpToPoint();
extern void ChrTenantOpLoop();
extern void ChrTenantOpNop();
extern void ChrTenantOpRestartAi();
extern void ChrTenantOpSenseOff();
extern void ChrTenantOpSenseOn();
extern void ChrTenantOpSightOff();
extern void ChrTenantOpSightOn();
extern void ChrTenantOpStartOccs();
extern void ChrTenantOpTurnTo();
extern void ChrTenantOpVanish();
extern void ChrTenantOpWait();
extern void ChrTenantOpWalkByLeaderAngle();
extern void ChrTenantOpWalkRandomDir();
extern void ChrTenantOpWalkToLeader();
extern void ChrTenantOpWalkToLeaderFlat();
extern void ChrTenantOpWalkToPoint();
extern void ChrTenantOpWarpToPoint();
extern u8 mapEntranceTableBuf[];
extern void OccAlphaBackwardStart();
extern void OccAlphaForwardStart();
extern void OccAlphaPulseBackward();
extern void OccAlphaPulseBackwardStart();
extern void OccAlphaPulseForward();
extern void OccAlphaPulseForwardStart();
extern void OccAlphaUpdate();
extern void OccCamScriptForward();
extern void OccCameraSetPosForwardStart();
extern void OccChrFaceForwardStart();
extern void OccChrFlagForward();
extern void OccChrFlagsForward();
extern void OccChrLookForward();
extern void OccChrLookForwardStart();
extern void OccChrMoveForward();
extern void OccChrMoveForwardStart();
extern void OccChrTurnForward();
extern void OccChrTurnForwardStart();
extern void OccFogFadeForward();
extern void OccFogFadeForwardStart();
extern void OccForwardStartDefault();
extern void OccKind13Nop();
extern void OccKind25Nop();
extern void OccKind28Nop();
extern void OccKind32Nop();
extern void OccLensFlareForward();
extern void OccLightBackwardStart();
extern void OccLightForwardStart();
extern void OccLightUpdate();
extern void OccMapEffectForwardStart();
extern void OccMapJumpForward();
extern void OccMapJumpForwardStart();
extern void OccMapReloadForwardStart();
extern void OccMiscBackward();
extern void OccMiscBackwardStart();
extern void OccMiscForward();
extern void OccMiscForwardStart();
extern void OccMotionCtrlForward();
extern void OccMotionForward();
extern void OccMotionForwardStart();
extern void OccMotionTimedForward();
extern void OccMotionTimedForwardStart();
extern void OccMoveBackward();
extern void OccMoveBackwardStart();
extern void OccMoveBegin();
extern void OccMoveEnd();
extern void OccMoveForward();
extern void OccMoveForwardStart();
extern void OccMovieForward();
extern void OccObjParamForward();
extern void OccObjParamForwardStart();
extern void OccPartCollapseBackward();
extern void OccPartCollapseBackwardStart();
extern void OccPartCollapseForward();
extern void OccPartCollapseForwardStart();
extern void OccPartFlipStart();
extern void OccPartFlipUpdate();
extern void OccPartPairBackward();
extern void OccPartPairBackwardStart();
extern void OccPartPairBegin();
extern void OccPartPairForward();
extern void OccPartPairForwardStart();
extern void OccParticleForwardStart();
extern void OccPathEnd();
extern void OccPathForward();
extern void OccPathForwardStart();
extern void OccPlayerLadderForward();
extern void OccQuakeForwardStart();
extern void OccRotateBackward();
extern void OccRotateBackwardStart();
extern void OccRotateBegin();
extern void OccRotateForward();
extern void OccRotateForwardStart();
extern void OccScaleBackward();
extern void OccScaleBackwardStart();
extern void OccScaleBegin();
extern void OccScaleForward();
extern void OccScaleForwardStart();
extern void OccScreenFxForwardStart();
extern void OccScriptForward();
extern void OccShakeBegin();
extern void OccShakeForward();
extern void OccShakeForwardStart();
extern void OccSoundForward();
extern void OccStateSetForward();
extern void OccStateSetForwardStart();
extern void OccSwitchForward();
extern void OccSwitchForwardStart();
extern void OccTrapForward();
extern void OccTrapForwardStart();
extern void OccUvScrollForward();
extern void OccUvScrollForwardStart();
extern void OccVibrateForwardStart();
extern void OccWipeForward();

Table occTable = { 0 }; /* 00666160: 0x80-byte entries */
/* pass (0 or 1) per OCC kind in OccUpdate */
u16 occKindPassTable[52] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0,
}; /* 00666168 */
/* Handlers per OCC kind (0 = none) */
Func occBeginTable[52] = {
    0, 0, OccMoveBegin, OccRotateBegin,
    OccScaleBegin, 0, 0, 0,
    0, 0, 0, OccPartPairBegin,
    0, 0, 0, OccShakeBegin,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
}; /* 006661D0 */
Func occEndTable[51] = {
    0, 0, OccMoveEnd, 0,
    0, 0, 0, 0,
    0, 0, OccPathEnd, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0,
}; /* 006662A0 */
u32 occsTable = 0; /* 0066636C: OCCS table (0x98-byte entries) */
Func occForwardStartTable[52] = {
    OccForwardStartDefault, OccMapJumpForwardStart, OccMoveForwardStart, OccRotateForwardStart,
    OccScaleForwardStart, OccSwitchForwardStart, OccAlphaForwardStart, OccMotionForwardStart,
    OccForwardStartDefault, OccPartFlipStart, OccPathForwardStart, OccPartPairForwardStart,
    OccTrapForwardStart, OccForwardStartDefault, OccForwardStartDefault, OccShakeForwardStart,
    OccForwardStartDefault, OccObjParamForwardStart, OccForwardStartDefault, OccForwardStartDefault,
    OccChrMoveForwardStart, OccFogFadeForwardStart, OccMiscForwardStart, OccLightForwardStart,
    OccUvScrollForwardStart, OccForwardStartDefault, OccStateSetForwardStart, OccForwardStartDefault,
    OccForwardStartDefault, OccAlphaPulseForwardStart, OccPartCollapseForwardStart, OccForwardStartDefault,
    OccForwardStartDefault, OccMotionTimedForwardStart, 0, OccForwardStartDefault,
    OccForwardStartDefault, 0, 0, OccScreenFxForwardStart,
    OccMapEffectForwardStart, OccCameraSetPosForwardStart, 0, OccMapReloadForwardStart,
    OccVibrateForwardStart, OccChrFaceForwardStart, OccChrTurnForwardStart, OccChrLookForwardStart,
    OccParticleForwardStart, 0, OccQuakeForwardStart, 0,
}; /* 00666370 */
Func occForwardTable[52] = {
    OccScriptForward, OccMapJumpForward, OccMoveForward, OccRotateForward,
    OccScaleForward, OccSwitchForward, OccAlphaUpdate, OccMotionForward,
    OccMotionCtrlForward, OccPartFlipUpdate, OccPathForward, OccPartPairForward,
    OccTrapForward, OccKind13Nop, OccCamScriptForward, OccShakeForward,
    OccSoundForward, OccObjParamForward, OccChrFlagForward, OccPlayerLadderForward,
    OccChrMoveForward, OccFogFadeForward, OccMiscForward, OccLightUpdate,
    OccUvScrollForward, OccKind25Nop, OccStateSetForward, OccMovieForward,
    OccKind28Nop, OccAlphaPulseForward, OccPartCollapseForward, OccWipeForward,
    OccKind32Nop, OccMotionTimedForward, 0, OccChrFlagsForward,
    OccLensFlareForward, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, OccChrTurnForward, OccChrLookForward,
    0, 0, 0, 0,
}; /* 00666440 */
Func occBackwardStartTable[52] = {
    0, 0, OccMoveBackwardStart, OccRotateBackwardStart,
    OccScaleBackwardStart, 0, OccAlphaBackwardStart, 0,
    0, OccPartFlipStart, 0, OccPartPairBackwardStart,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, OccMiscBackwardStart, OccLightBackwardStart,
    0, 0, 0, 0,
    0, OccAlphaPulseBackwardStart, OccPartCollapseBackwardStart, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
}; /* 00666510 */
Func occBackwardTable[51] = {
    0, 0, OccMoveBackward, OccRotateBackward,
    OccScaleBackward, 0, OccAlphaUpdate, 0,
    0, OccPartFlipUpdate, 0, OccPartPairBackward,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, OccMiscBackward, OccLightUpdate,
    0, 0, 0, 0,
    0, OccAlphaPulseBackward, OccPartCollapseBackward, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0,
}; /* 006665E0 */
Table *mapEntranceTable = (Table *)mapEntranceTableBuf; /* 006666AC: 0x40-byte entries */
/* Tenant jumps: curve step per frame and apex height per unit of the height
 * argument (large: >= 11, medium: 6-10, small: < 6) */
f32 chrTenantJumpStepLarge = 0.03f; /* 006666B0 */
f32 chrTenantJumpStepMedium = 0.04f; /* 006666B4 */
f32 chrTenantJumpStepSmall = 0.05f; /* 006666B8 */
f32 chrTenantJumpHeightLarge = 20.5f; /* 006666BC */
f32 chrTenantJumpHeightMedium = 18.5f; /* 006666C0 */
f32 chrTenantJumpHeightSmall = 16.5f; /* 006666C4 */
/* Tenant AI ops 0-0x1D (ChrTenantAiUpdate) */
Func chrTenantOpTable[30] = {
    ChrTenantOpNop, ChrTenantOpEnd, ChrTenantOpWait, ChrTenantOpWalkToPoint,
    ChrTenantOpJumpToPoint, ChrTenantOpWarpToPoint, ChrTenantOpChaseLeader, ChrTenantOpWalkByLeaderAngle,
    ChrTenantOpHopToLeader, ChrTenantOpWalkRandomDir, ChrTenantOpHopRandomDir, ChrTenantOpWalkToLeader,
    ChrTenantOpFleeLeader, ChrTenantOpSenseOff, ChrTenantOpSenseOn, ChrTenantOpSightOff,
    ChrTenantOpSightOn, ChrTenantOpAppear, ChrTenantOpVanish, ChrTenantOpRestartAi,
    ChrTenantOpFlyToLeader, ChrTenantOpFlyRandomDir, ChrTenantOpStartOccs, ChrTenantOpGoToLeaderPos,
    ChrTenantOpHide, ChrTenantOpWalkToLeaderFlat, ChrTenantOpGroundOff, ChrTenantOpGroundOn,
    ChrTenantOpTurnTo, ChrTenantOpLoop,
}; /* 006666C8 */
/* per op: 1 = runs in the same frame as the previous op */
s16 chrTenantOpChainFlags[31] = {
    1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1,
    1, 0, 0, 1, 0, 0, 1, 0,
    0, 0, 1, 1, 1, 0, 0,
}; /* 00666740 */
u8 chrTenantAiUnusedFlag = 0; /* 0066677E */
Table chrTenantAreaTable = { 0 }; /* 00666780: 0x68-byte wander areas */
