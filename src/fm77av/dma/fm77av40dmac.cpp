#include <iostream>
#include "cpputil.h"
#include "fm77av40dmac.h"

FM77AV40DMAC::FM77AV40DMAC(VMBase *vmBase) : Device(vmBase)
{
	Reset();
}

void FM77AV40DMAC::Reset(void)
{
	for(auto &d: state.regs)
	{
		d=0;
	}
}

bool FM77AV40DMAC::TxRQ(void) const
{
	return (0!=(state.regs[0x14]&1));
}
uint16_t FM77AV40DMAC::Address(void) const
{
	uint16_t data=state.regs[0];
	data<<=8;
	data|=state.regs[1];
	return data;
}
uint16_t FM77AV40DMAC::NumBytes(void) const
{
	uint16_t data=state.regs[2];
	data<<=8;
	data|=state.regs[3];
	return data;
}
bool FM77AV40DMAC::AddressGoUp(void) const
{
	return 0!=(state.regs[0x10]&8);
}
bool FM77AV40DMAC::FDCtoMEM(void) const
{
	return 0==(state.regs[0x10]&1);
}
void FM77AV40DMAC::SetDMAEnd(bool ended)
{
	state.regs[0x10]&=0x7F;
	if(true==ended)
	{
		state.regs[0x10]|=0x80;
	}
}
void FM77AV40DMAC::SetBusy(bool busy)
{
	state.regs[0x10]&=0xBF;
	if(true==busy)
	{
		state.regs[0x10]|=0x40;
	}
}
bool FM77AV40DMAC::IRQEnabled(void) const
{
	return 0!=(state.regs[0x15]&1);
}
void FM77AV40DMAC::SetIRQ(bool IRQ)
{
	state.regs[0x15]&0x7F;
	if(true==IRQ)
	{
		state.regs[0x15]|=0x80;
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
void FM77AV40DMAC::ReadFD99(void)
{
	std::cout << "DMAC Read  Reg=" << cpputil::Ubtox(state.addrLatch) << " Value=" << cpputil::Ubtox(state.regs[state.addrLatch]) << std::endl;
	if(0x15==state.addrLatch)
	{
		state.regs[0x15]&=0x7F;
	}
}
uint8_t FM77AV40DMAC::NonDestructiveReadFD99(void) const
{
	return state.regs[state.addrLatch];
}
