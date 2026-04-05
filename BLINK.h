/*
 * File: BLINK.h
 *
 * Model version                  : 1.2
 * C/C++ source code              : Fri Apr  3 17:11:53 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->C2000
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef BLINK_h_
#define BLINK_h_
#ifndef BLINK_COMMON_INCLUDES_
#define BLINK_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "c2000BoardSupport.h"
#include "DSP2833x_Device.h"
#include "DSP2833x_Gpio.h"
#endif                                 /* BLINK_COMMON_INCLUDES_ */

#include "BLINK_types.h"
#include <stddef.h>
#include <string.h>
#include "MW_target_hardware_resources.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Lookup;                       /* '<S1>/Lookup' */
} B_BLINK_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  int32_T DigitalOutput_FRAC_LEN;      /* '<Root>/Digital Output' */
  int32_T DigitalOutput1_FRAC_LEN;     /* '<Root>/Digital Output1' */
  uint16_T Output_DSTATE;              /* '<S2>/Output' */
} DW_BLINK_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: OutValues
   * Referenced by: '<S1>/Lookup'
   */
  real_T Lookup_tableData[4];

  /* Expression: TimeValues
   * Referenced by: '<S1>/Lookup'
   */
  real_T Lookup_bp01Data[4];
} ConstP_BLINK_T;

/* Real-time Model Data Structure */
struct tag_RTM_BLINK_T {
  const char_T * volatile errorStatus;
};

/* Block signals (default storage) */
extern B_BLINK_T BLINK_B;

/* Block states (default storage) */
extern DW_BLINK_T BLINK_DW;

/* Constant parameters (default storage) */
extern const ConstP_BLINK_T BLINK_ConstP;

/* Model entry point functions */
extern void BLINK_initialize(void);
extern void BLINK_step(void);
extern void BLINK_terminate(void);

/* Real-time Model object */
extern RT_MODEL_BLINK_T *const BLINK_M;
extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Data Type Propagation' : Unused code path elimination
 * Block '<S2>/Data Type Propagation' : Unused code path elimination
 * Block '<S3>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S4>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S1>/Output' : Eliminate redundant signal conversion block
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'BLINK'
 * '<S1>'   : 'BLINK/Repeating Sequence Interpolated'
 * '<S2>'   : 'BLINK/Repeating Sequence Interpolated/LimitedCounter'
 * '<S3>'   : 'BLINK/Repeating Sequence Interpolated/LimitedCounter/Increment Real World'
 * '<S4>'   : 'BLINK/Repeating Sequence Interpolated/LimitedCounter/Wrap To Zero'
 */
#endif                                 /* BLINK_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
