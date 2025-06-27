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

                LDR     R2, =0x51000008
                LDR     R0, =27 ;115200
                STR     R0, [R2]

                ;Write "TEST" to the text console and the UART
                LDR 	R1, =0x50000000		;VGA BASE
                LDR 	R2, =0x51000000		;UART BASE
                MOVS	R0, #'T'
                STR		R0, [R1]
                STR		R0, [R2]

                LDR 	R1, =0x50000000		;VGA BASE
                LDR 	R2, =0x51000000		;UART BASE
                MOVS	R0, #'E'
                STR		R0, [R1]
                STR		R0, [R2]

                LDR 	R1, =0x50000000		;VGA BASE
                LDR 	R2, =0x51000000		;UART BASE
                MOVS	R0, #'S'
                STR		R0, [R1]
                STR		R0, [R2]
                
                LDR 	R1, =0x50000000		;VGA BASE
                LDR 	R2, =0x51000000		;UART BASE
                MOVS	R0, #'T'
                STR		R0, [R1]
                STR		R0, [R2]



                ;Configure the timer

                ; Set Load Value
                LDR     R1, =0x52000000         ;timer load value register
                LDR     R0, =0xFFFFFFFF         ;maximum timer value, timer will decrement here.
                STR     R0, [R1]

                ;Set Compare Value
                LDR     R1, =0x52000010   ;Compare register
                LDR     R0, =0xFF000000         ;Comapre value
                STR     R0, [R1]

                ; Sample Control value = 0x11,10001 ->  clk16:FREE_RUN:enable  
                ; Sample Control value = 0x13,10011 ->  clk16:PERIODIC:enable
                ; Sample Control value = 0x15,10101 ->  clk16:COMPARE:enable
                ; Sample Control value = 0x17,10111 ->  clk16:PWM:enable

                LDR     R1, =0x52000008         ;timer control register
                MOVS    R0, #0x15               ;set prescaler, reload mode, start timer
                STR     R0, [R1]

                ; Write to Mask Register
                LDR     R1, =0x53000008     ;Mask Register
                MOVS    R0, #0x0F           ;Mask last all bits
                STR     R0, [R1]

                ;Set seven segment display to decimal mode
                LDR     R1, =0x54000010         ;timer load value register
                LDR     R0, =0x00               ;maximum load value
                STR     R0, [R1]

                ;Read from switch, and output to LEDs
AGAIN           LDR     R1, =0x53000004     ;GPIO direction reg
                MOVS    R0, #0x0F           ;set bit [7:4] as inputs and pin [3:0] as outputs
                STR     R0, [R1]
                
                LDR     R1, =0x53000000     ;GPIO data reg
                LDR     R2, [R1]            ;input data from the switch

                LDR     R1, =0x53000000     ;output to LED
                LSRS    R2, R2,#4
                STR     R2, [R1]

                ;Read the current timer value, and output to 7-segments
                LDR     R1, =0x52000004      ;timer current value register
                LDR     R3, [R1]
                LSRS    R3, R3, #16          ;choose the higher 16 bits

                                
                MOVS    R0, R3              ;display the 1st digit
                LDR     R2, =0x0F
                ANDS    R0, R0, R2
                ;MOVS    R0, #0x0F
                LDR     R1, =0x54000000
                STR     R0, [R1]


                LSRS    R0, R3, #4           ;display the 2nd digit
                LDR     R2, =0x0F
                ANDS    R0, R0, R2
                ;MOVS    R0, #0x09
                LDR     R1, =0x54000004
                STR     R0, [R1]
                                
                                
                                
                LSRS    R0, R3, #8           ;display the 3rd digit
                LDR     R2, =0x0F
                ANDS    R0, R0, R2
                ;MOVS    R0, #0x0F
                LDR     R1, =0x54000008
                STR     R0, [R1]
                                
                                
                LSRS    R0, R3, #12        ;display the 4th digit
                LDR     R2, =0x0F
                ANDS    R0, R0, R2
                ;MOVS    R0, #0x09
                LDR     R1, =0x5400000C
                STR     R0, [R1]

                B      AGAIN

                ENDP

                ALIGN       4              ; Align to a word boundary

        END