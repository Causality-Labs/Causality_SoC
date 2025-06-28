;------------------------------------------------------------------------------------------------------
; Design and Implementation of an AHB Interrupt Mechanism  
; 1)Input characters from keyboard (UART) and output to the terminal (using interrupt)
; 2)A counter is incremented from 1 to 10 and displayed on the 7-segment display (using interrupt)
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
                                                
                        DCD     Timer_Handler
                        DCD     UART_Handler
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
                
                ;Configure the baudrate of the UART peripheral
                ; 326 -> 9600
                ; 162 -> 19200
                ;  81 -> 38400
                ;  54 -> 57600
                ;  27 -> 115200
                LDR     R2, =0x51000008
                LDR     R0, =27
                STR     R0, [R2]

                LDR     R1, =0xE000E400           ;Interrupt Priority Register
                LDR     R0, =0x00004000           ;Priority: IRQ0(Timer): 0x00, IRQ1(UART): 0x40
                STR     R0, [R1]
                LDR     R1, =0xE000E100           ;Interrupt Set Enable Register
                ;Enable interrupts for UART and timer
                ; bit [1] -> UART, bit[0] -> Timer.
                LDR     R0, =0x00000003
                STR     R0, [R1]
        

                ;Configure the timer
                ; Sample Control value = 0x11,10001 ->  clk16:FREE_RUN:enable  
                ; Sample Control value = 0x13,10011 ->  clk16:PERIODIC:enable
                ; Sample Control value = 0x15,10101 ->  clk16:COMPARE:enable
                ; Sample Control value = 0x17,10111 ->  clk16:PWM:enable

                LDR     R1, =0x52000000     ;timer load value register
                LDR     R0, =0x02FAF080     ;=50,000,000 (system tick frequency)
                STR     R0, [R1]            
                LDR     R1, =0x52000008     ;timer control register
                MOVS    R0, #0x03           ;prescaler=1, enable timer, reload mode -> clk16:PERIODIC:enable
                STR     R0, [R1]

                LDR     R5, =0x00000030     ;counting-up counter, start from '0' (ascii=0x30)  

AGAIN                       
                B       AGAIN       


                ENDP

Timer_Handler   PROC
                EXPORT Timer_Handler
                PUSH    {R0,R1,R2,LR}

                LDR     R1, =0x5200000c     ;clear timer interrupt
                MOVS    R0, #0x01
                STR     R0, [R1]

                LDR     R1, =0x50000000     ;VGA BASE
                STR     R5, [R1]
                ADDS    R5, R5, #0x01
                CMP     R5, #0x3A
                BNE     NEXT
                LDR     R1, =0x52000008     ;timer control register
                MOVS    R0, #0x00           ;Stop timer if counts to 9
                STR     R0, [R1]                

NEXT            MOVS    R0, #' '
                STR     R0, [R1]
                
                POP     {R0,R1,R2,PC}                    ;return
                ENDP

UART_Handler    PROC
                EXPORT UART_Handler

                PUSH    {R0,R1,R2,LR}
                LDR     R1, =0x51000000               ;UART
                LDR     R0, [R1]                      ;Get Data from UART
                STR     R0, [R1]                      ;Write to UART

                POP     {R0,R1,R2,PC}
                ENDP

                ALIGN       4                    ; Align to a word boundary

        END                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
   