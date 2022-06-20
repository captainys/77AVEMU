#include "memory.h"
#include "mc6809util.h"



PhysicalMemory::PhysicalMemory()
{
	for(auto &d : data)
	{
		d=0xFF;
	}
}
// 0x3FD00 to 0x3FDFF MainCPU I/O
// 0x1D400 to 0x1D4FF SubCPU I/O
uint8_t PhysicalMemory::FetchByte(uint16_t addr)
{
	return data[addr];
}
uint16_t PhysicalMemory::FetchWord(uint16_t addr0,uint16_t addr1)
{
	return mc6809util::FetchWord(data[addr0],data[addr1]);
}
void PhysicalMemory::StoreByte(uint16_t addr,uint8_t d)
{
	data[addr]=d;
}
void PhysicalMemory::StoreWord(uint16_t addr0,uint16_t addr1,uint16_t d)
{
	mc6809util::StoreWord(data[addr0],data[addr1],d);
}

////////////////////////////////////////////////////////////

MainCPUAccessNoMMR::MainCPUAccessNoMMR(FM77AV *vmPtr,PhysicalMemory *physMemPtr) : Device(vmPtr)
{
	this->physMemPtr=physMemPtr;
}
/* virtual */ uint8_t MainCPUAccessNoMMR::FetchByte(uint16_t addr)
{
	return physMemPtr->FetchByte(MAINCPU_ADDR_BASE+addr);
}
/* virtual */ uint16_t MainCPUAccessNoMMR::FetchWord(uint16_t addr)
{
	return physMemPtr->FetchWord(
		MAINCPU_ADDR_BASE+addr,
	    MAINCPU_ADDR_BASE+((addr+1)&0xFFFF));
}
/* virtual */ void MainCPUAccessNoMMR::StoreByte(uint16_t addr,uint8_t data)
{
	physMemPtr->StoreByte(MAINCPU_ADDR_BASE+addr,data);
}
/* virtual */ void MainCPUAccessNoMMR::StoreWord(uint16_t addr,uint16_t data)
{
	physMemPtr->StoreWord(
	    MAINCPU_ADDR_BASE+addr,
	    MAINCPU_ADDR_BASE+((addr+1)&0xFFFF),
	    data);
}

////////////////////////////////////////////////////////////

MainCPUAccessMMR::MainCPUAccessMMR(FM77AV *vmPtr,PhysicalMemory *physMemPtr) : Device(vmPtr)
{
	this->physMemPtr=physMemPtr;
}
/* virtual */ uint8_t MainCPUAccessMMR::FetchByte(uint16_t addr)
{
	return physMemPtr->FetchByte(CPUAddrToPhysicalAddr(addr));
}
/* virtual */ uint16_t MainCPUAccessMMR::FetchWord(uint16_t addr)
{
	return physMemPtr->FetchWord(
		CPUAddrToPhysicalAddr(addr),
		CPUAddrToPhysicalAddr(addr+1));
}
/* virtual */ void MainCPUAccessMMR::StoreByte(uint16_t addr,uint8_t data)
{
	physMemPtr->StoreByte(CPUAddrToPhysicalAddr(addr),data);
}
/* virtual */ void MainCPUAccessMMR::StoreWord(uint16_t addr,uint16_t data)
{
	physMemPtr->StoreWord(
		CPUAddrToPhysicalAddr(addr),
		CPUAddrToPhysicalAddr(addr+1),
		data);

}

////////////////////////////////////////////////////////////

SubCPUAccess::SubCPUAccess(FM77AV *vmPtr,PhysicalMemory *physMemPtr) : Device(vmPtr)
{
}
/* virtual */ uint8_t SubCPUAccess::FetchByte(uint16_t addr)
{
	return physMemPtr->FetchByte(SUBCPU_ADDR_BASE+addr);
}
/* virtual */ uint16_t SubCPUAccess::FetchWord(uint16_t addr)
{
	return physMemPtr->FetchWord(
		SUBCPU_ADDR_BASE+addr,
		SUBCPU_ADDR_BASE+((addr+1)&0xFFFF));
}
/* virtual */ void SubCPUAccess::StoreByte(uint16_t addr,uint8_t data)
{
	physMemPtr->StoreByte(SUBCPU_ADDR_BASE+addr,data);
}
/* virtual */ void SubCPUAccess::StoreWord(uint16_t addr,uint16_t data)
{
	physMemPtr->StoreWord(
		SUBCPU_ADDR_BASE+addr,
		SUBCPU_ADDR_BASE+((addr+1)&0xFFFF),
		data);
}
