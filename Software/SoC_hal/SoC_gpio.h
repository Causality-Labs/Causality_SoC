#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "SoC_CM0.h"
#include "core_cm0.h"

void GPIO_init(uint8_t direction);
uint8_t GPIO_read_pin(uint8_t pin_number);
void GPIO_write_pin(uint8_t pin_number, uint8_t value);

#endif /* GPIO_H */
