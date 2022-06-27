#include "plainmemory.h"

PlainMemoryAccess::PlainMemoryAccess()
{
	for(auto &d : RAM)
	{
		d=0;
	}
}
/* virtual */ uint8_t PlainMemoryAccess::FetchByte(uint16_t addr)
{
	return RAM[addr];
}
/* virtual */ uint16_t PlainMemoryAccess::FetchWord(uint16_t addr)
{
	return (RAM[addr]<<8)|RAM[addr+1];
}
/* virtual */ void PlainMemoryAccess::StoreByte(uint16_t addr,uint8_t data)
{
	RAM[addr]=data;
}
/* virtual */ void PlainMemoryAccess::StoreWord(uint16_t addr,uint16_t data)
{
	RAM[addr]=(data>>8);
	RAM[addr+1]=(data&0xFF);
}
/* virtual */ uint8_t PlainMemoryAccess::NonDestructiveFetchByte(uint16_t addr) const
{
	return RAM[addr];
}
/* virtual */ uint16_t PlainMemoryAccess::NonDestructiveFetchWord(uint16_t addr) const
{
	return (RAM[addr]<<8)|RAM[addr+1];
}
