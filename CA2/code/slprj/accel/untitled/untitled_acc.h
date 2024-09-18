#include "__cf_untitled.h"
#ifndef RTW_HEADER_untitled_acc_h_
#define RTW_HEADER_untitled_acc_h_
#include <stddef.h>
#include <float.h>
#include <string.h>
#ifndef untitled_acc_COMMON_INCLUDES_
#define untitled_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn 
#define S_FUNCTION_LEVEL 2
#define RTW_GENERATED_S_FUNCTION
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "untitled_acc_types.h"
#include "multiword_types.h"
#include "mwmathutil.h"
#include "rtGetInf.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
typedef struct { creal_T B_4_16_0 ; real_T B_7_3_0 ; real_T B_7_8_0 ; real_T
B_6_1_0 ; real_T B_6_2_0 ; real_T B_6_3_0 ; real_T B_6_4_0 ; real_T B_6_7_0 ;
real_T B_6_8_0 ; real_T B_6_9_0 ; real_T B_6_10_0 ; real_T B_5_0_0 ; real_T
B_5_1_0 ; real_T B_4_0_0 ; real_T B_4_1_0 ; real_T B_4_3_0 ; real_T B_4_5_0 ;
real_T B_4_7_0 ; real_T B_4_8_0 ; real_T B_4_9_0 ; real_T B_4_11_0 ; real_T
B_4_13_0 ; real_T B_4_15_0 ; real_T B_4_19_0 ; real_T B_4_21_0 ; real_T
B_4_22_0 ; real_T B_3_0_0 ; real_T B_3_1_0 ; real_T B_2_0_0 ; real_T B_2_1_0
; real_T B_1_0_0 [ 5 ] ; real_T B_1_1_0 [ 2 ] ; real_T B_1_3_0 [ 4 ] ; real_T
B_0_0_1 ; boolean_T B_7_1_0 ; boolean_T B_7_2_0 ; char_T pad_B_7_2_0 [ 6 ] ;
} B_untitled_T ; typedef struct { real_T INPUT_1_1_1_Discrete [ 2 ] ; real_T
OUTPUT_1_0_Discrete ; real_T Memory_PreviousInput ; real_T
Memory_PreviousInput_d ; real_T Memory_PreviousInput_c ; real_T
STATE_1_Discrete ; real_T OUTPUT_1_1_Discrete ; struct { real_T modelTStart ;
} TransportDelay_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK_j ; struct { real_T modelTStart ; }
TransportDelay_RWORK_i ; void * OUTPUT_1_0_Simulator ; void *
OUTPUT_1_0_SimData ; void * OUTPUT_1_0_DiagMgr ; void * OUTPUT_1_0_ZcLogger ;
void * OUTPUT_1_0_TsIndex ; void * Scope1_PWORK ; void * Scope2_PWORK ;
struct { void * TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK ; struct { void *
TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK_h ; struct { void * TUbufferPtrs
[ 2 ] ; } TransportDelay_PWORK_e ; void *
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_Task_0_AlgLoopData ; void *
STATE_1_Simulator ; void * STATE_1_SimData ; void * STATE_1_DiagMgr ; void *
STATE_1_ZcLogger ; void * STATE_1_TsIndex ; void * OUTPUT_1_1_Simulator ;
void * OUTPUT_1_1_SimData ; void * OUTPUT_1_1_DiagMgr ; void *
OUTPUT_1_1_ZcLogger ; void * OUTPUT_1_1_TsIndex ; int32_T TrueRMS_sysIdxToRun
; int32_T TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
RMS_sysIdxToRun ; int32_T TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_o ;
int32_T TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_p ; int32_T
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_Task_0_sysIdxToRun ; int32_T
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_Task_0_blkIdxToRun ; int32_T
Synthesized_Atomic_Subsystem_For_Alg_Loop_1_Task_0_parentSysIdxToRun ;
int32_T MATLABFunction_sysIdxToRun ; int_T OUTPUT_1_0_Modes ; struct { int_T
Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize
; } TransportDelay_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_n ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_o ; int_T STATE_1_Modes ; int_T
OUTPUT_1_1_Modes ; int_T Saturationtoavoidnegativesqrt_MODE ; int32_T
STATE_1_MASS_MATRIX_PR ; int8_T TrueRMS_SubsysRanBC ; int8_T Sqrt_DWORK1 ;
int8_T RMS_SubsysRanBC ; boolean_T OUTPUT_1_0_FirstOutput ; boolean_T
STATE_1_FirstOutput ; boolean_T OUTPUT_1_1_FirstOutput ; boolean_T
TrueRMS_MODE ; boolean_T RMS_MODE ; char_T pad_RMS_MODE [ 4 ] ; }
DW_untitled_T ; typedef struct { real_T integrator_CSTATE ; real_T
integrator_CSTATE_g ; real_T integrator_CSTATE_g2 ; real_T
untitledElectromagnetic_Converterphi [ 5 ] ; } X_untitled_T ; typedef struct
{ real_T integrator_CSTATE ; real_T integrator_CSTATE_g ; real_T
integrator_CSTATE_g2 ; real_T untitledElectromagnetic_Converterphi [ 5 ] ; }
XDot_untitled_T ; typedef struct { boolean_T integrator_CSTATE ; boolean_T
integrator_CSTATE_g ; boolean_T integrator_CSTATE_g2 ; boolean_T
untitledElectromagnetic_Converterphi [ 5 ] ; } XDis_untitled_T ; typedef
struct { real_T integrator_CSTATE ; real_T integrator_CSTATE_g ; real_T
integrator_CSTATE_g2 ; real_T untitledElectromagnetic_Converterphi [ 5 ] ; }
CStateAbsTol_untitled_T ; typedef struct { real_T
Saturationtoavoidnegativesqrt_UprLim_ZC ; real_T
Saturationtoavoidnegativesqrt_LwrLim_ZC ; } ZCV_untitled_T ; typedef struct {
ZCSigState Saturationtoavoidnegativesqrt_UprLim_ZCE ; ZCSigState
Saturationtoavoidnegativesqrt_LwrLim_ZCE ; } PrevZCX_untitled_T ; typedef
struct { int_T ir [ 7 ] ; int_T jc [ 9 ] ; real_T pr [ 7 ] ; }
MassMatrix_untitled_T ; struct P_untitled_T_ { real_T P_0 ; real_T P_1 ;
real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ;
real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T
P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T P_18 ;
real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T P_22 ; real_T P_23 ; real_T
P_24 ; real_T P_25 ; real_T P_26 ; real_T P_27 ; real_T P_28 ; real_T P_29 ;
real_T P_30 ; real_T P_31 ; } ; extern P_untitled_T untitled_rtDefaultP ;
#endif
