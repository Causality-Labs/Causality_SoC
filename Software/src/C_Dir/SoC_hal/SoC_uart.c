#include "SoC_uart.h"

void uart_init(UART_BaudRate_t baud_rate, uint8_t parity_bit)
{
    UART->BAUD = baud_rate;
    UART->PARITY = parity_bit;

    return;
}

void uart_read_byte(char* byte)
{
    *byte = UART->DATA;

    return;
}

void uart_write_byte(char byte)
{
    UART->DATA = byte;
}
