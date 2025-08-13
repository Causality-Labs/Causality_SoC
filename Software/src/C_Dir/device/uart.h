#ifndef UART_H
#define UART_H

#include "core_cm0.h"
#include <stdint.h>

#define AHB_UART_BASE       0x51000000

typedef struct
{
  volatile unsigned int  DATA;
  volatile unsigned int  BAUD;
  volatile unsigned int  PARITY;
  volatile unsigned int  STATUS;
} UART_TypeDef;

#define UART            ((UART_TypeDef  *) AHB_UART_BASE)

typedef enum {
  B9600   = 326,
  B19200  = 162,
  B38400  = 81,
  B57600  = 54,
  B115200 = 27
} UART_BaudRate_t;

void uart_init(UART_BaudRate_t baud_rate, uint8_t parity_bit);
void uart_read_byte(char* byte);
void uart_write_byte(char byte);

#endif /* UART_H */
