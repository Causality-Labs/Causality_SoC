#include "seven_segment.h"

void seven_seg_write(char dig1, char dig2, char dig3, char dig4, uint8_t disp_mode)
{
    
    SEVSEG->DIGIT1 = dig4;
    SEVSEG->DIGIT2 = dig3;
    SEVSEG->DIGIT3 = dig2;
    SEVSEG->DIGIT4 = dig1;
    SEVSEG->DISP = disp_mode;

    return;
}