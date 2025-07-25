//--------------------------------------------------------
// Peripheral driver header file
//--------------------------------------------------------


#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include <stdint.h>


//--------------------------------------------------------
//           Peripheral memory map    
//--------------------------------------------------------


#define AHB_VGA_BASE        0x50000000
#define AHB_UART_BASE       0x51000000
#define AHB_TIMER_BASE      0x52000000
#define AHB_GPIO_BASE       0x53000000
#define AHB_7SEG_BASE       0x54000000
#define NVIC_INT_ENABLE     0xE000E100

// VGA Resolution register (distant offset)
#define VGA_RESOLUTION_REG  (*(volatile unsigned int*)(AHB_VGA_BASE + 0x3BF24))


// UART Baudrate configurations
typedef enum {
  B9600   = 326,
  B19200  = 162,
  B38400  = 81,
  B57600  = 54,
  B115200 = 27
} UART_BaudRate_t;

// VGA Resolution modes enum
typedef enum {
  VGA_2x2 = 0x01,
  VGA_4x4 = 0x02,
  VGA_8x8 = 0x03
} VGA_Resolution_t;

// VGA Line type
typedef enum {
  HORIZONTAL,
  VERTICAL,
} VGA_Line_t;

//--------------------------------------------------------
//           Peripheral type define    
//--------------------------------------------------------


// Timer define 
typedef struct
{
  volatile  unsigned int  INITVALUE;
  volatile  unsigned int  CURVALUE;
  volatile  unsigned int  CONTROL;
  volatile  unsigned int  CLEAR;
  volatile  unsigned int  CMP;
  volatile  unsigned int  CAPT;
} TIMER_TypeDef;


// UART define 
typedef struct
{
  volatile unsigned int  DATA;
  volatile unsigned int  BAUD;
  volatile unsigned int  PARITY;
} UART_TypeDef;



//GPIO define (Switch input)
typedef struct
{
  volatile  unsigned int  DATA;
  volatile  unsigned int  DIR;
  volatile  unsigned int  MASK;
} GPIO_TypeDef;


//7-segments define
typedef struct
{
  volatile  unsigned int  DIGIT1;
  volatile  unsigned int  DIGIT2;
  volatile  unsigned int  DIGIT3;
  volatile  unsigned int  DIGIT4;
  volatile  unsigned int  DISP;
} SEVENSEG_TypeDef;


// LED define 
typedef struct
{
  volatile  unsigned int  BITS;
} LED_TypeDef;

// VGA define 
typedef struct
{
  volatile  unsigned int  CONSOLE;
  volatile  unsigned int  IMG; 
} VGA_TypeDef;

//--------------------------------------------------------
//           Peripheral instances define   
//--------------------------------------------------------

#define TIMER           ((TIMER_TypeDef *) AHB_TIMER_BASE )
#define UART            ((UART_TypeDef  *) AHB_UART_BASE  )
#define GPIO            ((GPIO_TypeDef  *) AHB_GPIO_BASE  )
#define SEVSEG          ((SEVENSEG_TypeDef  *) AHB_7SEG_BASE  )
#define VGA             ((VGA_TypeDef   *) AHB_VGA_BASE   )

struct pt
{
  uint16_t x;
  uint16_t y;
};


//--------------------------------------------------------
// Peripheral driver functions
//--------------------------------------------------------
void VGA_set_resolution(VGA_Resolution_t mode);
VGA_Resolution_t VGA_get_resolution(void);
void VGA_plot_pixel(struct pt point, uint8_t colour);
void VGA_plot_line(struct pt point1, struct pt point2, uint8_t colour, VGA_Line_t Line_type);
void plot_vert_line(struct pt point1, struct pt point2, uint8_t colour);
void plot_hor_line(struct pt point1, struct pt point2, uint8_t colour);


void seven_seg_write(char dig1, char dig2, char dig3, char dig4, uint8_t disp_mode);

void timer_init(int load_value, int prescale, int mode);
void timer_enable(void);
void timer_irq_clear(void);

void GPIO_init(uint8_t direction);
uint8_t GPIO_read_pin(uint8_t pin_number);
void GPIO_write_pin(uint8_t pin_number, uint8_t value);

void uart_init(UART_BaudRate_t baud_rate, uint8_t parity_bit);
void uart_read_byte(char* byte);
void uart_write_byte(char byte);
