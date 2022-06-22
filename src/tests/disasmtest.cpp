#include <iostream>
#include <string>
#include "mc6809.h"
#include "plainmemory.h"

// 2000                                          ORG     $2000
// 2000  8677                                    LDA     #$77
// 2002  C611                                    LDB     #$11
// 2004  CC1973                                  LDD     #$1973
// 2007  A58D0FF5                                BITA    $3000,PCR
// 200B  A886                                    EORA    A,X
// 200D  A4A5                                    ANDA    B,Y
// 200F  A7CB                                    STA     D,U
// 2011  A680                                    LDA     ,X+
// 2013  E6A1                                    LDB     ,Y++
// 2015  A7C2                                    STA     ,-U
// 2017  AFC3                                    STX     ,--U
// 2019  AD9FFBFA                                JSR     [$FBFA]
// 201D  6E96                                    JMP     [A,X]
// 201F  3476                                    PSHS    A,B,X,Y,U
// 2021  3589                                    PULS    CC,DP,PC
// 2023  3652                                    PSHU    A,X,S
// 2025  35C8                                    PULS    DP,U,PC
// 2027  B7FD0F                                  STA     $FD0F

std::string code=
"8677"
"C611"
"CC1973"
"A58D0FF5"
"A886"
"A4A5"
"A7CB"
"A680"
"E6A1"
"A7C2"
"AFC3"
"AD9FFBFA"
"6E96"
"3476"
"3589"
"3652"
"35C8"
"B7FD0F"
;

class TestVM : public VMBase
{
public:
	virtual const char *DeviceName(void) const{return "TestVM";}
};

int main(void)
{
	TestVM vm;
	MC6809 cpu(&vm);
	PlainMemoryAccess mem;

	unsigned int addr=0x2000;
	for(int i=0; i<code.size(); i+=2)
	{
		char hex[3]={code[i],code[i+1],0};
		mem.RAM[addr++]=cpputil::Xtoi(hex);
	}

	uint16_t PC=0x2000;
	while(PC<addr)
	{
		auto nextPC=PC;
		auto inst=cpu.FetchInstruction(&mem,nextPC);

		std::cout << inst.length << " " << cpputil::Ustox(inst.opCode) << std::endl;

		auto disasm=cpu.Disassemble(inst,PC);
		std::cout << cpputil::Ustox(PC) << " " << disasm << std::endl;

		PC=nextPC;
	}

	return 0;
}
