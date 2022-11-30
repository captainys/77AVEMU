; This program is for testing if Sub-CPU halts a few clocks after #$80 is written to $FD05.
; Conclusion: There was no such delay observed in real FM77AV.

					ORG		$3FFE

RESULT				FDB		0

					PSHS	A,B,X,Y,U,DP

					LDA		#$FD
					TFR		A,DP

WAIT_READY			LDA		<$05
					BMI		WAIT_READY

					LDX		#0
					LDA		#$80
					STA		<$05
					LDA		<$05
					BMI		SUB_HALTED

WAIT_BUSY			LEAX	1,X
					LDA		<$05
					BPL		WAIT_BUSY

SUB_HALTED
					CLR		$FC82
					LDA		#$80
					STA		$FC80
					CLR		<$05

					STX		RESULT,PCR

					PULS	A,B,X,Y,U,DP,PC

