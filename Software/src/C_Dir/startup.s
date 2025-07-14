; Define stack size as 1024 bytes (0x400)
Stack_Size      EQU     0x00000400

; Create uninitialized RAM section for stack, 4-byte aligned
                AREA    STACK, NOINIT, READWRITE, ALIGN=4
Stack_Mem       SPACE   Stack_Size  ; Reserve 1024 bytes for stack
__initial_sp  ; Label marking top of stack (initial stack pointer)

; Define heap size as 1024 bytes (0x400)
Heap_Size       EQU     0x00000400

; Create uninitialized RAM section for heap, 4-byte aligned
                AREA    HEAP, NOINIT, READWRITE, ALIGN=4
__heap_base  ; Label marking start of heap
Heap_Mem        SPACE   Heap_Size  ; Reserve 1024 bytes for heap
__heap_limit  ; Label marking end of heap


; Vector Table Mapped to Address 0 at Reset
                        PRESERVE8
                        THUMB

                        AREA    RESET, DATA, READONLY
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
                IMPORT  __main
                LDR     R0, =__main               
                BX      R0                        ;Branch to __main
                ENDP

Timer_Handler   PROC
                EXPORT Timer_Handler
                IMPORT Timer_ISR
                PUSH    {R0,R1,R2,LR}
                BL Timer_ISR
                POP     {R0,R1,R2,PC}                    ;return
                ENDP

UART_Handler    PROC
                EXPORT UART_Handler
                IMPORT UART_ISR
                PUSH    {R0,R1,R2,LR}
                BL UART_ISR
                POP     {R0,R1,R2,PC}
                ENDP

                ALIGN       4                    ; Align to a word boundary

; User Initial Stack & Heap
                IF      :DEF:__MICROLIB
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                ELSE
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

        END