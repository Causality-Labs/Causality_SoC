;------------------------------------------------------------------------------------------------------
; Design and Implementation of an AHB timer, a GPIO peripheral, and a 7-segment display peripheral  
; 1)Display text string: "TEST" on VGA.
; 2)Input data from switches and output them to LEDs;
; 3)Display the timer value to the 7-segment display.
;------------------------------------------------------------------------------------------------------


; Vector Table Mapped to Address 0 at Reset

                        PRESERVE8
                        THUMB

                        AREA    RESET, DATA, READONLY               ; First 32 WORDS is VECTOR TABLE
                        EXPORT  __Vectors
                    
__Vectors               DCD     0x00003FFC
                        DCD     Reset_Handler
                        DCD     0           
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        
                        ; External Interrupts
                                                
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
                        DCD     0
              
                AREA |.text|, CODE, READONLY
;Reset Handler
Reset_Handler   PROC
                GLOBAL Reset_Handler
                ENTRY

                ;Write "TEST" to the text console and the UART




                ;Configure the timer
                LDR     R1, =0x52000000         ;timer load value register
                LDR     R0, =0xFFFFFFFF         ;maximum load value
                STR     R0, [R1]

                LDR     R1, =0x52000010         ;Compare register
                LDR     R0, =0xFF00FFFF         ;Comapre value
                STR     R0, [R1]

                LDR     R1, =0x52000008         ;timer control register
                MOVS    R0, #0x07               ;set prescaler, reload mode, start timer
                STR     R0, [R1]

                ; Write to Mask Register
                LDR     R1, =0x53000008     ;Mask Register
                MOVS    R0, #0x0F           ;Mask last all bits
                STR     R0, [R1]

                ;Read from switch, and output to LEDs
AGAIN           LDR     R1, =0x53000004     ;GPIO direction reg
                MOVS    R0, #0x0F           ;set bit [7:4] as inputs and pin [3:0] as outputs
                STR     R0, [R1]
                
                LDR     R1, =0x53000000     ;GPIO data reg
                LDR     R2, [R1]            ;input data from the switch

                LDR     R1, =0x53000000     ;output to LED
                LSRS    R2, R2, #4
                STR     R2, [R1]

                ;Read the current timer value, and output to 7-segments
                LDR     R1, =0x52000004      ;timer current value register
                LDR     R3, [R1]
                LSRS    R3, R3, #16          ;choose the higher 16 bits

                                
                MOVS    R0, R3              ;display the 1st digit
                LDR     R2, =0x0F
                ANDS    R0, R0, R2
                LDR     R1, =0x54000000
                STR     R0, [R1]


                LSRS    R0, R3, #4           ;display the 2nd digit
                LDR     R2, =0x0F
                ANDS    R0, R0, R2
                LDR     R1, =0x54000004
                STR             R0, [R1]
                                
                                
                                
                LSRS    R0, R3, #8           ;display the 3rd digit
                LDR     R2, =0x0F
                ANDS    R0, R0, R2
                LDR     R1, =0x54000008
                STR             R0, [R1]
                                
                                
                LSRS    R0, R3, #12        ;display the 4th digit
                LDR     R2, =0x0F
                ANDS    R0, R0, R2
                LDR     R1, =0x5400000C
                STR     R0, [R1]

                B      AGAIN

                ENDP

                ALIGN       4              ; Align to a word boundary

        END