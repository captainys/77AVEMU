				; by Soji Yamakawa 2017/10/24
				; Division Signed 16bit/Signed 16bit integers

				EXPORT	DIV16_NUMER
				EXPORT	DIV16_DENOM
				EXPORT	DIV16

DIV16_NUMER		RZB		2		; Input Numerator,   Output Remainder
DIV16_DENOM		RZB		2		; Input Denominator, Output Quotient

DIV16			PSHS	A,B,X,U

				LDD		DIV16_NUMER,PCR ; Updates NEGA flag
				STA		DIV16_NUMERSGN,PCR
				BPL		DIV16_NUMPOSI   ; Jump NEGA==0
				COMA
				COMB
				ADDD	#1
				STD		DIV16_NUMER,PCR
DIV16_NUMPOSI

				LDD 	DIV16_DENOM,PCR ; Updates NEGA flag
				STA		DIV16_DENOMSGN,PCR
				BPL		DIV16_DENPOSI   ; Jump NEGA==0
				COMA
				COMB
				ADDD	#1
				STD		DIV16_DENOM,PCR
DIV16_DENPOSI
				; Come out with D=DENOM

				LDX		#1
				STX		DIV16_MULTI,PCR
				LDX		#0		; X will be the unsigned quotient

				; Come in with D=DENOM
DIV16_UPSHIFTLP	CMPD	DIV16_NUMER,PCR
				BCC		DIV16_LOOP_IN
				LSLB
				ROLA
				BMI		DIV16_LOOP_IN

				STD		DIV16_DENOM,PCR
				LSL		DIV16_MULTI+1,PCR
				ROL		DIV16_MULTI,PCR
				BPL		DIV16_UPSHIFTLP

DIV16_LOOP_IN	LDD		DIV16_NUMER,PCR
DIV16_LOOP		CMPD	DIV16_DENOM,PCR
				BLO		DIV16_DONTADD

				SUBD	DIV16_DENOM,PCR
DIV16_MULTIM2	LEAX	$7FFF,X	; $7FFF will be replaced with MULTI
DIV16_MULTI		EQU		DIV16_MULTIM2+2

DIV16_DONTADD	LSR		DIV16_DENOM,PCR   ; Higher bits
				ROR		DIV16_DENOM+1,PCR ; Lower bits

				LSR		DIV16_MULTI,PCR
				ROR		DIV16_MULTI+1,PCR
				BCC		DIV16_LOOP

DIV16_END		STX		DIV16_DENOM,PCR ; Quotient in DENOM
				STD		DIV16_NUMER,PCR	; Remainder in NUMER
				LDD		#0

				TST		DIV16_NUMERSGN,PCR
				BPL		DIV16_SETSIGN

				; Remainder needs to be same sign as the numerator
				; A and B are already zero.
				SUBD	DIV16_NUMER,PCR
				STD		DIV16_NUMER,PCR
				; Remainder may be zero, in which case A register
				; does not reflect the sign of the numerator.
				; A must be reset to a negative number.
				LDA		#-1

DIV16_SETSIGN	; A is zero or positive if jumped from BPL, or if not -1.
				EORA	DIV16_DENOMSGN,PCR
				BPL		DIV16_RETURN

				LDD		#0
				SUBD	DIV16_DENOM,PCR
				STD		DIV16_DENOM,PCR

DIV16_RETURN	PULS	A,B,X,U,PC


DIV16_SIGN		RZB		2
DIV16_NUMERSGN	EQU		DIV16_SIGN
DIV16_DENOMSGN	EQU		DIV16_SIGN+1
