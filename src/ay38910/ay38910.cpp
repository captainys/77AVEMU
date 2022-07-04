#include <iostream>
#include "ay38910.h"
#include "cpputil.h"



void AY38910::Reset(void)
{
	for(auto &r : state.regs)
	{
		r=0;
	}
}
uint8_t AY38910::Read(uint8_t reg) const
{
	return state.regs[reg];
}
void AY38910::Write(uint8_t reg,uint8_t value)
{
	state.regs[reg]=value;
std::cout << "Reg " << cpputil::Ubtox(reg) << " <- " << cpputil::Ubtox(value) << std::endl;
}
