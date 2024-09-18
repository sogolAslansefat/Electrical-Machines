#include "__cf_untitled.h"
#include <math.h>
#include "untitled_acc.h"
#include "untitled_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T untitled_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T * bufSzPtr ,
int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T tMinusDelay ,
real_T * * tBufPtr , real_T * * uBufPtr , real_T * * xBufPtr , boolean_T
isfixedbuf , boolean_T istransportdelay , int_T * maxNewBufSzPtr ) { int_T
testIdx ; int_T tail = * tailPtr ; int_T bufSz = * bufSzPtr ; real_T * tBuf =
* tBufPtr ; real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if (
istransportdelay ) { numBuffer = 3 ; xBuf = * xBufPtr ; } testIdx = ( tail <
( bufSz - 1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] )
&& ! isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T *
tempX = ( NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024
; if ( newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU
= ( real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if (
tempU == ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * tBufPtr = tempT ; * uBufPtr = tempU ; if (
istransportdelay ) * xBufPtr = tempX ; } else { * tailPtr = testIdx ; }
return ( true ) ; } real_T untitled_acc_rt_TDelayInterpolate ( real_T
tMinusDelay , real_T tStart , real_T * tBuf , real_T * uBuf , int_T bufSz ,
int_T * lastIdx , int_T oldestIdx , int_T newIdx , real_T initOutput ,
boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput ) { int_T i ;
real_T yout , t1 , t2 , u1 , u2 ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } void rt_ssGetBlockPath (
SimStruct * S , int_T sysIdx , int_T blkIdx , char_T * * path ) {
_ssGetBlockPath ( S , sysIdx , blkIdx , path ) ; } void rt_ssSet_slErrMsg (
SimStruct * S , void * diag ) { if ( ! _ssIsErrorStatusAslErrMsg ( S ) ) {
_ssSet_slErrMsg ( S , diag ) ; } else { _ssDiscardDiagnostic ( S , diag ) ; }
} void rt_ssReportDiagnosticAsWarning ( SimStruct * S , void * diag ) {
_ssReportDiagnosticAsWarning ( S , diag ) ; } static void mdlOutputs (
SimStruct * S , int_T tid ) { boolean_T rtb_B_4_4_0 ; int32_T isHit ;
B_untitled_T * _rtB ; P_untitled_T * _rtP ; X_untitled_T * _rtX ;
DW_untitled_T * _rtDW ; _rtDW = ( ( DW_untitled_T * ) ssGetRootDWork ( S ) )
; _rtX = ( ( X_untitled_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_untitled_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( ( B_untitled_T * )
_ssGetModelBlockIO ( S ) ) ; ssCallAccelRunBlock ( S , 7 , _rtDW ->
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_Task_0_blkIdxToRun ,
SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit !=
0 ) && ( ssIsMajorTimeStep ( S ) != 0 ) ) { if ( _rtB -> B_7_1_0 ) { if ( !
_rtDW -> TrueRMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) )
{ ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( ( XDis_untitled_T *
) ssGetContStateDisabled ( S ) ) -> integrator_CSTATE = 0 ; _rtDW ->
TrueRMS_MODE = true ; } } else { if ( _rtDW -> TrueRMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( ( XDis_untitled_T * )
ssGetContStateDisabled ( S ) ) -> integrator_CSTATE = 1 ; _rtDW ->
TrueRMS_MODE = false ; } } } if ( _rtDW -> TrueRMS_MODE ) { _rtB -> B_6_1_0 =
_rtX -> integrator_CSTATE ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * *
) & _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_25 ; _rtB -> B_6_2_0
= untitled_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer , *
uBuffer , _rtDW -> TransportDelay_IWORK . CircularBufSize , & _rtDW ->
TransportDelay_IWORK . Last , _rtDW -> TransportDelay_IWORK . Tail , _rtDW ->
TransportDelay_IWORK . Head , _rtP -> P_26 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_6_3_0 = _rtP -> P_27 ; _rtB -> B_6_4_0 = _rtDW -> Memory_PreviousInput ; }
if ( ssGetT ( S ) >= _rtB -> B_6_3_0 ) { _rtB -> B_5_0_0 = _rtB -> B_6_1_0 -
_rtB -> B_6_2_0 ; _rtB -> B_5_1_0 = _rtP -> P_22 * _rtB -> B_5_0_0 ; _rtB ->
B_6_7_0 = _rtB -> B_5_1_0 ; } else { _rtB -> B_6_7_0 = _rtB -> B_6_4_0 ; }
_rtB -> B_6_8_0 = _rtB -> B_1_1_0 [ 1 ] * _rtB -> B_1_1_0 [ 1 ] ; if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Saturationtoavoidnegativesqrt_MODE
= _rtB -> B_6_7_0 >= _rtP -> P_29 ? 1 : _rtB -> B_6_7_0 > _rtP -> P_30 ? 0 :
- 1 ; } _rtB -> B_6_9_0 = _rtDW -> Saturationtoavoidnegativesqrt_MODE == 1 ?
_rtP -> P_29 : _rtDW -> Saturationtoavoidnegativesqrt_MODE == - 1 ? _rtP ->
P_30 : _rtB -> B_6_7_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtDW ->
Sqrt_DWORK1 != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW
-> Sqrt_DWORK1 = 0 ; } _rtB -> B_6_10_0 = muDoubleScalarSqrt ( _rtB ->
B_6_9_0 ) ; srUpdateBC ( _rtDW -> TrueRMS_SubsysRanBC ) ; } else { if ( _rtB
-> B_6_9_0 < 0.0 ) { _rtB -> B_6_10_0 = - muDoubleScalarSqrt (
muDoubleScalarAbs ( _rtB -> B_6_9_0 ) ) ; } else { _rtB -> B_6_10_0 =
muDoubleScalarSqrt ( _rtB -> B_6_9_0 ) ; } if ( _rtB -> B_6_9_0 < 0.0 ) {
_rtDW -> Sqrt_DWORK1 = 1 ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( (
isHit != 0 ) && ( ssIsMajorTimeStep ( S ) != 0 ) ) { if ( _rtB -> B_7_2_0 ) {
if ( ! _rtDW -> RMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S
) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset (
& ( ( ( XDis_untitled_T * ) ssGetContStateDisabled ( S ) ) ->
integrator_CSTATE_g ) , 0 , 2 * sizeof ( boolean_T ) ) ; _rtDW -> RMS_MODE =
true ; } } else { if ( _rtDW -> RMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & ( ( (
XDis_untitled_T * ) ssGetContStateDisabled ( S ) ) -> integrator_CSTATE_g ) ,
1 , 2 * sizeof ( boolean_T ) ) ; _rtDW -> RMS_MODE = false ; } } } if ( _rtDW
-> RMS_MODE ) { _rtB -> B_4_0_0 = _rtX -> integrator_CSTATE_g ; { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_h . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_h .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_4 ; _rtB -> B_4_1_0 = untitled_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> TransportDelay_IWORK_n .
CircularBufSize , & _rtDW -> TransportDelay_IWORK_n . Last , _rtDW ->
TransportDelay_IWORK_n . Tail , _rtDW -> TransportDelay_IWORK_n . Head , _rtP
-> P_5 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && (
ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) ) ) ; } isHit = ssIsSampleHit (
S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_4_3_0 = _rtP -> P_6 ; }
rtb_B_4_4_0 = ( ssGetT ( S ) >= _rtB -> B_4_3_0 ) ; isHit = ssIsSampleHit ( S
, 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_4_5_0 = _rtDW ->
Memory_PreviousInput_d ; } if ( rtb_B_4_4_0 ) { _rtB -> B_3_0_0 = _rtB ->
B_4_0_0 - _rtB -> B_4_1_0 ; _rtB -> B_3_1_0 = _rtP -> P_1 * _rtB -> B_3_0_0 ;
_rtB -> B_4_7_0 = _rtB -> B_3_1_0 ; } else { _rtB -> B_4_7_0 = _rtB ->
B_4_5_0 ; } _rtB -> B_4_8_0 = _rtX -> integrator_CSTATE_g2 ; { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_e . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_e .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_9 ; _rtB -> B_4_9_0 = untitled_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> TransportDelay_IWORK_o .
CircularBufSize , & _rtDW -> TransportDelay_IWORK_o . Last , _rtDW ->
TransportDelay_IWORK_o . Tail , _rtDW -> TransportDelay_IWORK_o . Head , _rtP
-> P_10 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && (
ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) ) ) ; } isHit = ssIsSampleHit (
S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_4_11_0 = _rtP -> P_11 ; }
rtb_B_4_4_0 = ( ssGetT ( S ) >= _rtB -> B_4_11_0 ) ; isHit = ssIsSampleHit (
S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_4_13_0 = _rtDW ->
Memory_PreviousInput_c ; } if ( rtb_B_4_4_0 ) { _rtB -> B_2_0_0 = _rtB ->
B_4_8_0 - _rtB -> B_4_9_0 ; _rtB -> B_2_1_0 = _rtP -> P_0 * _rtB -> B_2_0_0 ;
_rtB -> B_4_15_0 = _rtB -> B_2_1_0 ; } else { _rtB -> B_4_15_0 = _rtB ->
B_4_13_0 ; } _rtB -> B_4_16_0 . re = _rtB -> B_4_7_0 ; _rtB -> B_4_16_0 . im
= _rtB -> B_4_15_0 ; _rtB -> B_4_19_0 = ( muDoubleScalarSin ( _rtP -> P_15 *
ssGetTaskTime ( S , 0 ) + _rtP -> P_16 ) * _rtP -> P_13 + _rtP -> P_14 ) *
_rtB -> B_1_1_0 [ 1 ] ; _rtB -> B_4_21_0 = ( muDoubleScalarSin ( _rtP -> P_19
* ssGetTaskTime ( S , 0 ) + _rtP -> P_20 ) * _rtP -> P_17 + _rtP -> P_18 ) *
_rtB -> B_1_1_0 [ 1 ] ; _rtB -> B_4_22_0 = _rtP -> P_21 * muDoubleScalarHypot
( _rtB -> B_4_16_0 . re , _rtB -> B_4_16_0 . im ) ; if ( ssIsMajorTimeStep (
S ) != 0 ) { srUpdateBC ( _rtDW -> RMS_SubsysRanBC ) ; } } if ( _rtB ->
B_7_1_0 ) { _rtB -> B_7_3_0 = _rtB -> B_6_10_0 ; } else { _rtB -> B_7_3_0 =
_rtB -> B_4_22_0 ; } ssCallAccelRunBlock ( S , 7 , 4 , SS_CALL_MDL_OUTPUTS )
; ssCallAccelRunBlock ( S , 7 , 8 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 7 , 9 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock
( S , 7 , 11 , SS_CALL_MDL_OUTPUTS ) ; UNUSED_PARAMETER ( tid ) ; } static
void mdlOutputsTID2 ( SimStruct * S , int_T tid ) { B_untitled_T * _rtB ;
P_untitled_T * _rtP ; _rtP = ( ( P_untitled_T * ) ssGetModelRtp ( S ) ) ;
_rtB = ( ( B_untitled_T * ) _ssGetModelBlockIO ( S ) ) ; _rtB -> B_7_1_0 = (
_rtP -> P_31 != 0.0 ) ; _rtB -> B_7_2_0 = ! _rtB -> B_7_1_0 ;
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { int32_T isHit ;
B_untitled_T * _rtB ; P_untitled_T * _rtP ; DW_untitled_T * _rtDW ; _rtDW = (
( DW_untitled_T * ) ssGetRootDWork ( S ) ) ; _rtP = ( ( P_untitled_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_untitled_T * ) _ssGetModelBlockIO ( S )
) ; ssCallAccelRunBlock ( S , 1 , 0 , SS_CALL_MDL_UPDATE ) ; if ( _rtDW ->
TrueRMS_MODE ) { { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * *
) & _rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime =
ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK . Head = ( ( _rtDW ->
TransportDelay_IWORK . Head < ( _rtDW -> TransportDelay_IWORK .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK . Head + 1 ) : 0 )
; if ( _rtDW -> TransportDelay_IWORK . Head == _rtDW -> TransportDelay_IWORK
. Tail ) { if ( ! untitled_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK . CircularBufSize , & _rtDW -> TransportDelay_IWORK .
Tail , & _rtDW -> TransportDelay_IWORK . Head , & _rtDW ->
TransportDelay_IWORK . Last , simTime - _rtP -> P_25 , tBuffer , uBuffer , (
NULL ) , ( boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * tBuffer ) [ _rtDW -> TransportDelay_IWORK . Head ] =
simTime ; ( * uBuffer ) [ _rtDW -> TransportDelay_IWORK . Head ] = _rtB ->
B_6_1_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW
-> Memory_PreviousInput = _rtB -> B_6_7_0 ; } } if ( _rtDW -> RMS_MODE ) { {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_h .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_h . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_n . Head = ( ( _rtDW -> TransportDelay_IWORK_n
. Head < ( _rtDW -> TransportDelay_IWORK_n . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_n . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_n . Head == _rtDW -> TransportDelay_IWORK_n . Tail ) {
if ( ! untitled_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_n . CircularBufSize , & _rtDW -> TransportDelay_IWORK_n
. Tail , & _rtDW -> TransportDelay_IWORK_n . Head , & _rtDW ->
TransportDelay_IWORK_n . Last , simTime - _rtP -> P_4 , tBuffer , uBuffer , (
NULL ) , ( boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_n .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * tBuffer ) [ _rtDW -> TransportDelay_IWORK_n . Head ] =
simTime ; ( * uBuffer ) [ _rtDW -> TransportDelay_IWORK_n . Head ] = _rtB ->
B_4_0_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW
-> Memory_PreviousInput_d = _rtB -> B_4_7_0 ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_e . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_e . TUbufferPtrs
[ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_o .
Head = ( ( _rtDW -> TransportDelay_IWORK_o . Head < ( _rtDW ->
TransportDelay_IWORK_o . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK_o . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_o . Head == _rtDW -> TransportDelay_IWORK_o . Tail ) {
if ( ! untitled_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_o . CircularBufSize , & _rtDW -> TransportDelay_IWORK_o
. Tail , & _rtDW -> TransportDelay_IWORK_o . Head , & _rtDW ->
TransportDelay_IWORK_o . Last , simTime - _rtP -> P_9 , tBuffer , uBuffer , (
NULL ) , ( boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_o .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * tBuffer ) [ _rtDW -> TransportDelay_IWORK_o . Head ] =
simTime ; ( * uBuffer ) [ _rtDW -> TransportDelay_IWORK_o . Head ] = _rtB ->
B_4_8_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW
-> Memory_PreviousInput_c = _rtB -> B_4_15_0 ; } } UNUSED_PARAMETER ( tid ) ;
}
#define MDL_UPDATE
static void mdlUpdateTID2 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_untitled_T * _rtB ;
XDot_untitled_T * _rtXdot ; DW_untitled_T * _rtDW ; _rtDW = ( ( DW_untitled_T
* ) ssGetRootDWork ( S ) ) ; _rtXdot = ( ( XDot_untitled_T * ) ssGetdX ( S )
) ; _rtB = ( ( B_untitled_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 1 , 0 , SS_CALL_MDL_DERIVATIVES ) ; if ( _rtDW ->
TrueRMS_MODE ) { _rtXdot -> integrator_CSTATE = _rtB -> B_6_8_0 ; } else { (
( XDot_untitled_T * ) ssGetdX ( S ) ) -> integrator_CSTATE = 0.0 ; } if (
_rtDW -> RMS_MODE ) { _rtXdot -> integrator_CSTATE_g = _rtB -> B_4_19_0 ;
_rtXdot -> integrator_CSTATE_g2 = _rtB -> B_4_21_0 ; } else { { real_T * dx ;
int_T i ; dx = & ( ( ( XDot_untitled_T * ) ssGetdX ( S ) ) ->
integrator_CSTATE_g ) ; for ( i = 0 ; i < 2 ; i ++ ) { dx [ i ] = 0.0 ; } } }
}
#define MDL_PROJECTION
static void mdlProjection ( SimStruct * S ) { ssCallAccelRunBlock ( S , 1 , 0
, SS_CALL_MDL_PROJECTION ) ; }
#define MDL_FORCINGFUNCTION
static void mdlForcingFunction ( SimStruct * S ) { B_untitled_T * _rtB ;
XDot_untitled_T * _rtXdot ; DW_untitled_T * _rtDW ; _rtDW = ( ( DW_untitled_T
* ) ssGetRootDWork ( S ) ) ; _rtXdot = ( ( XDot_untitled_T * ) ssGetdX ( S )
) ; _rtB = ( ( B_untitled_T * ) _ssGetModelBlockIO ( S ) ) ;
ssCallAccelRunBlock ( S , 1 , 0 , SS_CALL_MDL_FORCINGFUNCTION ) ; if ( _rtDW
-> TrueRMS_MODE ) { _rtXdot -> integrator_CSTATE = _rtB -> B_6_8_0 ; } else {
( ( XDot_untitled_T * ) ssGetdX ( S ) ) -> integrator_CSTATE = 0.0 ; } if (
_rtDW -> RMS_MODE ) { _rtXdot -> integrator_CSTATE_g = _rtB -> B_4_19_0 ;
_rtXdot -> integrator_CSTATE_g2 = _rtB -> B_4_21_0 ; } else { { real_T * dx ;
int_T i ; dx = & ( ( ( XDot_untitled_T * ) ssGetdX ( S ) ) ->
integrator_CSTATE_g ) ; for ( i = 0 ; i < 2 ; i ++ ) { dx [ i ] = 0.0 ; } } }
}
#define MDL_MASSMATRIX
static void mdlMassMatrix ( SimStruct * S ) { ssCallAccelRunBlock ( S , 1 , 0
, SS_CALL_MDL_MASSMATRIX ) ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { B_untitled_T * _rtB ;
P_untitled_T * _rtP ; ZCV_untitled_T * _rtZCSV ; DW_untitled_T * _rtDW ;
_rtDW = ( ( DW_untitled_T * ) ssGetRootDWork ( S ) ) ; _rtZCSV = ( (
ZCV_untitled_T * ) ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( (
P_untitled_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( ( B_untitled_T * )
_ssGetModelBlockIO ( S ) ) ; if ( _rtDW -> TrueRMS_MODE ) { _rtZCSV ->
Saturationtoavoidnegativesqrt_UprLim_ZC = _rtB -> B_6_7_0 - _rtP -> P_29 ;
_rtZCSV -> Saturationtoavoidnegativesqrt_LwrLim_ZC = _rtB -> B_6_7_0 - _rtP
-> P_30 ; } else { { real_T * zcsv = & ( ( ( ZCV_untitled_T * )
ssGetSolverZcSignalVector ( S ) ) -> Saturationtoavoidnegativesqrt_UprLim_ZC
) ; int_T i ; for ( i = 0 ; i < 2 ; i ++ ) { zcsv [ i ] = 0.0 ; } } } }
static void mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0 ,
2332917756U ) ; ssSetChecksumVal ( S , 1 , 545141796U ) ; ssSetChecksumVal (
S , 2 , 2348304557U ) ; ssSetChecksumVal ( S , 3 , 3873668706U ) ; { mxArray
* slVerStructMat = NULL ; mxArray * slStrMat = mxCreateString ( "simulink" )
; char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat ,
1 , & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat =
mxGetField ( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == NULL ) {
status = 1 ; } else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; }
} mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "10.0" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != sizeof ( DW_untitled_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( B_untitled_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
P_untitled_T ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetModelRtp ( S , ( real_T *
) & untitled_rtDefaultP ) ; if ( ssGetSizeofDWork ( S ) == sizeof (
DW_untitled_T ) ) { { ( ( DW_untitled_T * ) ssGetRootDWork ( S ) ) ->
STATE_1_MASS_MATRIX_PR = 3 ; } } rt_InitInfAndNaN ( sizeof ( real_T ) ) ; ( (
P_untitled_T * ) ssGetModelRtp ( S ) ) -> P_29 = rtInf ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { { SimStruct * childS ;
SysOutputFcn * callSysFcns ; childS = ssGetSFunction ( S , 0 ) ; callSysFcns
= ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; } slAccRegPrmChangeFcn ( S , mdlOutputsTID2 ) ; }
static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
