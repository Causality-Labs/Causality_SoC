#include <stdint.h>
#include "SoC_gpio.h"

void GPIO_init(uint8_t direction)
{
    GPIO->DIR = direction;
    GPIO->MASK = 0xFF;

    return;
}

uint8_t GPIO_read_pin(uint8_t pin_number)
{
    return (GPIO->DATA >> pin_number) & 0x01;
}

void GPIO_write_pin(uint8_t pin_number, uint8_t value)
{
    if (value) {
        GPIO->DATA |= (1 << pin_number);
    } else {
        GPIO->DATA &= ~(1 << pin_number);
    }

    return;
}
