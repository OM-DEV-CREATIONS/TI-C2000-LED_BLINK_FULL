#include "c2000BoardSupport.h"
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "DSP2833x_GlobalPrototypes.h"
#include "rtwtypes.h"
#include "BLINK.h"
#include "BLINK_private.h"

void InitXintf16Gpio(void);
void init_board (void)
{
  DisableDog();

  /* Initialize the PLL control: PLLCR and DIVSEL
   * DSP28_PLLCR and DSP28_DIVSEL are defined in DSP2833x_Examples.h
   */
  InitPll(10,2);
  InitPeripheralClocks();
  EALLOW;

  /* Configure low speed peripheral clocks */
  SysCtrlRegs.LOSPCP.all = 0U;

  /* Configure high speed peripheral clocks */
  SysCtrlRegs.HISPCP.all = 1U;
  EDIS;

  /* Disable and clear all CPU interrupts */
  DINT;
  IER = 0x0000U;
  IFR = 0x0000U;
  InitPieCtrl();
  InitPieVectTable();
  InitCpuTimers();

#ifdef CPU1
#endif                                 // #ifdef CPU1

  EALLOW;

  /* Perform additional configuration of the XTINF for speed up */
  XintfRegs.XINTCNF2.bit.XTIMCLK = 0U; /* XTIMCLK=SYSCLKOUT/1*/
  XintfRegs.XINTCNF2.bit.CLKOFF = 0U;  /* XCLKOUT is enabled*/
  XintfRegs.XINTCNF2.bit.CLKMODE = 0U; /* XCLKOUT = XTIMCLK*/

  /* Make sure write buffer is empty before configuring buffering depth*/
  while (XintfRegs.XINTCNF2.bit.WLEVEL != 0U) {
    /* poll the WLEVEL bit*/
    ;
  }

  XintfRegs.XINTCNF2.bit.WRBUFF = 0U;  /* No write buffering*/

  /* Example: Assume Zone 7 is slow, so add additional BCYC cycles whenever
   * switching from Zone 7 to another Zone.  This will help avoid bus contention.
   */
  XintfRegs.XBANK.bit.BCYC = 3U;       /* Add 7 cycles*/
  XintfRegs.XBANK.bit.BANK = 3U;       /* select zone 7*/

  /* Zone 0 Configuration */
  /*
     XintfRegs.XTIMING0.bit.X2TIMING = 0U;       -- Timing scale factor = 1
     XintfRegs.XTIMING0.bit.XSIZE = 3U;          -- Always write as 11b
     XintfRegs.XTIMING0.bit.READYMODE = 1U;      -- XREADY is asynchronous
     XintfRegs.XTIMING0.bit.USEREADY = 0U;       -- Disable XREADY
     XintfRegs.XTIMING0.bit.XRDLEAD = 1U;        -- Read lead time
     XintfRegs.XTIMING0.bit.XRDACTIVE = 2U;      -- Read active time
     XintfRegs.XTIMING0.bit.XRDTRAIL = 1U;       -- Read trail time
     XintfRegs.XTIMING0.bit.XWRLEAD = 1U;        -- Write lead time
     XintfRegs.XTIMING0.bit.XWRACTIVE = 2U;      -- Write active time
     XintfRegs.XTIMING0.bit.XWRTRAIL = 1U;       -- Write trail time
   */
  XintfRegs.XTIMING0.all = (XintfRegs.XTIMING0.all & ~0x43FFFFU) | 0x394A9U;

  /* Zone 6 Configuration */
  /*
     XintfRegs.XTIMING6.bit.X2TIMING = 0U;       -- Timing scale factor = 1
     XintfRegs.XTIMING6.bit.XSIZE = 3U;          -- Always write as 11b
     XintfRegs.XTIMING6.bit.READYMODE = 1U;      -- XREADY is asynchronous
     XintfRegs.XTIMING6.bit.USEREADY = 0U;       -- Disable XREADY
     XintfRegs.XTIMING6.bit.XRDLEAD = 1U;        -- Read lead time
     XintfRegs.XTIMING6.bit.XRDACTIVE = 2U;      -- Read active time
     XintfRegs.XTIMING6.bit.XRDTRAIL = 1U;       -- Read trail time
     XintfRegs.XTIMING6.bit.XWRLEAD = 1U;        -- Write lead time
     XintfRegs.XTIMING6.bit.XWRACTIVE = 2U;      -- Write active time
     XintfRegs.XTIMING6.bit.XWRTRAIL = 1U;       -- Write trail time
   */
  XintfRegs.XTIMING6.all = (XintfRegs.XTIMING6.all & ~0x43FFFFU) | 0x394A9U;

  /* Zone 7 Configuration */
  /*
     XintfRegs.XTIMING7.bit.X2TIMING = 0U;       -- Timing scale factor = 1
     XintfRegs.XTIMING7.bit.XSIZE = 3U;          -- Always write as 11b
     XintfRegs.XTIMING7.bit.READYMODE = 1U;      -- XREADY is asynchronous
     XintfRegs.XTIMING7.bit.USEREADY = 0U;       -- Disable XREADY
     XintfRegs.XTIMING7.bit.XRDLEAD = 1U;        -- Read lead time
     XintfRegs.XTIMING7.bit.XRDACTIVE = 2U;      -- Read active time
     XintfRegs.XTIMING7.bit.XRDTRAIL = 1U;       -- Read trail time
     XintfRegs.XTIMING7.bit.XWRLEAD = 1U;        -- Write lead time
     XintfRegs.XTIMING7.bit.XWRACTIVE = 2U;      -- Write active time
     XintfRegs.XTIMING7.bit.XWRTRAIL = 1U;       -- Write trail time
   */
  XintfRegs.XTIMING7.all = (XintfRegs.XTIMING7.all & ~0x43FFFFU) | 0x394A9U;
  EDIS;
  InitXintf16Gpio();

  {
    /* Inline assembly 'RPT #6 || NOP' is used here to flush the pipeline
     * and ensure that the write is complete.
     */
    asm(" RPT #6 || NOP");
  }

  /* initial GPIO qualification settings.... */
  EALLOW;
  GpioCtrlRegs.GPAQSEL1.all = 0x0U;
  GpioCtrlRegs.GPAQSEL2.all = 0x0U;
  GpioCtrlRegs.GPBQSEL1.all = 0x0U;
  GpioCtrlRegs.GPBQSEL2.all = 0x0U;
  EDIS;
}

void InitXintf16Gpio(void)
{
  EALLOW;

  /*
     GpioCtrlRegs.GPCMUX1.bit.GPIO64 = 3U;  -- XD15
     GpioCtrlRegs.GPCMUX1.bit.GPIO65 = 3U;  -- XD14
     GpioCtrlRegs.GPCMUX1.bit.GPIO66 = 3U;  -- XD13
     GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 3U;  -- XD12
     GpioCtrlRegs.GPCMUX1.bit.GPIO68 = 3U;  -- XD11
     GpioCtrlRegs.GPCMUX1.bit.GPIO69 = 3U;  -- XD10
     GpioCtrlRegs.GPCMUX1.bit.GPIO70 = 3U;  -- XD19
     GpioCtrlRegs.GPCMUX1.bit.GPIO71 = 3U;  -- XD8
     GpioCtrlRegs.GPCMUX1.bit.GPIO72 = 3U;  -- XD7
     GpioCtrlRegs.GPCMUX1.bit.GPIO73 = 3U;  -- XD6
     GpioCtrlRegs.GPCMUX1.bit.GPIO74 = 3U;  -- XD5
     GpioCtrlRegs.GPCMUX1.bit.GPIO75 = 3U;  -- XD4
     GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 3U;  -- XD3
     GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 3U;  -- XD2
     GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 3U;  -- XD1
     GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 3U;  -- XD0

     GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 3U;  -- XA0/XWE1n
     GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 3U;  -- XA1
     GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 3U;  -- XA2
     GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 3U;  -- XA3
     GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 3U;  -- XA4
     GpioCtrlRegs.GPBMUX1.bit.GPIO45 = 3U;  -- XA5
     GpioCtrlRegs.GPBMUX1.bit.GPIO46 = 3U;  -- XA6
     GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 3U;  -- XA7

     GpioCtrlRegs.GPCMUX2.bit.GPIO80 = 3U;  -- XA8
     GpioCtrlRegs.GPCMUX2.bit.GPIO81 = 3U;  -- XA9
     GpioCtrlRegs.GPCMUX2.bit.GPIO82 = 3U;  -- XA10
     GpioCtrlRegs.GPCMUX2.bit.GPIO83 = 3U;  -- XA11
     GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 3U;  -- XA12
     GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 3U;  -- XA13
     GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 3U;  -- XA14
     GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 3U;  -- XA15
     GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 3U;  -- XA16
     GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 3U;  -- XA17
     GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 3U;  -- XA18
     GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3U;  -- XA19

     GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 3U;  -- XREADY
     GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 3U;  -- XRNW
     GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 3U;  -- XWE0

     GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 3U;  -- XZCS0
     GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 3U;  -- XZCS7
     GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3U;  -- XZCS6
   */
  GpioCtrlRegs.GPCMUX1.all = (GpioCtrlRegs.GPCMUX1.all & ~0xFFFFFFFFU) |
    0xFFFFFFFFU;
  GpioCtrlRegs.GPBMUX1.all = (GpioCtrlRegs.GPBMUX1.all & ~0xFFFFFFF0U) |
    0xFFFFFFF0U;
  GpioCtrlRegs.GPCMUX2.all = (GpioCtrlRegs.GPCMUX2.all & ~0xFFFFU) | 0xFFFFU;
  GpioCtrlRegs.GPAMUX2.all = (GpioCtrlRegs.GPAMUX2.all & ~0xFF000000U) |
    0xFF000000U;
  EDIS;
}
