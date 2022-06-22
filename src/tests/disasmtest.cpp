#include <iostream>
#include <string>
#include "mc6809.h"
#include "plainmemory.h"

// 2000                                          ORG             $2000
// 2000  8677                                    LDA             #$77
// 2002  C611                                    LDB             #$11
// 2004  CC1973                                  LDD             #$1973
// 2007  A58D0FF5                                BITA    $3000,PCR
// 200B  A886                                    EORA    A,X
// 200D  A4A5                                    ANDA    B,Y
// 200F  A7CB                                    STA             D,U



std::string code=
"8677"
"C611"
"CC1973"
"A58D0FF5"
"A886"
"A4A5"
"A7CB"
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
