; MMR access to sub-CPU ram from main CPU will return $FF if sub-CPU is not halt.
; $D401 should be $0D from LOADM"",,R if sub-CPU is halt, $FF is sub-CPU is running.

						ORG		$4000

						BRA		MAIN		; $4000
TRAP_POWER_OFF_AT		RTS					; $4002 To catch -poweroffat
RETURN_CODE				FCB		0			; $4003 for success condition

MAIN
						CLR		$6401

						LDA		#$30
						LDX		#$FD80
SETUP_LOOP				STA		,X+
						INCA
						CMPA	#$40
						BNE		SETUP_LOOP

						LDA		#$1D
						STA		$FD86		; Sub D000-DFFF to Main 6000-6FFF

						LDA		#$80
						STA		$FD93		; Enable MMR



HALT_AND_READ
HALT_SUB				LDA		$FD05
						BMI		HALT_SUB

						LDA		#$80
						STA		$FD05

CONFIRM_HALT			LDA		$FD05
						BPL		CONFIRM_HALT

						LDA		$6401	; Sub-CPU is halt.  It should read $0D.  Fail if $FF.
						CMPA	#$FF
						BNE		TEST_1_PASS

						; A=#$FF
						STA		RETURN_CODE

TEST_1_PASS
						LDA		#$80
						STA		$FC80
						CLR		$FC82
						CLR		$FD05



JUST_READ
						LDA		$6401	; Sub-CPU is running.  It should read $FF.  Fail if not $FF.
						CMPA	#$FF
						BEQ		TEST_2_PASS

						LDA		#$FF
						STA		RETURN_CODE

TEST_2_PASS
						BSR		TRAP_POWER_OFF_AT
						RTS
