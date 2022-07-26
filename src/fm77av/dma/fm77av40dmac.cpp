#include <iostream>
#include "cpputil.h"
#include "fm77av40dmac.h"

FM77AV40DMAC::FM77AV40DMAC(VMBase *vmBase) : Device(vmBase)
{
	Reset();
}

void FM77AV40DMAC::Reset(void)
{
	state.enabled=false;
	for(auto &d: state.regs)
	{
		d=0;
	}
}
void FM77AV40DMAC::WriteFD98(uint8_t data)
{
	state.addrLatch=(data&0x1F);
}
void FM77AV40DMAC::WriteFD99(uint8_t data)
{
	std::cout << "DMAC Write Reg=" << cpputil::Ubtox(state.addrLatch) << " Value=" << cpputil::Ubtox(data) << std::endl;
	state.regs[state.addrLatch]=data;
}
uint8_t FM77AV40DMAC::NonDestructiveReadFD99(void) const
{
	return state.regs[state.addrLatch];
}
