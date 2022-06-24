#include <iostream>
#include <string>
#include "mc6809.h"
#include "plainmemory.h"

std::string code=
"10CE6000"
"CC1388"
"ED8C12"
"CC0032"
"ED8C0E"
"8D0E"
"EC8C07"

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

	cpu.state.PC=0x2000;
	mem.RAM[0]=0;
	while(0==mem.RAM[0] && true!=vm.CheckAbort())
	{
		auto inst=cpu.FetchInstruction(mem,cpu.state.PC);

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

		cpu.RunOneInstruction(mem);
	}

	return res;
}
