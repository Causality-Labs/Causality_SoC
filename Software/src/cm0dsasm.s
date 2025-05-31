;------------------------------------------------------------------------------------------------------
; A Simple SoC  Application
; Toggle LEDs at a given frequency. 
;------------------------------------------------------------------------------------------------------



; Vector Table Mapped to Address 0 at Reset

						PRESERVE8 ;  Ensures 8-byte alignment of the stack
                		THUMB ; Assembles instructions in Thumb mode

        				AREA	RESET, DATA, READONLY ; First 32 WORDS is VECTOR TABLE, Defines a read-only data section named RESET — used for the vector table
        				EXPORT 	__Vectors ; Exports the symbol __Vectors so it can be linked at address 0
					
__Vectors		    	DCD		0x00003FFC							; 1K Internal Memory
        				DCD		Reset_Handler
        				DCD		0  			
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD 	0
        				DCD		0
        				DCD		0
        				DCD 	0
        				DCD		0
        				
        				; External Interrupts
						        				
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
        				DCD		0
              
                AREA |.text|, CODE, READONLY
;Reset Handler
Reset_Handler   PROC
                GLOBAL Reset_Handler
                ENTRY

READ_SWITCH		LDR 	R1, =0x51000000				;Read input from switch register
				LDR		R2, [R1]


WRITE_LED		LDR 	R1, =0x50000000				;Write to LED
				STR		R2, [R1]



				LDR		R0, =0x2FFFFF				;Delay
Loop			SUBS	R0,R0,#1
				BNE Loop


				B READ_SWITCH
				ENDP


				ALIGN 		4						; Align to a word boundary

		END                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
   