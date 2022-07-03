#include <iostream>
#include <string>
#include "mc6809.h"
#include "mc6809util.h"
#include "plainmemory.h"

//0000  CC0766                                          LDD             #$0766
//0003  1E89                                            EXG             A,B
//0005  8B99                                            ADDA    #$99
//0007  19                                              DAA
//0008  1E89                                            EXG             A,B
//000A  8999                                            ADCA    #$99
//000C  19                                              DAA
//000D  39                                              RTS

std::string code=
"CC0766"
"1E89"
"8B99"
"19"
"1E89"
"8999"
"19"
"39"
;



class TestVM : public VMBase
{
public:
	virtual const char *DeviceName(void) const{return "TestVM";}
};

// From Polar Star III.  By adding $99999999 to BCD, it can essentially subtract 1.  Same as adding $FFFF to subtract one.
bool DAA(int bcd1,int bcd2,bool verbose)
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

	unsigned int bcdToHex;
	bcdToHex=((bcd1/1000)%10)*0x1000+
	         ((bcd1/100)%10) *0x100+
	         ((bcd1/10)%10)  *0x10+
	          (bcd1%10);

	mem.RAM[0x2001]=(bcdToHex>>8);
	mem.RAM[0x2002]=(bcdToHex&0xFF);

	bcdToHex=((bcd2/1000)%10)*0x1000+
	         ((bcd2/100)%10) *0x100+
	         ((bcd2/10)%10)  *0x10+
	          (bcd2%10);
	mem.RAM[0x2006]=(bcdToHex&0xFF);
	mem.RAM[0x200B]=(bcdToHex>>8);

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

		if(cpu.state.PC==0x200D)
		{
			break;
		}

		cpu.RunOneInstruction(mem);
	}

	auto sum=(bcd1+bcd2)%10000;
	bcdToHex=((sum/1000)%10)*0x1000+
	         ((sum/100)%10) *0x100+
	         ((sum/10)%10)  *0x10+
	          (sum%10);
	unsigned int D=cpu.state.A();
	D<<=8;
	D|=cpu.state.B();

	if(D!=bcdToHex)
	{
		return 1;
	}

	return 0;
}

int main(void)
{
	for(int i=1; i<9999; ++i)
	{
		if(0!=DAA(i,9999,false))
		{
			printf("Failed at %d\n",i);
			return 1;
		}
	}
	for(int i=0; i<=1000; ++i)
	{
		for(int j=0; j<=1000; ++j)
		{
			if(0!=DAA(i,j,false))
			{
				printf("Failed at %d+%d\n",i,j);
			}
		}
	}
	std::cout << "All Passes." << std::endl;
	return 0;
}
