#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include <stdint.h>
#include "SoC_CM0.h"
#include "core_cm0.h"

#define DECIMAL_MODE 0x01
#define HEX_MODE 0x00

void seven_seg_write(char dig1, char dig2, char dig3, char dig4, uint8_t disp_mode);

#endif /* SEVEN_SEGMENT_H */
