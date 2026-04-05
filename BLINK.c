/*
 * File: BLINK.c
 *
 * Model version                  : 1.2
 * C/C++ source code generated on : Fri Apr  3 17:11:53 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Texas Instruments->C2000
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "BLINK.h"
#include "rtwtypes.h"
#include "BLINK_private.h"
#include <string.h>

/* Block signals (default storage) */
B_BLINK_T BLINK_B;

/* Block states (default storage) */
DW_BLINK_T BLINK_DW;

/* Real-time model */
static RT_MODEL_BLINK_T BLINK_M_;
RT_MODEL_BLINK_T *const BLINK_M = &BLINK_M_;
real_T look1_binlcpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex)
{
  real_T frac;
  real_T yL_0d0;
  uint32_T bpIdx;
  uint32_T iLeft;
  uint32_T iRght;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Clip'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0UL]) {
    iLeft = 0UL;
    frac = 0.0;
  } else if (u0 < bp0[maxIndex]) {
    /* Binary Search */
    bpIdx = maxIndex >> 1UL;
    iLeft = 0UL;
    iRght = maxIndex;
    while (iRght - iLeft > 1UL) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1UL;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1UL] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1UL;
    frac = 1.0;
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  yL_0d0 = table[iLeft];
  return (table[iLeft + 1UL] - yL_0d0) * frac + yL_0d0;
}

/* Model step function */
void BLINK_step(void)
{
  /* Lookup_n-D: '<S1>/Lookup' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   *  SampleTimeMath: '<S1>/Sample Time Math'
   *  UnitDelay: '<S2>/Output'
   *
   * About '<S1>/Sample Time Math':
   *  y = u * K where K = ( w * Ts )
   *   */
  BLINK_B.Lookup = (real_T)BLINK_DW.Output_DSTATE * 0.01;
  BLINK_B.Lookup = look1_binlcpw(BLINK_B.Lookup, BLINK_ConstP.Lookup_bp01Data,
    BLINK_ConstP.Lookup_tableData, 3UL);

  /* S-Function (c280xgpio_do): '<Root>/Digital Output' */
  {
    if (BLINK_B.Lookup) {
      GpioDataRegs.GPASET.bit.GPIO31 = 1U;
    } else {
      GpioDataRegs.GPACLEAR.bit.GPIO31 = 1U;
    }
  }

  /* S-Function (c280xgpio_do): '<Root>/Digital Output1' */
  {
    GpioDataRegs.GPBTOGGLE.bit.GPIO34 = (uint16_T)(BLINK_B.Lookup != 0);
  }

  /* Sum: '<S3>/FixPt Sum1' incorporates:
   *  Constant: '<S3>/FixPt Constant'
   *  UnitDelay: '<S2>/Output'
   */
  BLINK_DW.Output_DSTATE = (BLINK_DW.Output_DSTATE + 1U) & 255U;

  /* Switch: '<S4>/FixPt Switch' incorporates:
   *  Constant: '<S4>/Constant'
   *  Sum: '<S3>/FixPt Sum1'
   */
  if (BLINK_DW.Output_DSTATE > 200U) {
    BLINK_DW.Output_DSTATE = 0U;
  }

  /* End of Switch: '<S4>/FixPt Switch' */
}

/* Model initialize function */
void BLINK_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(BLINK_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &BLINK_B), 0,
                sizeof(B_BLINK_T));

  /* states (dwork) */
  (void) memset((void *)&BLINK_DW, 0,
                sizeof(DW_BLINK_T));

  /* Start for S-Function (c280xgpio_do): '<Root>/Digital Output' */
  EALLOW;
  GpioCtrlRegs.GPAMUX2.all &= 0x3FFFFFFFU;
  GpioCtrlRegs.GPADIR.all |= 0x80000000U;
  EDIS;

  /* Start for S-Function (c280xgpio_do): '<Root>/Digital Output1' */
  EALLOW;
  GpioCtrlRegs.GPBMUX1.all &= 0xFFFFFFCFU;
  GpioCtrlRegs.GPBDIR.all |= 0x4U;
  EDIS;
}

/* Model terminate function */
void BLINK_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
