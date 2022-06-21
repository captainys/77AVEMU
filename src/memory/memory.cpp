#include <iostream>
#include <fstream>
#include <string>
#include "memory.h"
#include "mc6809util.h"
#include "fm77av.h"
#include "cpputil.h"



PhysicalMemory::PhysicalMemory(VMBase *vmBase) : Device(vmBase)
{
	for(auto &d : state.data)
	{
		d=0;
	}
	for(auto &d : state.extVRAM)
	{
		d=0;
	}
	for(auto &d : ROM_BOOT_DOS)
	{
		d=0xFF;
	}
	for(auto &d : ROM_BOOT_BASIC)
	{
		d=0xFF;
	}
	for(auto &d : ROM_FBASIC)
	{
		d=0xFF;
	}
	for(auto &d : ROM_INITIATOR)
	{
		d=0xFF;
	}
	for(auto &d : ROM_KANJI)
	{
		d=0xFF;
	}
	for(auto &d : ROM_KANJI2)
	{
		d=0xFF;
	}
	for(auto &d : ROM_DIC)
	{
		d=0xFF;
	}
	for(auto &d : ROM_ASCII_FONT)
	{
		d=0xFF;
	}
	for(auto &d : ROM_SUBSYS_A)
	{
		d=0xFF;
	}
	for(auto &d : ROM_SUBSYS_B)
	{
		d=0xFF;
	}
	for(auto &d : ROM_SUBSYS_C)
	{
		d=0xFF;
	}

	for(auto &t : memType)
	{
		t=MEMTYPE_RAM;
	}
	for(unsigned int addr=SUBSYS_VRAM_BEGIN; addr<SUBSYS_VRAM_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_VRAM;
	}
	for(unsigned int addr=SUBSYS_SHARED_RAM_BEGIN; addr<SUBSYS_SHARED_RAM_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_SHARED_RAM;
	}
	for(unsigned int addr=SUBSYS_IO_BEGIN; addr<SUBSYS_IO_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_IO;
	}
	for(unsigned int addr=SUBSYS_FONT_ROM_BEGIN; addr<SUBSYS_FONT_ROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_FONT_ROM;
	}
	for(unsigned int addr=SUBSYS_MONITOR_ROM_BEGIN; addr<SUBSYS_MONITOR_ROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_SUBSYS_MONITOR_ROM;
	}
	for(unsigned int addr=MAINSYS_INITIATOR_ROM_BEGIN; addr<MAINSYS_INITIATOR_ROM_BEGIN; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_INITIATOR_ROM;
	}
	for(unsigned int addr=MAINSYS_FBASIC_ROM_BEGIN; addr<MAINSYS_FBASIC_ROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_FBASIC_ROM;
	}
	for(unsigned int addr=MAINSYS_SHARED_RAM_BEGIN; addr<MAINSYS_SHARED_RAM_END; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_SHARED_RAM;
	}
	for(unsigned int addr=MAINSYS_IO_BEGIN; addr<MAINSYS_IO_END; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_IO;
	}
	for(unsigned int addr=MAINSYS_BOOT_ROM_BEGIN; addr<MAINSYS_BOOT_ROM_END; ++addr)
	{
		memType[addr]=MEMTYPE_MAINSYS_BOOT_ROM;
	}
}

bool PhysicalMemory::LoadROMFiles(std::string ROMPath)
{
	struct ToLoad
	{
		bool mandatory;
		uint8_t *ptr;
		uint64_t length;
		std::string fName,fNameAlt;
	};

	std::vector <ToLoad> toLoad;
	FM77AV *fm77av=(FM77AV *)vmPtr;

	// FM-7
	//   BOOT_BAS.ROM
	//   BOOT_DOS.ROM
	//   FBASIC30.ROM
	//   SUBSYS_C.ROM or SUBSYS.ROM
	if(FM77AV::MACHINETYPE_FM7==fm77av->state.machineType)
	{
		std::vector <struct ToLoad> lst=
		{
			{true, ROM_BOOT_BASIC,BOOT_ROM_SIZE,                              "BOOT_BAS.ROM",""},
			{true, ROM_BOOT_DOS,  BOOT_ROM_SIZE,                              "BOOT_DOS.ROM",""},
			{true, ROM_FBASIC,    FBASIC_ROM_SIZE,                            "FBASIC30.ROM",""},
			{true, ROM_SUBSYS_C,  SUBSYS_MONITOR_ROM_SIZE+ASCII_FONT_ROM_SIZE,"SUBSYS_C.ROM","SUBSYS.ROM"},
			{false,ROM_KANJI,     KANJI_ROM_SIZE,                             "KANJI1.ROM","KANJI.ROM"},
		};
		std::swap(toLoad,lst);
	}
	// FM77AV
	//   BOOT_BAS.ROM
	//   BOOT_DOS.ROM
	//   FBASIC30.ROM
	//   INITIATE.ROM
	//   KANJI.ROM
	//   SUBSYSCG.ROM
	//   SUBSYS_A.ROM
	//   SUBSYS_B.ROM
	//   SUBSYS_C.ROM
	else if(FM77AV::MACHINETYPE_FM77AV==fm77av->state.machineType)
	{
		std::vector <struct ToLoad> lst=
		{
			{true, ROM_BOOT_BASIC,BOOT_ROM_SIZE,                              "BOOT_BAS.ROM",""},
			{true, ROM_BOOT_DOS,  BOOT_ROM_SIZE,                              "BOOT_DOS.ROM",""},
			{true, ROM_FBASIC,    FBASIC_ROM_SIZE,                            "FBASIC30.ROM",""},
			{true, ROM_INITIATOR, INITIATOR_ROM_SIZE,                         "INITIATE.ROM",""},
			{true ,ROM_KANJI,     KANJI_ROM_SIZE,                             "KANJI1.ROM","KANJI.ROM"},
			{true, ROM_ASCII_FONT,4*ASCII_FONT_ROM_SIZE,                      "SUBSYSCG.ROM",""},
			{true, ROM_SUBSYS_A,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_A.ROM",""},
			{true, ROM_SUBSYS_B,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_B.ROM",""},
			{true, ROM_SUBSYS_C,  SUBSYS_MONITOR_ROM_SIZE+ASCII_FONT_ROM_SIZE,"SUBSYS_C.ROM","SUBSYS.ROM"},
		};
		std::swap(toLoad,lst);
	}
	// FM77AV40
	//   BOOT_BAS.ROM
	//   BOOT_DOS.ROM
	//   FBASIC30.ROM
	//   INITIATE.ROM
	//   KANJI1.ROM or KANJI.ROM
	//   KANJI2.ROM
	//   SUBSYSCG.ROM
	//   SUBSYS_A.ROM
	//   SUBSYS_B.ROM
	//   SUBSYS_C.ROM
	//   DICROM.ROM
	else if(FM77AV::MACHINETYPE_FM77AV40==fm77av->state.machineType)
	{
		std::vector <struct ToLoad> lst=
		{
			{true, ROM_BOOT_BASIC,BOOT_ROM_SIZE,                              "BOOT_BAS.ROM",""},
			{true, ROM_BOOT_DOS,  BOOT_ROM_SIZE,                              "BOOT_DOS.ROM",""},
			{true, ROM_FBASIC,    FBASIC_ROM_SIZE,                            "FBASIC30.ROM",""},
			{true, ROM_INITIATOR, INITIATOR_ROM_SIZE,                         "INITIATE.ROM",""},
			{true ,ROM_KANJI,     KANJI_ROM_SIZE,                             "KANJI1.ROM","KANJI.ROM"},
			{false,ROM_KANJI2,    KANJI_ROM_SIZE,                             "KANJI2.ROM",""},
			{true, ROM_ASCII_FONT,4*ASCII_FONT_ROM_SIZE,                      "SUBSYSCG.ROM",""},
			{true, ROM_SUBSYS_A,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_A.ROM",""},
			{true, ROM_SUBSYS_B,  SUBSYS_MONITOR_ROM_SIZE,                    "SUBSYS_B.ROM",""},
			{true, ROM_SUBSYS_C,  SUBSYS_MONITOR_ROM_SIZE+ASCII_FONT_ROM_SIZE,"SUBSYS_C.ROM","SUBSYS.ROM"},
			{false,ROM_DIC,       DIC_ROM_SIZE,                               "DICROM.ROM",""},
		};
		std::swap(toLoad,lst);
	}

	for(auto rom : toLoad)
	{
		auto fullPath=cpputil::MakeFullPathName(ROMPath,rom.fName);
		std::ifstream ifp(fullPath,std::ios::binary);
		if(true!=ifp.is_open())
		{
			fullPath=cpputil::MakeFullPathName(ROMPath,rom.fNameAlt);
			ifp.open(fullPath,std::ios::binary);
		}
		if(true!=ifp.is_open())
		{
			if(true==rom.mandatory)
			{
				std::cout << "Failed to load " << fullPath << std::endl;
				std::cout << "Which is mandatory for emulating " << fm77av->MachineTypeString() << std::endl;
				return false;
			}
			else
			{
				continue;
			}
		}
		ifp.read((char *)rom.ptr,rom.length);
	}

	return true;
}

// 0x3FD00 to 0x3FDFF MainCPU I/O
// 0x1D400 to 0x1D4FF SubCPU I/O
uint8_t PhysicalMemory::FetchByte(uint16_t addr)
{
	return state.data[addr];
}
uint16_t PhysicalMemory::FetchWord(uint16_t addr0,uint16_t addr1)
{
	return mc6809util::FetchWord(state.data[addr0],state.data[addr1]);
}
void PhysicalMemory::StoreByte(uint16_t addr,uint8_t d)
{
	state.data[addr]=d;
}
void PhysicalMemory::StoreWord(uint16_t addr0,uint16_t addr1,uint16_t d)
{
	mc6809util::StoreWord(state.data[addr0],state.data[addr1],d);
}

////////////////////////////////////////////////////////////

MainCPUAccess::MainCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr) : Device(vmPtr)
{
	this->physMemPtr=physMemPtr;
}
/* virtual */ uint8_t MainCPUAccess::FetchByte(uint16_t addr)
{
	if(true==MMREnabled)
	{
		return physMemPtr->FetchByte(CPUAddrToPhysicalAddr(addr));
	}
	else
	{
		return physMemPtr->FetchByte(MAINCPU_ADDR_BASE+addr);
	}
}
/* virtual */ uint16_t MainCPUAccess::FetchWord(uint16_t addr)
{
	if(true==MMREnabled)
	{
		return physMemPtr->FetchWord(
			CPUAddrToPhysicalAddr(addr),
			CPUAddrToPhysicalAddr(addr+1));
	}
	else
	{
		return physMemPtr->FetchWord(
			MAINCPU_ADDR_BASE+addr,
			MAINCPU_ADDR_BASE+addr+1);
	}
}
/* virtual */ void MainCPUAccess::StoreByte(uint16_t addr,uint8_t data)
{
	if(true==MMREnabled)
	{
		physMemPtr->StoreByte(CPUAddrToPhysicalAddr(addr),data);
	}
	else
	{
		physMemPtr->StoreByte(MAINCPU_ADDR_BASE+addr,data);
	}
}
/* virtual */ void MainCPUAccess::StoreWord(uint16_t addr,uint16_t data)
{
	if(true==MMREnabled)
	{
		physMemPtr->StoreWord(
			CPUAddrToPhysicalAddr(addr),
			CPUAddrToPhysicalAddr(addr+1),
			data);
	}
	else
	{
		physMemPtr->StoreWord(
			MAINCPU_ADDR_BASE+addr,
			MAINCPU_ADDR_BASE+addr+1,
			data);
	}
}

////////////////////////////////////////////////////////////

SubCPUAccess::SubCPUAccess(class VMBase *vmPtr,PhysicalMemory *physMemPtr) : Device(vmPtr)
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
