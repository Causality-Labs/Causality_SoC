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

AGAIN			LDR     R1, =0x50000004     ; R1 = first word-address
				LDR     R2, =0x50000190     ; R2 = last word-address
				MOVS    R3, #0xFF           ; R3 = value to write

Top				STR     R3, [R1]            ; *R1 = 0xFF
				ADDS    R1, R1,#4           ; advance pointer by 4 bytes
				CMP     R1, R2              ; have we reached (or passed) the end?
				BLS     Top                ; if R1 = R2, repeat
				
				LDR     R1, =0x5000EE04    ; R1 = first word-address
				LDR     R2, =0x5000EF90     ; R2 = last word-address
				MOVS    R3, #0xFF           ; R3 = value to write

Bottom			STR     R3, [R1]            ; *R1 = 0xFF
				ADDS    R1, R1,#4           ; advance pointer by 4 bytes
				CMP     R1, R2              ; have we reached (or passed) the end?
				BLS     Bottom                ; if R1 = R2, repeat
				
				LDR     R1, =0x50000004     ; R1 = first word-address
				LDR     R2, =0x5000EE04     ; R2 = last word-address
				MOVS    R3, #0xFF           ; R3 = value to write
				
Left			STR   R3, [R1]
				ADDS  R1, R1, #127   ; first chunk (max 7 for Rn,Rn,#imm3 or #127 for SP-form)
				ADDS  R1, R1, #127
				ADDS  R1, R1, #127
				ADDS  R1, R1, #127
				ADDS  R1, R1, #4    ; 127×4 + 4 = 512, you’d adjust so total = 512
				CMP   R1, R2
				BLS   Left
		
				LDR     R1, =0x50000190     ; R1 = first word-address
				LDR     R2, =0x5000EF90     ; R2 = last word-address
				MOVS    R3, #0xFF           ; R3 = value to write
				
Right			STR   R3, [R1]
				ADDS  R1, R1, #127   ; first chunk (max 7 for Rn,Rn,#imm3 or #127 for SP-form)
				ADDS  R1, R1, #127
				ADDS  R1, R1, #127
				ADDS  R1, R1, #127
				ADDS  R1, R1, #4    ; 127×4 + 4 = 512, you’d adjust so total = 512
				CMP   R1, R2
				BLS   Right
				
				
				B AGAIN
				ENDP

				ALIGN 		4					 ; Align to a word boundary 

		END                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
    