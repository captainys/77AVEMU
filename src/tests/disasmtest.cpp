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
// 202A  1E89                                    EXG     A,B
// 202C  1F01                                    TFR     D,X
// 202E  1E83                                    EXG     A,U
// 2030  1F40                                    TFR     S,D
// 2032  10AE9D1FC9                              LDY     [$4000,PCR]

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
"1E89"
"1F01"
"1E83"
"1F40"
"10AE9D1FC9"
;

std::string correctOutput[]=
{
"LDA     #$77",
"LDB     #$11",
"LDD     #$1973",
"BITA    $3000,PCR",
"EORA    A,X",
"ANDA    B,Y",
"STA     D,U",
"LDA     ,X+",
"LDB     ,Y++",
"STA     ,-U",
"STX     ,--U",
"JSR     [$FBFA]",
"JMP     [A,X]",
"PSHS    A,B,X,Y,U",
"PULS    CC,DP,PC",
"PSHU    A,X,S",
"PULS    DP,U,PC",
"STA     $FD0F",
"EXG     A,B",
"TFR     D,X",
"EXG     A,U",
"TFR     S,D",
"LDY     [$4000,PCR]",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
};



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

	int res=0;

	uint16_t PC=0x2000;
	int i=0;
	while(PC<addr)
	{
		auto nextPC=PC;
		auto inst=cpu.FetchInstruction(&mem,nextPC);

		auto byteCode=cpu.FormatByteCode(inst);
		while(byteCode.size()<11)
		{
			byteCode.push_back(' ');
		}

		auto disasm=cpu.Disassemble(inst,PC);
		std::cout << cpputil::Ustox(PC) << " " << byteCode << " " << disasm << std::endl;

		if(correctOutput[i]!=disasm)
		{
			std::cout << "ERROR!!" << std::endl;
			res=1;
		}

		PC=nextPC;
		++i;
	}

	return res;
}
