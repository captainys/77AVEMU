			ORG		$2000
			LDA		#$77
			LDB		#$11
			LDD		#$1973
			BITA	$3000,PCR
			EORA	A,X
			ANDA	B,Y
			STA		D,U
