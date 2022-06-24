; asm6809 divtest.asm div16.asm
				ORG		$2000
				LDS		#$6000
				LDD		#5000
				STD		DIV16_NUMER,PCR
				LDD		#50
				STD		DIV16_DENOM,PCR
				BSR		DIV16
				LDX		DIV16_DENOM,PCR
				LDY		DIV16_NUMER,PCR

				LDA		#$FF
				STA		$0

INF				BRA		INF

