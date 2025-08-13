#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include "core_cm0.h"
#include <stdint.h>

#define AHB_7SEG_BASE       0x54000000

typedef struct
{
  volatile  unsigned int  DIGIT1;
  volatile  unsigned int  DIGIT2;
  volatile  unsigned int  DIGIT3;
  volatile  unsigned int  DIGIT4;
  volatile  unsigned int  DISP;
} SEVENSEG_TypeDef;

#define SEVSEG          ((SEVENSEG_TypeDef  *) AHB_7SEG_BASE)
#define DECIMAL_MODE 0x01
#define HEX_MODE 0x00

void seven_seg_write(char dig1, char dig2, char dig3, char dig4, uint8_t disp_mode);

#endif /* SEVEN_SEGMENT_H */