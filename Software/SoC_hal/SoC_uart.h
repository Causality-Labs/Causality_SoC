#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "SoC_CM0.h"
#include "core_cm0.h"

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
