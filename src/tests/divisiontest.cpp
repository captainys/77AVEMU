#include <iostream>
#include <string>
#include "mc6809.h"
#include "mc6809util.h"
#include "plainmemory.h"

std::string code=
"10CE6000"
"CC1388"  // 2004  CC1388                                          LDD             #5000
"ED8C16"
"CC0032"  // 200A  CC0032                                          LDD             #50
"ED8C12"
"8D12"
"AE8C0D"
"10AE8C07"

"86FF"
"B70000"

"20FE"

"0000"
"0000"

"3456"

"EC8CF7"
"A78C7E"
"2A08"
"43"
"53"
"C30001"
"ED8CEA"

"EC8CE9"
"A78C6F"
"2A08"
"43"
"53"
"C30001"
"ED8CDC"

"8E0001"
"AF8C26"
"8E0000"

"10A38CCD"
"240F"
"58"
"49"
"2B0B"

"ED8CC6"
"688C14"
"698C10"
"2AEB"

"EC8CB9"
"10A38CB7"
"2507"

"A38CB2"
"30897FFF"

"648CAB"
"668CA9"

"648CF5"
"668CF3"
"24E5"

"AF8C9D"
"ED8C98"
"CC0000"

"6D8C1C"
"2A08"

"A38C8D"
"ED8C8A"

"86FF"

"A88C10"
"2A0B"

"CC0000"
"A38DFF7E"
"ED8DFF7A"

"35D6"
"0000"
;



class TestVM : public VMBase
{
public:
	virtual const char *DeviceName(void) const{return "TestVM";}
};

bool Divide(int numer,int denom,bool verbose)
{
	TestVM vm;
	static MC6809 cpu(&vm);
	PlainMemoryAccess mem;

	cpu.Reset();

	unsigned int addr=0x2000;
	for(int i=0; i<code.size(); i+=2)
	{
		char hex[3]={code[i],code[i+1],0};
		mem.RAM[addr++]=cpputil::Xtoi(hex);
	}

	mc6809util::StoreWordSigned(mem.RAM+0x2005,numer);
	mc6809util::StoreWordSigned(mem.RAM+0x200B,denom);

	cpu.state.PC=0x2000;
	mem.RAM[0]=0;
	while(0==mem.RAM[0] && true!=vm.CheckAbort())
	{
		auto inst=cpu.FetchInstruction(mem,cpu.state.PC);

		if(true==verbose)
		{
			for(auto str : cpu.GetStatusText())
			{
				std::cout << str << std::endl;
			}

			auto byteCode=cpu.FormatByteCode(inst);
			while(byteCode.size()<11)
			{
				byteCode.push_back(' ');
			}

			auto disasm=cpu.Disassemble(inst,cpu.state.PC);
			std::cout << cpputil::Ustox(cpu.state.PC) << " " << byteCode << " " << disasm << std::endl;
		}

		cpu.RunOneInstruction(mem);
	}

	int quo=cpu.state.X;
	int rem=cpu.state.Y;
	quo=(quo&0x7FFF)-(quo&0x8000);
	rem=(rem&0x7FFF)-(rem&0x8000);

	std::cout << numer << '/' << denom << std::endl;
	std::cout << "Quotient  " << quo << std::endl;
	std::cout << "Remainder " << rem << std::endl;
	std::cout << "By C++ Quotient  " << (numer/denom) << std::endl;
	std::cout << "By C++ Remainder " << (numer%denom) << std::endl;

	if(quo!=numer/denom || rem!=numer%denom || 0==mem.RAM[0])
	{
		return 1;
	}

	return 0;
}

int main(void)
{
	if(0!=Divide(5000,100,false) ||
	   0!=Divide(101,100,false) ||
	   0!=Divide(-1000,7,false) ||
	   0!=Divide(-5000,-47,false) ||
	   0!=Divide(12345,765,false))
	{
		return 1;
	}
	std::cout << "All Passes." << std::endl;
	return 0;
}
