#ifndef SOC_CM0_H
#define SOC_CM0_H

#define AHB_VGA_BASE        0x50000000
#define AHB_VGA_RESOLUTION_REG  (*(volatile unsigned int*)(AHB_VGA_BASE + 0x3BF24))
#define AHB_UART_BASE       0x51000000
#define AHB_TIMER_BASE      0x52000000
#define AHB_GPIO_BASE       0x53000000
#define AHB_7SEG_BASE       0x54000000

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/

/* ToDo: use this Cortex interrupt numbers if your device is a CORTEX-M0 device                   */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Cortex-M0 Non Maskable Interrupt              */
  HardFault_IRQn                = -13,      /*!<  3 Cortex-M0 Hard Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M0 SV Call Interrupt                   */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M0 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M0 System Tick Interrupt               */

/******  CMSDK Specific Interrupt Numbers *********************************************************/
  Timer_IRQn                  = 0,
  UART_IRQn                  = 1,
  GPIO_IRQn                 = 2,
} IRQn_Type;

typedef struct
{
  volatile  unsigned int  CONSOLE;
  volatile  unsigned int  IMG; 
} VGA_TypeDef;

typedef struct
{
  volatile unsigned int  DATA;
  volatile unsigned int  BAUD;
  volatile unsigned int  PARITY;
  volatile unsigned int  STATUS;
} UART_TypeDef;

typedef struct
{
  volatile  unsigned int  INITVALUE;
  volatile  unsigned int  CURVALUE;
  volatile  unsigned int  CONTROL;
  volatile  unsigned int  CLEAR;
  volatile  unsigned int  CMP;
  volatile  unsigned int  CAPT;
} TIMER_TypeDef;

typedef struct
{
  volatile  unsigned int  DATA;
  volatile  unsigned int  DIR;
  volatile  unsigned int  MASK;
} GPIO_TypeDef;

typedef struct
{
  volatile  unsigned int  DIGIT1;
  volatile  unsigned int  DIGIT2;
  volatile  unsigned int  DIGIT3;
  volatile  unsigned int  DIGIT4;
  volatile  unsigned int  DISP;
} SEVENSEG_TypeDef;

#define VGA             ((VGA_TypeDef   *) AHB_VGA_BASE)
#define TIMER           ((TIMER_TypeDef *) AHB_TIMER_BASE )
#define GPIO            ((GPIO_TypeDef  *) AHB_GPIO_BASE)
#define SEVSEG          ((SEVENSEG_TypeDef  *) AHB_7SEG_BASE)
#define UART            ((UART_TypeDef  *) AHB_UART_BASE)
/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M0 Processor and Core Peripherals */
#define __CM0_REV                 0x0000    /*!< Core Revision r0p0                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
#define __MPU_PRESENT             0         /*!< MPU present or not                               */

/*@}*/ /* end of group CMSDK_CMSIS */

/*
 * ==========================================================================
 * ------------------------- System memory  ---------------------------------
 * ==========================================================================
 */
 


#define RAM_SIZE 					0x20000				// 16 MB of PSRAM on Digilent Board

#define RAM_START 					0x40000
#define RAM_END						RAM_START + RAM_SIZE

#endif /* SOC_CM0_H*/
