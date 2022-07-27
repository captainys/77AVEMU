; Confirmed on real FM77AV.
;   TFR U,A transfers low 8-bits to A.
				PSHS	A,U
				LDU		#$7711
				TFR		U,A
				STA		DST,PCR
				PULS	A,U,PC
DST				FCB		0
