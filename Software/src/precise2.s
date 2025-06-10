;------------------------------------------------------------------------------------------------------
; Design and Implementation of an AHB VGA Peripheral
; 1)Display text string: "TEST" on VGA. 
; 2)Change the color of the four corners of the image region.
;------------------------------------------------------------------------------------------------------

; Vector Table Mapped to Address 0 at Reset

						PRESERVE8
                		THUMB

        				AREA	RESET, DATA, READONLY	  			; First 32 WORDS is VECTOR TABLE
        				EXPORT 	__Vectors
					
__Vectors		    	DCD		0x00003FFC
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

;Write "TEST" to the text console

				LDR 	R1, =0x50000000
				MOVS	R0, #'T'
				STR		R0, [R1]

				LDR 	R1, =0x50000000
				MOVS	R0, #'E'
				STR		R0, [R1]

				LDR 	R1, =0x50000000
				MOVS	R0, #'S'
				STR		R0, [R1]
				
				LDR 	R1, =0x50000000
				MOVS	R0, #'T'
				STR		R0, [R1]

;Rectangle in my frame buffer
dat
AGAIN			LDR     R1, =0x50000004     ; R1 = first word-address
				LDR     R2, =0x500000C8     ; R2 = last word-address
				MOVS    R3, #0xFF           ; R3 = value to write

Top				STR     R3, [R1]            ; *R1 = 0xFF
				SUBS    R3, R3,#1
				ADDS    R1, R1,#4           ; advance pointer by 4 bytes
				CMP     R1, R2              ; have we reached (or passed) the end?
				BLS     Top                ; if R1 = R2, repeat
				
				
                LDR     R1, =0x50000104     ; R1 = first word-address
				LDR     R2, =0x500001C8     ; R2 = last word-address

Top2			STR     R3, [R1]            ; *R1 = 0xFF
				SUBS    R3, R3,#1
				ADDS    R1, R1,#4           ; advance pointer by 4 bytes
				CMP     R1, R2              ; have we reached (or passed) the end?
				BLS     Top2                ; if R1 = R2, repeat
				
				B AGAIN
				ENDP

				ALIGN 		4					 ; Align to a word boundary 

		END                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
    