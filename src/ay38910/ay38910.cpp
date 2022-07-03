#include "ay38910.h"



void AY38910::Reset(void)
{
	for(auto &r : state.regs)
	{
		r=0;
	}
}
uint8_t AY38910::ReadRegister(uint8_t reg) const
{
	return state.regs[state.regLatch];
}
void AY38910::WriteRegister(uint8_t reg,uint8_t value)
{
	state.regs[state.regLatch]=value;
}
