; This program is to verify if 6809 was dummy-reading an extra byte PULS A,X,Y instruction.
; Dragon Buster FM77AV shows a vertical bar on the right of the screen.
; The bar was supposed to be cleared by hardware-drawing, which works by dummy-reading VRAM.
; Main-CPU uses this hardware-drawing when showing a field map, $26 bytes per line.
; When clearing the screen, it uses PULS instruction and appeared to dummy-read $25 bytes per line.
; Unless PULS is dummy-reading (or pre-reading) an additional byte from the RAM (sending a read
; signal to the hardware-draing chip), one byte per line is left uncleared.

; The test on actual FM77AV hardware confirmed that PULS A,X,Y indeed reads 6 bytes from the VRAM.
; Hardware-drawing chip sees the memory-read and fills 48 pixels in a row.

; PULS A,B,X,Y let the hardware-drawing fill 7 bytes, and PULS A,B,X,Y,U 9 bytes.

; PULS instruction reads one extra byte in conclusion.

					PSHS	clsCC
					ORCC	#$50
					STS		SAVESTACK,PCR

					LDX		#$FD80
					LDA		#$30
MMRLOOP				STA		,X+
					INCA
					CMPA	#$40
					BNE		MMRLOOP

					LDD		#$101D
					STD		$FD85		; $5000 Mapped to VRAM First 4K, $6000 Mapped to SubCPU I/O

					LDA		#$80
					STA		$FD05

					LDA		#$80
					STA		$FD93		; Enable MMR


					LDD		#$8007
					STA		$6430		; Sub D430 mapped to 6430.  Disable NMI, Active Page 0
					STD		$6410		; Enable Hardware Drawing, No compare, PSET, Color 7
					CLR		$6412		; No mask
					LDA		#8
					STA		$641B		; All banks enabled

					LDS		#$5280
					PULS	A,X,Y
					LDS		#$52D0
					PULS	A,B,X,Y
					LDS		#$5320
					PULS	A,B,X,Y,U

					CLR		$FD05

					LDS		SAVESTACK,PCR
					PULS	CC,PC

SAVESTACK			FDB		0
