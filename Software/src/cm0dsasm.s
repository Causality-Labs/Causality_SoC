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
                


AGAIN           
                ;Read from switch, and output to LEDs
                
                LDR     R1, =0x53000004     ;GPIO direction reg
                MOVS    R0, #00             ;direction input
                STR     R0, [R1]
                
                LDR     R1, =0x53000000     ;GPIO data reg
                LDR     R2, [R1]            ;input data from the switch
                
                LDR     R1, =0x53000004     ;change direction to output
                MOVS    R0, #01
                STR     R0, [R1]            

                LDR     R1, =0x53000000     ;output to LED
                STR     R2, [R1]

                ;Read the current timer value, and output to 7-segments
                
    


                ENDP

                ALIGN       4                    ; Align to a word boundary

        END