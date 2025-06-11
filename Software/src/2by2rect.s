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
RES_SET         LDR     R1, =0x50F00000
	            LDR     R0, =0x01
				STR     R0, [R1]
				MOVS    R3, #0xFF           ; R3 = value to write

AGAIN			LDR     R1, =0x50000004     ; R1 = first word-address
				LDR     R2, =0x50000320     ; R2 = last word-address


Top				STR     R3, [R1]            ; *R1 = 0xFF
                SUBS    R3, R3, #1
				ADDS    R1, R1,#4           ; advance pointer by 4 bytes
				CMP     R1, R2              ; have we reached (or passed) the end?
				BLS     Top                ; if R1 = R2, repeat

				LDR     R1, =0x50000320     ; R1 = first word-address
				LDR     R2, =0x5003BF20     ; R2 = last word-address
				
Right			STR   R3, [R1]            ; must add 512 to move down 1
				SUBS  R3, R3, #1
				LDR   R4, =0x400          ; Load constant 0x100 into R4
                ADDS  R1, R1, R4          ; Advance pointer by 0x100
				CMP   R1, R2
				BLS   Right
				
				LDR     R1, =0x5003BC04    ; R1 = first word-address
				LDR     R2, =0x5003BF20   ; R2 = last word-address

Bottom			STR     R3, [R1]            ; *R1 = 0xFF
                SUBS    R3, R3,#1
				ADDS    R1, R1,#4           ; advance pointer by 4 bytes
				CMP     R1, R2              ; have we reached (or passed) the end?
				BLS     Bottom                ; if R1 = R2, repeat
				
				LDR     R1, =0x50000004     ; R1 = first word-address
				LDR     R2, =0x5003BC04     ; R2 = last word-address
				
Left			STR   R3, [R1]            ; must add 512 to move down 1
				SUBS  R3, R3, #1
				LDR   R4, =0x400          ; Load constant 0x100 into R4
                ADDS  R1, R1, R4          ; Advance pointer by 0x100
				CMP   R1, R2
				BLS   Left

				LDR		R0, =0x2FFFFF				;Delay
Loop			SUBS	R0,R0,#1
				BNE Loop
				
				B AGAIN
				ENDP

				ALIGN 		4					 ; Align to a word boundary 

		END                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
    