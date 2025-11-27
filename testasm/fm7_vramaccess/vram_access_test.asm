					ORG		$2000

RESULT1_LOC			EQU		$FC00
;                                           FM7($00)      FM77AV($3F)
; Write without VRAM Access Flag
; b0 Blue Plane  1->Same 2->Different        0               1
; b1 Red Plane   1->Same 2->Different        0               1
; b2 Green Plane 1->Same 2->Different        0               1
; Read without VRAM Access Flag
; b3 Blue Plane  1->Same 2->Different        0               1
; b4 Red Plane   1->Same 2->Different        0               1
; b5 Green Plane 1->Same 2->Different        0               1

RESULT2_LOC			EQU		$FC01
;                                           FM7($0F)      FM77AV($3F)
; Write and Read with VRAM-Access Flag set
; b0 Blue Plane  1->Same 2->Different        1               1
; b1 Red Plane   1->Same 2->Different        1               1
; b2 Green Plane 1->Same 2->Different        1               1
; Write and Read without VRAM Access Flag, but mask CRTC access for Blue plane
; b3 Blue Plane  1->Same 2->Different        1               1
; b4 Red Plane   1->Same 2->Different        0               1
; b5 Green Plane 1->Same 2->Different        0               1

RESULT3_LOC			EQU		$FC02
;                                           FM7($22)      FM77AV($3F)
; Write and Read without VRAM Access Flag, but mask CRTC access for Green Plane
; b0 Blue Plane  1->Same 2->Different        0               1
; b1 Red Plane   1->Same 2->Different        1               1
; b2 Green Plane 1->Same 2->Different        0               1
; Write and Read without VRAM Access Flag, but mask CRTC access for Red Plane
; b3 Blue Plane  1->Same 2->Different        0               1
; b4 Red Plane   1->Same 2->Different        0               1
; b5 Green Plane 1->Same 2->Different        1               1


MAIN
					ORCC	#$50

					LDA		$FD0F
					CLR		RESULT1_LOC
					CLR		RESULT2_LOC
					CLR		RESULT3_LOC

					; Write Test
					; Transfer F-BASIC ROM to VRAM without Setting the VRAM-Access Flag

					LDU		#0	; U To (in sub RAM)
					LBSR	WRITE_NO_FLAG

					LDU		#$4000	; U To (in sub RAM)
					LBSR	WRITE_NO_FLAG

					LDU		#$8000	; U To (in sub RAM)
					LBSR	WRITE_NO_FLAG

					LDX		#$0000	; X From (in sub RAM)
					LDA		#1
					LDY		#RESULT1_LOC
					LBSR	READ_COMPARE_WITH_FLAG

					LDX		#$4000	; X From (in sub RAM)
					LDA		#2
					LDY		#RESULT1_LOC
					LBSR	READ_COMPARE_WITH_FLAG

					LDX		#$8000	; X From (in sub RAM)
					LDA		#4
					LDY		#RESULT1_LOC
					LBSR	READ_COMPARE_WITH_FLAG



					; Read Test
					; Transfer with flag, but Read without flag and compare
					LDU		#0	; U To (in sub RAM)
					LBSR	WRITE_WITH_FLAG

					LDU		#$4000	; U To (in sub RAM)
					LBSR		WRITE_WITH_FLAG

					LDU		#$8000	; U To (in sub RAM)
					LBSR	WRITE_WITH_FLAG

					LDX		#$0000	; X From (in sub RAM)
					LDA		#8
					LDY		#RESULT1_LOC
					LBSR	READ_COMPARE_NO_FLAG

					LDX		#$4000	; X From (in sub RAM)
					LDA		#16
					LDY		#RESULT1_LOC
					LBSR		READ_COMPARE_NO_FLAG

					LDX		#$8000	; X From (in sub RAM)
					LDA		#32
					LDY		#RESULT1_LOC
					LBSR		READ_COMPARE_NO_FLAG



					; Write and Read with VRAM Access Flag
					LDU		#0	; U To (in sub RAM)
					LBSR		WRITE_WITH_FLAG

					LDU		#$4000	; U To (in sub RAM)
					LBSR		WRITE_WITH_FLAG

					LDU		#$8000	; U To (in sub RAM)
					LBSR		WRITE_WITH_FLAG

					LDX		#$0000	; X From (in sub RAM)
					LDA		#1
					LDY		#RESULT2_LOC
					LBSR	READ_COMPARE_WITH_FLAG

					LDX		#$4000	; X From (in sub RAM)
					LDA		#2
					LDY		#RESULT2_LOC
					LBSR	READ_COMPARE_WITH_FLAG

					LDX		#$8000	; X From (in sub RAM)
					LDA		#4
					LDY		#RESULT2_LOC
					LBSR	READ_COMPARE_WITH_FLAG


					; Read write without flag, but block CRTC access to Blue Plane
					LDA		#$10
					STA		$FD37

					LDU		#0	; U To (in sub RAM)
					LBSR	WRITE_NO_FLAG

					LDU		#$4000	; U To (in sub RAM)
					LBSR	WRITE_NO_FLAG

					LDU		#$8000	; U To (in sub RAM)
					LBSR	WRITE_NO_FLAG

					LDX		#$0000	; X From (in sub RAM)
					LDA		#8
					LDY		#RESULT2_LOC
					LBSR	READ_COMPARE_NO_FLAG

					LDX		#$4000	; X From (in sub RAM)
					LDA		#16
					LDY		#RESULT2_LOC
					LBSR	READ_COMPARE_NO_FLAG

					LDX		#$8000	; X From (in sub RAM)
					LDA		#32
					LDY		#RESULT2_LOC
					LBSR	READ_COMPARE_NO_FLAG



					; Read write without flag, but block CRTC access to Red Plane
					LDA		#$20
					STA		$FD37

					LDU		#0	; U To (in sub RAM)
					LBSR	WRITE_NO_FLAG

					LDU		#$4000	; U To (in sub RAM)
					LBSR	WRITE_NO_FLAG

					LDU		#$8000	; U To (in sub RAM)
					LBSR	WRITE_NO_FLAG

					LDX		#$0000	; X From (in sub RAM)
					LDA		#1
					LDY		#RESULT3_LOC
					BSR		READ_COMPARE_NO_FLAG

					LDX		#$4000	; X From (in sub RAM)
					LDA		#2
					LDY		#RESULT3_LOC
					BSR		READ_COMPARE_NO_FLAG

					LDX		#$8000	; X From (in sub RAM)
					LDA		#4
					LDY		#RESULT3_LOC
					BSR		READ_COMPARE_NO_FLAG


					; Read write without flag, but block CRTC access to Green Plane
					LDA		#$40
					STA		$FD37

					LDU		#0	; U To (in sub RAM)
					BSR		WRITE_NO_FLAG

					LDU		#$4000	; U To (in sub RAM)
					BSR		WRITE_NO_FLAG

					LDU		#$8000	; U To (in sub RAM)
					BSR		WRITE_NO_FLAG

					LDX		#$0000	; X From (in sub RAM)
					LDA		#8
					LDY		#RESULT3_LOC
					BSR		READ_COMPARE_NO_FLAG

					LDX		#$4000	; X From (in sub RAM)
					LDA		#16
					LDY		#RESULT3_LOC
					BSR		READ_COMPARE_NO_FLAG

					LDX		#$8000	; X From (in sub RAM)
					LDA		#32
					LDY		#RESULT3_LOC
					BSR		READ_COMPARE_NO_FLAG

					CLR		$FD37

					RTS



; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

; X  VRAM Address
READ_COMPARE_WITH_FLAG
					PSHS	A,Y
					LDU		#$3000	; U To (in main RAM)
					LDD		#$4000; D Length
					LBSR	FROM_SUB_CPU
					BSR		COMPARE
					PULS	A,Y
					BNE		READ_COMPARE_DONE
					BSR		OR_RESULT
READ_COMPARE_DONE
					RTS

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

; X  VRAM Address
READ_COMPARE_NO_FLAG
					PSHS	A,Y
					LDU		#$3000	; U To (in main RAM)
					LDD		#$4000; D Length
					LBSR	FROM_SUB_CPU_NO_FLAG
					BSR		COMPARE
					PULS	A,Y
					BNE		READ_COMPARE_DONE
					BSR		OR_RESULT
					RTS

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

; U  VRAM Address
WRITE_NO_FLAG
					LDX		#$8000	; X From (in main RAM)
					LDD		#$4000	; D Length
					LBSR	TO_SUB_CPU_NO_FLAG
					RTS

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

; U  VRAM Address
WRITE_WITH_FLAG
					LDX		#$8000	; X From (in main RAM)
					LDD		#$4000	; D Length
					BSR		TO_SUB_CPU
					RTS

; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

OR_RESULT
					ORA		,Y
					STA		,Y
					RTS


; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 


COMPARE				LDX		#$3000
					LDU		#$8000
COMPARE_LOOP		LDD		,X++
					CMPD	,U++
					BNE		COMPARE_BREAK
					CMPU	#$C000
					BNE		COMPARE_LOOP
COMPARE_BREAK
					LDD		,--X
					CMPD	,--U
					RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

MAX_ONE_TIME_TFR	EQU		($80-(SUBCPU_CMD_DATA_BUFFER-SUBCPU_TFR_CMD))

; Input D bytes left
;       Y Points to SUBCPU_TFR_SRC
; This function does:
;   D=min(MAX_ONE_TIME_TFR,D)
;   [SUBCPU_TFR_LEN]=D
TAKE_MIN_TFR_LEN
					; Transfer Length=min(bytes_left,MAX_ONE_TIME_TFR)
					CMPD	#MAX_ONE_TIME_TFR
					BLE		TAKE_MIN_TFR_LEN_RTS
					LDD		#MAX_ONE_TIME_TFR
TAKE_MIN_TFR_LEN_RTS
					STD		(SUBCPU_TFR_LEN-SUBCPU_TFR_SRC),Y
					RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; X From (in main RAM)
; U To (in sub RAM)
; D Length
TO_SUB_CPU
					LEAY	SUBCPU_TFR_SRC,PCR

					STU		(SUBCPU_TFR_DST-SUBCPU_TFR_SRC),Y	; U is free now.
					LDU		#($D380+SUBCPU_CMD_DATA_BUFFER-SUBCPU_TFR_CMD)
					STU		(SUBCPU_TFR_SRC-SUBCPU_TFR_SRC),Y

TO_SUB_CPU_OUTER_LOOP
					PSHS	A,B

					; Transfer Length=min(bytes_left,$MAX_ONE_TIME_TFR)
					BSR		TAKE_MIN_TFR_LEN	; This updates D and [SUBCPU_TFR_LEN]

					BSR		HALT_SUBCPU
					BSR		SET_YAMAUCHI_COMMAND
					LDU		#($FC80+SUBCPU_CMD_DATA_BUFFER-SUBCPU_TFR_CMD)
					; B is one-time transfer length. (From TAKE_MIN_TFR_LEN)
					; X is source.
					; U is destination.
					BSR		TFR_X_TO_U_FOR_B	; X,U incremented, A destroyed, B=0

					CLR		$FD05

					; X is incremented already.
					LDD		(SUBCPU_TFR_LEN-SUBCPU_TFR_SRC),Y
					ADDD	(SUBCPU_TFR_DST-SUBCPU_TFR_SRC),Y
					STD		(SUBCPU_TFR_DST-SUBCPU_TFR_SRC),Y
					; Destination address incremented.

					PULS	A,B

					; D is remeaining byte-count again.
					SUBD	(SUBCPU_TFR_LEN-SUBCPU_TFR_SRC),Y
					BGT		TO_SUB_CPU_OUTER_LOOP

					RTS


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; U To (in main RAM)
; X From (in sub RAM)
; D Length
FROM_SUB_CPU
					LEAY	SUBCPU_TFR_SRC,PCR

					STX		(SUBCPU_TFR_SRC-SUBCPU_TFR_SRC),Y	; X is free now.
					LDX		#($D380+SUBCPU_CMD_DATA_BUFFER-SUBCPU_TFR_CMD)
					STX		(SUBCPU_TFR_DST-SUBCPU_TFR_SRC),Y

					PSHS	A,B
					BSR		HALT_SUBCPU	; Destroys A
					PULS	A,B

FROM_SUB_CPU_OUTER_LOOP
					PSHS	A,B

					; Transfer Length=min(bytes_left,$MAX_ONE_TIME_TFR)
					BSR		TAKE_MIN_TFR_LEN	; This updates D and [SUBCPU_TFR_LEN]

					BSR		SET_YAMAUCHI_COMMAND
					CLR		$FD05

					BSR		HALT_SUBCPU	; B register is preserved.

					LDX		#($FC80+SUBCPU_CMD_DATA_BUFFER-SUBCPU_TFR_CMD)
					; B is one-time transfer length. (From TAKE_MIN_TFR_LEN)
					; X is source.
					; U is destination.
					BSR		TFR_X_TO_U_FOR_B	; X,U incremented, A destroyed, B=0

					; U is incremented already.
					LDD		(SUBCPU_TFR_LEN-SUBCPU_TFR_SRC),Y
					ADDD	(SUBCPU_TFR_SRC-SUBCPU_TFR_SRC),Y
					STD		(SUBCPU_TFR_SRC-SUBCPU_TFR_SRC),Y
					; Destination address incremented.

					PULS	A,B

					; D is remeaining byte-count again.
					SUBD	(SUBCPU_TFR_LEN-SUBCPU_TFR_SRC),Y
					BGT		FROM_SUB_CPU_OUTER_LOOP

					; Sub-CPU is HALT when come out of the loop.
					LDA		#$80
					STA		$FC80
					CLR		$FD05

					RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

TFR_X_TO_U_FOR_B
					LDA		,X+
					STA		,U+
					DECB
					BNE		TFR_X_TO_U_FOR_B
					RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SET_YAMAUCHI_COMMAND
					PSHS	A,B,X,U
					LDU		#$FC80
					LEAX	SUBCPU_TFR_CMD,PCR
					LDB		#(SUBCPU_CMD_DATA_BUFFER-SUBCPU_TFR_CMD)
					BSR		TFR_X_TO_U_FOR_B
					PULS	A,B,X,U,PC

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Don't change B.  Above code assumes B does not change in this.
HALT_SUBCPU			LDA		$FD05
					BMI		HALT_SUBCPU
					LDA		#$80
					STA		$FD05
HALT_CHECK			LDA		$FD05
					BPL		HALT_CHECK
					RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SUBCPU_TFR_CMD
					FCB		0,0
					FCB		$3F

					; FCB		"YAMAUCHI"	FM-8 Requied these 8 bytes to be "YAMAUCHI", but not FM-7 and later.

SUBCPU_CMD_SET_VRAMACCESS
					TST		$D409	; 3 bytes
					RTS				; 1 byte
SUBCPU_CMD_CLR_VRAMACCESS
					STA		$D409	; 3 bytes
					RTS				; 1 bytes

					FCB		$93
					FDB		$D380+(SUBCPU_CMD_SET_VRAMACCESS-SUBCPU_TFR_CMD)
					FCB		$91
SUBCPU_TFR_SRC		FDB	0
SUBCPU_TFR_DST			FDB	0
SUBCPU_TFR_LEN			FDB	0
					FCB		$93
					FDB		$D380+(SUBCPU_CMD_CLR_VRAMACCESS-SUBCPU_TFR_CMD)
					FCB		$90
SUBCPU_CMD_DATA_BUFFER



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; X From (in main RAM)
; U To (in sub RAM)
; D Length
TO_SUB_CPU_NO_FLAG
					TFR		D,Y

					LDA		SUBCPU_CMD_SET_VRAMACCESS,PCR
					PSHS	A
					LDA		#$39	; RTS
					STA		SUBCPU_CMD_SET_VRAMACCESS,PCR

					TFR		Y,D
					LBSR	TO_SUB_CPU

					PULS	A
					STA		SUBCPU_CMD_SET_VRAMACCESS,PCR
					RTS


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; U To (in main RAM)
; X From (in sub RAM)
; D Length
FROM_SUB_CPU_NO_FLAG
					TFR		D,Y

					LDA		SUBCPU_CMD_SET_VRAMACCESS,PCR
					PSHS	A
					LDA		#$39	; RTS
					STA		SUBCPU_CMD_SET_VRAMACCESS,PCR

					TFR		Y,D
					LBSR	FROM_SUB_CPU

					PULS	A
					STA		SUBCPU_CMD_SET_VRAMACCESS,PCR
					RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
