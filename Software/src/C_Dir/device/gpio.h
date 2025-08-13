#ifndef GPIO_H
#define GPIO_H

#include "core_cm0.h"
#include <stdint.h>

#define AHB_GPIO_BASE       0x53000000

typedef struct
{
  volatile  unsigned int  DATA;
  volatile  unsigned int  DIR;
  volatile  unsigned int  MASK;
} GPIO_TypeDef;

#define GPIO            ((GPIO_TypeDef  *) AHB_GPIO_BASE)

void GPIO_init(uint8_t direction);
uint8_t GPIO_read_pin(uint8_t pin_number);
void GPIO_write_pin(uint8_t pin_number, uint8_t value);

#endif /* GPIO_H */